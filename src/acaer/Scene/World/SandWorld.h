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
#include "acaer/Scene/World/SandWorldChunk.h"

#include "acaer/Helper/Util/Math.h"


//*** DEFINES ***

//*** NAMESPACE ***
namespace Acaer {


    class SandWorld {
    public:

        // Get & Set
        const Cell& GetCell(s32 x, s32 y) { return GetChunk(x, y)->GetCell(x, y); }

        const auto& GetChunkVector() { return m_chunks; }
        
        void SetCell(s32 x, s32 y, const Cell& cell) {
            if (SandWorldChunk* chunk = GetChunk(x, y)) {
                chunk->SetCell(x, y, cell);
            }
	    } 

        // Move
        bool MoveDown(s32 x, s32 y, const Cell& cell) {
            if (IsEmpty(x, y + 1)) {
                MoveCell(x, y, x, y + 1);
                return true;
            }
            return false;
        }
            
        bool MoveDownSide(s32 x, s32 y, const Cell& cell) {
            bool downLeft  = IsEmpty(x - 1, y + 1);
            bool downRight = IsEmpty(x + 1, y + 1);

            // If both are true randomly decide the move
            if (downLeft && downRight) {
                downLeft  = Math::rand_u(1);
                downRight = !downLeft;
            }

            if		(downLeft)	MoveCell(x, y, x - 1, y + 1);
            else if (downRight) MoveCell(x, y, x + 1, y + 1);

            return downLeft || downRight;
        }

        bool MoveSide(s32 x, s32 y, const Cell& cell) {
            for (int i = 1; i > 0; i--) {
                bool left  = IsEmpty(x - i, y);
                bool right = IsEmpty(x + i, y);

                // If both are true randomly decide the move
                if (left && right) {
                    left  = Math::rand_u(1);
                    right = !left;
                }

                     if (left)  MoveCell(x, y, x - i, y);
                else if (right)	MoveCell(x, y, x + i, y);

                if (left || right) return true;
            }
            return false;
        }

        void MoveCell(s32 x, s32 y, s32 to_x, s32 to_y) { 
            if (SandWorldChunk* src = GetChunk(x, y)) {
                if (SandWorldChunk* dst = GetChunk(to_x, to_y)) {
                    dst->MoveCell(src, x, y, to_x, to_y);
                }
            }
        }
        
        void OnUpdate() {

            RemoveEmptyChunks();

            for (auto* chunk : m_chunks) {
                for (size_t x = 0; x < SAND_WORLD_CHUNK_SIZE_X;  x++) {
                    for (size_t y = 0; y < SAND_WORLD_CHUNK_SIZE_Y; y++) {
                        Cell& cell = chunk->GetCell(x + y * SAND_WORLD_CHUNK_SIZE_X);
        
                        s32 px = x + chunk->GetPos().x;
                        s32 py = y + chunk->GetPos().y;
        
                             if (cell.props & CellProperties::MOVE_DOWN      && MoveDown    (px, py, cell)) {}
                        else if (cell.props & CellProperties::MOVE_DOWN_SIDE && MoveDownSide(px, py, cell)) {}
                        else if (cell.props & CellProperties::MOVE_SIDE      && MoveSide    (px, py, cell)) {}
                    }
                }
            }
        
            for (auto* chunk : m_chunks) {
                chunk->CommitCells();
            }
        }

        // ** Helper Functions **

        /**
         * @brief Check if Cell is in bounds
         * 
         * @param x     pos x
         * @param y     pos y
         * @return b8   true if in Bounds of current chunk
         */
        b8 InBounds(s32 x, s32 y) {
            if (SandWorldChunk* chunk = GetChunk(x, y)) {
                return chunk->InBounds(x, y);
            }
            return false;
        }
 
        /**
         * @brief Check if Cell is empty
         * 
         * @param x     pos x
         * @param y     pos y
         * @return b8   true if cell is empty
         */
        b8 IsEmpty(s32 x, s32 y) { return InBounds(x, y) && GetChunk(x, y)->IsEmpty(x, y); }

        /**
         * @brief Gets Chunk at current Cell pos
         * @note If there is no chunk at the given position, one 
         *       will be created
         * 
         * @param x     pos x
         * @param y     pos y
         * @return SandWorldChunk*  pointer to chunk
         */
        SandWorldChunk* GetChunk(s32 x, s32 y) {
            auto location = GetChunkLocation(x, y);
            AC_CORE_TRACE("Get Chunk [ {0} | {1} ]", location.first, location.second);
            SandWorldChunk* chunk = GetChunkDirect(location);
            if (!chunk) {
                chunk = CreateChunk(location);
            }
            return chunk;
        }
    
        SandWorldChunk* GetChunkDirect(std::pair<s32, s32> location) {
            auto itr = m_chunkLookup.find(location);
            if (itr == m_chunkLookup.end()) {
                return nullptr;
            }
            return itr->second;
        }
    
        std::pair<s32, s32> GetChunkLocation(s32 x, s32 y) {
            return { 
                (s32)floor(float(x) / SAND_WORLD_CHUNK_SIZE_X),
                (s32)floor(float(y) / SAND_WORLD_CHUNK_SIZE_Y)
            };
        }

    private:

        SandWorldChunk* CreateChunk(std::pair<s32, s32> location) {
            auto [lx, ly] = location;

            
            // Worldlimit
            //if (lx < -10 || ly < -10 || lx > 10 || ly > 10) {
            //    return nullptr;
            //}

            SandWorldChunk* chunk = new SandWorldChunk(lx, ly);
            m_chunkLookup.insert({ location, chunk });
            m_chunks.push_back(chunk);

            AC_CORE_TRACE("Creating Chunk [{0}, {1}]", lx, ly);
            AC_CORE_TRACE("    m_chunks size [{0}]", m_chunks.size());

            return chunk;
        }

        void RemoveEmptyChunks() {
            for (size_t i = 0; i < m_chunks.size(); i++) {
                SandWorldChunk* chunk = m_chunks.at(i);
                if (chunk->GetFilledCellCount() == 0) {
                    m_chunkLookup.erase(GetChunkLocation(chunk->GetPos().x, chunk->GetPos().y));
                    m_chunks[i] = m_chunks.back();
                    m_chunks.pop_back();
                    i--;
                    delete chunk;
                }
            }
        }

    private:
        // ** Members **
        std::vector<SandWorldChunk*> m_chunks;
        std::unordered_map<std::pair<s32, s32>, SandWorldChunk*, Math::pair_hash> m_chunkLookup;

    };
}