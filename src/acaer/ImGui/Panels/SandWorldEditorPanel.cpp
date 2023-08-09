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

        // Selectable Cell Types
        const char* items[] = { "Empty", "Sand", "Water", "Stone"};
        static s32 cellType = 1;
        ImGui::ListBox("Type", &cellType, items, IM_ARRAYSIZE(items), 4);

        static s32 burshSize = 1;
        ImGui::SliderInt("Brush Size", &burshSize, 1, 10);


        // isMouseHeld is set to true when Mouse::Left is pressed and to false
        // when Mouse::Left is released
        static b8 isMouseHeld = false;
        // FIXME (flex): Possible adding of multiple callbacks
        m_Context->m_EventManager.addMousePressedCallback(sf::Mouse::Left, [&](const sf::Event&) { isMouseHeld = true; });
        m_Context->m_EventManager.addMouseReleasedCallback(sf::Mouse::Left, [&](const sf::Event&) { isMouseHeld = false; });

        // ** Logic **
        if (inPlaceMode && isMouseHeld && !m_isImGuiFocused) {
            sf::Vector2i pos(m_Context->m_Window.mapPixelToCoords(sf::Mouse::getPosition(m_Context->m_Window)) / AC_GLOBAL_SCALE);
            DrawFilledCircle(pos, burshSize, GetCell(cellType));
        }
		ImGui::End();
    }

    Cell SandWorldEditorPanel::GetCell(s32 cellType) {
        Cell cell;
        switch (cellType) {
            // NOTE (flex): case 0 is default -> empty
            case 1: // Sand
                cell.color = {181, 157, 80, 255};
                cell.props = CellProperties::MOVE_DOWN | CellProperties::MOVE_DOWN_SIDE;
                cell.type = CellType::SAND;
                break;
            case 2: // Water
                cell.color = {58, 164, 222, 255};
                cell.props = CellProperties::MOVE_DOWN | CellProperties::MOVE_SIDE;
                cell.type = CellType::WATER;
                break;
            case 3: // Rock
                cell.color = {107, 110, 109, 255};
                cell.props = CellProperties::NONE;
                cell.type = CellType::ROCK;
                break;
            default: // Empty
                cell.color = {0, 0, 0, 0};
                cell.props = CellProperties::NONE;
                cell.type = CellType::EMPTY;
                break;
        }
        return cell;
    }

    void SandWorldEditorPanel::DrawFilledCircle(const sf::Vector2i &pos, u8 size, const Cell &cell) {
        s16 r = size / 2;    // radius [px]

        s16 cx = pos.x + r;
        s16 cy = pos.y + r;

        s16 x0 = 0;
        s16 y0 = r;
        s16 decision = 1 - r;

        while (x0 <= y0) {
            for (s16 i = cx - x0; i <= cx + x0; i++) {
                DrawCell({i, cy + y0}, cell);
                DrawCell({i, cy - y0}, cell);
            }
            for (s16 i = cx - y0; i <= cx + y0; i++) {
                DrawCell({i, cy + x0}, cell);
                DrawCell({i, cy - x0}, cell);
            }
            x0++;
            if (decision < 0) {
                decision += 2 * x0 + 1;
            }
            else {
                y0--;
                decision += 2 * (x0 - y0) + 1;
            }
        }
    }

    void SandWorldEditorPanel::DrawCell(const sf::Vector2i &pos, const Cell &cell) {
        m_Context->m_SandWorld->SetCell(pos.x, pos.y, cell);
    }
}