#ifndef EPUCKPOSITION_H
#define EPUCKPOSITION_H
#include <boost/uuid/uuid.hpp>

class ePuckPosition
{
	boost::uuids::uuid ID;
	int time;
	int x;
	int y;
	double yaw; //Note that this is in radians
public:
	ePuckPosition(boost::uuids::uuid _ID, int _time, int _x, int _y, double _yaw);
	boost::uuids::uuid getID() const;
	int getTime() const;
	int getX() const;
	int getY() const;
	double getYaw() const;
};

bool operator<(const ePuckPosition &l, const ePuckPosition &r);
#endif // EPUCKPOINT_H
