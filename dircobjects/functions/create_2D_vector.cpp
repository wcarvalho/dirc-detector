#include <iostream>
#include <vector>
using namespace std;

/*================================================
create a 2D vector of size chosen by user
================================================*/

void create_2D_vector(vector< vector<double> > &array, int rows, int number_of_columns){
  
  int i = 0, j = 0;

  array.clear();
  for(i=0; i<rows; i++){
    array.push_back( vector<double>() );
    for (j=0; j<number_of_columns; j++){
      array[i].push_back(0);
    }
  }
}