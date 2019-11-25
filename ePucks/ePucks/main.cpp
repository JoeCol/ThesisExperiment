#include <stdio.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <iomanip>
#include "playerSup.h"
#include "support.h"

#include "agents/EmotionalAgent.h"
#include "agents/JossAgent.h"
#include "agents/MendaciousAgent.h"
#include "agents/RanAgent.h"
#include "agents/TesterAgent.h"
#include "agents/TFTAgent.h"
#include "agents/VeraciousAgent.h"


std::vector<Agent*> createAgents(int numOfEmotionalRobots, int numOfNonEmotionalRobots, double highMoodPercent, double medMoodPercent, double highRepPercent, double medRepPercent, double initialDefectPer, int agentTypes[6], int emotionalTypes[9], bool useMood, bool onlyEmotionalInitial, double timeAffect);
std::map<boost::uuids::uuid, Agent*> assignAgents(std::vector<Agent*> agents, std::set<boost::uuids::uuid> IDs);
void printResults(std::map<boost::uuids::uuid, Agent*> epucks, std::string saveFolder, int scenarioNumber, int runNumber);

int main(int argc, char **argv)
{
    srand((int)time(NULL));
	std::pair<std::set<boost::uuids::uuid>, std::vector<ePuckPosition*> > results;
	std::vector<Agent*> agents;
	std::map<boost::uuids::uuid, Agent*> epucks;
    
    bool useMood = true;
    bool onlyEmotionalInitial = true;
    int numberOfEmotionalRobots = 144;
    int numberOfNonEmotionalRobots = 0;
    int simTimeMilli = 600000;
    int agentTypes[6] = {0,0,0,0,0,0};
    int emotionalTypes[9] = {0,0,0,0,0,0,0,0};
    int scenarioNumber = 0;
    int runNumber = 0;
    double highMoodPer = 0.33;
    double medMoodPer = 0.33;
    double highRepPer = 1;
    double medRepPer = 0;
    double initialDefectPer = 0.5;
    double timeAffect = 0.0;
    std::string worldFile = "";
    std::string arenaFilePath = "";
    std::string arenaName = "";
    std::string saveFolder = "";
    std::string matDirectory = "";
    
    std::cout << "Generating Stage Files" << '\r' << std::flush;
    getArguments(argc, argv, arenaFilePath, worldFile, arenaName, saveFolder, matDirectory, scenarioNumber, numberOfEmotionalRobots, numberOfNonEmotionalRobots, agentTypes, emotionalTypes, simTimeMilli, initialDefectPer, highRepPer, medRepPer, highMoodPer, medMoodPer, useMood, onlyEmotionalInitial, timeAffect, runNumber);
	generatePlayerFiles(numberOfEmotionalRobots + numberOfNonEmotionalRobots, simTimeMilli, worldFile, arenaName, arenaFilePath);
    std::cout << "Running Stage          " << '\r' << std::flush;
    results = runStageSim(worldFile);
    std::cout << "Assigning Agents       " << '\r' << std::flush;
    agents = createAgents(numberOfEmotionalRobots, numberOfNonEmotionalRobots, highMoodPer, medMoodPer, highRepPer, medRepPer, initialDefectPer, agentTypes, emotionalTypes, useMood, onlyEmotionalInitial, timeAffect);
	epucks = assignAgents(agents, results.first);
    std::cout << "Playing Games          " << '\r' << std::flush;
	doGames(saveFolder, numberOfEmotionalRobots + numberOfNonEmotionalRobots, epucks, results.second, simTimeMilli, useMood, scenarioNumber, runNumber);
    printResults(epucks, saveFolder, scenarioNumber, runNumber);
    std::cout << "Done Scenario " << scenarioNumber << " Run " << runNumber << std::endl;
	return 0;
}

std::map<boost::uuids::uuid, Agent*> assignAgents(std::vector<Agent*> agents, std::set<boost::uuids::uuid> IDs)
{
	std::map<boost::uuids::uuid, Agent*> epucks;
	int i = 0;
	for (std::set<boost::uuids::uuid>::iterator it=IDs.begin(); it!=IDs.end(); ++it)
	{
		epucks[*it] = agents[i];
        i++;
	}
	return epucks;
}

void printResults(std::map<boost::uuids::uuid, Agent*> epucks, std::string saveFolder, int scenarioNumber, int runNumber)
{
    std::ofstream winners;
    std::ofstream totals;
    winners.open(saveFolder + "winners.csv", std::ofstream::app);
    totals.open(saveFolder + "totals.csv", std::ofstream::app);
    totals << std::setprecision(2);
    
    std::map<std::string, int> mapwinners;
    for (std::map<boost::uuids::uuid, Agent*>::iterator i = epucks.begin(); i != epucks.end(); ++i)
    {
        mapwinners[i->second->getAgentName()]++;
        totals << scenarioNumber << "," << runNumber << "," << i->second->getGuid() << "," << i->second->getAgentName() << "," << i->second->getNumberOfGames() << "," << i->second->getTotalScore() << "," << i->second->getAverageScore() << "," << i->second->getTotalDistance() << std::endl;
    }
    for (std::map<std::string, int>::iterator i = mapwinners.begin(); i != mapwinners.end(); ++i)
    {
        winners << scenarioNumber << "," << runNumber << "," << i->first << "," << i->second << std::endl;
    }
    winners.close();
    totals.close();
}

