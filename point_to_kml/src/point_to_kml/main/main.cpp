#include <iostream>

#include <gflags/gflags.h>

#include "converter/pos_kml_converter.hpp"

DEFINE_string(i,     "",       "* point file, format: name x y");
DEFINE_string(o,     "",       "* output kml file");
DEFINE_string(proj_cmd,     "",       "* coordinate system convert command; proj4");

inline void EnableMemLeakCheck(void)
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(423);
}

void main(int argc, char ** argv)
{
	EnableMemLeakCheck();

	google::ParseCommandLineFlags(&argc, &argv, true);

	do 
	{
		if (FLAGS_i.empty())
		{
			std::cout<<"error: invalid input"<<std::endl;
			break;
		}

		if (FLAGS_o.empty())
		{
			std::cout<<"error: invalid output"<<std::endl;
			break;
		}

		std::cout<<"input: "<<FLAGS_i<<std::endl
			<<"output: "<<FLAGS_o<<std::endl
			<<"proj_cmd: "<<FLAGS_proj_cmd<<std::endl;

// 		cvt::PosKmlConverter cvt;
// 		cvt.SetInputPath(FLAGS_input_path);
// 		if (cvt.Convert(FLAGS_kml_file, option,
// 			FLAGS_img_width, FLAGS_img_height, focal_in_pixel,
// 			FLAGS_ground_elev))
// 		{
// 			std::cout<<"Convert failed."<<std::endl;
// 		}
	} while (0);

	std::cout<<"photo filtering."<<std::endl;
}