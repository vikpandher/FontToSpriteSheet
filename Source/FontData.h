// =============================================================================
// @BRIEF 
// @AUTHOR Vik Pandher
// @DATE 

#pragma once

#include <unordered_map>



namespace ftss
{
    struct GlyphMetrics
    {
        unsigned int width_px;
        unsigned int height_px;
        unsigned int horiBearingX_px;
        unsigned int horiBearingY_px;
        unsigned int horiAdvance_px;
        unsigned int vertBearingX_px;
        unsigned int vertBearingY_px;
        unsigned int vertAdvance_px;

        float textureLeft;
        float textureRight;
        float textureBottom;
        float textureTop;
    };

    struct FontData
    {
        FontData();

        void Clear();

        unsigned int lineSpacing_px;
        std::unordered_map<unsigned char, GlyphMetrics> glyphMetricsMap;
    };

    inline FontData::FontData()
        : lineSpacing_px(0)
    {}

    inline void FontData::Clear()
    {
        glyphMetricsMap.clear();
    }
}