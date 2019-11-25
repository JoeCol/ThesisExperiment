//
//  RLEmotional.hpp
//  RLAgents
//
//  Created by Joe Collenette on 05/12/2016.
//  Copyright © 2016 Joe Collenette. All rights reserved.
//

#ifndef RLEmotional_hpp
#define RLEmotional_hpp

#include <map>
#include "Agent.h"
#include <vector>
#include <deque>

enum StateMode {SM_NONE, SM_AGENT, SM_ALL};
enum MoodAffectMode {MA_NONE, MA_FIXED, MA_MOOD, MA_SARSA};

class RLEmotional : public Agent
{
    class State
    {
    public:
        Agent* agent;
        MoodType m;
        State(Agent* a, MoodType mood);
    };

private:
    double epsilon;
    double moodAffect;
    MoodAffectMode mam;
    double mood;
    StateMode stateVersion;
    std::vector<State*> states;
    std::map<std::pair<State*, Action>, double> QStates;
    std::map<std::pair<State*, Action>, std::deque<int>> Memory;
    void learn(std::pair<State*, Action>  a, int reward);
    double estimateFutureRewards(std::pair<State*, Action>  a);
    State* getState(Agent* a, MoodType m);
    std::pair<State*, Action> newState(Agent* a, MoodType m, int score, Action action);
    bool stagHunt;
public:
    RLEmotional(StateMode useStates, bool StagHunt, MoodAffectMode _mam, double fixedAmount);
    ~RLEmotional();
    Action play(Agent* opp, int time);
    void moodUpdate(int score, double average, int oppScore, double oppAverage);
    void updateScores(int _Score);
    void result(Action action, int _Score, Agent* opp, int oppscore);
    MoodType getMoodType();
};




#endif /* RLEmotional_hpp */
