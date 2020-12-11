#ifndef STATE_H
#define STATE_H

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <math.h>
using namespace std;

struct State
{
    bool terminal = false;
    float util;
    float reward;
    int policy = 0; // 1 = up 2 = left 3 = down = 4 = right
    vector<vector<float>> transistionValues;
    vector<vector<float>> data; // used during exploration to store the state a action puts you in
    /*
    action a; state x state y state z
    where x, y, z is the amount of times you ended up in state x, y, z given that you took action a
    */
   vector<vector<float>> AItransistionValues; // used after finishing exploration
};

#endif