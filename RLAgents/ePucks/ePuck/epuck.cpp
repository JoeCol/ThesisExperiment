#include "epuck.h"
#include <sstream>
#include <boost/bind.hpp>
#include <float.h>
#include "support.h"
#include <math.h>

epuck::epuck(boost::uuids::uuid _ID, Agent* player)
{
	ID = _ID;
	agent = player;
}

epuck::~epuck()
{
	
}

double Distance(double dX0, double dY0, double dX1, double dY1)
{
    return sqrt((dX1 - dX0)*(dX1 - dX0) + (dY1 - dY0)*(dY1 - dY0));
}

Agent* epuck::getAgent()
{
	return agent;
}

boost::uuids::uuid epuck::getID()
{
	return ID;
}
