#ifndef M_PI
#define M_PI 3.14159265358979312
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../headers/generator.h"


using namespace std;

vector<string> ReadValues(char value[256]){

	istringstream reader(value);
	string temp;
	vector<string> values;
	values.clear();
	while(getline(reader, temp, ',')){
		string::iterator end_pos = remove(temp.begin(), temp.end(), ' ');
		temp.erase(end_pos, temp.end());
		values.push_back(temp);
	}

	return values;
}


void ReadFloats(double into[2], vector<string> from){
	
	string delimiter = "pi";
	for (unsigned int val = 0; val < from.size(); ++val){
			string &cur_val = from[val];
			int pi_pos = cur_val.find(delimiter);
			string value = cur_val.substr(0, pi_pos);

			if (pi_pos == -1) into[val] = atof(cur_val.c_str());
			else{
				if (value.empty())
					into[val] = M_PI;
				else
					into[val] = atof(value.c_str())*M_PI;
			}
		}

}


void SetParameter(char marker[256], char value[256], int& nevents, int& maxPars, int nparticle_range[2], double etarange[2], double ptrange[2], double phirange[2], double &charge, vector<string> &types, bool& replace){

	vector<string> values = ReadValues(value);

	int cmpsize = 20;				// compare size

	string temp_marker = marker;


	if (temp_marker.find("maxpars") < 100 ){
		maxPars = atoi(values[0].c_str());
	}

	if (temp_marker.find("nparticles") < 100){
		for (unsigned int val = 0; val < values.size(); ++val){
			nparticle_range[val] = atoi(values[val].c_str());
		}
	}

	if (temp_marker.find("eta") < 100){
		ReadFloats(etarange, values);
	}

	if (temp_marker.find("pt") < 100){
		ReadFloats(ptrange, values);
	}

	if (temp_marker.find("phi") < 100){
		ReadFloats(phirange, values);
	}

	if (temp_marker.find("charge") < 100){
		charge = atof(values[0].c_str());
	}

	if (temp_marker.find("types") < 100){
		types = values;
	}

	if (temp_marker.find("replace") < 100){
		replace = true;
	}

}

void TakeInParameters(string file, int& nevents, int& maxPars, int nparticle_range[2], double etarange[2], double ptrange[2], double phirange[2], double &charge, vector<string> &types, bool& replace){

	ifstream ifs;
	ifs.open(file.c_str());

	char marker[256];
	char value[256];
	if(!ifs.good()){
		cout << "ERROR: There is something wrong with this file\n";
		exit(1);
	}
	while (ifs.good()){
		ifs.getline(marker, 256, ':');
		ifs.getline(value, 256);
		// cout << "marker = " << marker;
		// cout << "vale = " << value << endl;
		SetParameter(marker, value, nevents, maxPars, nparticle_range, etarange, ptrange, phirange, charge, types, replace);
	}
}






