#include "support.h"
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <math.h>

void createOccupancyVector(std::vector< std::vector<int> > &toFill, std::string filename)
{
	png::image< png::rgb_pixel > image(filename);
	int w = image.get_width();
	int h = image.get_height();
	
	png::image< png::rgb_pixel > test(w, h);
	for (int i = 0; i < h; i++) //y
	{
		std::vector<int> row;
		for (int j = 0; j < w; j++) //x
		{
			png::basic_rgb_pixel<unsigned char> p = image.get_pixel(j, i);
			if (p.red == 0)
			{
				row.push_back(1);
				test.set_pixel(j, i, png::rgb_pixel(0, 0, 0));
			}
			else
			{
				row.push_back(0);
				test.set_pixel(j, i, png::rgb_pixel(255, 255, 255));
			}
		}
		toFill.push_back(row);
	}
}

void createRandomEnvironment(std::string filename, std::string save)
{
	png::image<png::rgb_pixel> original(filename);
	bool added;
	int x = 0;
	int y = 0;
	for (int i = 0; i < 20; i++)
	{
		added = false;
		while(!added)
		{
			x = rand() % 460;
			y = rand() % 460;
			if ((x > 25 && x < 435) && (y > 30 && y < 435))
			{
				added = checkifblank(original, x, y);
			}
		}
		for (int j = x - 25; j < x + 26; j++)
		{
			for (int k = y - 25; k < y + 26; k++)
			{
				original.set_pixel(j, k, png::rgb_pixel(0, 0, 0));
			}
		}
	}
	original.write(save);
}

bool checkifblank(png::image<png::rgb_pixel> environment, int x, int y)
{
	for (int i = x - 25; i < x + 26; i++)
	{
		for (int j = y - 25; j < y + 26; j++)
		{
			if (environment.get_pixel(i, j).red != 255)
			{
				return false;
			}
		}
	}
	return true;
}

bool isValidLocation(std::vector< std::vector<int> > &toFill, int x, int y)
{
	u_long max = toFill.size() - 10;
	if (x <= 10 || x >= max || y <= 10 || y >= max)
	{
		return false;
	}
	
	for(int i = x - 10; i < x + 10; i++)
	{
		for(int j = y - 10; j < y + 10; j++)
		{
			if (toFill[j][i] != 0)
			{
				return false;
			}
		}
	}
	return true;
}

void setLocation(std::vector< std::vector<int> > &toFill, int x, int y)
{
	u_long max = toFill.size();
	for(int i = x - 6; i < x + 6; i++)
	{
		for(int j = y - 6; j < y + 6; j++)
		{
			if (i >= 0 && j >= 0 && i < max && j < max)
			{
				toFill[j][i] = 2;
			}
		}
	}
}

double convertPixelToMeter(int p)
{
	return (p / 100.0) - 2.5;
}

void toText(std::vector< std::vector<int> > &toFill)
{
	std::ofstream text;
	text.open("text.txt");
	for (std::vector< std::vector<int> >::size_type i = 0; i < toFill.size(); i++) //y
	{
		for (std::vector< std::vector<int> >::size_type j = 0; j < toFill[i].size(); j++) //x
		{
			text << toFill[i][j];
		}
		text << std::endl;
	}
	text.close();
}

std::string intToStr(int i)
{
	std::string toRet = boost::lexical_cast<std::string>(i);
	return toRet;
}

int strToInt(std::string s)
{
	return boost::lexical_cast<int>(s);
}

int meterStrToCMInt(std::string s)
{
	double meter = boost::lexical_cast<double>(s);
	return (int)(meter * 100);
}

int getRandomInt(int start, int end)
{
	return (rand() % end) + start;
}

int diff(int x, int y)
{
	return std::max(x,y)-std::min(x,y);
}

int radToDegree(double rad)//Move range from -3.14 to 3.14 into -180 to 180
{
	double i = (rad * 180);
	i = i / M_PI;
	return i; 
}

bool isOpposite(int x, int y, double yaw, int px, int py, double pyaw)
{
	int dyaw = radToDegree(yaw);
	int dpyaw = radToDegree(pyaw);
	
	int oyaw;
	if (dyaw > 0)
	{
		oyaw = dyaw - 180;
	}
	else
	{
		oyaw = dyaw + 180;
	}
	
	if (dpyaw > oyaw - 8 && dpyaw < oyaw + 8)
	{
		if (dyaw >= 90) //facing top left, so must be higher and to left
		{
			return (y < py) && (x > px);
		}
		else if (dyaw >= 0) //facing top right, so must be higher and to right
		{
			return (y < py) && (x < px);
		}
		else if (dyaw <= -90) //facing bottom left, so must be lower and to left
		{
			return (y > py) && (x > px);
		}
		else //facing bottom left, so must be lower and to right
		{
			return (y > py) && (x < px);
		}
	}
	else
	{
		return false; //not facing opposite
	}
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) 
	{
        elems.push_back(item);
    }
    return elems;
}

