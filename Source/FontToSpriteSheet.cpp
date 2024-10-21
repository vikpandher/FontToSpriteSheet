// =============================================================================
// @BRIEF 
// @AUTHOR Vik Pandher
// @DATE 

#include "FontToSpriteSheet.h"

// http://freetype.sourceforge.net/freetype2/docs/reference/ft2-index.html
#include "ft2build.h"
#include FT_FREETYPE_H

#include <iostream>



namespace ftss
{
    // public ------------------------------------------------------------------

    bool FontToSpriteSheet::LoadTextureDataAndFontData(
        TextureData& textureData,
        FontData& fontData,
        const std::vector<unsigned char>& characterList,
        const std::string& filePath,
        unsigned int fontHeightInPixels,
        unsigned int horizontalSpacing,
        unsigned int verticalSpacing)
    {
        FT_Library library;
        FT_Error error = FT_Init_FreeType(&library);
        if (error)
        {
            std::cerr << "ERROR: could not initalize the FreeType library" << std::endl;

            return false;
        }

        FT_Face face;
        error = FT_New_Face(library, filePath.c_str(), 0, &face);
        if (error) {
            std::cerr << "ERROR: failed to load the font" << std::endl;

            return false;
        }

        return LoadTextureDataAndFontData_H(
            textureData,
            fontData,
            characterList,
            face,
            fontHeightInPixels,
            horizontalSpacing,
            verticalSpacing
        );
    }

    bool FontToSpriteSheet::LoadTextureDataAndFontDataFromMemory(
        TextureData& textureData,
        FontData& fontData,
        const std::vector<unsigned char>& characterList,
        const unsigned char* dataPtr,
        size_t dataSize,
        unsigned int fontHeightInPixels,
        unsigned int horizontalSpacing,
        unsigned int verticalSpacing)
    {
        FT_Library library;
        FT_Error error = FT_Init_FreeType(&library);
        if (error)
        {
            std::cerr << "ERROR: could not initalize the FreeType library" << std::endl;

            return false;
        }

        FT_Face face;
        error = FT_New_Memory_Face(library, dataPtr, dataSize, 0, &face);
        if (error) {
            std::cerr << "ERROR: failed to load the font" << std::endl;

            return false;
        }

        return LoadTextureDataAndFontData_H(
            textureData,
            fontData,
            characterList,
            face,
            fontHeightInPixels,
            horizontalSpacing,
            verticalSpacing
        );
    }

    // protected ---------------------------------------------------------------

