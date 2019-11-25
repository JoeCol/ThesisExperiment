#ifndef PLAYERSUP_H
#define PLAYERSUP_H
#include <string>
#include "agents/RLEmotional.hpp"
#include "ePuck/epuck.h"
#include <vector>
#include <set>
#include <boost/uuid/uuid.hpp>
#include "ePuck/ePuckPosition.h"
#include <map>

void generatePlayerFiles(int numOfRobots, int simTimeMilli, std::string worldFile, std::string arenaName, std::string arenaFile);
	
std::pair< std::set<boost::uuids::uuid>, std::vector<ePuckPosition*> >  runStageSim(std::string worldFile);

std::pair<Action, Action> playSingleGame(RLEmotional* a, RLEmotional* b, int time, bool staghunt);

void doGames(std::string saveDir, int numOfRobots, std::map<boost::uuids::uuid, RLEmotional*> agents, std::vector<ePuckPosition*> pos, int secs, bool staghunt);

#endif // PLAYER_H
