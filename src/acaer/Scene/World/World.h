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
#include "acaer/Scene/World/WorldChunk.h"

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
        World(size_t chunkWidth, size_t chunkHeight, f32 scale)
            : m_chunkWidth ((size_t)(chunkWidth / scale))
            , m_chunkHeight((size_t)(chunkHeight / scale))
            , m_scale(scale) {}

    
        bool InBounds(int x, int y) {
            if (WorldChunk* chunk = GetChunk(x, y)) {
                return chunk->InBounds(x, y);
            }
            return false;
        }

        bool IsEmpty(int x, int y) {
		    return InBounds(x, y) && GetChunk(x, y)->IsEmpty(x, y);
	    }
 
        Cell& GetCell(int x, int y) {
		    return GetChunk(x, y)->GetCell(x, y);
	    }
    
        void SetCell(int x, int y, const Cell& cell) {
            if (WorldChunk* chunk = GetChunk(x, y)) {
                chunk->SetCell(x, y, cell);
            }
        }
    
        void MoveCell(int x, int y, int xto, int yto) {
            if (WorldChunk* src = GetChunk(x, y)) {
                if (WorldChunk* dst = GetChunk(xto, yto)) {
                    dst->MoveCell(src, x, y, xto, yto);
                }
            }
        }

        bool CanMoveDown(int x, int y, const Cell& cell);
	    bool CanMoveDownSide(int x, int y, const Cell& cell);

        void CommitCells();

        void OnUpdate();
        void OnRender(sf::RenderWindow &window);


    	WorldChunk* GetChunk(int x, int y) {
            auto location = GetChunkLocation(x, y);

            WorldChunk* chunk = GetChunkDirect(location);
            if (!chunk) {
                chunk = CreateChunk(location);
            }

            return chunk;
	    }

        WorldChunk* GetChunkDirect(std::pair<int, int> location) {
            auto itr = m_chunkLookup.find(location);
            if (itr == m_chunkLookup.end()) {
                return nullptr;
            }
            return itr->second;
        }
    
        std::pair<int, int> GetChunkLocation(int x, int y) {
            return { 
                (int)floor(float(x) / m_chunkWidth), 
                (int)floor(float(y) / m_chunkHeight)
            };
        }

    private:
        WorldChunk* CreateChunk(std::pair<int, int> location) {
            auto [lx, ly] = location;

            // TODO: world bounderies
            //if (lx < -200 || ly < -200 || lx >  200 || ly >  200) {
            //    return nullptr;
            //}

            WorldChunk* chunk = new WorldChunk(m_chunkWidth, m_chunkHeight, lx, ly);
            m_chunkLookup.insert({ location, chunk });
            {
               // std::unique_lock lock(m_chunkMutex);
                m_chunks.push_back(chunk);
            }

            return chunk;
        }

    private:
        //** Members **
        const size_t m_chunkWidth;   // width of world
        const size_t m_chunkHeight;  // height of world
        const f32    m_scale = AC_WORLD_DEFAULT_SCALE;   // scale

        std::vector<WorldChunk*> m_chunks;
        std::unordered_map<std::pair<int, int>, WorldChunk*, pair_hash> m_chunkLookup;
    };
}