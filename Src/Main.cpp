// =============================================================================
// @FILE
// @BRIEF 
// @AUTHOR Vik Pandher
// @DATE 

#include <iostream>



int CompareStrings(const char* string1, const char* string2);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "ERROR: Not enough arguments" << std::endl;
        std::cerr << "    Try /? or /help" << std::endl;

        return 1;
    }

    if (CompareStrings(argv[1], "/?") == 0 ||
        CompareStrings(argv[1], "/help") == 0)
    {
        if (argc > 2)
        {
            std::cerr << "ERROR: Too many arguments" << std::endl;
            std::cerr << "    Try /? or /help" << std::endl;

            return 1;
        }

        std::cout << "Usage:" << std::endl;
        std::cout << "    ./" << PROJECT_NAME << " <size> <horizontal_spacing> <vertical_spacing> <input_file> <output_file_1> <output_file_2>" << std::endl;
        std::cout << std::endl;
        std::cout << "Description:" << std::endl;
        std::cout << "    TODO" << std::endl;

        // TODO: add help description

        return 0;
    }

    if (argc < 7)
    {
        std::cerr << "ERROR: Not enough arguments" << std::endl;
        std::cerr << "    Try /? or /help" << std::endl;

        return 1;
    }

    if (argc > 7)
    {
        std::cerr << "ERROR: Too many arguments" << std::endl;
        std::cerr << "    Try /? or /help" << std::endl;

        return 1;
    }

    int font_size = std::atoi(argv[1]); // returns 0 if the conversion fails
    if (font_size < 0)
    {
        font_size = 0;
    }

    // TODO: process arguments

    return 0;
}

int CompareStrings(const char* string1, const char* string2)
{
    while (*string1 != '\0' && *string2 != '\0' && *string1 == *string2)
    {
        string1++;
        string2++;
    }

    if (*string1 == '\0' && *string2 == '\0') {
        return 0; // Strings are equal
    }
    else {
        return *string1 - *string2; // Return the ASCII difference
    }
}