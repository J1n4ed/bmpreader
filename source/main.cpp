#include <string>
#include <iostream>
#include "../headers/reader.h"

int main (int argc, char ** argv)
{
    // usage 
    // ./program <sample file path>

    std::string path;    

    if (argc != 2)
    {
        std::cerr << "\nWrong number of arguments!" << std::endl;
    }
    else
    {
        for (int i = 0; i < argc; ++i)
        {
            std::cout << argv[i] << '\n';
        }

        std::string path = argv[1];

        std::cout << "Path argument is: " << path;

        Bitmap picture (&path);
        picture.paint();
    }

    std::cout << "\nAny key to continue...\n";
    std::cin.get();
    return 0;
}