

// *** INCLUDES ***
#include "acaer/Scene/World/World.h"
#include "acaer/Helper/Util/Math.h"


// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    void World::OnUpdate() {
		RemoveEmptyChunks();

		for (WorldChunk* chunk : m_chunks) {
			UpdateChunk(chunk);
		}
 
		for (WorldChunk* chunk : m_chunks) {
			chunk->CommitCells();
		}
    }

	void World::OnRender(sf::RenderWindow &window) {

		//AC_CORE_TRACE("Chunks: {0}", sizeof(m_chunks));
		for (WorldChunk* chunk : m_chunks) {

			//! Debug
			Renderer::RenderChunckBorder(window,chunk->getWidth(), chunk->getHeight(), chunk->getPosX(), chunk->getPosY());
			
			RenderChunk(window, chunk);
		}
	}

	void World::UpdateChunk(WorldChunk* chunk) {
		for (int x = 0; x < chunk->getWidth();  x++) {
			for (int y = 0; y < chunk->getHeight(); y++) {
				Cell& cell = chunk->GetCell(x + y * chunk->getWidth());
	
				int px = (int)x + chunk->getPosX();
				int py = (int)y + chunk->getPosY();
	
				UpdateCell(px, py, cell);
			}
		}
	}

	void World::RenderChunk(sf::RenderWindow &window, WorldChunk* chunk) {
		for (int x = 0; x < chunk->getWidth();  x++) {
			for (int y = 0; y < chunk->getHeight(); y++) {
				Cell& cell = chunk->GetCell(x + y * chunk->getWidth());
	
				int px = (int)x + chunk->getPosX();
				int py = (int)y + chunk->getPosY();
	
				if (cell.type != CellType::EMPTY) {
					Renderer::RenderCell(window, px, py, cell.color);
				}
			}
		}
	}

	void World::RemoveEmptyChunks() {
		for (size_t i = 0; i < m_chunks.size(); i++) {
			WorldChunk* chunk = m_chunks.at(i);
	
			if (chunk->getFillCellCount() == 0) {
				m_chunkLookup.erase(GetChunkLocation(chunk->getPosX(), chunk->getPosY()));
				m_chunks[i] = m_chunks.back(); m_chunks.pop_back();
				i--;
	
				delete chunk;
			}
		}
	}

	WorldChunk* World::GetChunk(int x, int y) {
		auto location = GetChunkLocation(x, y);

		WorldChunk* chunk = GetChunkDirect(location);
		if (!chunk) {
			chunk = CreateChunk(location);
		}

		return chunk;
	}

	WorldChunk* World::GetChunkDirect(std::pair<int, int> location) {
		auto itr = m_chunkLookup.find(location);
		if (itr == m_chunkLookup.end()) {
			return nullptr;
		}
		return itr->second;
	}

	std::pair<int, int> World::GetChunkLocation(int x, int y) {
		return { 
			(int)floor(float(x) / m_chunkWidth), 
			(int)floor(float(y) / m_chunkHeight)
		};
	}

	WorldChunk* World::CreateChunk(std::pair<int, int> location) {
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


	bool World::CanMoveDown(int x, int y, const Cell& cell) {
		bool down = IsEmpty(x, y + 1);	// SFML +y is down
		if (down) {
			MoveCell(x, y, x, y + 1);	// SFML +y is down
		}
		return down;
	}

	bool World::CanMoveDownSide(int x, int y, const Cell& cell) {
		bool downLeft  = IsEmpty(x - 1, y + 1);	// SFML +y is down
		bool downRight = IsEmpty(x + 1, y + 1);	// SFML +y is down

		if (downLeft && downRight) {
			downLeft  = rand() % 2;
			downRight = !downLeft;
		}

		     if (downLeft)  MoveCell(x, y, x - 1, y + 1);
		else if (downRight) MoveCell(x, y, x + 1, y + 1);
 
		return downLeft || downRight;
	}
}