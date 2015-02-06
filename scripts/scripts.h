#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;


// returns n by 2 array for files
vector< vector< double > > filearray(string file){

	ifstream f;
	f.open(file.c_str());

	vector< vector< double > > x; x.clear();
	vector< double > temp; temp.clear();

	int size = pow(2,5);
	char val1[size];
	char val2[size];

	if (! (f.good()) ){
		cout << "file '" << file << "' is no good!\n";
		exit(1);
	}

	while(f.good()){
		f.getline(val1, size, ',');
		f.getline(val2, size);
		temp.push_back(atof(val1));
		temp.push_back(atof(val2));
		x.push_back(temp);
		temp.clear();

	}
	return x;
}

void file_to_dir(string const &directory, string &file){
	stringstream ss; ss.str("");
	ss << directory << "/" << file;
	file = ss.str();
}