/**
 * @file WorldChunk.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-04-01
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
#define _DEST 2

//*** NAMESPACE ***
namespace Acaer {

	/**
	 * @brief WorldChunk for Game
	 * 
	 */
	class WorldChunk {
	public:
		WorldChunk(size_t width, size_t height, int x, int y)
			: m_width(width)
			, m_height(height)
			, m_x(x * (int)width)
			, m_y(y * (int)height)
			, m_filledCellCount(0) {
			m_cells = new Cell[width * height];
		}
	
		~WorldChunk() {
			delete[] m_cells;
		}
	
		size_t GetIndex(int x, int y) {
			return (x - m_x) + (y - m_y) * m_width;
		}	
	
		bool InBounds(int x, int y) {
			return  x >= m_x && x < int(m_x + m_width) &&
			  		y >= m_y && y < int(m_y + m_height);
		}
	
		bool IsEmpty(int x, int y) { return GetCell(x, y).type == CellType::EMPTY; }
	
	
		Cell& GetCell(int x, int y) { return GetCell(GetIndex(x, y)); }
		Cell& GetCell(size_t index) { return m_cells[index]; }
	
		void SetCell(int x, int y, const Cell& cell) { SetCell(GetIndex(x, y), cell); }

		void SetCell(size_t index, const Cell& cell) {
			Cell& dest = m_cells[index];

			// Filling a cell
			if (dest.type == CellType::EMPTY && cell.type != CellType::EMPTY) {
				//std::unique_lock lock(m_filledCellCountMutex);
				m_filledCellCount++;
			}

			// Removing a filled cell
			else if (dest.type != CellType::EMPTY && cell.type == CellType::EMPTY) {
				//std::unique_lock lock(m_filledCellCountMutex);
				m_filledCellCount--;
			}

			dest = cell;

			//KeepAlive(index);
		}
	
		void MoveCell(WorldChunk* source, int x, int y,int xto, int yto) {
			m_changes.emplace_back(source, (int)source->GetIndex(x, y), (int)GetIndex(xto, yto));
		}

		void CommitCells() {
			// remove moves that have their destinations filled
			for (size_t i = 0; i < m_changes.size(); i++) {
				const Cell& dest = m_cells[std::get<_DEST>(m_changes[i])];

				if (dest.type != CellType::EMPTY) {
					m_changes[i] = m_changes.back(); m_changes.pop_back();
					i--;
				}
			}

			// sort by destination
			std::sort(m_changes.begin(), m_changes.end(),
				[](auto& a, auto& b) { return std::get<_DEST>(a) < std::get<_DEST>(b); }
			);

			// pick random source for each destination
			size_t iprev = 0;
			m_changes.emplace_back(nullptr, -1, -1); // to catch final move
			for (size_t i = 0; i < m_changes.size() - 1; i++) {
				if (std::get<_DEST>(m_changes[i + 1]) != std::get<_DEST>(m_changes[i])) {
					size_t rand = iprev + rand_u(i - iprev);
					auto [chunk, src, dst] = m_changes[rand];
					SetCell(dst, chunk->GetCell(src));
					chunk->SetCell(src, Cell());

					iprev = i + 1;
				}
			}
			m_changes.clear();
		}

		const size_t getFillCellCount() { return m_filledCellCount; }

		const size_t getWidth()  { return m_width; }
		const size_t getHeight() { return m_height; }

		const s32 getPosX() { return m_x; }
		const s32 getPosY() { return m_y; }
		
	private:
		// ** Members **
		const size_t m_width, m_height;
		const s32 m_x, m_y;
		Cell* m_cells;
		std::vector<std::tuple<WorldChunk*, int, int>> m_changes; // source chunk, source, destination

		size_t m_filledCellCount;
	};
}