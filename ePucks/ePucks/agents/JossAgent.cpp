#include "JossAgent.h"
#include <stdlib.h>

Action JossAgent::play(Agent* opp, int time)
{
	numOfGames++;
	if ((rand() % 10) < 1)
	{
		return DEFECT;
	}
	return prev;
}

void JossAgent::result(Action action, int _Score, Agent* opp)
{
	prev = action;
	score += _Score;
}

JossAgent::JossAgent(Action initial) : Agent(initial, "Joss Agent")
{
}


JossAgent::~JossAgent()
{
}
