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

        ImGui::Checkbox("Place Mode", &m_PlaceMode);

        if (m_PlaceMode) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                Cell cell;
                cell.color = {255, 255, 0, 255};
                cell.props = CellProperties::NONE;
                cell.type = CellType::ROCK;

                sf::Vector2f posF = m_Context->m_Window.mapPixelToCoords(sf::Mouse::getPosition(m_Context->m_Window));
                sf::Vector2i pos(posF / AC_GLOBAL_SCALE);

                if (m_Context->m_SandWorld->IsInBounds(pos.x, pos.y)) {
                    AC_CORE_TRACE("Setting Cell");
                    m_Context->m_SandWorld->SetCell(pos.x, pos.y, cell);
                }
            }

        }
		ImGui::End();
    }
}