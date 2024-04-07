// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include "defines.h"
#include "ghost.h"
#include "map.h"
#include <math.h>

char sees_pacman(int pacman_y, int pacman_x, int ghost_y, int ghost_x) {
    //first checks if pacman is actively on the same spot as a ghost (getting nibbled)
    if((pacman_y==ghost_y)&&pacman_x==ghost_x){
        return EATING_PACMAN;
    }

    //checks if y coords align (might see horizontally)
    if(pacman_y==ghost_y){
        //takes the difference between the coords to determine direction
        int coordDiff = pacman_x-ghost_x;
        //checks all spaces between characters
        for (int i = 0; i < abs(coordDiff) - 2; i++) {
            //checks for wall on next spot (eventually checks all spaces between).
            // Multiplies by difference /abs of difference to take sign
            if(is_wall(pacman_y, ghost_x + (i*(coordDiff/abs(coordDiff)))) == WALL){
                return SEES_NOTHING;
            }
        }
        //if for loop is cleared (hasn't returned "SEES_NOTHING", returns direction based off of sign
        if(coordDiff>0){
            return RIGHT;
        }
        else{
            return LEFT;
        }
    }
    //literally a copy/paste of the above code with x and y reversed. Wish I could compress this more,
    //but with the way the variables are set up for GradeScope, it's not worth it
    if(pacman_x==ghost_x){
        //takes the difference between the coords to determine direction
        int coordDiff = pacman_y-ghost_y;
        //checks all spaces between characters
        for (int i = 0; i < abs(coordDiff) - 2; i++) {
            //checks for wall on next spot (eventually checks all spaces between).
            // Multiplies by difference /abs of difference to take sign
            if(is_wall(ghost_y+(i*(coordDiff/abs(coordDiff))), ghost_x) == WALL){
                return SEES_NOTHING;
            }
        }
        //if for loop is cleared (hasn't returned "SEES_NOTHING", returns direction based off of sign
        if(coordDiff>0){
            return DOWN;
        }
        else{
            return UP;
        }

    }

    //if there is no alignment in coords, cannot see by default
    return SEES_NOTHING;
}

