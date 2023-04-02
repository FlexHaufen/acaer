

// *** INCLUDES ***
#include "acaer/Scene/World/World.h"
#include "acaer/Scene/Renderer/Renderer.h"
#include "acaer/Helper/Util/Math.h"


// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    void World::OnUpdate() {
        // Draw cells with mouse
		// Paste tiles

		// Update cells

		// TODO: Better Update Loop
		for (WorldChunk* chunk : m_chunks) {
			for (size_t x = 0; x < chunk->getWidth();  x++) {
				for (size_t y = 0; y < chunk->getHeight(); y++) {
					Cell& cell = chunk->GetCell(x + y * chunk->getWidth());
	
					int px = x + chunk->getPosX();
					int py = y + chunk->getPosY();
	
					//AC_CORE_TRACE("x: {0}, y: {1}, px {2}, py: {3}", x, y, px, py);

				 	if 		(cell.props & CellProperties::MOVE_DOWN      && CanMoveDown    (px, py, cell)) {}
					else if (cell.props & CellProperties::MOVE_DOWN_SIDE && CanMoveDownSide(px, py, cell)) {}
					//else if (cell.props & CellProperties::MOVE_SIDE      && CanMoveSide    (px, py, cell)) {}
				}
			}
		}
 
		for (WorldChunk* chunk : m_chunks) {
			chunk->CommitCells();
		}
 
		// Copy sand colors to a texture
		// Draw the texture on the screen
		// Remove tiles
    }

	void World::OnRender(sf::RenderWindow &window) {

		//AC_CORE_TRACE("Chunks: {0}", sizeof(m_chunks));
		for (WorldChunk* chunk : m_chunks) {
			Renderer::RenderChunckBorder(window,chunk->getWidth(), chunk->getHeight(), chunk->getPosX(), chunk->getPosY());
			for (size_t x = 0; x < chunk->getWidth();  x++) {
				for (size_t y = 0; y < chunk->getHeight(); y++) {
					Cell& cell = chunk->GetCell(x + y * chunk->getWidth());

					int px = x + chunk->getPosX();
					int py = y + chunk->getPosY();

					if (cell.type != CellType::EMPTY) {
						Renderer::RenderCell(window, px, py, cell.color);
					}
				}
			}
		}
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