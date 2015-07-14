#include   "fitter.h"

// takes in the global bin index "bin" from the lookup table file, as well as the number of bins for each of the variables (x,y,theta,phi,v)
// the ordering (1,2,3,4,5) <--> (x,y,theta,phi,v)
// nbins{i} is the number of bins for variable {i}.  Note that the number of xbins is not needed
// fills bin{i} with the bin index for variable {i}, which can then be passed to getBinCenters
void bin5D(unsigned int global_bin, unsigned int nbins2, unsigned int nbins3, unsigned int nbins4, unsigned int nbins5, unsigned int& bin1, unsigned int& bin2, unsigned int& bin3, unsigned int& bin4, unsigned int& bin5)
{
	// don't current need this function but when I do it'll be available
  unsigned int temp1 = nbins2*nbins3*nbins4*nbins5;
  bin1 = global_bin/temp1;
  unsigned int temp2 = global_bin - bin1*temp1;
  temp1 = nbins3*nbins4*nbins5;
  bin2 = temp2/temp1;
  temp2 -= bin2*temp1;
  temp1 = nbins4*nbins5;
  bin3 = temp2/temp1;
  temp2 -= bin3*temp1;
  temp1 = nbins5;
  bin4 = temp2/temp1;
  temp2 -= bin4*temp1;
  bin5 = temp2;
}

void LookUpTable::setNDimension(int n){
	N = n;
	// nbins = new int[N];
	// lengths = new double[N];
	// steps = new double[N];
	// mins = new double[N];
}

void LookUpTable::setLengths(double givenLengths_low[], double givenLengths_hi[]){
	for (unsigned int i = 0; i < N; ++i){
		lengths.push_back(givenLengths_hi[i]-givenLengths_low[i]);
		mins.push_back(givenLengths_low[i]);
	}
}

void LookUpTable::setNBins(int givenBins[]){
	for (unsigned int i = 0; i < N; ++i){
		nbins.push_back(givenBins[i]);
		steps.push_back((double)(lengths[i])/nbins[i]);
	}
}


void LookUpTable::tableFromTxt(string file){
	ifstream f;
	f.open(file.c_str());

	string line;
	int nLines = 0;

	if(!f.good()){
		cout << "ERROR: There is something wrong with the file " << file << endl;
		exit(1);
	}

	while(getline(f, line))
		++nLines;

	nValues = nLines;

	// go to beginning
	f.clear();
	f.seekg (0, f.beg);

	nLines = 0;
	while(getline(f, line)){
		stringstream  lineStream(line);
		double value;
    // Read an integer at a time from the line
    int which = 0;
    while(lineStream >> value)
    {
    	if (which == 1) MEAN.push_back(value);
  		if (which == 2)	RMS.push_back(value);
    	++which;
    }
		++nLines;
	}

	f.close();

}


// gets the bin of each value
void LookUpTable::getBin(double const values[], int bin[]){
	for (unsigned int i = 0; i < N; ++i){
		bin[i] = (int)(( (values[i]-mins[i]))/steps[i]);
		if (bin[i]==nbins[i]) bin[i] = nbins[i] - 1;
		if ( (bin[i] < 0) || (bin[i]) > nbins.at(i) ){
			cout << "bins are out of bounds!!\nEXITING\n";
			exit(1);
		}
	}
}

// gets the bin center for each value
void LookUpTable::getBinCenters(int bin[], double centers[]){
	for (unsigned int i = 0; i < N; ++i){
	  centers[i] = mins[i] + steps[i]*0.5 + bin[i]*steps[i];
	}
}

// knowing the bins, get the globalbin
int LookUpTable::getGlobalBin(int bin[]){
	int p[N]; p[0] = bin[0];
	for (unsigned int i = 1; i < N; ++i){
		p[i] = bin[i] + nbins[i]*p[i-1];
	}
	return p[N-1];
}

// look up mean from lookuptable
double LookUpTable::getMean(int globalBin){
	return MEAN[globalBin];
}

// look up rms from lookuptable
double LookUpTable::getRMS(int globalBin){
	return RMS[globalBin];
}



