#include "VeraciousAgent.h"


Action VeraciousAgent::play(Agent* opp, int time)
{
	numOfGames++;
	return prev;
}

void VeraciousAgent::result(Action action, int _score, Agent* opp)
{
	score += _score;
}

VeraciousAgent::VeraciousAgent() : Agent(COOP, "Veracious")
{
}


VeraciousAgent::~VeraciousAgent()
{
}
