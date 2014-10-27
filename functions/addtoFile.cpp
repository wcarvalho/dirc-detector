#include <fstream>
#include <string>
#include "../headers/functions.h"

void addtoFile(std::string file, std::string toAdd)
{
	std::ofstream ofs; ofs.open(file.c_str(), std::ofstream::out | std::ofstream::app);
	int pos = toAdd.rfind("/");
	++pos;									// go right beyond "/"
	std::string temp = toAdd.substr(pos);
	ofs << temp << std::endl;
	ofs.close();
}

std::string copyFile(std::string file, std::string suffix, std::string ending){

	std::string file0 = returnEnd(file, "/");
	file0 = returnBeg(file0, suffix.c_str());								// returned the orginal file name without the suffix
	std::string tempcopy = file0; tempcopy.append(ending.c_str());			// makes a new string with new filename
	
	std::string prefix = returnBeg(file, file0);
	std::string copy = prefix; copy.append(tempcopy).append(suffix.c_str());	//makes full copy of new file with prefix

	
	std::string cmd = "exec cp "; cmd.append(file).append(" ");// copy file 1	
	cmd.append(copy);
	
	system(cmd.c_str());

	return copy;
}