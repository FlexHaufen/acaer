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
        
        void SetCell(s32 x, s32 y, const Cell& cell);

        // Move
        bool MoveDown(s32 x, s32 y, const Cell& cell);
            
        bool MoveDownSide(s32 x, s32 y, const Cell& cell);

        bool MoveSide(s32 x, s32 y, const Cell& cell);

        void MoveCell(s32 x, s32 y, s32 to_x, s32 to_y);

        void OnUpdate();

        // ** Helper Functions **

        /**
         * @brief Check if Cell is in bounds
         * 
         * @param x     pos x
         * @param y     pos y
         * @return b8   true if in Bounds of current chunk
         */
        b8 InBounds(s32 x, s32 y) { if (SandWorldChunk* chunk = GetChunk(x, y)) { return chunk->InBounds(x, y); } return false; }
 
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
        SandWorldChunk* GetChunk(s32 x, s32 y);
    
        SandWorldChunk* GetChunkDirect(std::pair<s32, s32> location);
    
        std::pair<s32, s32> GetChunkLocation(s32 x, s32 y);

    private:

        SandWorldChunk* CreateChunk(std::pair<s32, s32> location);

        void RemoveEmptyChunks();

    private:
        // ** Members **
        std::vector<SandWorldChunk*> m_chunks = {};
        std::unordered_map<std::pair<s32, s32>, SandWorldChunk*, Math::pair_hash> m_chunkLookup;

    };
}