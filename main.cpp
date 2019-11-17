#include <iostream>
#include "png_toolkit.h"
#include "Filters.h"
#include "Manager.h"

int main( int argc, char *argv[] )
{
    // toolkit filter_name base_pic_name sudent_tool student_pic_name limitPix limitMSE
    // toolkit near test images!
    try
    {
        if (argc != 4)
            throw "Not enough arguments";

		Manager manager;

		manager.Parser(argv[1]);
		manager.MakeFilters();
		manager.Run(argv[2], argv[3]);

    }
    catch (const char *str)
    {
        std::cout << "Error: " << str << std::endl;
        return 1;
    }

    return 0;
}
