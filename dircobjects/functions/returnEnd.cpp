#include <string>

std::string returnEnd(std::string of, std::string from){

	int pos = of.rfind(from.c_str());
	++pos;									// go right beyond location of from
	return of.substr(pos);
}

std::string returnBeg(std::string of, std::string from){

	int pos = of.find(from.c_str());
	return of.substr(0, pos);
}