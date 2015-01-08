#include "dirc_objects.h"
#include "../headers/functions.h"
#include "../headers/simulator.h"
#include "../headers/Simulate.h"
#include <iomanip>

void Simulate::DistancetoWalls(bool print)
{
	for (unsigned int comp = 0; comp < 3; comp++)
	{
		if (Vec(comp) >= 0 ) Distance[comp] = Dim[comp] - coord[comp];
		else Distance[comp] = -coord[comp];
	}

	if (print) { TabToLevel(3); cout << "Dist = "; }
	for (unsigned int comp = 0; comp < 3; comp++){
		if (print) cout <<setw(10)<< Distance[comp];
	}
	if (print) cout << endl;
}

//======================================================================
//_____________check which wall will be hit__________________
void Simulate::WhichWall(bool print)
{
	double key1, key2;
  double velocity_map_first = 0, velocity_map_second = 0;

std::multimap<double, int> time_map;
  std::map<int, double> velocity_map;

  for (unsigned int comp = 0; comp < 3; comp++)
  {
  	TimesToWall[comp] = Distance[comp]/Vec(comp);
		time_map.insert(std::make_pair(TimesToWall[comp], comp+1));
	  velocity_map[comp] = Vec(comp);
  }

  std::map<double,int>::iterator it=time_map.begin();

  // set wall to first wall of time_map
  wall = it->second;
  TimeToWall = it->first;

	if (print) { TabToLevel(3); cout << "Time = "; }
	for (unsigned int comp = 0; comp < 3; comp++){
		if (print) cout <<setw(10)<< TimesToWall[comp];
	}
	if (print) cout << endl;
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
		if (print)
		{
			for (unsigned int comp = 0; comp < 3; comp++){
				TabToLevel(3); cout << "Velocity"<< comp << " = " << Vec(comp) << endl;		
			}
		}
  }
	if (print) { TabToLevel(3); cout << "Wall = " << wall << endl; }
}

double Simulate::TimeForDistance(double D, bool print){

	double t = D; // normally divided by v but v is always one because its a unit vector
	// if (print) cout << "\t\tTime = " << t << endl;
	return t;

}

void Simulate::TravelDistance(double D, bool print)
{
	double t = TimeForDistance(D, print);
	double dist[3];
	dist[0] = t*Vec(0);
	dist[1] = t*Vec(1);
	dist[2] = t*Vec(2);
	
	if (print) { TabToLevel(3); cout << "WillTravel = " << D << endl; }
	if (print) PrintVec();
	if (print) { TabToLevel(3); cout << "Init = "; }
	for (unsigned int comp = 0; comp < 3; comp++){
		if (print) cout <<setw(10)<< coord[comp];
	}
	if (print) cout << endl;



	// non-printing-----
	for (unsigned int i = 0; i<3;i++)
		coord[i] = coord[i] + dist[i];
	// ---------------



	if (print) { TabToLevel(3); cout << "Final = "; }
	for (unsigned int comp = 0; comp < 3; comp++){
		if (print) cout <<setw(10)<< coord[comp];
	}
	if (print) cout << endl;
	Traveled = Traveled + D;
}

void Simulate::GotoWall(bool print)
{
	DistancetoWalls(print);
	WhichWall(print);
	TravelDistance(WillTravel(), print);
}

void Simulate::Reflect(bool print)
{
	if (print) PrintVec();
	if (wall == 1)
		FlipX();

	if (wall == 2)
		FlipY();

	if (wall == 3)
		FlipZ();

	if (print) PrintVec();
}

void Simulate::PrintVec(){
	TabToLevel(3); cout << "Vector = ";
	for (unsigned int comp = 0; comp < 3; comp++){
		cout <<setw(10)<< Vec(comp);
	}
	cout << endl;	
}
