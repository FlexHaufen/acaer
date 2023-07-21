/**
 * @file SandWorld.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-07-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/World/Cell.h"

#include "acaer/Helper/Util/Math.h"


//*** DEFINES ***
#define SAND_WORLD_SIZE_X   64
#define SAND_WORLD_SIZE_Y   64

//*** NAMESPACE ***
namespace Acaer {


    class SandWorld {
    public:

        SandWorld() {}
        ~SandWorld() {}

        size_t GetIndex(size_t x, size_t y) { return (x + y * SAND_WORLD_SIZE_X); }

        // Get & Set Cell
        const Cell& GetCell(size_t idx) { return m_Cells[idx]; }
        const Cell& GetCell(size_t x, size_t y) { return GetCell(GetIndex(x, y)); }
        const std::array<Cell, SAND_WORLD_SIZE_X * SAND_WORLD_SIZE_X>& GetCellArray() { return m_Cells; };

        void SetCell(size_t x, size_t y, const Cell &cell) { m_Cells[GetIndex(x, y)] = cell; }

        // Move
        void MoveCell(size_t x, size_t y, size_t to_x, size_t to_y) { m_ChangedCells.emplace_back(GetIndex(to_x, to_y), GetIndex(x, y)); }
        void CommitCells() {
            // remove moves with filled destination
            for (size_t i = 0; i < m_ChangedCells.size();) {
                if (m_Cells[m_ChangedCells[i].first].type != CellType::EMPTY) {
                    std::swap(m_ChangedCells[i], m_ChangedCells.back());
                    m_ChangedCells.pop_back();
                } else {
                    i++;
                }
            }

            // sort by destination
            std::sort(m_ChangedCells.begin(), m_ChangedCells.end(), [&](auto a, auto b) {
                return (a.first < b.first);
            });

            // pick random source for each destination
            size_t iprev = 0;
            m_ChangedCells.emplace_back(-1, -1); // to catch final move
            for (size_t i = 0; i < m_ChangedCells.size() - 1; i++) {
                if (m_ChangedCells[i + 1].first != m_ChangedCells[i].first) {
                    size_t rand = iprev + Math::rand_u(i - iprev);

                    size_t dst = m_ChangedCells[rand].first;
                    size_t src = m_ChangedCells[rand].second;

                    m_Cells[dst] = m_Cells[src];
                    m_Cells[src] = Cell();

                    iprev = i + 1;
                }
            }
            m_ChangedCells.clear();
        }
 
        bool MoveDown(size_t x, size_t y, const Cell& cell) {
            for (int i = 1; i > 0; i--) {
                bool down = IsEmpty(x, y - i);
                if (down) {
                    MoveCell(x, y, x, y - i);
                    return true;
                }
            }
            return false;
        }
            
        bool MoveDownSide(size_t x, size_t y, const Cell& cell) {
            bool downLeft  = IsEmpty(x - 1, y - 1);
            bool downRight = IsEmpty(x + 1, y - 1);

            //ShuffleIfTrue(downLeft, downRight);

            if		(downLeft)	MoveCell(x, y, x - 1, y - 1);
            else if (downRight) MoveCell(x, y, x + 1, y - 1);

            return downLeft || downRight;
        }

        bool MoveSide(size_t x, size_t y, const Cell& cell) {
            for (int i = 1; i > 0; i--) {
                bool left  = IsEmpty(x - i, y);
                bool right = IsEmpty(x + i, y);

                //ShuffleIfTrue(left, right);

                    if (left)  MoveCell(x, y, x - i, y);
                else if (right)	MoveCell(x, y, x + i, y);

                if (left || right) return true;
            }
            return false;
        }

        //void OnUpdate() {
        //    for (size_t x = 0; x < SAND_WORLD_SIZE_X;  x++) {
		//        for (size_t y = 0; y < SAND_WORLD_SIZE_Y; y++) {
        //            const Cell &cell = GetCell(x, y);
        //            if      (cell.props & CellProperties::MOVE_DOWN      && MoveDown    (x, y, cell)) {}
        //            else if (cell.props & CellProperties::MOVE_DOWN_SIDE && MoveDownSide(x, y, cell)) {}
        //            else if (cell.props & CellProperties::MOVE_SIDE      && MoveSide    (x, y, cell)) {}
        //        }
        //    }
        //}

        // Helper functions
        b8 IsInBounds(size_t x, size_t y) { return (x < SAND_WORLD_SIZE_X && y < SAND_WORLD_SIZE_Y); }
        b8 IsEmpty(size_t x, size_t y) { return (IsInBounds(x, y) && GetCell(x, y).type == CellType::EMPTY); }

    private:
        // ** Members **

        std::array<Cell, SAND_WORLD_SIZE_X * SAND_WORLD_SIZE_X> m_Cells;
        std::vector<std::pair<size_t, size_t>> m_ChangedCells;

    };
}