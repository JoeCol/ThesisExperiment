#include "ePuckPosition.h"

ePuckPosition::ePuckPosition(boost::uuids::uuid _ID, int _time, int _x, int _y, double _yaw)
{
	ID = _ID;
	time = _time;
	x = _x;
	y = _y;
	yaw = _yaw;
}

boost::uuids::uuid ePuckPosition::getID() const
{
	return ID;
}

int ePuckPosition::getTime() const
{
	return time;
}

int ePuckPosition::getX() const
{
	return x;
}

int ePuckPosition::getY() const
{
	return y;
}

double ePuckPosition::getYaw() const
{
	return yaw;
}

bool operator<(const ePuckPosition &l, const ePuckPosition &r)
{
    return l.getTime() < r.getTime();
}