    bool FontToSpriteSheet::LoadTextureDataAndFontData_H(
        TextureData& textureData,
        FontData& fontData,
        const std::vector<unsigned char>& characterList,
        FT_Face face,
        unsigned int fontHeightInPixels,
        unsigned int horizontalSpacing,
        unsigned int verticalSpacing)
    {
        FT_Error error = FT_Set_Pixel_Sizes(face, 0, fontHeightInPixels);
        if (error)
        {
            std::cerr << "ERROR: could not set font pixel sizes" << std::endl;

            return false;
        }

        std::unordered_map<unsigned char, GlyphMetrics>& glyphMetricsMap = fontData.glyphMetricsMap;

        for (size_t i = 0; i < characterList.size(); ++i)
        {
            const unsigned char& c = characterList[i];
            if (fontData.glyphMetricsMap.find(c) != fontData.glyphMetricsMap.end())
            {
                std::cout << "WARNING: reloading character glyph for: " << c << std::endl;
            }
            else
            {
                glyphMetricsMap[c];
            }
        }

        const unsigned int METRICS_UNIT_MULTIPLIER = 64; // because values are expressed in 26.6 fractional pixel format
        fontData.lineSpacing_px = face->size->metrics.height / METRICS_UNIT_MULTIPLIER;

        // load characters first time to calculate the size of the texture
        for (std::unordered_map<unsigned char, GlyphMetrics>::iterator iter = glyphMetricsMap.begin(); iter != glyphMetricsMap.end(); ++iter)
        {
            const unsigned char& c = iter->first;
            error = FT_Load_Char(face, c, FT_LOAD_RENDER);
            if (error)
            {
                std::cerr << "ERROR: could not load character glyph for: " << c << std::endl;

                return false;
            }

            unsigned char& pixelMode = face->glyph->bitmap.pixel_mode;
            if (pixelMode != FT_Pixel_Mode::FT_PIXEL_MODE_GRAY)
            {
                std::cerr << "ERROR: glyph pixel mode not supported" << std::endl;

                return false;
            }

            unsigned int& currentBitmapHeight = face->glyph->bitmap.rows;
            unsigned int& currentBitmapWidth = face->glyph->bitmap.width;
            if (currentBitmapHeight + verticalSpacing * 2 > textureData.height)
            {
                textureData.height = currentBitmapHeight + verticalSpacing * 2;
            }
            textureData.width += currentBitmapWidth + horizontalSpacing;
        }
        textureData.width += horizontalSpacing;

        free(textureData.data);
        textureData.data = (unsigned char*)malloc(sizeof(unsigned char*) * textureData.height * textureData.width * 4);
        if (textureData.data == nullptr)
        {
            std::cerr << "ERROR: memory allocation failed" << std::endl;

            return false;
        }

        // top side spacing
        for (unsigned int j = 0; j < verticalSpacing; ++j)
        {
            for (unsigned int i = 0; i < textureData.width; ++i)
            {
                unsigned int textureDataIndex = GetTextureIndex_H(
                    i,
                    0,
                    textureData.width,
                    j,
                    0,
                    textureData.height);

                textureData.data[textureDataIndex] = 0;
                textureData.data[textureDataIndex + 1] = 0;
                textureData.data[textureDataIndex + 2] = 0;
                textureData.data[textureDataIndex + 3] = 0;
            }
        }

        unsigned int nextCharacterOffsetX = 0;
        unsigned int nextCharacterOffsetY = verticalSpacing;

        for (std::unordered_map<unsigned char, GlyphMetrics>::iterator iter = glyphMetricsMap.begin(); iter != glyphMetricsMap.end(); ++iter)
        {
            // left side spacing per glyph
            for (unsigned int j = 0; j < textureData.height - verticalSpacing; ++j)
            {
                for (unsigned int i = 0; i < horizontalSpacing; ++i)
                {
                    unsigned int textureDataIndex = GetTextureIndex_H(
                        i,
                        nextCharacterOffsetX,
                        textureData.width,
                        j,
                        nextCharacterOffsetY,
                        textureData.height);

                    textureData.data[textureDataIndex] = 0;
                    textureData.data[textureDataIndex + 1] = 0;
                    textureData.data[textureDataIndex + 2] = 0;
                    textureData.data[textureDataIndex + 3] = 0;
                }
            }
            nextCharacterOffsetX += horizontalSpacing;

            const unsigned char& c = iter->first;
            error = FT_Load_Char(face, c, FT_LOAD_RENDER);
            if (error)
            {
                std::cerr << "ERROR: could not load character glyph for: " << c << std::endl;

                return false;
            }

            GlyphMetrics& glyphMetrics = iter->second;
            glyphMetrics.height_px = face->glyph->bitmap.rows;
            glyphMetrics.width_px = face->glyph->bitmap.width;
            glyphMetrics.horiBearingX_px = face->glyph->metrics.horiBearingX / METRICS_UNIT_MULTIPLIER;
            glyphMetrics.horiBearingY_px = face->glyph->metrics.horiBearingY / METRICS_UNIT_MULTIPLIER;
            glyphMetrics.horiAdvance_px = face->glyph->metrics.horiAdvance / METRICS_UNIT_MULTIPLIER;
            glyphMetrics.vertBearingX_px = face->glyph->metrics.vertBearingX / METRICS_UNIT_MULTIPLIER;
            glyphMetrics.vertBearingY_px = face->glyph->metrics.vertBearingY / METRICS_UNIT_MULTIPLIER;
            glyphMetrics.vertAdvance_px = face->glyph->metrics.vertAdvance / METRICS_UNIT_MULTIPLIER;
            glyphMetrics.textureLeft = (float)nextCharacterOffsetX / (float)textureData.width;
            glyphMetrics.textureRight = (float)(nextCharacterOffsetX + glyphMetrics.width_px) / (float)textureData.width;
            if (s_textureFlippedVerticaly)
            {
                glyphMetrics.textureBottom = (float)(textureData.height - glyphMetrics.height_px - nextCharacterOffsetY) / (float)textureData.height;
                glyphMetrics.textureTop = (float)(textureData.height - nextCharacterOffsetY) / (float)textureData.height;
            }
            else
            {
                glyphMetrics.textureBottom = (float)(glyphMetrics.height_px + nextCharacterOffsetY) / (float)textureData.height;
                glyphMetrics.textureTop = (float)(nextCharacterOffsetY) / (float)textureData.height;
            }

            for (unsigned int j = 0; j < glyphMetrics.height_px; ++j)
            {
                for (unsigned int i = 0; i < glyphMetrics.width_px; ++i)
                {
                    unsigned int textureDataIndex = GetTextureIndex_H(
                        i,
                        nextCharacterOffsetX,
                        textureData.width,
                        j,
                        nextCharacterOffsetY,
                        textureData.height);

                    textureData.data[textureDataIndex] = 255;
                    textureData.data[textureDataIndex + 1] = 255;
                    textureData.data[textureDataIndex + 2] = 255;
                    unsigned int sourceIndex = i + j * glyphMetrics.width_px;
                    textureData.data[textureDataIndex + 3] = face->glyph->bitmap.buffer[sourceIndex];
                }
            }

            // spacing below glyph
            for (unsigned int j = glyphMetrics.height_px; j < textureData.height - verticalSpacing; ++j)
            {
                for (unsigned int i = 0; i < glyphMetrics.width_px; ++i)
                {
                    unsigned int textureDataIndex = GetTextureIndex_H(
                        i,
                        nextCharacterOffsetX,
                        textureData.width,
                        j,
                        nextCharacterOffsetY,
                        textureData.height);

                    textureData.data[textureDataIndex] = 0;
                    textureData.data[textureDataIndex + 1] = 0;
                    textureData.data[textureDataIndex + 2] = 0;
                    textureData.data[textureDataIndex + 3] = 0;
                }
            }

            nextCharacterOffsetX += glyphMetrics.width_px;
        }

        // right side spacing
        for (unsigned int j = 0; j < textureData.height - verticalSpacing; ++j)
        {
            for (unsigned int i = 0; i < horizontalSpacing; ++i)
            {
                unsigned int textureDataIndex = GetTextureIndex_H(
                    i,
                    nextCharacterOffsetX,
                    textureData.width,
                    j,
                    nextCharacterOffsetY,
                    textureData.height);

                textureData.data[textureDataIndex] = 0;
                textureData.data[textureDataIndex + 1] = 0;
                textureData.data[textureDataIndex + 2] = 0;
                textureData.data[textureDataIndex + 3] = 0;
            }
        }

        // bottom side spacing
        for (unsigned int j = 0; j < verticalSpacing; ++j)
        {
            for (unsigned int i = 0; i < textureData.width; ++i)
            {
                unsigned int textureDataIndex = GetTextureIndex_H(
                    i,
                    0,
                    textureData.width,
                    j,
                    textureData.height - verticalSpacing,
                    textureData.height);

                textureData.data[textureDataIndex] = 0;
                textureData.data[textureDataIndex + 1] = 0;
                textureData.data[textureDataIndex + 2] = 0;
                textureData.data[textureDataIndex + 3] = 0;
            }
        }

        textureData.bytesPerPixel = 4;

        return true;
    }

    unsigned int FontToSpriteSheet::GetTextureIndex_H(
        unsigned int x,
        unsigned int xOffset,
        unsigned int textureWidth,
        unsigned int y,
        unsigned int yOffset,
        unsigned int textureHeight)
    {
        unsigned int textureIndex;
        if (s_textureFlippedVerticaly)
        {
            textureIndex = (x + xOffset + (textureHeight - 1 - y - yOffset) * textureWidth) * 4;
        }
        else
        {
            textureIndex = (x + xOffset + (y + yOffset) * textureWidth) * 4;
        }
        return textureIndex;
    }

    bool FontToSpriteSheet::s_textureFlippedVerticaly = true;
}