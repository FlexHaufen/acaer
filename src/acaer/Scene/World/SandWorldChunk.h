/**
 * @file SandWorldChunk.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-07-21
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
#define SAND_WORLD_CHUNK_SIZE_X   64
#define SAND_WORLD_CHUNK_SIZE_Y   64

#define PAIR    2

//*** NAMESPACE ***
namespace Acaer {


    class SandWorldChunk {
    public:

        /**
         * @brief Construct a new SandWorldChunk
         * 
         * @param x         pos x   [chunk Position]
         * @param y         pos y   [chunk Position]
         */
        SandWorldChunk(s32 x, s32 y) 
        : m_PosWorld({x * SAND_WORLD_CHUNK_SIZE_X, y * SAND_WORLD_CHUNK_SIZE_Y})
        , m_FilledCellCount(0) {
            UpdateRect();
        }

        // ** Getter & Setter **

        /**
         * @brief Get Index of given position
         * 
         * @param x         pos x   [world Position]
         * @param y         pos y   [world Position]
         * @return size_t   index
         */
        size_t GetIndex(s32 x, s32 y) { return ((x - m_PosWorld.x) + (y - m_PosWorld.y) * SAND_WORLD_CHUNK_SIZE_X); }

        /**
         * @brief Get Cell at given position
         * 
         * @param x         pos x   [world Position]
         * @param y         pos y   [world Position]
         * @return Cell&    Cell
         */
        Cell& GetCell(s32 x, s32 y) { return GetCell(GetIndex(x, y)); }

        /**
         * @brief Get Cell at given index
         * 
         * @param idx       index   
         * @return Cell&    Cell
         */
        Cell& GetCell(size_t idx) { 
            if (idx >= 0 && idx < m_Cells.size()) {
                return m_Cells[idx]; 
            }
            AC_CORE_WARN("GetCell Access Violation at Chunk [{0} / {1}]", m_PosWorld.x, m_PosWorld.y);
            AC_CORE_WARN("    idx was [{0}]. Returning Cell at idx 0", idx);
            return m_Cells[0];
        }

        /**
         * @brief Set Cell at given position
         * 
         * @param x         pos x   [world Position]
         * @param y         pos y   [world Position]
         */
        void SetCell(s32 x, s32 y, const Cell &cell) { SetCell(GetIndex(x, y), cell); }
        
        /**
         * @brief Set Cell at given index
         * 
         * @param idx       index
         */
        void SetCell(size_t idx, const Cell &cell) { 
            if (idx >= 0 && idx < m_Cells.size()) {
                Cell& dest = m_Cells[idx];
                // Fill a Cell
                if (dest.type == CellType::EMPTY && cell.type != CellType::EMPTY) {
                    m_FilledCellCount++;
                }
                // Remove a Cell
		        else if (dest.type != CellType::EMPTY && cell.type == CellType::EMPTY) {
			        m_FilledCellCount--;
		        }
                dest = cell;

                KeepAlive(idx);

                return;
            }
            AC_CORE_WARN("SetCell Access Violation at Chunk [{0} / {1}]", m_PosWorld.x, m_PosWorld.y);
            AC_CORE_WARN("    idx was [{0}]. Returning Cell at idx 0", idx);
            return;
        }

        // ** Helper Functions **
        
        /**
         * @brief Check if Cell is empty at given position
         * 
         * @param x         pos x
         * @param y         pos y
         * @return b8       true if cell is empty
         */
        b8 IsEmpty(s32 x, s32 y) { return IsEmpty(GetIndex(x, y)); }
	    
        /**
         * @brief Check if Cell is empty at given index
         * 
         * @param idx       index
         * @return b8       true if cell is empty
         */
        b8 IsEmpty(size_t idx) { return GetCell(idx).type == CellType::EMPTY; }

        /**
         * @brief Check if cell is in bounds of current chunk
         * 
         * @param x         pos x
         * @param y         pos y
         * @return b8       true if in Bounds of current chunk
         */
        b8 InBounds(s32 x, s32 y) { return (x >= m_PosWorld.x && x < (m_PosWorld.x + SAND_WORLD_CHUNK_SIZE_X) && y >= m_PosWorld.y && y < ( m_PosWorld.y + SAND_WORLD_CHUNK_SIZE_Y)); }

        /**
         * @brief Get position of chunk
         * 
         * @return v2<s32> pos
         */
        v2<s32> GetPos() { return m_PosWorld; }

        /**
         * @brief Get the FilledCellCount
         * 
         * @return size_t count
         */
        size_t GetFilledCellCount() { return m_FilledCellCount; }

        /**
         * @brief Get  ChunkRectMin
         * 
         * @return v2<s32> 
         */
        v2<s32> GetChunkRectMin() { return m_RectMin; }

        /**
         * @brief Get  ChunkRectMax
         * 
         * @return v2<s32> 
         */
        v2<s32> GetChunkRectMax() { return m_RectMax; }

        // ** Updating Cells **

        /**
         * @brief Move Cell to new location
         * 
         * @param src   Source Chunk
         * @param x     current pos x
         * @param y     current pos y
         * @param to_x  new pos x
         * @param to_y  new pos y
         */
        void MoveCell(SandWorldChunk* src, s32 x, s32 y, s32 to_x, s32 to_y) {
            m_ChangedCells.emplace_back(src, src->GetIndex(x, y), GetIndex(to_x, to_y));
        }

        // TODO (flex): Add function description
        void CommitCells() { 
            // remove moves that have their destinations filled
			for (size_t i = 0; i < m_ChangedCells.size(); i++) {
				if (!IsEmpty(std::get<PAIR>(m_ChangedCells[i]))) {
					m_ChangedCells[i] = m_ChangedCells.back();
                    m_ChangedCells.pop_back();
					i--;
				}
			}

			// sort by destination
			std::sort(m_ChangedCells.begin(), m_ChangedCells.end(), [](auto& a, auto& b) { 
                return std::get<PAIR>(a) < std::get<PAIR>(b); 
            });

			// pick random source for each destination
			size_t iprev = 0;
			m_ChangedCells.emplace_back(nullptr, -1, -1); // to catch final move
			for (size_t i = 0; i < m_ChangedCells.size() - 1; i++) {
				if (std::get<PAIR>(m_ChangedCells[i + 1]) != std::get<PAIR>(m_ChangedCells[i])) {
					size_t rand = iprev + Math::rand_u(i - iprev);  // FIXME (flex): Possible BUG
					auto [chunk, src, dst] = m_ChangedCells[rand];
					SetCell(dst, chunk->GetCell(src));
					chunk->SetCell(src, Cell());

					iprev = i + 1;
				}
			}
			m_ChangedCells.clear();
        }


        void KeepAlive(s32 x, s32 y) { KeepAlive(GetIndex(x, y)); }

        void KeepAlive(size_t index) {
            s32 x = (s32)(index % SAND_WORLD_CHUNK_SIZE_X);
            s32 y = (s32)(index / SAND_WORLD_CHUNK_SIZE_X);
    
            m_RectWorkingMin.x = Math::clamp(std::min(x - 2, m_RectWorkingMin.x), 0, SAND_WORLD_CHUNK_SIZE_X);
            m_RectWorkingMin.y = Math::clamp(std::min(y - 2, m_RectWorkingMin.y), 0, SAND_WORLD_CHUNK_SIZE_Y);
            m_RectWorkingMax.x = Math::clamp(std::max(x + 2, m_RectWorkingMax.x), 0, SAND_WORLD_CHUNK_SIZE_X);
            m_RectWorkingMax.y = Math::clamp(std::max(y + 2, m_RectWorkingMax.y), 0, SAND_WORLD_CHUNK_SIZE_Y);
        }
    
        void UpdateRect() {
            // Update current
            m_RectMin = m_RectWorkingMin;
            m_RectMax = m_RectWorkingMax;
            // Reset working
            m_RectWorkingMin = {SAND_WORLD_CHUNK_SIZE_X, SAND_WORLD_CHUNK_SIZE_Y};
            m_RectWorkingMax = {-1, -1};
        }


    private:
        // ** Members **
        const v2<s32> m_PosWorld;   // Chunk pos in world [Cell]

        size_t m_FilledCellCount;   // Number of cells in chunk that are not EMPTY
        
        v2<s32> m_RectMin;          // Dirty Rect
        v2<s32> m_RectMax;          //
        v2<s32> m_RectWorkingMin;   // Working Dirty Rect
        v2<s32> m_RectWorkingMax;   //

        std::array<Cell, SAND_WORLD_CHUNK_SIZE_X * SAND_WORLD_CHUNK_SIZE_Y> m_Cells;
        std::vector<std::tuple<SandWorldChunk*, size_t, size_t>> m_ChangedCells;
    };
}