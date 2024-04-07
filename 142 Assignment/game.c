// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include "defines.h"
#include "game.h"

extern char * map, * dot_map;
extern int height;
extern int width;

int check_win(int pacman_y, int pacman_x, int ghosts_y[NUM_GHOSTS], int ghosts_x[NUM_GHOSTS]) {
    //scans the entire dot map for dots
    for(int i = 0; i<(width*height+1); i++){
        //if even a single dot is left, the game is not yet finished
        //(unless pacman is being munched on)
        if(dot_map[i] == DOT){
            return KEEP_GOING;
        }
    }

    //if the for loop concludes without finding any dots, there must be none left
    //therefore, a win has been achieved
    return YOU_WIN;
}

int check_loss(int pacman_y, int pacman_x, int ghosts_y[NUM_GHOSTS], int ghosts_x[NUM_GHOSTS]) {
    //idk why we would return "EATING_PACMAN" in ghost.c and not do anything with it
    //then check again here, but I'll roll with it...

    //checks both ghost coords
    for (int i=0; i<NUM_GHOSTS; i++) {
        //if pacman is on the same coords as a ghost, he is not having a good time
        if ((pacman_x == ghosts_x[i]) && (pacman_y == ghosts_y[i])) {
            return YOU_LOSE;
        }
    }

    //if both (all, technically, as this scales) ghosts are checked and are not overlapped with pacman,
    //pacman is safe to continue gobbling dots
    return KEEP_GOING;

}

int numberOfActors(char *map) {
    //scans the entire map array to verify the number of each type of actor (pacman and ghosts)

    //temporary variables to track the number of each actor identified
    int pacCount = 0;
    int ghostCount = 0;

    //scans the entire array for actor characters
    for (int i = 0; i < height * width; i++) {
        if (map[i] == PACMAN) {
            pacCount++;
        } else if (map[i] == GHOST) {
            ghostCount++;
        }
    }

    //if there is a mismatch between the expected number of actors and those in play, returns an error
    if (pacCount != 1) {
        return ERR_NO_PACMAN;
    }
    if (ghostCount != 2) {
        return ERR_NO_GHOSTS;
    }

    //if no mismatches occurred, this line will be reached and return a clean result
    return NO_ERROR;
}
