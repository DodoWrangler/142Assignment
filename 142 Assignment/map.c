// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"
#include "colours.h"
#include "map.h"

extern char *map, *dot_map;
extern int width, height;

int move_actor(int * y, int * x, char direction, int eat_dots) {
    //takes the character present at the subject coordinates in a temporary tracker
    char temp = map[((*y) * width) + *x];
    //sets a pair of variables, so I can just write the direction checks once
    int xDir, yDir;
    //based off of the direction requested, sets x and y directions accordingly
    if(direction==UP){
        xDir=0;
        yDir=-1;
    }else if(direction==DOWN){
        xDir=0;
        yDir=1;
    }else if(direction==LEFT){
        xDir=-1;
        yDir=0;
    }else{
        xDir=1;
        yDir=0;
    }
    //checks if attempting to move off of the map (exceeding boundaries) and if next move is a wall
    //this statement devolved quickly...
    if(((width-1)<(*x+xDir))||((*x+xDir)<0)||((height-1)<(*y+yDir))||((*y+yDir)<0)){
        return MOVED_INVALID_DIRECTION;
    } else if((is_wall(*y+yDir, *x+xDir)==WALL)){
        return MOVED_WALL;
    }else{
            //if the prior statement was cleared, character is good to move
            //updates coordinates based off of variables assigned by direction earlier
            map[((*y) * width) + *x] = dot_map[((*y) * width) + *x];
            *x = *x + xDir;
            *y = *y + yDir;

            //if eating dots (ie pacman is the actor currently munching through this fcn), updates the maps
            if(eat_dots==1){
                //replaces dot in the space just moved to with a blank
                dot_map[((*y) * width) + *x] = EMPTY;
            }
    }
    return MOVED_OKAY;
}

//function for use in other fcns
int is_wall(int y, int x) {
    //checks the coords given to see if it's a wall
    if (map[(y * width) + x] == 'W') {
        return WALL;
    }
    //if it wasn't a wall... it's not a wall.
    return NOT_WALL;
}

//fcn to load the map the FIRST TIME from a given file
char * load_map(char * filename, int *map_height, int *map_width) {
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    //checks if file opened sucessfully; if not, will return error on next check
    if(fp==NULL){
        return NULL;
    }

    //allocates memory to the map array safely
    char *tempPoint = (char *) malloc((*map_height * *map_width) * sizeof(char *) + sizeof(char *));
    //checks that the temp pointer was properly created and data transferred
    if (tempPoint != NULL) {
        //sets map to the data present at the temporary pointer
        map = tempPoint;
        //wipes the memory at the old temp pointer
        tempPoint = NULL;
    } else {
        //an error occurred if this code activates, thus memory is freed and error code will be returned on next check
        free(map);
        fclose(fp);
        return NULL;
    }

    //scans the contents of the file for all characters, stored in the map array
    for (int i = 0; i <= *map_height * *map_width - 1; ++i) {
        fscanf(fp, "%c  ", &map[i]);
    }
    //terminates with null character, forming proper string
    map[(*map_width * *map_height)] = '\0';

    //closes the file to save memory
    fclose(fp);
    //if the fcn made it this far, returns the map array
    return map;

}

void print_map(char* mapArray, int* mapHeight, int* mapWidth){
    //changes to wall colour
    change_text_colour(BLUE);
    //prints top map cap
    for(int i=0;i<width+2;i++){
        printf("W ");
    }
    printf("\n");
    //prints body of map, first for loop cycles through each row
    for(int i=0; i<=*mapHeight-1; i++){
        //prints left wall
        change_text_colour(BLUE);
        printf("W ");
        //prints each column entry of current row
        for (int j=0; j<*mapWidth; j++){
            //color shifts depending on the character about to be printed

            //creates temporary character tracker. Makes it easier for me to come in and change things :)
            char nextPrint = mapArray[((*mapHeight)*(i))+(j)];
            if(nextPrint == '.'){
            change_text_colour(WHITE);
            } else if(nextPrint == 'G'){
                change_text_colour(PINK);
            } else if(nextPrint == 'P'){
                change_text_colour(YELLOW);
            }else{
                change_text_colour(BLUE);
            }
            printf("%c ", nextPrint);
        }
        //end wall and next row
        change_text_colour(BLUE);
        printf("W\n");
    }
    //prints bottom map cap
    change_text_colour(BLUE);
    for(int i=0;i<width+2;i++){
        printf("W ");
    }
    printf("\n");
}

//fcn designed to find the dimensions of a given map text file
int findDims(char * fileName){

    FILE *fp = NULL;
    //opens the text file and attaches to temp pointer
    fp = fopen(fileName, "r");
    //checks if file opened sucessfully; if not, will return error on next check
    if(fp==NULL){
        return ERR_NO_MAP;
    }

    //creates a map array starting at 1 character
    char *map = (char *) malloc(1 * sizeof(char *));

    //active coordinates
    int x = 0, y = 0;
    int counter = 0;
    //variables given a neutral character to remove artifacts
    //temporary tracker for holding a single character of the txt file
    char temp = ' ';
    width = -1;

    while (1) {
        //check next character, hold at temp variable
        fscanf(fp, "%c", &temp);
        //checks the content currently held in temp and alters coords to match results
        //does NOT just increase coord for any character - done to avoid artifacts/spacing dependency
        if ((x) == width && temp != '\n') {
            height = (y + 1);
            break;
        } else if (temp == '\n') {
            width = x;
            map = (char*) realloc(map, counter*sizeof(char));
            x = 0;
            y++;
        } else if (temp == DOT || temp == WALL || temp == GHOST || temp == PACMAN) {
            counter++;
            map = (char*) realloc(map, counter * sizeof(char));
            x++;
        }
    }
    //extra space for null termination
    map = (char*) realloc(map, (counter+1)*sizeof(char));

    //closes file to save memory
    fclose(fp);
    return 0;
}

//fcn to create the dot map
//because relevant variables are global, does not need input
char* createDotMap(){

    //safely reserves memory for the dot map and copies it to the map
    dot_map = (char *) (char **) malloc(width * sizeof(char *));
    for (int i = 0; i < (width*height); i++) {
        if (map[i] == GHOST) {
            dot_map[i] = DOT;
        } else if (map[i] == PACMAN) {
            dot_map[i] = EMPTY;
        }else {
            dot_map[i] = map[i];
        }
    }
}

int checkMap(char* array){
    //checks both map and dot map to ensure that they are not null
    if(array==NULL){
        return ERR_NO_MAP;
    }
}
