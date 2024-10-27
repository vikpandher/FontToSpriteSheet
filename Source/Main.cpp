// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#include "FontToSpriteSheet.h"

#include <iostream>
#include <sys/stat.h>



int CompareStrings(const char* string1, const char* string2);
bool FileExists(const std::string& filePath);
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
        std::cout << "    This program converts a true type font (.ttf) file and a list of characters" << std::endl;
        std::cout << "    (.txt) into a font sprite sheet. The dont sprite sheet is comprized of a" << std::endl;
        std::cout << "    portable network graphics (.png) file and a sprite sheet font discription file." << std::endl;
        std::cout << std::endl;
        std::cout << "Parameters:" << std::endl;
        std::cout << "    <size>                  Font height in pixels" << std::endl;
        std::cout << "    <horizontal_spacing>    Horizontal spacing between glpyh sprites" << std::endl;
        std::cout << "    <vertical_spacing>      Vertical spacing between glpyh sprites" << std::endl;
        std::cout << "    <input_file_1>          The true type font file (.ttf)" << std::endl;
        std::cout << "    <input_file_2>          The file with a list of characters (.txt)" << std::endl;
        std::cout << "    <output_file_1>         The portable network graphics file (.png)" << std::endl;
        std::cout << "    <output_file_2>         The sprite sheet font discription file" << std::endl;
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

    if (font_size == 0)
    {
        std::cerr << "ERROR: argv[1] cannot be 0" << std::endl;
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

    if (!FileExists(input_file_1))
    {
        std::cerr << "ERROR: argv[4], file doesn't exist" << std::endl;
        return 1;
    }

    if (!FileExists(input_file_2))
    {
        std::cerr << "ERROR: argv[5], file doesn't exist" << std::endl;
        return 1;
    }

    // if (FileExists(output_file_1))
    // {
    //     std::cerr << "ERROR: argv[6], file already exists" << std::endl;
    //     return 1;
    // }

    // if (FileExists(output_file_2))
    // {
    //     std::cerr << "ERROR: argv[7], file already exists" << std::endl;
    //     return 1;
    // }

    std::vector<unsigned char> characterList;
    if (!ftss::LoadCharacterListFromFile(characterList, input_file_2))
    {
        std::cerr << "ERROR: loading character list failed" << std::endl;
        return 1;
    }

    ftss::TextureData textureData;
    ftss::FontData fontData;
    if (!ftss::LoadTextureDataAndFontData(
        textureData,
        fontData,
        characterList,
        input_file_1,
        font_size,
        horizontal_spacing,
        vertical_spacing))
    {
        std::cerr << "ERROR: loading texture data and font data failed" << std::endl;
        return 1;
    }

    if (!ftss::WriteTextureData(textureData, output_file_1))
    {
        std::cerr << "ERROR: writing texture data failed" << std::endl;
        return 1;
    }

    if (!ftss::WriteFontData(fontData, output_file_2))
    {
        std::cerr << "ERROR: writing font data failed" << std::endl;
        return 1;
    }

    ftss::FontData fontData_copy;
    if (!ftss::ReadFontData(fontData_copy, output_file_2))
    {
        std::cerr << "ERROR: reading font data failed" << std::endl;
        return 1;
    }

    if (fontData != fontData_copy)
    {
        std::cerr << "ERROR: font data file check failed" << std::endl;
        return 1;
    }

    std::cout << "Successfully generated " << output_file_1 << " and " << output_file_2 << std::endl;

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

bool FileExists(const std::string& filePath)
{
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
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