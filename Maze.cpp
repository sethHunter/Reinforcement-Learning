#include "Maze.h"

Maze::Maze(const string& filename, int allowedG)
{
    srand(static_cast <unsigned int> (time(0)));
    allowedGrowth = allowedG;

    ifstream stream; // stream for the reinforcement
    ifstream stateStream; // stream for state files
    stream.open(filename);
    if(stream.is_open())
    {
        string temp, stateFile, stateTemp;

        stream >> actions;
        getline(stream, temp);
        stream >> states;
        getline(stream, temp);
        stream >> gamma;

        //reisze all vectors to match the problem
        maze.resize(states);
        for (size_t s = 0; s < states; s++)
        {
            maze.at(s).transistionValues.resize(actions);
            maze.at(s).data.resize(actions);
            maze.at(s).AItransistionValues.resize(actions);
            for (size_t x = 0; x < actions; x++)
            {
                maze.at(s).transistionValues.at(x).resize(states);
                maze.at(s).data.at(x).resize(states);
                maze.at(s).AItransistionValues.at(x).resize(states);
            }
        }
        
        // Load in the state file informaation
        for (size_t s = 0; s < states; s++)
        {
            getline(stream, temp);
            stream >> maze.at(s).reward;

            getline(stream, temp);
            stream >> maze.at(s).terminal;
            if(maze.at(s).terminal == 1)
                maze.at(s).util = maze.at(s).reward;

            getline(stream, temp);
            stream >> stateFile;
            //cout << endl << endl << "--------------------------------------State " << s << " File " << stateFile << endl << endl;

            if(maze.at(s).terminal != 1)
            {
                stateStream.open(stateFile);
                if (stateStream.is_open())
                {
                    for (size_t x = 0; x < actions; x++)
                    {
                        getline(stateStream, stateTemp);
                        for (size_t y = 0; y < states; y++)
                        {
                            stateStream >> maze.at(s).transistionValues.at(x).at(y);
                            //cout << s << " " << x << " " << y << " " << maze.at(s).transistionValues.at(x).at(y) << endl;
                        }
                    }
                    stateStream.close();
                } else
                {
                    cout << "failed to read in" << endl;
                }
            }
        }
        stream.close();
        generateUtil();
    }else 
    {
        cout << "Failed to read in";
    }
}

Maze::~Maze(){}

void Maze::printUtil()
{
    cout << endl << endl;
    for (size_t s = 0; s < states; s++)
    {
        cout << "State " << s+1 << " Policy ";
        switch (maze.at(s).policy)
        {
        case 1:
            cout << "up      ";
            break;
        case 2:
            cout << "left     ";
            break;
        case 3:
            cout << "down    ";
            break;
        case 4:
            cout << "right   ";
            break;
        case 0:
            cout << "terminal";
            break;
        } 
        cout << " Utility " << floor(maze.at(s).util * 1000.0) / 1000.0 << " " << endl;
    }
}

void Maze::generateUtil()
{
    bool noChange = false;
    while(!noChange)
    {
        noChange = true;
        for(int s = 0; s < states; ++s)
        {
            if (1 != maze.at(s).terminal)
            {
                float oldUtil = maze.at(s).util;
                maze.at(s).util = newUtil(s);
                if ((maze.at(s).util - oldUtil) > changeFactor)
                    noChange = false;
            }
        }
    }
}

/*
this returns the max of up, down, left, right 
*/
float Maze::newUtil(int s)
{
    float maxUtil = -1, util = 0;
    for (size_t x = 0; x < actions; x++)
    {
        util = 0;
        for (size_t y = 0; y < states; y++)
        {
            if (0 != maze.at(s).transistionValues.at(x).at(y))
            {
                //cout << "State " << s+1 << " " << x << "," << y << " has transistuon value " << maze.at(s).transistionValues.at(x).at(y) << " state util is " <<  maze.at(y).util << endl;
                util += maze.at(s).transistionValues.at(x).at(y) * maze.at(y).util;
            }
        }

        //if this util is better then maxUtil then assign it
        if (maxUtil < util)
        {
            maxUtil = util;
        }
    }
    return  maze.at(s).reward + gamma * maxUtil;
}

