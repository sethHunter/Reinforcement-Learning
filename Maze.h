#ifndef MAZE_H
#define MAZE_H

#include "State.h"

class Maze
{
    private:
        int actions = 0, states = 0;
        int changeFactor = .0001;
        int gamma = 1;
        int allowedGrowth = 0;

        vector<State> maze;
        void generateUtil(); // generate util
        float newUtil(int s);
        float newReinLearnUtil(int s);

    public:
        Maze(const string& filename, int allowedG);
        ~Maze();
        void printMaze();
        void optimalPolicy();
        void printUtil();
        
        void reinforcementLearning();
        void crunchData();
        void reinLearnGenerateUtil();
        void optimalReinLearnPolicy();
};

#endif