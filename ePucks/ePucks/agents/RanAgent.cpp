#include "RanAgent.h"
#include <stdlib.h>

Action RanAgent::play(Agent* opp, int time)
{
	numOfGames++;
	return (Action)(rand() % 2);
}

void RanAgent::result(Action action, int _Score, Agent* opp)
{
	score += _Score;
}

RanAgent::RanAgent() : Agent(COOP, "Random Agent")
{
}


RanAgent::~RanAgent()
{
}
