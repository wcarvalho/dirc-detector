#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include "dirc_objects.h"
#include "../headers/generator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Get Distance to nearest wall for particle
================================================================================================*/

void Get_ParticleWall(Particle &particle, string Output)
{
	double key1, key2;
  double velocity_map_first = 0, velocity_map_second = 0;

	if (Output == "yes"){
		TabToLevel(4); cout << "Get_ParticleWall:\n";
	}

  std::multimap<double, int> time_map;
  std::map<int, double> velocity_map;

  particle.X_Time = fabs(particle.X_Distance/particle.UnitVector.X());
  particle.Y_Time = fabs(particle.Y_Distance/particle.UnitVector.Y());
  particle.Z_Time = fabs(particle.Z_Distance/particle.UnitVector.Z());

	time_map.insert(std::make_pair(particle.X_Time, 1));
  time_map.insert(std::make_pair(particle.Y_Time, 2));
  time_map.insert(std::make_pair(particle.Z_Time, 3));
  velocity_map[1] = particle.UnitVector.X();
  velocity_map[2] = particle.UnitVector.Y();
  velocity_map[3] = particle.UnitVector.Z();
 
  std::map<double,int>::iterator it=time_map.begin();

  // set wall to first wall of time_map
  particle.Wall = it->second;
  particle.Time = it->first;

  // check if the first two wall times are equal, if so find wall based on velocity
	key1 = it->first;
  ++it;
  key2 = it->first;
	if (Output == "yes"){
		TabToLevel(5); cout << "X_Time = " << particle.X_Time << endl;
		TabToLevel(5); cout << "Y_Time = " << particle.Y_Time << endl;
		TabToLevel(5); cout << "Z_Time = " << particle.Z_Time << endl;
	}
  if (fabs(key1) == fabs(key2))
  {
    --it;
    key1 = it->second;
    ++it;
    key2 = it->second;
    velocity_map_first = velocity_map[key1];
    velocity_map_second = velocity_map[key2];
    if (velocity_map_first > velocity_map_second) { particle.Wall = key1; }
    if (velocity_map_second > velocity_map_first) { particle.Wall = key2; }
		if (Output == "yes"){
		TabToLevel(5); cout << "X_Velocity = " << particle.UnitVector.X() << endl;
		TabToLevel(5); cout << "Y_Velocity = " << particle.UnitVector.Y() << endl;
		TabToLevel(5); cout << "Z_Velocity = " << particle.UnitVector.Z() << endl;
		}
  }
	if (Output == "yes")
	{
		TabToLevel(5); cout << "Wall = " << particle.Wall << endl;
		TabToLevel(5); cout << "Time to Wall = " << particle.Time << endl;
	}

}