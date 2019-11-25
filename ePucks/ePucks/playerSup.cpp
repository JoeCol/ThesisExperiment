#include "playerSup.h"
#include <fstream>
#include <iomanip>
#include "support.h"
#include <unistd.h>
#include "agents/EmotionalAgent.h"
#include <set>
#include <regex>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/algorithm/string.hpp>

void generatePlayerFiles(int numOfRobots, int simTimeMilli, std::string worldFile, std::string arenaName, std::string arenaFile)
{
	std::vector< std::vector<int> > locations;
	createOccupancyVector(locations, arenaFile);
	double asize = (locations.size() / 100.0);
	
	std::ofstream world;	
	world.open(worldFile.c_str(), std::ofstream::trunc);
	world << std::setprecision(2);
	
	world << "include \"map.inc\"" << std::endl;
	world << "include \"ePuck.inc\"" << std::endl;
	world << std::endl;
	world << "quit_time " << (simTimeMilli / 1000) << std::endl;
	world << std::endl;
	world << "speedup 0" << std::endl;
	world << std::endl;
	world << "window" << std::endl;
	world << "(" << std::endl;
	world << "size [ 400.000 400.000 ] # in pixels" << std::endl;
	world << "scale 65   # pixels per meter" << std::endl;
	world << "rotate [ 0  0 ]" << std::endl;
	world << "show_data 1" << std::endl;
	world << ")" << std::endl;
	world << std::endl;
	world << "floorplan" << std::endl;
	world << "(" << std::endl;
  	world << "bitmap \"" + arenaName + "\"" << std::endl;
  	world << "size [" << asize << " " << asize << " 0.05]" << std::endl;
	world << ")" << std::endl;
	world << std::endl;
	
	for (int i = 0; i < numOfRobots; i++)
	{
		int rotation = getRandomInt(-180, 180);
		int x = getRandomInt(1, (int)locations.size());//480) - 240) / 100.0;
		int y = getRandomInt(1, (int)locations.size());//480) - 240) / 100.0;
		
		while (!isValidLocation(locations, x, y))
		{
			x = getRandomInt(1, (int)locations.size());
			y = getRandomInt(1, (int)locations.size());
		}
		setLocation(locations, x, y);
		//Coords to meters
		
		double cx = (x / 100.0) - (asize / 2);
		double cy = 0 - ((y / 100.0) - (asize / 2));
		
		world << "epuck" << std::endl;
		world << "(" << std::endl;
		world << "name \"epuck" << i << "\"" << std::endl;
		world << "pose [" << cx << " " << cy << " 0 " << rotation << "]" << std::endl;
		world << "ctrl \"wander\"" << std::endl;
		world << ")" << std::endl;
		world << std::endl;
	}
	world.flush();
	world.close();
}

std::pair< std::set<boost::uuids::uuid>, std::vector<ePuckPosition*> >  runStageSim(std::string worldFile)
{
	FILE* stageoutput = popen(("/usr/local/bin/stage -g " + worldFile).c_str(), "r");
	char buff[512];
	std::set<boost::uuids::uuid> IDS;
	std::vector<ePuckPosition*> positions;
	std::vector<std::string> line;
	//std::regex linematch(".+,\\d+,(-?\\d+\\.*\\d*,){2}(-?\\d+\\.*\\d*)");
	std::regex linematch("\\d+,");
    std::cout << "Running Stage 0%" << '\r' << std::flush;
	while (fgets(buff, sizeof(buff), stageoutput) != NULL)
	{
		if (std::regex_search(buff, linematch))
		{
			std::string strBuff = boost::lexical_cast<std::string>(buff);
			boost::trim(strBuff);
			line = split(strBuff, ',', line);
			if ((strToInt(line[1]) % 1000000) == 0)
			{
                std::cout << "Running Stage " << (int)((strToInt(line[1]) / 600000000.0) * 100) << "%"<< '\r' << std::flush;
				boost::uuids::string_generator strgen;
				boost::uuids::uuid tmp = strgen(line[0]);
				IDS.insert(tmp);
				int timei = strToInt(line[1]);
				int xpos = meterStrToCMInt(line[2]);
				int ypos = meterStrToCMInt(line[3]);
				double yaw = boost::lexical_cast<double>(line[4]);
				ePuckPosition* epp = new ePuckPosition(tmp, timei, xpos, ypos, yaw);
				positions.push_back(epp);
			}
			line.clear();
		}
	}
	pclose(stageoutput);
    std::cout << "Running Stage 100%" << '\r' << std::flush;
	return std::pair<std::set<boost::uuids::uuid>, std::vector<ePuckPosition*>>(IDS, positions);
}

