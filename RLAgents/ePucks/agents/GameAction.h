//
//  GameAction.h
//  RLAgents
//
//  Created by Joe Collenette on 05/12/2016.
//  Copyright © 2016 Joe Collenette. All rights reserved.
//

#ifndef GameAction_h
#define GameAction_h
#include <vector>
enum Action {DEFECT = 0, COOP = 1};

std::pair<int, int> getGamePayoff(Action a, Action b, bool StagHunt);
double getTotalFor(Action a, bool mutual);
double getTotalGames();
void resetTotals();

#endif /* GameAction_h */
