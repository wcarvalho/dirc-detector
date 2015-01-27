#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include "dirc_objects.h"
#include "../headers/simulator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Get Distance to nearest wall for photon
================================================================================================*/

void Get_PhotonWall(Photon &photon, string Output)
{
	double key1, key2;
  double velocity_map_first = 0, velocity_map_second = 0;

	if (Output == "yes"){
		TabToLevel(4); cout << "Get_PhotonWall:\n";
	}

  std::multimap<double, int> time_map;
  std::map<int, double> velocity_map;

  photon.X_Time = fabs(photon.X_Distance/photon.UnitVector.X());
  photon.Y_Time = fabs(photon.Y_Distance/photon.UnitVector.Y());
  photon.Z_Time = fabs(photon.Z_Distance/photon.UnitVector.Z());

	time_map.insert(std::make_pair(photon.X_Time, 1));
  time_map.insert(std::make_pair(photon.Y_Time, 2));
  time_map.insert(std::make_pair(photon.Z_Time, 3));
  velocity_map[1] = photon.UnitVector.X();
  velocity_map[2] = photon.UnitVector.Y();
  velocity_map[3] = photon.UnitVector.Z();
 
  std::map<double,int>::iterator it=time_map.begin();

  // set wall to first wall of time_map
  photon.Wall = it->second;
  photon.Time = it->first;

  // check if the first two wall times are equal, if so find wall based on velocity
	key1 = it->first;
  ++it;
  key2 = it->first;
	if (Output == "yes"){
		TabToLevel(5); cout << "X_Time = " << photon.X_Time << endl;
		TabToLevel(5); cout << "Y_Time = " << photon.Y_Time << endl;
		TabToLevel(5); cout << "Z_Time = " << photon.Z_Time << endl;
	}
  if (fabs(key1) == fabs(key2))
  {
    --it;
    key1 = it->second;
    ++it;
    key2 = it->second;
    velocity_map_first = velocity_map[key1];
    velocity_map_second = velocity_map[key2];
    if (velocity_map_first > velocity_map_second) { photon.Wall = key1; }
    if (velocity_map_second > velocity_map_first) { photon.Wall = key2; }
		if (Output == "yes"){
		TabToLevel(5); cout << "X_Velocity = " << photon.UnitVector.X() << endl;
		TabToLevel(5); cout << "Y_Velocity = " << photon.UnitVector.Y() << endl;
		TabToLevel(5); cout << "Z_Velocity = " << photon.UnitVector.Z() << endl;
		}
  }
	if (Output == "yes")
	{
		TabToLevel(5); cout << "Wall = " << photon.Wall << endl;
		TabToLevel(5); cout << "Time to Wall = " << photon.Time << endl;
	}

}