std::pair<Action, Action> playSingleGame(Agent* a, Agent* b, int time, bool useMood)
{

	Action aa = a->play(b, time);
	Action ab = b->play(a, time);
	int aScore;
	int bScore;
	if (aa == ab && aa == COOP)
	{
		aScore = 3; 
		bScore = 3;
	}
	else if (aa == ab && ab == DEFECT)
	{
		aScore = 1;
		bScore = 1;
	}
	else if (aa == DEFECT)
	{
		aScore = 5;
		bScore = 0;
	}
	else
	{
		aScore = 0;
		bScore = 5;
	}
	
	if (useMood)
	{
		a->moodUpdate(aScore, a->getAverageScore(), bScore, b->getAverageScore());
		b->moodUpdate(bScore, b->getAverageScore(), aScore, a->getAverageScore());
	}
	a->result(ab, aScore, b);
	b->result(aa, bScore, a);
	return std::pair<Action, Action>(aa, ab);
}

void playSingleGame(Agent* a, Agent* b, std::ofstream &gamesPlayed, int time, bool useMood)
{
	if (useMood)
	{
		gamesPlayed << a->getMood() << ",";
		gamesPlayed << b->getMood() << ",";
	}
	std::pair<Action, Action> p = playSingleGame(a, b, time, useMood);
	gamesPlayed << p.first << "," << p.second << ",";
	gamesPlayed << a->getTotalScore() << "," << b->getTotalScore(); 
}

bool comparePairs(std::pair<int, std::pair<boost::uuids::uuid, boost::uuids::uuid> > i, 
	std::pair<int, std::pair<boost::uuids::uuid, boost::uuids::uuid> > j)
{
	return (i.first == j.first) && (i.second.first == j.second.first) && (i.second.second == j.second.second);
}

void doGames(std::string saveDir, int numOfRobots, std::map<boost::uuids::uuid, Agent*> agents, std::vector<ePuckPosition*> pos,
				int secs, bool useMood, int scenarioNumber, int runNumber)
{
	std::ofstream gamesPlayed;
	gamesPlayed.open((saveDir + "played.csv").c_str(), std::ofstream::app);
	
	std::vector<std::pair<int, std::pair<boost::uuids::uuid, boost::uuids::uuid> > > games;
	std::ofstream robotDist;
	robotDist.open((saveDir + "robotDistribution.csv").c_str(), std::ofstream::app);
    
    
    struct{
        bool operator()(ePuckPosition* l, ePuckPosition* r)
        {
            return l->getTime() < r->getTime();
        }
    } sortPositions;
	std::sort(pos.begin(), pos.end(), sortPositions);
	for (uint i = 0; i != pos.size(); i++) //84900000=84secs 9millisec, 100000=1millisecond
	{
        robotDist << scenarioNumber << "," << runNumber << "," << pos[i]->getTime() << "," << pos[i]->getID() << "," << pos[i]->getX() << "," << pos[i]->getY() << "," << pos[i]->getYaw() << std::endl;
        agents.at(pos[i]->getID())->addDistance(pos[i]->getX(), pos[i]->getY());
		for(uint j = i + 1; j != pos.size(); j++)
		{
			if (pos[i]->getTime() == (pos[j]->getTime()))
			{
				if ((diff(pos[i]->getX(), pos[j]->getX()) < 20) && (diff(pos[i]->getY(), pos[j]->getY()) < 20))
				{ 
					bool valid = isOpposite(pos[j]->getX(), pos[j]->getY(), pos[j]->getYaw(), 
											pos[i]->getX(), pos[i]->getY(), pos[i]->getYaw());
					if (valid)
					{
						bool add = true;
						if (!games.empty())
						{
							std::pair<int, std::pair<boost::uuids::uuid, boost::uuids::uuid> > sameFD(pos[i]->getTime(), std::pair<boost::uuids::uuid, boost::uuids::uuid>(pos[j]->getID(), pos[i]->getID()));
										
							add = !(std::binary_search(games.begin(), games.end(), sameFD));
						}
						
						if(add)
						{
							games.push_back(std::pair<int, std::pair<boost::uuids::uuid, boost::uuids::uuid> >(pos[i]->getTime(), std::pair<boost::uuids::uuid, boost::uuids::uuid>(pos[i]->getID(), pos[j]->getID())));
										
							gamesPlayed << scenarioNumber << "," << runNumber << "," << pos[i]->getTime() << "," << pos[i]->getID() << "," << pos[j]->getID() << ",";
							playSingleGame(agents.at(pos[i]->getID()), agents.at(pos[j]->getID()), gamesPlayed, pos[i]->getTime(), useMood);
							gamesPlayed << "," << pos[j]->getX() << "," << pos[j]->getY() << std::endl;
							
						}
					}
				}
			}
			else
			{
				break;
			}
		}
	}
	gamesPlayed.close();
	robotDist.close();
}
