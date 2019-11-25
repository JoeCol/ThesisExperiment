#ifndef AGENT_H
#define AGENT_H
#include <string>
#include <boost/uuid/uuid.hpp>
#include "GameAction.h"

enum MoodType {MT_LOW, MT_NEUTRAL, MT_HIGH};

class Agent
{
private:
    Action prev;
    double distance;
    std::string agentName;
    int prevX, prevY;
    boost::uuids::uuid GUID;
protected:
    int score;
    int numOfGames;
public:
	Agent(Action _inital, std::string _agentName);
	~Agent();	
	Action getInital();
	void setInitial(Action _inital);
	int getTotalScore();	
	std::string getAgentName();
	virtual Action play(Agent* opp, int time) = 0;
	virtual void result(Action action, int _score, Agent* opp, int oppscore) = 0;
    virtual MoodType getMoodType() = 0;
    
	int getNumberOfGames();
	double getAverageScore();
	std::string getGuid();
    void addDistance(int x, int y);
    
    double getTotalDistance();
    void reset();
};
#endif
