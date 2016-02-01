#pragma once

#include <string>
#include <vector>

#include <Eigen/Core>

namespace cvt
{
	class PosKmlConverter
	{
	public:
		typedef enum
		{
			OO_NONE = 0,
			OO_POINT = 1,
			OO_PROJ_IMAGE = 2,
			OO_LINE_STRING = 4
		} OutputOption;

		typedef struct POSPoint 
		{
			std::string name;

			double x, y, z;
		} POSPoint;

	public:
		PosKmlConverter();
		PosKmlConverter(const std::string & in_file);
		~PosKmlConverter();

		int SetInputPath(const std::string & input_path);

		int Convert(const std::string & kml_file,
			std::string proj_cmd = "");

	protected:
		int load_pos(std::string proj_cmd = "");
		
	private:
		std::string _in_file;

		std::vector<POSPoint> _gps_points;
	};
}