/**
 * @file SandWorld.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***ad
#include "acaer/Scene/World/SandWorld.h"




// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

        void SandWorld::SetCell(s32 x, s32 y, const Cell& cell) {
            if (SandWorldChunk* chunk = GetChunk(x, y)) {
                chunk->SetCell(x, y, cell);
                return;
            }
            AC_CORE_WARN("SetCell: Chunk at [{0} / {1}] not found!", x, y);
	    } 

        bool SandWorld::MoveDown(s32 x, s32 y, const Cell& cell) {
            if (IsEmpty(x, y + 1)) {
                MoveCell(x, y, x, y + 1);
                return true;
            }
            return false;
        }
            
        bool SandWorld::MoveDownSide(s32 x, s32 y, const Cell& cell) {
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

        bool SandWorld::MoveSide(s32 x, s32 y, const Cell& cell) {
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

        void SandWorld::MoveCell(s32 x, s32 y, s32 to_x, s32 to_y) { 
            if (SandWorldChunk* src = GetChunk(x, y)) {
                if (SandWorldChunk* dst = GetChunk(to_x, to_y)) {
                    dst->MoveCell(src, x, y, to_x, to_y);
                    return;
                }
                AC_CORE_WARN("MoveCell: Chunk at [{0} / {1}] not found!", to_x, to_y);
                return;
            }
            AC_CORE_WARN("MoveCell: Chunk at [{0} / {1}] not found!", x, y);
        }
        
        void SandWorld::OnUpdate() {

            RemoveEmptyChunks();

            for (auto* chunk : m_chunks) {
                for (s32 x = chunk->GetChunkRectMin().x; x < chunk->GetChunkRectMax().x;  x++) {
                    for (s32 y = chunk->GetChunkRectMin().y; y < chunk->GetChunkRectMax().y; y++) {
                        Cell& cell = chunk->GetCell(x + y * AC_SAND_WORLD_CHUNK_SIZE_X);

                        if (cell.props == CellProperties::NONE) {
                            continue;
                        }

                        v2<s32> pos = v2<s32>(x, y) + chunk->GetPos();
        
                        // ** Update Cell **
                             if (cell.props & CellProperties::MOVE_DOWN      && MoveDown    (pos.x, pos.y, cell)) {}
                        else if (cell.props & CellProperties::MOVE_DOWN_SIDE && MoveDownSide(pos.x, pos.y, cell)) {}
                        else if (cell.props & CellProperties::MOVE_SIDE      && MoveSide    (pos.x, pos.y, cell)) {}
                    }
                }
            }
            for (auto* chunk : m_chunks) {
                chunk->CommitCells();
            }
            for (auto* chunk : m_chunks) {
                chunk->UpdateRect();
            }
        }

        SandWorldChunk* SandWorld::GetChunk(s32 x, s32 y) {
            auto location = GetChunkLocation(x, y);
            SandWorldChunk* chunk = GetChunkDirect(location);
            if (!chunk) {
                chunk = CreateChunk(location);
            }
            return chunk;
        }
    
        SandWorldChunk* SandWorld::GetChunkDirect(std::pair<s32, s32> location) {
            auto itr = m_chunkLookup.find(location);
            if (itr == m_chunkLookup.end()) {
                return nullptr;
            }
            return itr->second;
        }
    
        std::pair<s32, s32> SandWorld::GetChunkLocation(s32 x, s32 y) {
            return { 
                (s32)floor(float(x) / AC_SAND_WORLD_CHUNK_SIZE_X),
                (s32)floor(float(y) / AC_SAND_WORLD_CHUNK_SIZE_Y)
            };
        }

        SandWorldChunk* SandWorld::CreateChunk(std::pair<s32, s32> location) {
            auto [lx, ly] = location;

            
            // Worldlimit
            //if (lx < -10 || ly < -10 || lx > 10 || ly > 10) {
            //    return nullptr;
            //}

            SandWorldChunk* chunk = new SandWorldChunk(lx, ly);
            m_chunkLookup.insert({ location, chunk });
            m_chunks.push_back(chunk);
            return chunk;
        }

        void SandWorld::RemoveEmptyChunks() {
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
}