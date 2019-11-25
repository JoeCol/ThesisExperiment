//
//  RLEmotional.cpp
//  RLAgents
//
//  Created by Joe Collenette on 05/12/2016.
//  Copyright © 2016 Joe Collenette. All rights reserved.
//

#include "RLEmotional.hpp"
#include <math.h>
#include <stdlib.h>

RLEmotional::State::State(Agent* a, MoodType mood)
{
    agent = a;
    m = mood;
}

RLEmotional::State* RLEmotional::getState(Agent* a, MoodType m)
{
    for (int i = 0; i < states.size(); i++)
    {
        if (states[i]->agent == a)
        {
            if (stateVersion == SM_AGENT)
            {
                return states[i];
            }
            else if (m == states[i]->m)
            {
                return states[i];
            }
        }
    }
    return NULL;
}

RLEmotional::RLEmotional(StateMode useState, bool StagHunt, MoodAffectMode _mam, double fixedAmount) : Agent(DEFECT, "RLEmotional")
{
    mood = 50;
    mam = _mam;
    moodAffect = fixedAmount;
    stateVersion = useState;
    stagHunt = StagHunt;
    epsilon = 0.1;
}

std::pair<RLEmotional::State*, Action> RLEmotional::newState(Agent *a, MoodType m, int score, Action action)
{
    std::pair<State*, Action> c;
    State* st;
    switch (stateVersion) {
        case SM_AGENT:
            st = new State(a, MT_NEUTRAL);
            break;
        case SM_ALL:
            st = new State(a, m);
            break;
        case SM_NONE:
            st = new State(NULL, MT_NEUTRAL);
            break;
    }
    c.first = st;
    c.second = action;
    QStates[c] = score;
    states.push_back(st);
    return c;
}

Action RLEmotional::play(Agent* opp, int time)
{
    Action todo;
    State* s;
    if (stateVersion == SM_NONE)
    {
        s = getState(NULL, MT_NEUTRAL);
    }
    else
    {
        s = getState(opp, opp->getMoodType());
    }
    if (s != NULL)
    {
        double change = epsilon;
        
        std::pair<State*, Action> c;
        c.first = s;
        c.second = COOP;
        std::pair<State*, Action> d;
        d.first = s;
        d.second = DEFECT;
        double r = (rand() % 100) / 100.0;
        if (QStates[d] > QStates[c])
        {
            if (mood > 70.0 && mam == MA_FIXED)
            {
                change = moodAffect;
            }
            else if (mood > 70.0 && mam == MA_MOOD)
            {
                change = ((mood - 50.0) / 100.0);
            }
            todo = r < (1 - change) ? DEFECT : COOP;
        }
        else if (QStates[d] < QStates[c])
        {
            if (mood < 30.0 && mam == MA_FIXED)
            {
                change = moodAffect;
            }
            else if (mood < 30.0 && mam == MA_MOOD)
            {
                change = ((50.0 - mood) / 100.0);
            }
            todo = r < (1 - change) ? COOP : DEFECT;
        }
        else
        {
            r = (rand() % 100) / 100.0;
            todo = r > 0.5 ? COOP : DEFECT;
        }
    }
    else
    {
        double r = (rand() % 100) / 100.0;
        todo = r > 0.5 ? COOP : DEFECT;
    }
    return todo;
}

MoodType RLEmotional::getMoodType()
{
    if (mood > 70)
    {
        return MT_HIGH;
    }
    else if (mood < 30)
    {
        return MT_LOW;
    }
    return MT_NEUTRAL;
}

void RLEmotional::result(Action action, int _Score, Agent* opp, int oppscore)
{
    State* s;
    if (stateVersion != SM_NONE)
    {
        s = getState(opp, opp->getMoodType());
    }
    else
    {
        s = getState(NULL, MT_NEUTRAL);
    }
    if (s != NULL)
    {
        std::pair<State*, Action> c;
        c.first = s;
        c.second = action;
        
        if (Memory[c].size() > 20)
        {
            Memory[c].pop_front();
        }
        Memory[c].push_back(_Score);
        
        
        moodUpdate(_Score, getAverageScore(), oppscore, opp->getAverageScore());
        learn(c, _Score);
    }
    else
    {
        std::pair<State*, Action> c;
        c = newState(opp, opp->getMoodType(), _Score, action);
        
        if (Memory[c].size() > 20)
        {
            Memory[c].pop_front();
        }
        Memory[c].push_back(_Score);
        
        moodUpdate(_Score, getAverageScore(), oppscore, opp->getAverageScore());
    }
}

void RLEmotional::updateScores(int _Score)
{
    numOfGames++;
    score += _Score;
}

void RLEmotional::moodUpdate(int score, double average, int oppScore, double oppAverage)
{
    double ab = (100 - mood) / 100.0;
    double u = average - ((ab * std::max(oppAverage - average, 0.0) - (ab * std::max(average - oppAverage, 0.0))));
    double dif = (score - average) + u;
    
    mood = std::min(99.9999, (mood + dif));
    mood = std::max(0.0001, mood);
}

void RLEmotional::learn(std::pair<State*, Action> a, int reward)
{
    if (mam == MA_SARSA)
    {
        QStates[a] += 0.1 * (reward + (0.95 * QStates[a]) - QStates[a]);
    }
    else
    {
        QStates[a] += 0.1 * (reward + (0.95 * estimateFutureRewards(a)) - QStates[a]);
    }
}

double RLEmotional::estimateFutureRewards(std::pair<State*, Action> a)
{
    double percentToLookAt = (100.0 - mood) / 100.0;
    int actualAmount = ceil(Memory[a].size() * percentToLookAt);
    
    int sum = 0;
    for (int i = actualAmount - 1; i >= 0; i--)
    {
        sum += Memory[a][i];
    }
    return sum / (double)actualAmount;
    
}

RLEmotional::~RLEmotional()
{
    
}
