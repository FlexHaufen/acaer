/**
 * @file World.h
 * @author flexhaufen
 * @brief World
 * @version 0.1
 * @date 2023-03-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"

#include "acaer/Scene/World/Cell.h"

//*** DEFINES ***
#define AC_WORLD_DEFAULT_SCALE 1


//*** NAMESPACE ***
namespace Acaer {
    
    /**
     * @brief World for Game
     *        cellular automata
     */
    class World {
    public:

        /**
         * @brief Construct a new World object
         * 
         * @param width 
         * @param height 
         */
        World(size_t width, size_t height)
            : m_width(width)
            , m_height(height) {
            m_cells = new Cell[m_width * m_height];
        }

        ~World() { delete[] m_cells; }

        /**
         * @brief Get a cell from world
         * 
         * @param index 
         * @return const Cell& 
         */
        const Cell& GetCell(size_t index)       { return m_cells[index]; }

        /**
         * @brief Get a cell from world
         * 
         * @param x     pos x
         * @param y     pos y
         * @return const Cell& 
         */
        const Cell& GetCell(size_t x, size_t y) { return GetCell(GetIndex(x, y)); }

        /**
         * @brief Get index of cell for given pos
         * 
         * @param x     pos x
         * @param y     pos y
         * @return size_t index
         */
        size_t GetIndex(size_t x, size_t y);

        bool InBounds(size_t x, size_t y) { return x < m_width && y < m_height; }
	    bool IsEmpty (size_t x, size_t y) { return InBounds(x, y) && GetCell(x, y).type == CellType::EMPTY; }
        void SetCell(size_t x, size_t y, const Cell& cell) { m_cells[GetIndex(x, y)] = cell; }

        void MoveCell(size_t x, size_t y, size_t xto, size_t yto);
        bool CanMoveDown(size_t x, size_t y, const Cell& cell);
	    bool CanMoveDownSide(size_t x, size_t y, const Cell& cell);

        void CommitCells();

        void OnUpdate();
        void OnRender(sf::RenderWindow &window);

    private:
        //** Members **
        const size_t m_width  = 0;  // width of world
        const size_t m_height = 0;  // height of world
        const f64    m_scale = AC_WORLD_DEFAULT_SCALE;   // scale

        Cell* m_cells = nullptr;    // pointer to cell Array
        std::vector<std::pair<size_t, size_t>> m_changes;   // dst, src
    };
}