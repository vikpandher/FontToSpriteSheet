// =============================================================================
// @BRIEF 
// @AUTHOR Vik Pandher
// @DATE 

#include "FontToSpriteSheet.h"

#include <cerrno>
#include <fstream>
#include <iostream>



int CompareStrings(const char* string1, const char* string2);
bool ConvertStringToUnsignedInt(const char* string, unsigned long& result);

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
        std::cout << "    ./" << PROJECT_NAME << " <size> <horizontal_spacing> <vertical_spacing> <input_file_1> <input_file_2> <output_file_1> <output_file_2>" << std::endl;
        std::cout << std::endl;
        std::cout << "Description:" << std::endl;
        std::cout << "    TODO" << std::endl;

        // TODO: add help description

        return 0;
    }

    if (argc < 8)
    {
        std::cerr << "ERROR: Not enough arguments" << std::endl;
        std::cerr << "    Try /? or /help" << std::endl;

        return 1;
    }

    if (argc > 8)
    {
        std::cerr << "ERROR: Too many arguments" << std::endl;
        std::cerr << "    Try /? or /help" << std::endl;

        return 1;
    }

    unsigned long font_size;
    if (!ConvertStringToUnsignedInt(argv[1], font_size))
    {
        std::cerr << "ERROR: argv[1] must be of type unsigned int" << std::endl;

        return 1;
    }

    unsigned long horizontal_spacing;
    if (!ConvertStringToUnsignedInt(argv[2], horizontal_spacing))
    {
        std::cerr << "ERROR: argv[2] must be of type unsigned int" << std::endl;

        return 1;
    }

    unsigned long vertical_spacing;
    if (!ConvertStringToUnsignedInt(argv[3], vertical_spacing))
    {
        std::cerr << "ERROR: argv[3] must be of type unsigned int" << std::endl;

        return 1;
    }

    const char* input_file_1 = argv[4];
    const char* input_file_2= argv[5];
    const char* output_file_1 = argv[6];
    const char* output_file_2 = argv[7];

    if (CompareStrings(input_file_1, input_file_2) == 0)
    {
        std::cerr << "ERROR: argv[4] and argv[5] cannot be the same value" << std::endl;

        return 1;
    }
    
    if (CompareStrings(input_file_1, output_file_1) == 0)
    {
        std::cerr << "ERROR: argv[4] and argv[6] cannot be the same value" << std::endl;

        return 1;
    }
    
    if (CompareStrings(input_file_1, output_file_2) == 0)
    {
        std::cerr << "ERROR: argv[4] and argv[7] cannot be the same value" << std::endl;

        return 1;
    }

    if (CompareStrings(input_file_2, output_file_1) == 0)
    {
        std::cerr << "ERROR: argv[5] and argv[6] cannot be the same value" << std::endl;

        return 1;
    }

    if (CompareStrings(input_file_2, output_file_2) == 0)
    {
        std::cerr << "ERROR: argv[5] and argv[7] cannot be the same value" << std::endl;

        return 1;
    }

    if (CompareStrings(output_file_1, output_file_2) == 0)
    {
        std::cerr << "ERROR: argv[6] and argv[7] cannot be the same value" << std::endl;

        return 1;
    }

    std::ifstream input_file_stream_1(input_file_1);
    if (!input_file_stream_1.is_open())
    {
        std::cerr << "ERROR: argv[4] must be a readable file" << std::endl;

        return 1;
    }

    std::ifstream input_file_stream_2(input_file_2);
    if (!input_file_stream_2.is_open())
    {
        std::cerr << "ERROR: argv[5] must be a readable file" << std::endl;

        return 1;
    }

    std::ofstream output_file_stream_1(output_file_1);
    if (!output_file_stream_1.is_open())
    {
        std::cerr << "ERROR: argv[6], couldn't open output file" << std::endl;

        return 1;
    }

    std::ofstream output_file_stream_2(output_file_2);
    if (!output_file_stream_2.is_open())
    {
        std::cerr << "ERROR: argv[7], couldn't open output file" << std::endl;

        return 1;
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

bool ConvertStringToUnsignedInt(const char* string, unsigned long& result)
{
    errno = 0;

    char* end;
    unsigned long value = std::strtoul(string, &end, 10);

    // Checking for conversion errors
    if (errno == ERANGE)
    {
        std::cerr << "ERROR: Value out of range for unsigned long: " << string << std::endl;
        return false;
    }
    else if (*end != '\0')
    {
        std::cerr << "ERROR: Invalid number: " << string << std::endl;
        return false;
    }
    else if (value > std::numeric_limits<unsigned int>::max())
    {
        std::cerr << "ERROR: Value exceeds unsigned int max: " << string << std::endl;
        return false;
    }

    result = value;
    return true;
}