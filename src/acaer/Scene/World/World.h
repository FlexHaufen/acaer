/**
 * @file SandWorld.h
 * @author flexhaufen
 * @brief SandWorld
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
#include "acaer/Scene/World/WorldChunk.h"

#include "acaer/Scene/Renderer/Renderer.h"
#include "acaer/Scene/Renderer/DebugRenderer.h"

//*** DEFINES ***
#define AC_WORLD_DEFAULT_SCALE 1


//*** NAMESPACE ***
namespace Acaer {
    
    /**
     * @brief SandWorld for Game
     *        cellular automata
     */
    class SandWorld {
    public:

        /**
         * @brief Construct a new SandWorld object
         * 
         * @param width 
         * @param height 
         */
        SandWorld(size_t chunkWidth, size_t chunkHeight, f32 scale)
            : m_chunkWidth ((size_t)(chunkWidth / scale))
            , m_chunkHeight((size_t)(chunkHeight / scale))
            , m_scale(scale) {
            AC_CORE_INFO("Creating World [{0} x {1}, {2}]", chunkWidth, chunkHeight, scale);
        }

        /**
         * @brief Update function
         * 
         */
        void OnUpdate();

        /**
         * @brief Get the Chunks of a World object
         * 
         * @return std::vector<WorldChunk*> World Chunks
         */
        std::vector<WorldChunk*> GetChunkVector() { return m_chunks; }

        // ** Cells **
        bool IsInBounds(s32 x, s32 y) {
            if (WorldChunk* chunk = GetChunk(x, y)) {
                return chunk->IsInBounds(x, y);
            }
            return false;
        }

        bool IsEmpty(s32 x, s32 y) {
		    return IsInBounds(x, y) && GetChunk(x, y)->IsEmpty(x, y);
	    }
 
        Cell& GetCell(s32 x, s32 y) {
		    return GetChunk(x, y)->GetCell(x, y);
	    }
    
        void SetCell(s32 x, s32 y, const Cell& cell) {
            if (WorldChunk* chunk = GetChunk(x, y)) {
                chunk->SetCell(x, y, cell);
            }
        }

    private:
        // ** Cells **
        void UpdateCell(s32 x, s32 y, Cell& cell) {
            if      (cell.props & CellProperties::MOVE_DOWN      && CanMoveDown    (x, y, cell)) {}
            else if (cell.props & CellProperties::MOVE_DOWN_SIDE && CanMoveDownSide(x, y, cell)) {}
            //else if (cell.props & CellProperties::MOVE_SIDE      && CanMoveSide    (x, y, cell)) {}
        }
        
        void MoveCell(s32 x, s32 y, s32 xto, s32 yto) {
            if (WorldChunk* src = GetChunk(x, y)) {
                if (WorldChunk* dst = GetChunk(xto, yto)) {
                    dst->MoveCell(src, x, y, xto, yto);
                }
            }
        }

        bool CanMoveDown(s32 x, s32 y, const Cell& cell);

	    bool CanMoveDownSide(s32 x, s32 y, const Cell& cell);

        void CommitCells();


        // ** Chunks **
        void UpdateChunk(WorldChunk* chunk);

        void RemoveEmptyChunks();

    	WorldChunk* GetChunk(s32 x, s32 y);

        WorldChunk* GetChunkDirect(std::pair<s32, s32> location);
    
        std::pair<s32, s32> GetChunkLocation(s32 x, s32 y);

        WorldChunk* CreateChunk(std::pair<s32, s32> location);

    private:
        //** Members **
        const size_t m_chunkWidth;   // width of world
        const size_t m_chunkHeight;  // height of world
        const f32    m_scale = AC_WORLD_DEFAULT_SCALE;   // scale

        std::vector<WorldChunk*> m_chunks;
        std::unordered_map<std::pair<s32, s32>, WorldChunk*, pair_hash> m_chunkLookup;
    };
}