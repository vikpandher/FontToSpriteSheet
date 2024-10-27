// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "FontData.h"
#include "TextureData.h"

#include <string>
#include <vector>



typedef struct FT_FaceRec_* FT_Face;

namespace ftss
{
    bool LoadCharacterListFromFile(
        std::vector<unsigned char>& characterList,
        const std::string& filePath);

    bool LoadTextureDataAndFontData(
        TextureData& textureData,
        FontData& fontData,
        const std::vector<unsigned char>& characterList,
        const std::string& filePath,
        unsigned int fontHeightInPixels = 48,
        unsigned int horizontalSpacing = 1,
        unsigned int verticalSpacing = 1);

    bool LoadTextureDataAndFontDataFromMemory(
        TextureData& textureData,
        FontData& fontData,
        const std::vector<unsigned char>& characterList,
        const unsigned char* dataPtr,
        size_t dataSize,
        unsigned int fontHeightInPixels = 48,
        unsigned int horizontalSpacing = 1,
        unsigned int verticalSpacing = 1);

    bool WriteTextureData(
        const TextureData& textureData,
        const std::string& filePath);

    bool WriteFontData(
        const FontData& fontData,
        const std::string& filePath);

    bool ReadFontData(
        FontData& fontData,
        const std::string& filePath);

    // Used in LoadTextureDataAndFontData and LoadTextureDataAndFontDataFromMemory
    bool LoadTextureDataAndFontData_H(
        TextureData& textureData,
        FontData& fontData,
        const std::vector<unsigned char>& characterList,
        FT_Face face,
        unsigned int fontHeightInPixels = 48,
        unsigned int horizontalSpacing = 1,
        unsigned int verticalSpacing = 1);

    // Used in LoadTextureDataAndFontData_H
    unsigned int GetTextureIndex_H(
        unsigned int x,
        unsigned int xOffset,
        unsigned int textureWidth,
        unsigned int y,
        unsigned int yOffset,
        unsigned int textureHeight);

    static bool s_textureFlippedVertically = false;
    static bool s_textureCoordinatesFlippedVertically = true;
}