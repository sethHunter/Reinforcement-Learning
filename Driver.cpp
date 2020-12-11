#include "Maze.h"

int main (int argc, char *argv[])
{
    string file = argv[1];
    int allowedG = atoi(argv[2]);
    Maze maze(file, allowedG); //this reads in and setups everything up including calling the generateUtil function
    maze.optimalPolicy(); 
    maze.printUtil();

    maze.reinforcementLearning();
    maze.crunchData();
    maze.optimalReinLearnPolicy();
    maze.printUtil();

    return 0;
}