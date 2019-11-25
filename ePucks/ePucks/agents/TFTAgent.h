#ifndef TFTAGENT_H
#define TFTAGENT_H
#include "Agent.h"
class TFTAgent :
	public Agent
{
protected:
	Action play(Agent* opp, int time);
	void result(Action action, int _Score, Agent* opp);
public:
	TFTAgent(Action inital);
	~TFTAgent();
};

#endif
