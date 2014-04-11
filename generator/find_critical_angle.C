#include <iostream>
#include <string>
#include <math.h>
#include "../headers/functions.h"
using namespace std;

/*================================================================================================
Get critical angle for total internal reflection
================================================================================================*/

void find_critical_angle(double &critical_angle, string Output)
{
  double n1 = 1;
  double n2 = 1.474;

  critical_angle = asin(n1/n2);
  if (Output == "yes")
  {
  	TabToLevel(2); cout <<"Find Critical Angle:\n";
  	TabToLevel(3); cout << "Critical Angle = " << critical_angle << endl;
  }

  
}