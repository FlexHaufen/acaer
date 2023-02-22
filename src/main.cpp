/**
 * @file main.cpp
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-02-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

//*** INCLUDES ***
#include "acaer/Acaer.h"

//*** MAIN ***
int main(int argc, char **argv) {

	Acaer::Core* ac_game = new Acaer::Core();
	ac_game->Run();
	delete ac_game;


    return 0;
}