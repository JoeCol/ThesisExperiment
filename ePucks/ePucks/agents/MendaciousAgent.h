#ifndef MAGENT_H
#define MAGENT_H
#include "Agent.h"

class MendaciousAgent : public Agent
{
protected:
	Action play(Agent* opp, int time);
	void result(Action action, int _score, Agent* opp);
public:
	MendaciousAgent();
	~MendaciousAgent();
};

#endif
