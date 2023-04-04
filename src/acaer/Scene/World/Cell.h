/**
 * @file Cell.h
 * @author flexhaufen
 * @brief "We're going to go on. Cells.
           They were all put together at a time. Cells.
           Millions and billions of them. Cells.
           Were you ever arrested? Cells.
           Did you spend much time in the cell? Cells.
           Have you ever been in an instituion? Cells.
           Do they keep you in a cell? Cells.
           When you're not performing your duties do they keep you in a little box? Cells.
           Interlinked." - Blade Runner 2049
 * @version 0.1
 * @date 2023-03-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"



//*** DEFINES ***


//*** NAMESPACE ***
namespace Acaer {

    /**
     * @brief Movement behaviour properties
     *        of a Cell
     */
    enum class CellProperties {
        NONE                = 0b00000000,
        MOVE_DOWN           = 0b00000001,
        MOVE_DOWN_SIDE      = 0b00000010,
        MOVE_SIDE           = 0b00000100
    };
 
    /**
     * @brief Cell Type
     * 
     */
    enum class CellType {
        EMPTY,
        SAND,
        WATER,
        ROCK
    };

    // operator overloads
    inline CellProperties operator|(CellProperties a, CellProperties b) { return CellProperties((s32)(a) | (s32)(b)); }
    inline auto operator&( CellProperties a, CellProperties b) {return (s32)(a) & (s32)(b); }
    

    /**
     * @brief Cell
     * 
     */
    struct Cell {
        CellType       type  = CellType::EMPTY;         // type
        CellProperties props = CellProperties::NONE;    // properties

        vColor         color = {0, 0, 0, 0};            // color of cell
    };
}