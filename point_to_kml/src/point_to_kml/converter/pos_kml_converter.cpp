#include "converter/pos_kml_converter.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#define NOMINMAX
#include <windows.h>

#include <tinyxml2.h>
#include <proj_api.h>

namespace cvt
{


	PosKmlConverter::PosKmlConverter( const std::string & in_file ) :
		_in_file(in_file)
	{

	}

	PosKmlConverter::PosKmlConverter()
	{
		_in_file = "";
	}

	PosKmlConverter::~PosKmlConverter()
	{

	}

	int PosKmlConverter::load_pos( std::string proj_cmd /*= ""*/ )
	{
		int ret = -1;

		projPJ longlat_proj = NULL;
		projPJ src_proj = NULL;

		do 
		{
			std::ifstream file(_in_file);
			if (!file.good()) break;
			

			// initial proj
			if (!proj_cmd.empty())
			{
				longlat_proj = pj_init_plus("+proj=longlat +datum=WGS84 +ellps=WGS84");
				src_proj = pj_init_plus(proj_cmd.c_str());
				if(!longlat_proj || !src_proj)  
				{
					std::cout<<"Initial proj parameters failed."<<std::endl;
					std::cout<<"proj_cmd: "<<proj_cmd<<std::endl;
					break;
				}
			}
			

			std::vector<POSPoint> tmp_points;

			std::string line;
			std::stringstream sstr;
			std::getline(file, line);
			while (file.good())
			{
				POSPoint pt;

				sstr.clear(); sstr.str("");
				sstr << line;

				sstr >> pt.name;

				sstr >> pt.x;
				sstr >> pt.y;
				sstr >> pt.z;


				// convert
				if (longlat_proj && src_proj)
				{
					double x = pt.x;
					double y = pt.y;
					if(pj_transform(src_proj, longlat_proj, 1, 0, 
						&x, &y, NULL))
					{
						std::cout<<"pj_transform failed at pos conversion. "
							<<pt.x<<" "<<pt.y<<" "<<pt.z<<std::endl;
						continue;
					}
					pt.x = x * RAD_TO_DEG;
					pt.y = y * RAD_TO_DEG;
				}


				tmp_points.push_back(pt);

				line.clear();
				std::getline(file, line);
			}


			_gps_points.swap(tmp_points);

			ret = 0;
		} while (0);

		if(src_proj)
		{
			pj_free(src_proj);
			src_proj = NULL;
		}
		if(longlat_proj)
		{
			pj_free(longlat_proj);
			longlat_proj = NULL;
		}

		return ret;
	}

	int PosKmlConverter::SetInputPath( const std::string & input_path )
	{
		int ret = -1;

		do 
		{
			_in_file = input_path;

			ret = 0;
		} while (0);

		return ret;
	}

	int PosKmlConverter::Convert( const std::string & kml_file, 
		std::string proj_cmd /*= ""*/ )
	{
		int ret = -1;

		do 
		{
			if (_gps_points.empty())
			{
				if (load_pos(proj_cmd))
				{
					std::cout<<"load_pos failed."<<std::endl;
					break;
				}				
			}


			tinyxml2::XMLDocument doc;
			tinyxml2::XMLDeclaration *decl = doc.NewDeclaration("xml version=\"1.0\"");
			doc.InsertEndChild(decl);

			tinyxml2::XMLElement * kml_node = doc.NewElement("kml");
			if (!kml_node) break;
			kml_node->SetAttribute("xmlns", "http://www.opengis.net/kml/2.2");
			kml_node->SetAttribute("xmlns:gx", "http://www.google.com/kml/ext/2.2");
			doc.InsertEndChild(kml_node);

			tinyxml2::XMLElement * document_node = doc.NewElement("Document");
			if (!document_node) goto error0;
			kml_node->InsertEndChild(document_node);

			tinyxml2::XMLElement * xml_folder = doc.NewElement("Folder");
			if (!xml_folder) goto error0;
			document_node->InsertEndChild(xml_folder);

			tinyxml2::XMLElement * folder_name_node = doc.NewElement("name");
			if (!folder_name_node) goto error0;
			folder_name_node->InsertEndChild(doc.NewText("CameraPos"));
			xml_folder->InsertEndChild(folder_name_node);				

			size_t num_pts = _gps_points.size();
			for (size_t i_p = 0; i_p < num_pts; ++i_p)
			{
				const POSPoint & gps_pt = _gps_points[i_p];

				tinyxml2::XMLElement * placemark_node = doc.NewElement("Placemark");
				if (!placemark_node) goto error0;
				xml_folder->InsertEndChild(placemark_node);

				tinyxml2::XMLElement * name_node = doc.NewElement("name");
				if (!name_node) goto error0;
				name_node->InsertEndChild(doc.NewText(gps_pt.name.c_str()));
				placemark_node->InsertEndChild(name_node);

				tinyxml2::XMLElement * xml_multi_geometry = doc.NewElement("MultiGeometry");
				if (!xml_multi_geometry) goto error0;
				placemark_node->InsertEndChild(xml_multi_geometry);

				tinyxml2::XMLElement * point_node = doc.NewElement("Point");
				if (!point_node) goto error0;
				xml_multi_geometry->InsertEndChild(point_node);

				tinyxml2::XMLElement * altitude_mode_node = doc.NewElement("altitudeMode");
				if (!altitude_mode_node) goto error0;
				altitude_mode_node->InsertEndChild(doc.NewText("absolute"));
				point_node->InsertEndChild(altitude_mode_node);

				tinyxml2::XMLElement * coordinates_node = doc.NewElement("coordinates");
				if (!coordinates_node) goto error0;
				point_node->InsertEndChild(coordinates_node);

				std::stringstream sstr;
				sstr<< std::setprecision(15)<< gps_pt.x<<", "<<gps_pt.y<<", "<<gps_pt.z;
				coordinates_node->InsertEndChild(doc.NewText(sstr.str().c_str()));
			}


			if (doc.SaveFile(kml_file.c_str())  != tinyxml2::XML_NO_ERROR)
			{
				std::cout<<"xml savefile failed."<<std::endl;
				break;
			}

			ret = 0;
		} while (0);
error0:

		return ret;
	}

}