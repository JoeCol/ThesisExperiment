#ifndef JAGENT_H
#define JAGENT_H
#include "Agent.h"

class JossAgent : public Agent
{
protected:
	Action play(Agent* opp, int time);
	void result(Action action, int _Score, Agent* opp);
public:
	JossAgent(Action initial);
	~JossAgent();
};
#endif
