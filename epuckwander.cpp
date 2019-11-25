#include "stage.hh"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
//sudo mv libepuckwander.so /usr/local/lib/Stage-4.1/ Run this after compile
static const double topSpeed = 0.1;
static const int rturnSpeed = 78;
static const double turnSpeed = 0.78; //45 degress per second to radians
static const double maxDist = 0.1;

typedef struct
{
	Stg::ModelPosition* pos;
	Stg::ModelRanger* laser;
	boost::uuids::uuid ID;
} robot_t;

int LaserUpdate(Stg::Model* mod, robot_t* robot);
int PositionUpdate(Stg::Model* mod, robot_t* robot);

// Stage calls this when the model starts up
extern "C" int Init( Stg::Model* mod, Stg::CtrlArgs* args )
{
	srand(time(NULL));
	robot_t* robot = new robot_t;
	robot->pos = (Stg::ModelPosition*)mod;
	robot->pos->AddCallback(Stg::Model::CB_UPDATE, (Stg::model_callback_t)PositionUpdate, robot);
	robot->pos->Subscribe();

	robot->laser = (Stg::ModelRanger*)mod->GetChild("ranger:0");
	robot->laser->AddCallback(Stg::Model::CB_UPDATE, (Stg::model_callback_t)LaserUpdate, robot);
	robot->laser->Subscribe();
	
	robot->ID = boost::uuids::random_generator()();
	return 0;
}

// inspect the ranger data and decide what to do
int LaserUpdate( Stg::Model* mod, robot_t* robot )
{
  // get the data
	if ((robot->laser->GetSensors()[0].ranges[0] < maxDist)
		|| (robot->laser->GetSensors()[1].ranges[0] < maxDist)
		|| (robot->laser->GetSensors()[2].ranges[0] < maxDist))
	{
		robot->pos->SetXSpeed(0);
		robot->pos->SetTurnSpeed(turnSpeed);
	}
	else if ((robot->laser->GetSensors()[6].ranges[0] < maxDist)
		|| (robot->laser->GetSensors()[7].ranges[0] < maxDist)
		|| (robot->laser->GetSensors()[5].ranges[0] < maxDist))
	{
		robot->pos->SetXSpeed(0);
		robot->pos->SetTurnSpeed(-turnSpeed);
	}
	else
	{
		robot->pos->SetXSpeed(topSpeed);
		robot->pos->SetTurnSpeed(((rand() % (2 * rturnSpeed)) - rturnSpeed) / 100.0);
	}
	return 0; // run again
}

int PositionUpdate( Stg::Model* mod, robot_t* robot )
{
	Stg::Pose pose = robot->pos->GetPose();
	std::cout << robot->ID << "," << (mod->GetWorld()->SimTimeNow() / 10000) << "," << pose.x << "," << pose.y << "," << pose.a << std::endl;

  return 0; // run again
}
