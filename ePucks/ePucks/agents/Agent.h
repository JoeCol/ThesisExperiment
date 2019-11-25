#ifndef AGENT_H
#define AGENT_H
#include <string>
#include <boost/uuid/uuid.hpp>

enum Action {DEFECT = 0, COOP = 1};

class Agent
{
private:
    
protected:
    int score;
    int numOfGames;
    Action prev;
    double mood;
    double distance;
    std::string agentName;
    int prevX, prevY;
    boost::uuids::uuid GUID;
	virtual void setAgentName(std::string aName);
public:
	Agent(Action _inital, std::string _agentName);
	~Agent();	
	Action getInital();
	void setInitial(Action _inital);
	int getTotalScore();	
	std::string getAgentName();
	virtual Action play(Agent* opp, int time) = 0;
	virtual void result(Action action, int _score, Agent* opp) = 0;

	virtual int getTolerance();
	virtual int getResponce();
	virtual int getRLevel();
	virtual void addHighestScorer(Agent* agent);
	virtual double getMood();
	virtual void moodUpdate(int score, double average, int oppScore, double oppAverage);
	int getNumberOfGames();
	double getAverageScore();
	std::string getGuid();
    void addDistance(int x, int y);
    double getTotalDistance();
};
#endif