void LookUpTable::Interpolate(double values[], double &mean, double &rms){
	switch(Case) {
		case 1: // no Interpolation
			NoInterpolation(values, mean, rms);
		break;
		case 2: // LinearInterpolation
			LinearInterpolation(values, mean, rms);
		break;
		case 3: // Splines
			cout << "Spline Interpolation is not yet defined\n"; exit(1);
		break;
	}
}

void LookUpTable::NoInterpolation(double values[], double &mean, double &rms){
	int* bin; bin = new int[N];
	getBin(values, bin);
	mean = getMean(getGlobalBin(bin));
	delete bin;
}
void LookUpTable::LinearInterpolation(double values[], double &mean, double &rms){

	int* bin_start; bin_start = new int[N];
	int* bin_end; bin_end = new int[N];

	int* bin; bin = new int[N];
	double* centers; centers = new double[N];

	getBin(values, bin);
	getBinCenters(bin, centers);

	double delta_domain_squared = 0.;

	for (unsigned int i = 0; i < N; ++i){
		delta_domain_squared += steps[i]*steps[i];
		double* value = &values[i];
		double* center = &centers[i];
		bool greater = *value > *center;
		bool lessthan = *value < *center;

		if (greater){
			bin_start[i] = bin[i];
			bin_end[i] = bin[i] + 1;
		}
		else if (lessthan){
			bin_start[i] = bin[i] - 1;
			bin_end[i] = bin[i];
		}
		else{
			mean = getMean(getGlobalBin(bin));
			delete bin_start;
			delete bin_end;
			delete bin;
			delete centers;
			return;
		}
	}
	double delta_domain = sqrt(delta_domain_squared);

	double range_start = getMean(getGlobalBin(bin_start));
	double range_end = getMean(getGlobalBin(bin_end));
	double delta_range = range_end - range_start;

	double slope = delta_range/delta_domain;

	/* use idea: y2 = y1 + delta_x*m, where
	y2 = desired mean
	y1 = range_start;
	x1 = centers_start
	x2 = values
	m = slope
	delta_x = |x2-x1| */

	double centers_start[N]; 	getBinCenters(bin_start, centers_start);			// find the centers for values associated with range_end

	double delta_x = 0.;
	for(unsigned int i = 0; i < N; ++i)
		delta_x += (values[i]-centers_start[i])*(values[i]-centers_start[i]);

	delta_x = sqrt(delta_x);

	mean = range_start+delta_x*slope;

	for (unsigned int i = 0; i < N; ++i){
		cout << "value = " << values[i] << endl;
		cout << "bin = " << bin[i] << ",\t";
		cout << "bin_center = " << centers[i] << endl;
		cout << "bin_start = " << bin_start[i] << ",\t";
		cout << "centers_start = " << centers_start[i] << "\n\n";
	}
	cout << "range_default = " << getMean(getGlobalBin(bin)) << endl;
	cout << endl;


	print = true;
	if (print){
		cout << "mean_before = " << range_start << endl;
		// cout << "mean_at = " << mean << endl;
		cout << "mean_after = " << range_end << endl;
	}

	delete bin_start;
	delete bin_end;
	delete bin;
	delete centers;
}


std::pair<double, double> LookUpTable::getMeanRmsPair(double values[]){

	int bin[N];
	//------------------------
	// temporary change to correct for domain of phi, bin 3. It counts from 0, instead of -pi (i.e. made lookuptable for 0->2pi, but values are from -pi->pi)
	double pi = 3.14159265358979312;
	values[3] += 2*pi;
	//------------------------

	double mean = 0.;
	double rms = 0.;
	Interpolate(values, mean, rms);

	pair<double, double> MeanRms(mean, rms);
	values[3] -= 2*pi;
	return MeanRms;
}


std::pair<double, double> LookUpTableWrapper(double const& x, double const& y, double const& theta, double const& phi, double const& v){
	double pi = TMath::Pi();
	double lengths_low[5] = {0, 0, 0, 0, .7};
	double lengths_hi[5] = {490, 3.5, pi/2, 2*pi, 1};
	int nbins[5] = {20, 5, 10, 10, 5};
	string file = "settings/LookUpTable_mod";
	static LookUpTable L(5, lengths_low, lengths_hi, nbins, file, 1);

	double X[5];
	X[0] = x;
	X[1] = y;
	X[2] = theta;
	X[3] = phi;
	X[4] = v;

	return L.getMeanRmsPair(X);
}
