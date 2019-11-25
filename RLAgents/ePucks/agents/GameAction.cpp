//
//  GameAction.cpp
//  RLAgents
//
//  Created by Joe Collenette on 06/12/2016.
//  Copyright © 2016 Joe Collenette. All rights reserved.
//

#include <stdio.h>
#include "GameAction.h"

int numOfCoops;
int numOfDefects;
int numOfNonmutual;
int totalGames;

std::pair<int, int> getGamePayoff(Action a, Action b, bool StagHunt)
{
    std::pair<int, int> toRet;
    if (!StagHunt)
    {
        
        if (a == b && a == COOP)
        {
            toRet.first = 3;
            toRet.second = 3;
            numOfCoops++;
        }
        else if (a == b && b == DEFECT)
        {
            toRet.first = 1;
            toRet.second = 1;
            numOfDefects++;
        }
        else if (a == DEFECT)
        {
            toRet.first = 5;
            toRet.second = 0;
            numOfNonmutual++;
        }
        else
        {
            toRet.first = 0;
            toRet.second = 5;
            numOfNonmutual++;
        }
    }
    else
    {
        if (a == b && a == COOP)
        {
            toRet.first = 3;
            toRet.second = 3;
            numOfCoops++;
        }
        else if (a == b && b == DEFECT)
        {
            toRet.first = 1;
            toRet.second = 1;
            numOfDefects++;
        }
        else if (a == DEFECT)
        {
            toRet.first = 2;
            toRet.second = 0;
            numOfNonmutual++;
        }
        else
        {
            toRet.first = 0;
            toRet.second = 2;
            numOfNonmutual++;
        }
    }
    totalGames++;
    return toRet;
}

double getTotalFor(Action a, bool mutual)
{
    if (mutual)
    {
        switch (a) {
            case COOP:
                return numOfCoops;
                break;
            case DEFECT:
                return numOfDefects;
                break;
        }
    }
    else
    {
        return numOfNonmutual;
    }
}

double getTotalGames()
{
    return totalGames;
}

void resetTotals()
{
    numOfCoops = 0;
    numOfDefects = 0;
    numOfNonmutual = 0;
    totalGames = 0;
}
