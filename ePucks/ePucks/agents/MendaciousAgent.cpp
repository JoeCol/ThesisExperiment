#include "MendaciousAgent.h"


Action MendaciousAgent::play(Agent* opp, int time)
{
	numOfGames++;
	return prev;
}

void MendaciousAgent::result(Action action, int _score, Agent* opp)
{
	score += _score;
}

MendaciousAgent::MendaciousAgent() : Agent(DEFECT, "Mendacious")
{
}


MendaciousAgent::~MendaciousAgent()
{
}
