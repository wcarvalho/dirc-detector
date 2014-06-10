#include "dirc_objects.h"
#include "../headers/functions.h"
#include "../headers/simulator.h"
#include "../headers/Simulate.h"

void Simulate::DistancetoWalls(string Output)
{
	for (unsigned int comp = 0; comp < 3; comp++)
	{
		if (Vec(comp) >= 0 ) { Distance[comp] = Dim[comp] - coord[comp]; }
		else { Distance[comp] = -Dim[comp]; }
		if (Output == "yes"){ TabToLevel(3); cout << "Distance " << comp << " = " << Distance[comp] << endl; }
	}
}

//======================================================================
//_____________check which wall will be hit__________________
void Simulate::WhichWall(string Output)
{
	double key1, key2;
  double velocity_map_first = 0, velocity_map_second = 0;

std::multimap<double, int> time_map;
  std::map<int, double> velocity_map;

  for (unsigned int comp = 0; comp < 3; comp++)
  {
  	TimesToWall[comp] = fabs(Distance[comp]/Vec(comp));
		time_map.insert(std::make_pair(TimesToWall[comp], comp+1));
	  velocity_map[comp] = Vec(comp);
  }

  std::map<double,int>::iterator it=time_map.begin();

  // set wall to first wall of time_map
  wall = it->second;
  TimeToWall = it->first;
  if (Output == "yes")
	{
		for (unsigned int comp = 0; comp < 3; comp++){
			TabToLevel(3); cout << "Time "<< comp+1 << " = " << TimesToWall[comp] << endl;		
		}
	}

  // check if the first two wall times are equal, if so find wall based on velocity
	key1 = it->first;
  ++it;
  key2 = it->first;
  if (fabs(key1) == fabs(key2))
  {
    --it;
    key1 = it->second;
    ++it;
    key2 = it->second;
    velocity_map_first = velocity_map[key1];
    velocity_map_second = velocity_map[key2];
    if (velocity_map_first > velocity_map_second) { wall = key1; }
    if (velocity_map_second > velocity_map_first) { wall = key2; }
		if (Output == "yes")
		{
			for (unsigned int comp = 0; comp < 3; comp++){
				TabToLevel(3); cout << "Velocity"<< comp << " = " << Vec(comp) << endl;		
			}
		}
  }
	if (Output == "yes")
	{
		TabToLevel(3); cout << "Wall = " << wall << endl;
	}
}

void Simulate::GotoWall(string Output)
{
	DistancetoWalls(Output);
	WhichWall(Output);

	for (unsigned int i = 0; i < 3; i++)
	{
		coord[i] = coord[i] + Vec(i)*TimeToWall;
	}
	Traveled = WillTravel();
	if (Output == "yes")
	{
		TabToLevel(3); cout << "X,Y,Z = ";
		for (unsigned int i = 0; i < 3; i++)
		{
			cout << coord[i] << " ";
		}
		cout << endl;
		TabToLevel(3); cout << "Total Distance Traveled = " << Traveled << endl;
	}
}

void Simulate::TravelDistance(double D)
{
	double dist[3];
	dist[0] = D*cos(Theta);
	dist[1] = D*sin(Theta)*sin(Phi);
	dist[2] = D*sin(Theta)*cos(Phi);

	for (unsigned int i = 0; i<3;i++)
	{
		coord[i] = coord[i] + dist[i];
	}
	Traveled = Traveled + D;
}