void Maze::optimalPolicy()
{
    for (size_t s = 0; s < states; s++)
    {
        float tempUtil = 0, maxUtil = 0;
        int tempPolicy, maxPolicy;
        if(maze.at(s).terminal != 1)
        {
            for (size_t x = 0; x < actions; x++)
            {
                tempUtil = 0;
                for (size_t y = 0; y < states; y++)
                {
                    tempUtil += maze.at(y).util * maze.at(s).transistionValues.at(x).at(y);
                    tempPolicy = x + 1;
                }
                if(tempUtil > maxUtil)
                {
                    maxUtil = tempUtil;
                    maxPolicy = tempPolicy;
                }
            }
            maze.at(s).policy = maxPolicy;
        }
    }
}

void Maze::reinforcementLearning()
{
    for (size_t i = 0; i < allowedGrowth; i++)
    {
        int state = rand()%(states), action = rand()%(actions), y = rand()%(100), chance, newState;
        while(maze.at(state).terminal != 1)
        {
            action = rand()%(actions);
            y = rand()%(100);
            chance = 0;
            for (size_t s = 0; s < states; ++s)
            {
                if (0 != maze.at(state).transistionValues.at(action).at(s))
                {
                    chance += maze.at(state).transistionValues.at(action).at(s) * 100;
                    if (y < chance)
                    {
                        newState = s;
                        s = states;
                    }
                }
            }
            ++maze.at(state).data.at(action).at(newState);
            state = newState;
        }
    }
}

void Maze::crunchData()
{
    for (size_t s = 0; s < states; s++)
    {
        for (size_t x = 0; x < actions; x++)
        {
            int total = 0;
            for (size_t y = 0; y < states; y++)
            {
                total += maze.at(s).data.at(x).at(y);
            }

            for (size_t z = 0; z < states; z++)
            {
                maze.at(s).AItransistionValues.at(x).at(z) = maze.at(s).data.at(x).at(z) / total;
            }
        }
    }
    reinLearnGenerateUtil();
}

void Maze::reinLearnGenerateUtil()
{
    bool noChange = false;
    while(!noChange)
    {
        noChange = true;
        for(int s = 0; s < states; ++s)
        {
            if (1 != maze.at(s).terminal)
            {
                float oldUtil = maze.at(s).util;
                maze.at(s).util = newReinLearnUtil(s);
                if ((maze.at(s).util - oldUtil) > changeFactor)
                    noChange = false;
            }
        }
    }
}

float Maze::newReinLearnUtil(int s)
{
    float maxUtil = -1, util = 0;
    for (size_t x = 0; x < actions; x++)
    {
        util = 0;
        for (size_t y = 0; y < states; y++)
        {
            if (0 != maze.at(s).AItransistionValues.at(x).at(y))
            {
                //cout << "State " << s+1 << " " << x << "," << y << " has transistuon value " << maze.at(s).AItransistionValues.at(x).at(y) << " state util is " <<  maze.at(y).util << endl;
                util += maze.at(s).AItransistionValues.at(x).at(y) * maze.at(y).util;
            }
        }

        //if this util is better then maxUtil then assign it
        if (maxUtil < util)
        {
            maxUtil = util;
        }
    }
    return  maze.at(s).reward + gamma * maxUtil;
}

void Maze::optimalReinLearnPolicy()
{
    for (size_t s = 0; s < states; s++)
    {
        float tempUtil = 0, maxUtil = 0;
        int tempPolicy, maxPolicy;
        if(maze.at(s).terminal != 1)
        {
            for (size_t x = 0; x < actions; x++)
            {
                tempUtil = 0;
                for (size_t y = 0; y < states; y++)
                {
                    tempUtil += maze.at(y).util * maze.at(s).AItransistionValues.at(x).at(y);
                    tempPolicy = x + 1;
                }
                if(tempUtil > maxUtil)
                {
                    maxUtil = tempUtil;
                    maxPolicy = tempPolicy;
                }
            }
            maze.at(s).policy = maxPolicy;
        }
    }
}