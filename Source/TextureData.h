// =============================================================================
// @BRIEF 
// @AUTHOR Vik Pandher
// @DATE 

#pragma once

#include <cstdlib>



namespace ftss
{
    struct TextureData
    {
        TextureData();

        ~TextureData();

        void Clear();
        
        unsigned char* data;
        unsigned int width;
        unsigned int height;
        unsigned int bytesPerPixel;
    };

    inline TextureData::TextureData()
        : data(nullptr)
        , width(0)
        , height(0)
        , bytesPerPixel(0)
    {}

    inline TextureData::~TextureData()
    {
        free(data);
        data = nullptr;
    }

    inline void TextureData::Clear()
    {
        free(data);
        data = nullptr;
        width = 0;
        height = 0;
        bytesPerPixel = 0;
    }
}