#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

/*================================================
Print a 2D vector
================================================*/

void Print_2D_Vector(vector< vector<double> > array)
{
  
  int i, j;

  for(i=0; i<array.size(); i++){
    cout << setw(17) << "(" << i << ")" << setw(5) << "|";
    for (j=0; j<array[0].size(); j++){
    	// printf ("%f \t|", 5, array[i][j]);
    }
    cout << endl;
  }

}