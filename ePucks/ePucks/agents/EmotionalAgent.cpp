#include "EmotionalAgent.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#include <boost/lexical_cast.hpp>
#include "support.h"

void EmotionalAgent::checkReplicate(Agent* opp)
{
	double dopAverage = opp->getAverageScore();
	double dmyAverage = getAverageScore();
	
	int myAverage = (dmyAverage * 100.0);
	int opAverage = (dopAverage * 100.0);
	if (myAverage > opAverage)
	{
		winnerMemory[this]++;
		if (winnerMemory[this] >= rLevel)
		{
			winnerMemory.clear();
		}
	}
	else if (myAverage < opAverage)
	{
		winnerMemory[opp]++;
		if (winnerMemory[opp] >= rLevel)
		{
			responce = opp->getResponce();
			tolerance = opp->getTolerance();
			rLevel = opp->getRLevel();
			prev = opp->getInital();
			setAgentName(opp->getAgentName());
			winnerMemory.clear();
		}
	}
}

Action EmotionalAgent::play(Agent* opp, int time)
{
    if (useTime)
    {
        int timeDifference = time - prevTime;
        timeDifference /= 1000000; //from microseconds to seconds
        double moodAffected = timeDifference * timeAffect;
        if (mood > 50.0)
        {
            mood = std::max(mood - moodAffected, 50.0);
        }
        else if (mood < 50.0)
        {
            mood = std::min(mood + moodAffected, 50.0);
        }
        prevTime = time;
    }
	if (rLevel > 0 && numOfGames != 0 && numOfGames % 5 == 0)
	{
		checkReplicate(opp);
	}
	numOfGames++;
	if (agentActions.find(opp) == agentActions.end())
	{
		if (mood < 30.0 && useMood)
		{
			agentActions[opp] = DEFECT;
		}
		else if (mood > 70.0 && useMood)
		{
			agentActions[opp] = COOP;
		}
		else
		{
			agentActions[opp] = prev;
		}
	}
	if (mood > 90.0 && useMood)
	{
		agentActions[opp] = COOP;
	}
	else if (mood < 10.0 && useMood)
	{
		agentActions[opp] = DEFECT;
	}
	return agentActions[opp];
}

void EmotionalAgent::result(Action action, int _Score, Agent* opp)
{
	//prev = action;
	score += _Score;
	if (rLevel == 0)
	{
		return;
	}
	if (angerMemory.find(opp) == angerMemory.end())
	{
		angerMemory[opp] = 0;
	}
	if (gratitudeMemory.find(opp) == gratitudeMemory.end())
	{
		gratitudeMemory[opp] = 0;
	}
	if (action == COOP)
	{
		gratitudeMemory[opp]++;
	}
	else
	{
		angerMemory[opp]++;
	}

	if (tolerance <= angerMemory[opp] && agentActions[opp] != DEFECT)
	{
		angerMemory[opp] = 0;
		gratitudeMemory[opp] = 0;
		agentActions[opp] = DEFECT;
	}
	else if (responce <= gratitudeMemory[opp] && agentActions[opp] != COOP)
	{
		gratitudeMemory[opp] = 0;
		angerMemory[opp] = 0;
		agentActions[opp] = COOP;
	}
}

void EmotionalAgent::moodUpdate(int score, double average, int oppScore, double oppAverage)
{
	if (rLevel != 0)
	{
		double alpbet = (100 - mood) / 100.0;
		double u = average - ((alpbet * std::max(oppAverage - average, 0.0)) - (alpbet * std::max(average - oppAverage, 0.0)));
		double difScores = (score - average) + u;
		mood = std::min(99.9, (mood + difScores));
		mood = std::max(0.1, mood);
	}
	else
	{
		mood = INT_MIN;
	}
}

double EmotionalAgent::getMood()
{
	return mood;
}

EmotionalAgent::EmotionalAgent(int _charNum, Action inital) : Agent(inital,  "E")
{
	agentName += boost::lexical_cast<std::string>(_charNum);
	setCharType(_charNum);
	rLevel = 0;
	mood = 0;
	useMood = false;
    useTime = false;
}

EmotionalAgent::EmotionalAgent(int _charNum, Action inital, int _rLevel, int _mood, double _timeAffect) : Agent(inital,  "E")
{
	agentName += boost::lexical_cast<std::string>(_charNum);
	setCharType(_charNum);
	rLevel = _rLevel;
	mood = _mood;
	useMood = true;
    useTime = timeAffect != 0.0;
    prevTime = 0;
    timeAffect = _timeAffect;
}

EmotionalAgent::EmotionalAgent(int _charNum, Action inital, int _rLevel) : Agent(inital,  "E")
{
	agentName += boost::lexical_cast<std::string>(_charNum);
	setCharType(_charNum);
	rLevel = _rLevel;
	mood = 0;
	useMood = false;
}

EmotionalAgent::~EmotionalAgent()
{
}

int EmotionalAgent::getCharNum()
{
	int toReturn = -1;
	switch (tolerance)
	{
	case 1:
		toReturn = responce;
		break;
	case 2:
		toReturn = responce + 3;
		break;
	case 3:
		toReturn = responce + 6;
		break;
	}

	if (toReturn == -1)
	{
		std::cout << "Problem with number" << std::endl;
	}
	return toReturn;
}

void EmotionalAgent::setCharType(int type)
{
	tolerance = ceil(type / 3.0);
	responce = (type % 3) == 0 ? 3 : type % 3;
	switch (type)
	{
		case 1:
			tolerance = 1;
			responce = 1;
			break;
		case 2:
			tolerance = 1;
			responce = 2;
			break;
		case 3:
			tolerance = 1;
			responce = 3;
			break;
		case 4:
			tolerance = 2;
			responce = 1;
			break;
		case 5:
			tolerance = 2;
			responce = 2;
			break;
		case 6:
			tolerance = 2;
			responce = 3;
			break;
		case 7:
			tolerance = 3;
			responce = 1;
			break;
		case 8:
			tolerance = 3;
			responce = 2;
			break;
		case 9:
			tolerance = 3;
			responce = 3;
			break;
	}

	

	initalResponce = responce;
	initalTolerance = tolerance;

	angerMemory.clear();
	gratitudeMemory.clear();
}

int EmotionalAgent::getInitialCharNum()
{
	int toReturn = -1;
	switch (initalTolerance)
	{
	case 1:
		toReturn = initalResponce;
		break;
	case 2:
		toReturn = initalResponce + 3;
		break;
	case 3:
		toReturn = initalResponce + 6;
		break;
	}

	if (toReturn == -1)
	{
		std::cout << "Problem with number" << std::endl;
	}
	return toReturn;
}

int EmotionalAgent::getTolerance()
{
	return tolerance;
}

int EmotionalAgent::getResponce()
{
	return responce;
}

int EmotionalAgent::getRLevel()
{
	return rLevel;
}
