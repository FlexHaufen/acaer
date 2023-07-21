/**
 * @file Renderer.cpp
 * @author flaxhaufen
 * @brief 
 * @version 0.1
 * @date 2023-02-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/Scene/Renderer/Renderer.h"
#include "acaer/Scene/Entity/Entity.h"
#include "acaer/Scene/Entity/ScriptableEntity.h"

#include "acaer/Helper/Convert/Convert.h"


// *** NAMESPACE ***
namespace Acaer {

    void Renderer::RenderSprite(const Component::Sprite &sprite) {
        m_Window.draw(sprite.spriteTexture);
    }

    void Renderer::RenderCell(size_t x, size_t y, vColor c) {
        sf::RectangleShape rec;
        rec.setPosition(sf::Vector2f((f32)x * AC_GLOBAL_SCALE, (f32)y  * AC_GLOBAL_SCALE));
        rec.setSize(sf::Vector2f(1 * AC_GLOBAL_SCALE, 1 * AC_GLOBAL_SCALE));

        rec.setFillColor(Convert::vColor_to_sfColor(c));       // Setting the fillcolor to nothing
        m_Window.draw(rec);
    }

    //void Renderer::RenderChunk(WorldChunk *chunk) {
    //    for (s32 x = 0; x < chunk->getWidth();  x++) {
    //        for (s32 y = 0; y < chunk->getHeight(); y++) {
    //            Cell& cell = chunk->GetCell(x + y * chunk->getWidth());
    //
    //            s32 px = (s32)x + chunk->getPosX();
    //            s32 py = (s32)y + chunk->getPosY();
    //
    //            if (cell.type != CellType::EMPTY) {
    //                // TODO (flex): Add renderer ref
    //                RenderCell(px, py, cell.color);
    //            }
    //        }
    //    }
    //}

}