std::vector<Agent*> createAgents(int numOfEmotionalRobots, int numOfNonEmotionalRobots, double highMoodPercent, double medMoodPercent, double highRepPercent, double medRepPercent, double initialDefectPer, int agentTypes[6], int emotionalTypes[9], bool useMood, bool onlyEmotionalInitial, double timeAffect)
{
	std::vector<Agent*> agents;
    std::vector<int> repLevels;
    std::vector<int> moodLevels;
    std::vector<Action> iActions;
    
    int highMoodCount = floor(highMoodPercent * numOfEmotionalRobots);
	int medMoodCount = floor(medMoodPercent * numOfEmotionalRobots);
    int highRepCount = floor(highRepPercent * numOfEmotionalRobots);
    int medRepCount = floor(medRepPercent * numOfEmotionalRobots);
    
    int initialDefectorCount = 0;
    int total = numOfEmotionalRobots;
    if (onlyEmotionalInitial)
    {
        initialDefectorCount = floor(initialDefectPer * numOfEmotionalRobots);
    }
    else
    {
        initialDefectorCount = floor(initialDefectPer * (numOfEmotionalRobots + numOfNonEmotionalRobots));
        total += numOfNonEmotionalRobots;
    }
	int endOfMediumMoodCount = highMoodCount + medMoodCount;
    int endOfMediumRepCount = highRepCount + medRepCount;
    
    for (int i = 0; i < total; i++)
    {
        if (i < highMoodCount)
        {
            moodLevels.push_back(70);
        }
        else if (i < endOfMediumMoodCount)
        {
            moodLevels.push_back(50);
        }
        else
        {
            moodLevels.push_back(30);
        }
        
        if (i < initialDefectorCount)
        {
            iActions.push_back(DEFECT);
        }
        else
        {
            iActions.push_back(COOP);
        }
        
        if (i < highRepCount)
        {
            repLevels.push_back(3);
        }
        else if (i < endOfMediumRepCount)
        {
            repLevels.push_back(2);
        }
        else
        {
            repLevels.push_back(1);
        }
    }

    random_shuffle(iActions.begin(), iActions.end());
	random_shuffle(repLevels.begin(), repLevels.end());
    random_shuffle(moodLevels.begin(), moodLevels.end());
	
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < emotionalTypes[i]; j++)
        {
            EmotionalAgent* tAgent;
            if (useMood)
            {
                tAgent = new EmotionalAgent(i + 1, iActions.back(), repLevels.back(), moodLevels.back(), timeAffect);
                moodLevels.pop_back();
            }
            else
            {
                tAgent = new EmotionalAgent(i + 1, iActions.back(), repLevels.back());
            }
            agents.push_back(tAgent);
            iActions.pop_back();
            repLevels.pop_back();
        }
    }
    
    for (int i = 0; i < 6; i++)
    {
        Action toDo = COOP;
        if (!onlyEmotionalInitial)
        {
            toDo = iActions.back();
        }
        switch(i)
        {
            case JAGENT:
                for (int j = 0; j < agentTypes[JAGENT]; j++)
                {
                    JossAgent* tAgent = new JossAgent(toDo);
                    agents.push_back(tAgent);
                }
                break;
            case MAGENT:
                for (int j = 0; j < agentTypes[MAGENT]; j++)
                {
                    MendaciousAgent* tAgent = new MendaciousAgent();
                    agents.push_back(tAgent);
                }
                break;
            case RAGENT:
                for (int j = 0; j < agentTypes[RAGENT]; j++)
                {
                    RanAgent* tAgent = new RanAgent();
                    agents.push_back(tAgent);
                }
                break;
            case TAGENT:
                for (int j = 0; j < agentTypes[TAGENT]; j++)
                {
                    TesterAgent* tAgent = new TesterAgent();
                    agents.push_back(tAgent);
                }
                break;
            case TFTAGE:
                for (int j = 0; j < agentTypes[TFTAGE]; j++)
                {
                    TFTAgent* tAgent = new TFTAgent(toDo);
                    agents.push_back(tAgent);
                }
                break;
            case VAGENT:
                for (int j = 0; j < agentTypes[VAGENT]; j++)
                {
                    VeraciousAgent* tAgent = new VeraciousAgent();
                    agents.push_back(tAgent);
                }
                break;
        }
        if (!onlyEmotionalInitial)
        {
            iActions.pop_back();
        }
    }
	random_shuffle(agents.begin(), agents.end());
	return agents;
}
