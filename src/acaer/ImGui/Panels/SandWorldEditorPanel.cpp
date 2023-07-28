/**
 * @file SandWorldEditorPanel.cpp
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-07-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/ImGui/Panels/SandWorldEditorPanel.h"


// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    void SandWorldEditorPanel::OnImGuiRender() {
		ImGui::Begin("SandWorld Editor Panel");
        m_isPanelFocused = ImGui::IsWindowFocused() || ImGui::IsWindowHovered();    //! Importànte


        // If Place mode is enabled custom cells can be placed
        static b8 inPlaceMode = false;
        ImGui::Checkbox("Place Mode", &inPlaceMode);

        // Mode determains erase = 0 or draw = 1
        static s32 mode = 0;
        ImGui::RadioButton("Erase", &mode, 0); ImGui::SameLine();
        ImGui::RadioButton("Draw", &mode, 1); 

        static s32 burshSize = 0;
        ImGui::SliderInt("Brush Size", &burshSize, 0, 10);


        // isMouseHeld is set to true when Mouse::Left is pressed and to false
        // when Mouse::Left is released
        static b8 isMouseHeld = false;
        // FIXME (flex): Possible adding of multiple callbacks
        m_Context->m_EventManager.addMousePressedCallback(sf::Mouse::Left, [&](const sf::Event&) { isMouseHeld = true; });
        m_Context->m_EventManager.addMouseReleasedCallback(sf::Mouse::Left, [&](const sf::Event&) { isMouseHeld = false; });



        // ** Logic **
        if (inPlaceMode) {
            static s32 cellType = 0;
            ImGui::RadioButton("Sand",  &cellType, 0); ImGui::SameLine();
            ImGui::RadioButton("Water", &cellType, 1); ImGui::SameLine();
            ImGui::RadioButton("Rock",  &cellType, 2); 
            if (isMouseHeld) {

                sf::Vector2i pos(m_Context->m_Window.mapPixelToCoords(sf::Mouse::getPosition(m_Context->m_Window)) / AC_GLOBAL_SCALE);
                
                if (!m_isImGuiFocused) {
                    if (mode) {
                        DrawCell(pos, cellType);
                    }
                    else {
                        RemoveCell(pos);
                    }
                }
            }
        }
		ImGui::End();
    }

    void SandWorldEditorPanel::DrawCell(const sf::Vector2i &pos, s32 cellType) {
        Cell cell;
        switch (cellType) {
            case 0: // Sand
                cell.color = {181, 157, 80, 255};
                cell.props = CellProperties::MOVE_DOWN | CellProperties::MOVE_DOWN_SIDE;
                cell.type = CellType::SAND;
                m_Context->m_SandWorld->SetCell(pos.x, pos.y, cell);
                break;
            case 1: // Water
                cell.color = {58, 164, 222, 255};
                cell.props = CellProperties::MOVE_DOWN | CellProperties::MOVE_SIDE;
                cell.type = CellType::WATER;
                m_Context->m_SandWorld->SetCell(pos.x, pos.y, cell);
                break;
            case 2: // Rock
                cell.color = {107, 110, 109, 255};
                cell.props = CellProperties::NONE;
                cell.type = CellType::ROCK;
                m_Context->m_SandWorld->SetCell(pos.x, pos.y, cell);
                break;
            default:
                break;
        }
    }

    void SandWorldEditorPanel::RemoveCell(const sf::Vector2i &pos) {
        Cell cell;
        cell.color = {0, 0, 0, 0};
        cell.props = CellProperties::NONE;
        cell.type = CellType::EMPTY;

        m_Context->m_SandWorld->SetCell(pos.x, pos.y, cell);
    }
}