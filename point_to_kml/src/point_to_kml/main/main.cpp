#include <iostream>

#include <gflags/gflags.h>

#include "converter/pos_kml_converter.hpp"

DEFINE_string(i,     "",       "* point file, format: name x y z | format: name longitude latitude altitude)");
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
			std::cout<<FLAGS_i<<std::endl;
			break;
		}

		if (FLAGS_o.empty())
		{
			std::cout<<"error: invalid output"<<std::endl;
			std::cout<<FLAGS_o<<std::endl;
			break;
		}

		std::cout<<"input: "<<FLAGS_i<<std::endl
			<<"output: "<<FLAGS_o<<std::endl
			<<"proj_cmd: "<<FLAGS_proj_cmd<<std::endl;

		cvt::PosKmlConverter cvt(FLAGS_i);
		if (cvt.Convert(FLAGS_o, FLAGS_proj_cmd))
		{
			std::cout<<"Convert failed."<<std::endl;
		}
	} while (0);

	std::cout<<"photo filtering."<<std::endl;
}