#ifndef FileProps
#define FileProps

#include <iostream>
#include <string>

using namespace std;

class FileProperties 
{
public:
	FileProperties(string FullPath) : fullpath(FullPath) {
		get_filename();
		get_directory();
	}
	~FileProperties(){}


	void get_filename();
	void get_directory();
	void copyfile();
	void setnewpath(string newpath){ fullpath = newpath; get_filename(); get_directory(); }
	void change_directory(string direc){
		fullpath = direc.append(filename); get_filename(); directory = direc;
	}
	void appendFileToDirectory(string directory, string &file){
		file = directory.append(file);
	}
	
	string fullpath;
	string filename;
	string directory;
	string suffix;
	
};

#endif