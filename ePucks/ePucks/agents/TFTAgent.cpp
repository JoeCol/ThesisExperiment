
#include "TFTAgent.h"


Action TFTAgent::play(Agent* opp, int time)
{
	numOfGames++;
	return prev;
}

void TFTAgent::result(Action action, int _Score, Agent* opp)
{
	prev = action;
	score += _Score;
}

TFTAgent::TFTAgent(Action inital) : Agent(inital, "Tit for Tat")
{
}


TFTAgent::~TFTAgent()
{
}
