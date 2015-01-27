#include <iostream>
#include <vector>
#include "dirc_objects.h"
#include "../headers/functions.h"
#include "TVector3.h"


using namespace std;

/*================================================================================================
All functions pertinent to Displayer class
================================================================================================*/


void Displayer::Angle(double theta, double phi)
{
	TabToLevel(3); cout << "Theta = " << theta << ", phi = " << phi << endl;
}
// void Vector_1D(vector<double> array)
// {
// 	int j;

//   cout << setw(17) << "(" << i << ")" << setw(5) << "|";
//   for (j=0; j<array[0].size(); j++){
//   	printf ("%f \t|", 5, array[i][j]);
//   }

// }

// void Vector_2D(vector< vector<double> > array)
// {
//   int i;
//   Displayer d;

//   for(i=0; i<array.size(); i++){
//     d.Vector_1D(array[i]);
//     cout << endl;
//   }
// }

// void Array_1D::Displayer(double *array, int w)
// {
// 	int j;

//   cout << setw(17) << "|";
//   for (j=0; j<w; j++){
//   	printf ("%f \t|", 5, array[j]);
//   }

// }

// void Array_2D::Displayer(double **array, int l, int w)
// {
//   int i;
//   Displayer d;

//   for(i=0; i<l; i++){
//     d.Vector_1D(array[i], w);
//     cout << endl;
//   }
// }