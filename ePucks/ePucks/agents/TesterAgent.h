#ifndef TAGENT_H
#define TAGENT_H
#include "Agent.h"

class TesterAgent : public Agent
{
private:
	int roundNo;
	bool passedTest;
protected:
	Action play(Agent* opp, int time);
	void result(Action action, int _score, Agent* opp);
public:
	TesterAgent();
	~TesterAgent();
};

#endif
