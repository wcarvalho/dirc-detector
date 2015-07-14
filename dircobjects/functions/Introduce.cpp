#include <iostream>
#include <string>
#include "functions.h"

using namespace std;

void Introduce(string title, string Output)
{
  if (Output == "yes")
  {
  	TabToLevel(2); cout << title << ":\n";
  }

}