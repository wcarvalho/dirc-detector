#include <map>
#include <string>
#include "../headers/functions.h"

static bool compare(std::pair<std::string, double> i, std::pair<std::string, double> j) 
{ 
  return i.second < j.second; 
}

double getMin(std::map<std::string, double> mymap) 
{
  std::pair<std::string, double> min = *min_element(mymap.begin(), mymap.end(), 
                                                 &compare);
  return min.second; 
}