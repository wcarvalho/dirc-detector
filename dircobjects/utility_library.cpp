#include "utility_library.h"

using namespace std;

namespace dirc
{
	std::string appendStrings(std::string string1, std::string string2, std::string string3, std::string string4, std::string string5)
	{
		std::stringstream ss; ss.str("");
		ss << string1 << string2 << string3 << string4 << string5;
		return ss.str();
	}

	std::string stringDouble(std::string s, double d)
	{
		stringstream ss; ss.str("");
		ss << s << d;
		return ss.str();
	}

	void vectorminmax(std::vector<double> const& x, double &min, double &max){
		min = *min_element(x.begin(), x.end());
		max = *max_element(x.begin(), x.end());
	}

	// template <typename T>
	// void readinFile(std::string file, std::vector<T> vec, std::string delimeter){

	// }
}