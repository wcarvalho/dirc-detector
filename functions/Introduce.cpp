#include <iostream>
#include <string>
#include "../headers/functions.h"

using namespace std;

void Introduce(string title, string Output)
{
  if (Output == "yes")
  {
  	TabToLevel(2); cout << title << ":\n";
  }

}