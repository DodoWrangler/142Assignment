// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

// don't forget to update your project configuration to select "Emulate terminal in the output console"

// Make sure to include all relevant libraries
#include <stdio.h>
#include <stdlib.h>

// colours.h contains functions to change text colour and read single characters without requiring an enter
#include "colours.h"
// defines.h contains useful definitions to keep your code readable
#include "defines.h"
// map.h, game.h, and ghost.h contain prototypes of functions you must implement
#include "map.h"
#include "game.h"
#include "ghost.h"

// These global variables must be used to store map information.
// Almost every function needs these variables, so keeping them as globals helps keep things organized.
// map is a pointer to a dynamically allocated map for displaying to the user
// dot_map is a pointer to a dynamically allocated map for keeping track of what dots are left
char *map = NULL, *dot_map = NULL;
// width and height store the width and height of map, NOT counting outer walls
int width, height;



/**
 * Main entry point into your program.
 * Make sure that main returns appropriate status codes depending on what
 * happens.  The codes you must use are:
 *   NO_ERROR when no error occurs
 *   ERR_NO_MAP when no map file is found
 *   ERR_NO_PACMAN when no pacman is found on the map
 *   ERR_NO_GHOSTS when fewer than 2 ghosts are found on the map
 *
 * Make sure that any allocated memory is freed before returning.
 * @return a status code
 */
int main(void) {
    setbuf(stdout, NULL);

    //tracker of the game state (win/loss/continue)
    int gameState = KEEP_GOING;

    //path name of txt file in use for the map
    char* mapPath = "C:\\Users\\steve\\OneDrive\\Desktop\\142 Assignment\\map.txt";

    //first determines the dimensions of the given map
    findDims(mapPath);

    //performs initial load of map array from txt file
    map = load_map(mapPath, &height, &width);

    //checks the map array before attempting to print in the next fcn
    if(checkMap(map)==ERR_NO_MAP){
        return ERR_NO_MAP;
    }

    //prints the map to terminal
    print_map(map, &height, &width);

    //creates dot map for future reference
    createDotMap();

    //checks both maps for validity before continuing (may as well do both, just to be sure)
    if(checkMap(map)==ERR_NO_MAP|| checkMap(dot_map)==ERR_NO_MAP){
        return ERR_NO_MAP;
    }

    //finds pacman's start position
    int pac[2];
    for (int i = 0; i < (width*height+1); i++) {
        if (map[i] == PACMAN) {
            pac[0] = i % width;
            pac[1] = (i - (i % width)) / height;
        }
    }

    //finds ghost(s) start position(s)
    //array to track ghost coords
    int ghostX[2], ghostY[2];
    //counter to track current subject ghost
    int counter = 0;
    //cycles through map array to locate ghosts
    for (int i = 0; i < (width*height+1); i++) {
        if (map[i] == GHOST) {
            ghostX[counter] = i % width;
            ghostY[counter] = (i - (i % width)) / height;
            //increases counter to move attention to second (next) ghost
            counter++;
        }
    }

    //indefinitely looping to allow for "continuous" play. Will exit on win/loss or error
    while(gameState==KEEP_GOING){

        //user movement
        //tracker for status of movement (set to neutral start value)
        int pacMove = 1;
        //loops until valid user input is detected
        while(pacMove!=MOVED_OKAY) {
            //gets user input in form of a character (key press)
            char input = (char) getch();
            //moves pacman according to input
            pacMove = move_actor(&pac[1], &pac[0], input, 1);
        }
        map[((pac[1]) * width) + pac[0]] = PACMAN;

        //checks for loss
        if (check_loss(pac[1], pac[0], ghostY, ghostX)==YOU_LOSE){
            gameState=2;
        }

        //ghost movement
        //runs once for each ghost
        for(int k=0;k<NUM_GHOSTS;k++) {
            //checks if the ghost sees pacman
            if (sees_pacman(pac[1], pac[0], ghostY[k], ghostX[k]) != SEES_NOTHING) {
                //moves the ghost in the direction it sees pacman (already known to be a clear path)
                move_actor(&ghostY[k], &ghostX[k], sees_pacman(pac[1], pac[0], ghostY[k], ghostX[k]), 0);
            } else {
                //if pacman is undetected, moves the ghost in a random direction
                int ghostMoved = 0;
                while (ghostMoved == 0) {
                    //picks a random number from 0-3 (4 possible outcomes <-> 4 directions)
                    int dir = rand() % 4;
                    //checks that the selected direction is valid. If so, breaks the loop (function moves during check)
                    if (dir == 0) {
                        if (move_actor(&ghostY[k], &ghostX[k], UP, 0) == MOVED_OKAY) {
                            ghostMoved = 1;
                        }
                    } else if (dir == 1) {
                        if (move_actor(&ghostY[k], &ghostX[k], DOWN, 0) == MOVED_OKAY) {
                            ghostMoved = 1;
                        }
                    } else if (dir == 2) {
                        if (move_actor(&ghostY[k], &ghostX[k], LEFT, 0) == MOVED_OKAY) {
                            ghostMoved = 1;
                        }
                    } else if (dir == 3) {
                        if (move_actor(&ghostY[k], &ghostX[k], RIGHT, 0) == MOVED_OKAY) {
                            ghostMoved = 1;
                        }
                    }

                }
            }
        }

        //checks for loss
        if (check_loss(pac[1], pac[0], ghostY, ghostX)==YOU_LOSE){
            gameState=2;
        }

        //checks for win
        if((check_win(pac[1], pac[0], ghostY, ghostX) != KEEP_GOING) && (check_loss(pac[1], pac[0], ghostY, ghostX)!= YOU_LOSE)){
            gameState=1;
        }

        //places characters at appropriate positions
        map[((pac[1]) * width) + pac[0]] = PACMAN;
        map[((ghostY[0]) * width) + ghostX[0]] = GHOST;
        map[((ghostY[1]) * width) + ghostX[1]] = GHOST;
        //prints the map with updated positions
        print_map(map, &height, &width);
    }
//    //prints one last time with final positions
//    //places characters at appropriate positions
//    map[((pac[1]) * width) + pac[0]] = PACMAN;
//    map[((ghostY[0]) * width) + ghostX[0]] = GHOST;
//    map[((ghostY[1]) * width) + ghostX[1]] = GHOST;
//    //prints the map with updated positions
//    print_map(map, &height, &width);
    endgame:

    if(gameState==1){
        printf("Congratulations! You win!\n");
        return YOU_WIN;
    }
    if(gameState==2){
        printf ("Sorry, you lose.\n");
        return YOU_LOSE;
    }
}

