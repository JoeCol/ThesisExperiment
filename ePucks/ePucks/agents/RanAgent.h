#ifndef RAGENT_H
#define RAGENT_H
#include "Agent.h"

class RanAgent :
	public Agent
{
	Action play(Agent* opp, int time);
	void result(Action action, int _Score, Agent* opp);
public:
	RanAgent();
	~RanAgent();
};

#endif
