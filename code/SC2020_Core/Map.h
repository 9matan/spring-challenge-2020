#pragma once

#include <assert.h>

#include "Cell.h"
#include "GameConfig.h"

namespace SC2020
{
    struct SMap
    {
    public:
        SMap()
            : m_width(0)
            , m_height(0)
        {}
        SMap(size_t const width, size_t const height)
            : m_width(width)
            , m_height(height)
        {
            m_cells.resize(width * height);
        }

        // x, y
        inline SCell const& GetCell(SVec2 const pos) const { return m_cells[GetIndex(pos)]; }
        inline SCell& GetCell(SVec2 const pos) { return m_cells[GetIndex(pos)]; }

        inline size_t GetWidth() const { return m_width; }
        inline size_t GetHeight() const { return m_height; }
        inline size_t GetSize() const { return m_cells.size(); }

        inline bool IsValid(SVec2 const pos) const { return pos[0] >= 0 && pos[0] < m_width && pos[1] >= 0 && pos[1] < m_height; }
        inline size_t GetIndex(SVec2 const pos) const { return pos[0] + pos[1] * m_width; }

    private:
        CVectorInPlace<SCell, MAX_MAP_HEIGHT * MAX_MAP_WIDTH> m_cells;
        size_t m_width;
        size_t m_height;
    };
}