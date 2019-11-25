#ifndef PLAYERSUP_H
#define PLAYERSUP_H
#include <string>
#include "agents/Agent.h"
#include "ePuck/epuck.h"
#include <vector>
#include <set>
#include <boost/uuid/uuid.hpp>
#include "ePuck/ePuckPosition.h"
#include <map>

void generatePlayerFiles(int numOfRobots, int simTimeMilli, std::string worldFile, std::string arenaName, std::string arenaFile);
	
std::pair< std::set<boost::uuids::uuid>, std::vector<ePuckPosition*> >  runStageSim(std::string worldFile);
	
void playSingleGame(Agent* a, Agent* b, std::ofstream &gamesPlayed, int time, bool useMood);

std::pair<Action, Action> playSingleGame(Agent* a, Agent* b, int time, bool useMood);

void doGames(std::string saveDir, int numOfRobots, std::map<boost::uuids::uuid, Agent*> agents, std::vector<ePuckPosition*> pos,
				int secs, bool useMood, int scenarioNumber, int runNumber);

#endif // PLAYER_H
