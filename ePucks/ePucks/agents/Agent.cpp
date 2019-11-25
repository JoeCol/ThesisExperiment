#include "Agent.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <limits.h>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

void Agent::setAgentName(string aName)
{
	agentName = aName;
    prevX = INT_MAX;
}

Agent::Agent(Action _inital, string _agentName)
{
	agentName = _agentName;
	prev = _inital;
	score = 0;
	numOfGames = 0;
	GUID = boost::uuids::random_generator()();
    prevX = INT_MAX;
}

void Agent::addDistance(int x, int y)
{
    if (prevX != INT_MAX)
    {
        distance += sqrt((prevX - x)*(prevX - x) + (prevY - y)*(prevY - y));
    }
    prevX = x;
    prevY = y;
}

double Agent::getTotalDistance()
{
    return distance;
}

std::string Agent::getGuid()
{
	return boost::uuids::to_string(GUID);
}

Action Agent::getInital()
{
	return prev;
}

void Agent::setInitial(Action _inital)
{
	prev = _inital;
}

int Agent::getTotalScore()
{
	return score;
}

string Agent::getAgentName()
{
	return agentName;
}

int Agent::getTolerance()
{
	return 0;
}

int Agent::getResponce()
{
	return 0;
}

int Agent::getRLevel()
{
	return 0;
}

void Agent::addHighestScorer(Agent * agent)
{
}

int Agent::getNumberOfGames()
{
	return numOfGames;
}

double Agent::getAverageScore()
{
	if (score != 0 && numOfGames != 0)
	{
		return (double)score / (double)numOfGames;
	}
	else
	{
		return 0.0;
	}
}

Agent::~Agent()
{

}

void Agent::moodUpdate(int score, double average, int oppScore, double oppAverage)
{
	mood = INT_MIN;
}

double Agent::getMood()
{
	return mood;
}
