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
			
			UpdateRect();
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

			KeepAlive(index);
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

		void KeepAlive(int x, int y) {
			KeepAlive(GetIndex(x, y));
		}

		void KeepAlive(size_t index) {
			int x = index % m_width;
			int y = index / m_width;
	
			m_minW.x = clamp(std::min(x - 2, m_minW.x), 0, (s32)m_width);
			m_minW.y = clamp(std::min(y - 2, m_minW.y), 0, (s32)m_height);
			m_maxW.x = clamp(std::max(x + 2, m_maxW.x), 0, (s32)m_width);
			m_maxW.y = clamp(std::max(y + 2, m_maxW.y), 0, (s32)m_height);
		}

		void UpdateRect() {
			// Update current; reset working
			m_min.x = m_minW.x;  m_minW.x = m_width;
			m_min.y = m_minW.y;  m_minW.y = m_height;
			m_max.x = m_maxW.x;  m_maxW.x = -1;
			m_max.y = m_maxW.y;  m_maxW.y = -1;	
		}
 

		const size_t getFillCellCount() { return m_filledCellCount; }

		const size_t getWidth()  { return m_width; }
		const size_t getHeight() { return m_height; }

		const s32 getPosX() { return m_x; }
		const s32 getPosY() { return m_y; }

		const v2<int> getMin() { return m_min; }
		const v2<int> getMax() { return m_max; }
		
	private:
		// ** Members **
		const size_t m_width, m_height;
		const s32 m_x, m_y;

		v2<s32> m_min, m_max;		// Dirty Rect
		v2<s32> m_minW, m_maxW;		// Working dirty Rect

		Cell* m_cells;
		std::vector<std::tuple<WorldChunk*, int, int>> m_changes; // source chunk, source, destination

		size_t m_filledCellCount;
	};
}