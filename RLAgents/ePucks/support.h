#ifndef IMGSUPPORT_H
#define IMGSUPPORT_H
#include <vector>
#include <string>
#include <png++/png.hpp>
#include "RLEmotional.hpp"

enum agentType {JAGENT = 0, MAGENT = 1, RAGENT = 2, TAGENT = 3, TFTAGE = 4, VAGENT = 5};

const int hw = 5.0 / 0.1; //ePuck size 0.07 with 0.03 gap heightwidth of epuck. i.e how many epucks fit in height of 5 meters

void createOccupancyVector(std::vector< std::vector<int> > &toFill, std::string filename);
bool isValidLocation(std::vector< std::vector<int> > &toFill, int x, int y);
void setLocation(std::vector< std::vector<int> > &toFill, int x, int y);
double convertPixelToMeter(int p);
std::string intToStr(int i);
int strToInt(std::string s);
int meterStrToCMInt(std::string s);
int getRandomInt(int start, int end);
bool isOpposite(int x, int y, double yaw, int px, int py, double pyaw);
int diff(int x, int y);
int radToDegree(double rad);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
void toText(std::vector< std::vector<int> > &toFill);
void createRandomEnvironment(std::string filename, std::string save);
bool checkifblank(png::image<png::rgb_pixel> environment, int x, int y);

void getArguments(int argc, char** argv, std::string &arenaFilePath, std::string &worldFile, std::string &arenaName, std::string &saveFolder, std::string &matDirectory, int &numOfRobots, StateMode &stateMode, bool &StagHunt, MoodAffectMode &mam, double &moodAffect);

#endif // IMGSUPPORT_H