void getArguments(int argc, char** argv, std::string &arenaFilePath, std::string &worldFile, std::string &arenaName, std::string &saveFolder, std::string &matDirectory, int &scenarioNumber, int &numberOfEmotionalRobots, int &numberOfNonEmotionalRobots, int (&agentTypes)[6], int (&emotionalTypes)[9], int &simTimeMilli, double &initialDefectPer, double &highRepPer, double &medRepPer, double &highMoodPer, double &medMoodPer, bool &useMood, bool &onlyEmotionalInitial, double &timeAffect, int &runNumber)
{
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i],"-arenaFile") == 0) //arena file
		{
			arenaFilePath = argv[i + 1];
            arenaName = boost::filesystem::path(argv[i + 1]).filename().string();
            i++;
		}
        else if (strcmp(argv[i], "-worldDirectory") == 0)
        {
            worldFile = argv[i + 1];
            i++;
        }
		else if (strcmp(argv[i], "-saveDirectory") == 0) //saveTo
		{
			saveFolder = argv[i + 1];
            boost::filesystem::create_directories(saveFolder);
            i++;
		}
        else if (strcmp(argv[i], "-emotionalRobotCount") == 0)
        {
            numberOfEmotionalRobots = boost::lexical_cast<int>(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-standardRobotCount") == 0)
        {
            numberOfNonEmotionalRobots = boost::lexical_cast<int>(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-standardAgent") == 0)
        {
            for (int j = 0; j < 6; j++)
            {
                agentTypes[boost::lexical_cast<int>(argv[i + 1])] = boost::lexical_cast<int>(argv[i + 2]);
                i = i + 2;
            }
        }
        else if (strcmp(argv[i], "-emotionalAgent") == 0)
        {
            for (int j = 0; j < 9; j++)
            {
                emotionalTypes[boost::lexical_cast<int>(argv[i + 1])] = boost::lexical_cast<int>(argv[i + 2]);
                i = i + 2;
            }
        }
        else if (strcmp(argv[i], "-simTimeSecs") == 0)
        {
            simTimeMilli = boost::lexical_cast<int>(argv[i + 1]) * 1000;
            i++;
        }
		else if (strcmp(argv[i], "-percentInitialDefect") == 0) //defectorPercent
		{
			initialDefectPer = boost::lexical_cast<double>(argv[i + 1]);
            i++;
		}
        else if (strcmp(argv[i], "-percentHighReplication") == 0)
        {
            highMoodPer = boost::lexical_cast<double>(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-percentMedReplication") == 0)
        {
            medMoodPer = boost::lexical_cast<double>(argv[i + 1]);
            i++;
        }
		else if (strcmp(argv[i], "-percentHighMood") == 0)
		{
			highMoodPer = boost::lexical_cast<double>(argv[i + 1]);
            i++;
		}
		else if (strcmp(argv[i], "-percentMedMood") == 0)
		{
			medMoodPer = boost::lexical_cast<double>(argv[i + 1]);
            i++;
		}
        else if (strcmp(argv[i], "-noMood") == 0)
        {
            useMood = false;
        }
        else if (strcmp(argv[i], "-percentageOverAllAgents") == 0)
        {
            onlyEmotionalInitial = false;
        }
		else if (strcmp(argv[i], "-ran") == 0)
		{
			int num = rand() % 1000;
			createRandomEnvironment(worldFile + "random.png", worldFile + "randomgenerated" + boost::lexical_cast<std::string>(num) + ".png");
			arenaName = "randomgenerated" + boost::lexical_cast<std::string>(num) + ".png";
			arenaFilePath = worldFile + "/randomgenerated" + boost::lexical_cast<std::string>(num) + ".png";
		}
        else if (strcmp(argv[i], "-scenario") == 0)
        {
            scenarioNumber = boost::lexical_cast<int>(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-run") == 0)
        {
            runNumber = boost::lexical_cast<int>(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-matDirectory") == 0)
        {
            matDirectory = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i], "-timeAffect") == 0)
        {
            timeAffect = boost::lexical_cast<double>(argv[i + 1]);
            i++;
        }
		
	}
}
