#ifndef EPUCK_H
#define EPUCK_H

#include "../agents/Agent.h"
#include <boost/uuid/uuid.hpp>

class epuck
{
private:
	Agent* agent;
	boost::uuids::uuid ID;
public:
    epuck(boost::uuids::uuid _ID, Agent* player);
    ~epuck();
	Agent* getAgent();
	boost::uuids::uuid getID();
};

#endif // EPUCK_H
