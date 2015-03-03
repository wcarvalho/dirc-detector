#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

string histName0(int event, int particle){
	stringstream histname;
	histname << "Event_" << setfill('0') <<setw(3) << event << "_Particle_" << particle+1;
  return histname.str();
}