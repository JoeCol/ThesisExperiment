#ifndef EAGENT_H
#define EAGENT_H
#include "Agent.h"
#include <map>

class EmotionalAgent :
	public Agent
{
private:
	int initalTolerance;
	int initalResponce;

	int rLevel;
	bool useMood;
    bool useTime;
    double timeAffect;
    int prevTime;
	
	int tolerance;
	int responce;

	//int anger;
	//int gratitude;

	std::map<Agent*, int> winnerMemory;
	std::map<Agent*, int> angerMemory;
	std::map<Agent*, int> gratitudeMemory;
	std::map<Agent*, Action> agentActions;
	void checkReplicate(Agent* opp);
protected:
	Action play(Agent* opp, int time);
	void result(Action action, int _Score, Agent* opp);
public:
	EmotionalAgent(int _charNum, Action inital);
	EmotionalAgent(int _charNum, Action inital, int _rLevel, int _mood, double _timeAffect);
	EmotionalAgent(int _charNum, Action inital, int _rLevel);
	~EmotionalAgent();

	void setCharType(int type);
	int getInitialCharNum();

	int getTolerance();
	int getResponce();
	int getRLevel();
	int getCharNum();
	void moodUpdate(int score, double average, int oppScore, double oppAverage);
	double getMood();
};

#endif
