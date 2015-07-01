#include "dirc_objects.h"
#include "../headers/functions.h"
#include "simulator.h"
#include "Simulate.h"
#include <iomanip>
#include <unordered_map>

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
void Simulate::OldWhichWall(bool print)
{
	double key1, key2;
  double velocity_map_first = 0, velocity_map_second = 0;

	std::unordered_multimap<double, int> time_map_unordered;
  std::unordered_map<int, double> velocity_map_unordered;

  for (unsigned int comp = 0; comp < 3; comp++)
  {
	  velocity_map_unordered[comp] = Vec(comp);
  	TimesToWall[comp] = Distance[comp]/(Vec(comp));
		time_map_unordered.insert(std::make_pair(TimesToWall[comp], comp+1));
  }

  std::multimap<double, int> time_map(time_map_unordered.begin(), time_map_unordered.end());
  std::map<int, double> velocity_map(velocity_map_unordered.begin(), velocity_map_unordered.end());
  auto it=time_map.begin();
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
	// double t = D/Vec.Mag();
	double t = 100*D/(Vec.Mag()*3); //
	return std::move(t);
}

void Simulate::TravelDistance(double D, bool print)
{
	double t = TimeForDistance(D, print);
	time_traveled += t;
	double dist[3];
	dist[0] = t*3*Vec(0)/100;
	dist[1] = t*3*Vec(1)/100;
	dist[2] = t*3*Vec(2)/100;

	if (print) { TabToLevel(3); cout << "WillTravel = " << D << endl; }
	if (print) PrintVec();
	if (print) { TabToLevel(3); cout << "Init = "; }
	for (unsigned comp = 0; comp < 3; comp++){
		if (print) cout <<setw(10)<< coord[comp];
	}
	if (print) cout << endl;



	// non-printing-----
	for (unsigned int i = 0; i<3; ++i)
		coord[i] = coord[i] + dist[i];
	// ---------------



	if (print) { TabToLevel(3); cout << "Final = "; }
	for (unsigned int comp = 0; comp < 3; comp++){
		if (print) cout <<setw(10)<< coord[comp];
	}
	if (print) cout << endl;
	Traveled = Traveled + D;

	for (unsigned int i = 0; i<3; ++i){
		if ((Dim[i] - coord[i] > Dim[i] + 1e-10) || ( Dim[i] - coord[i] < -1e-10)){
			// cout << "\tOUT OF BOUNDS: coord " << i << " = " << coord[i] << "\tbounds: 0, " << Dim[i] << endl;
			// cout << Dim[i] - coord[i] << endl;
			// exit(1);
		}
	}
}
double Simulate::WillTravel(){
  return Vec.Mag()*TimeToWall;
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
	// cout << "\t\tTheta = " << Theta << endl;
	// cout << "\t\tPhi = " << Theta << endl;

	if (wall == 1)
		FlipX();

	if (wall == 2)
		FlipY();

	if (wall == 3)
		FlipZ();

	if (print) PrintVec();

	// cout << "\t\tTheta = " << Theta << endl;
	// cout << "\t\tPhi = " << Theta << endl;
}

void Simulate::PrintVec(){
	TabToLevel(3); cout << "Vector = ";
	for (unsigned int comp = 0; comp < 3; comp++){
		cout <<setw(10)<< Vec(comp);
	}
	cout << endl;
}

void Simulate::WhichWall(bool print){

  static double v[3];
  static double t[3];

  int min_time_element = 5;
  double min_time = 1.e10;
  int max_v_element = 5;
  double max_velocity = 0.;
  for (unsigned i = 0; i < 3; ++i){
    v[i] = Vec(i);
    t[i] = Distance[i]/v[i];
    if (fabs(t[i]) < fabs(min_time)){
      min_time = t[i];
      min_time_element = i;
    }
    if (fabs(v[i]) > fabs(max_velocity)){
      max_velocity = v[i];
      max_v_element = i;
    }
  }

  if (print) { cout << "\t\tTime = "; }
  for (unsigned int comp = 0; comp < 3; comp++){
    if (print) cout <<setw(10)<< t[comp];
  }
  if (print) cout << endl;
  wall = min_time_element + 1;
  TimeToWall = min_time;
  if(print) cout << "\t\tWall = " << wall << "\n";
}



//unfinfished
// void Simulate::GoTo(int const whichcoordinate, double const value){

// 	double& x = coord[0];
// 	double& y = coord[1];
// 	double& z = coord[2];

// 	double& l = Dim[0];
// 	double& w = Dim[1];
// 	double& h = Dim[2];

// 	switch(whichcoordinate){
// 		case 1:
// 			double delta_x = value - coord[whichcoordinate];
// 			double delta_y = TMath::Tan(Phi)*delta;
// 			double delta_z = xy(delta_x, delta_y)/TMath::Tan(Theta)
// 			int ny = (int)dy/w;
// 			if (!(even(ny))) FlipY();

// 			break;
// 		case 2:
// 			double delta_y = value - coord[whichcoordinate];
// 			double delta_x = TMath::Tan(phi)/delta;
// 			double delta_z = xy(delta_x, delta_y)/TMath::Tan(Theta)
// 			break;
// 		case 3:
// 			double delta_z = value - coord[whichcoordinate];
// 			double xy = delta_z*TMath::Tan(Theta);
// 			double delta_y = xy*TMath::Sin(Phi);
// 			double delta_x = xy*TMath::Cos(Phi)
// 			break;
// 	}



// 	DistanceTo();
// 	// while (coord[whichcoordinate] <= value){

// 	// }

// 	auto xy = [](double const x, double const y){
// 		return sqrt(x*x+y*y);
// 	}


// 	// coord[0] += delta_x;
// 	// coord[1] += delta_y;
// 	// coord[2] += delta_z;

// }

bool even(int const value){
	if (value % 2 == 0) return true;
	else return false;
}