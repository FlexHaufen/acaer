/**
 * @file SandWorldEditorPanel.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-07-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"

#include "acaer/ImGui/ImGuiPanel.h"


// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    /**
     * @brief Entity Browser panel
     *        Displays all enities in current scene  
     */
    class SandWorldEditorPanel : public ImGuiPanel {

	public:

        /**
         * @brief ImGui Render
         * 
         */
		void OnImGuiRender();
    
    private:

        /**
         * @brief Draws given cell at given pos
         * 
         * @param pos   Position
         * @param cell  Cell
         */
        void DrawCell(const sf::Vector2i &pos, const Cell &cell);

        /**
         * @brief Draws filled circle out of given cell
         *        with given size
         * 
         * @param pos   Position
         * @param size  Size of circle [d]
         * @param cell  Cell
         */
        void DrawFilledCircle(const sf::Vector2i &pos, u8 size, const Cell &cell);

        /**
         * @brief Gets a Cell out of type
         * 
         * @param cellType 
         * @return Cell 
         */
        Cell GetCell(s32 cellType);

    private:

    };
}