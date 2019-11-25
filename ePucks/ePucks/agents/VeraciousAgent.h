#ifndef VAGENT_H
#define VAGENT_H
#include "Agent.h"

class VeraciousAgent :
	public Agent
{
protected:
	Action play(Agent* opp, int time);
	void result(Action action, int _score, Agent* opp);
public:
	VeraciousAgent();
	~VeraciousAgent();
};

#endif
