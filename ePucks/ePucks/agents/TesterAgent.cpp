#include "TesterAgent.h"

Action TesterAgent::play(Agent* opp, int time)
{
	numOfGames++;
	if (passedTest)
	{
		return prev;
	}
	else
	{
		if (roundNo == 1)
		{
			return DEFECT;
		}
		else
		{
			return COOP;
		}
	}
	
}

void TesterAgent::result(Action action, int _score, Agent* opp)
{
	prev = action;
	score += _score;
	passedTest = passedTest ? true : prev == DEFECT && roundNo == 1;
	roundNo = (roundNo % 3) + 1;
	
	
}

TesterAgent::TesterAgent() : Agent(DEFECT, "Tester")
{
	roundNo = 1;
	passedTest = false;
}


TesterAgent::~TesterAgent()
{
}
