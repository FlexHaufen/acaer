

// *** INCLUDES ***
#include "acaer/Scene/World/World.h"
#include "acaer/Scene/Renderer/Renderer.h"
#include "acaer/Helper/Util/Math.h"


// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

	size_t World::GetIndex(size_t x, size_t y) { 
		if (x >= m_width || y >= m_height) {
			AC_CORE_ERROR("Access violation. Index of cell is bigger than actual world size");
			return 0;
		}
		return x + y * m_width; 
	}


    void World::MoveCell(size_t x, size_t y, size_t xto, size_t yto) {
		m_changes.emplace_back(GetIndex(xto, yto), GetIndex(x, y));
	}


    void World::CommitCells() {
		// remove moves that have their destinations filled
		for (size_t i = 0; i < m_changes.size(); i++) {
			if (m_cells[m_changes[i].first].type != CellType::EMPTY) {
				m_changes[i] = m_changes.back(); m_changes.pop_back();
				i--;
			}
		}

        // sort by destination
		std::sort(m_changes.begin(), m_changes.end(),
			[](auto& a, auto& b) { return a.first < b.first; }
		);
    	
		// pick random source for each destination
		size_t iprev = 0;

		m_changes.emplace_back(-1, -1); // to catch final move

		for (size_t i = 0; i < m_changes.size() - 1; i++) {
			if (m_changes[i + 1].first != m_changes[i].first) {

				size_t randNum = iprev + rand_i(i - iprev);

				size_t dst = m_changes[randNum].first;
				size_t src = m_changes[randNum].second;

				m_cells[dst] = m_cells[src];
				m_cells[src] = Cell();

				iprev = i + 1;
			}
		}
		m_changes.clear();
	}

    bool World::CanMoveDown(size_t x, size_t y, const Cell& cell) {
		bool down = IsEmpty(x, y + 1);	// SFML +y is down
		if (down) {
			MoveCell(x, y, x, y + 1);	// SFML +y is down
		}

		return down;
	}

	bool World::CanMoveDownSide(size_t x, size_t y, const Cell& cell) {
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

    void World::OnUpdate() {
        // Update cells

		for (size_t x = 0; x < m_width;  x++) {
		    for (size_t y = 0; y < m_height; y++) {
                const Cell& cell = GetCell(x, y);

                if      (cell.props & CellProperties::MOVE_DOWN      && CanMoveDown    (x, y, cell)) {}
                else if (cell.props & CellProperties::MOVE_DOWN_SIDE && CanMoveDownSide(x, y, cell)) {}
                //else if (cell.props & CellProperties::MOVE_SIDE      && CanMoveSide    (x, y, cell)) {}
            }
        }

		CommitCells();
		// Update the sand texture
		// Draw the sand to the screen
    }

    void World::OnRender(sf::RenderWindow &window) {
        for (size_t x = 0; x < m_width;  x++) {
		    for (size_t y = 0; y < m_height; y++) {
                const Cell& cell = GetCell(x, y);

                if (cell.type != CellType::EMPTY) {
                    Renderer::RenderCell(window, x, y, cell.color);
                }
            }
        }
    }
}