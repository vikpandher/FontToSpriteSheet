// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include <unordered_map>



namespace ftss
{
    struct GlyphMetrics
    {
        bool operator==(const GlyphMetrics& other) const;
        bool operator!=(const GlyphMetrics& other) const;

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

    inline bool GlyphMetrics::operator==(const GlyphMetrics& other) const
    {
        return width_px == other.width_px &&
            height_px == other.height_px &&
            horiBearingX_px == other.horiBearingX_px &&
            horiBearingY_px == other.horiBearingY_px &&
            horiAdvance_px == other.horiAdvance_px &&
            vertBearingX_px == other.vertBearingX_px &&
            vertBearingY_px == other.vertBearingY_px &&
            vertAdvance_px == other.vertAdvance_px &&
            textureLeft == other.textureLeft &&
            textureRight == other.textureRight &&
            textureBottom == other.textureBottom &&
            textureTop == other.textureTop;
    }

    inline bool GlyphMetrics::operator!=(const GlyphMetrics& other) const
    {
        return !(*this == other);
    }

    struct FontData
    {
        FontData();

        void Clear();

        bool operator==(const FontData& other) const;
        bool operator!=(const FontData& other) const;

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

    inline bool FontData::operator==(const FontData& other) const
    {
        if (lineSpacing_px != other.lineSpacing_px)
        {
            return false;
        }
        if (glyphMetricsMap.size() != other.glyphMetricsMap.size())
        {
            return false;
        }
        for (const auto& pair : glyphMetricsMap)
        {
            auto it = other.glyphMetricsMap.find(pair.first);
            if (it == other.glyphMetricsMap.end() || !(pair.second == it->second))
            {
                return false;
            }
        }
        return true;
    }

    inline bool FontData::operator!=(const FontData& other) const
    {
        return !(*this == other);
    }
}