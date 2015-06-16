#ifndef __DIRCFILEPROPERTIES__
#define __DIRCFILEPROPERTIES__

#include <iostream>
#include <string>

namespace dirc
{
	class FileProperties
	{
	public:
		FileProperties(std::string &FullPath);
		~FileProperties();

		std::string get_before(std::string item, std::string of);
		std::string get_after(std::string item, std::string of);
		std::string get_after_with(std::string item, std::string of);

		void get_filename();
		void get_suffix();
		std::string get_filename_without_suffix();
		std::string get_path_without_suffix();

		void get_directory();
		void copyfile();
		void setnewpath(std::string newpath){ fullpath = newpath; get_filename();	get_suffix(); get_directory(); }
		void change_directory(std::string direc){
			fullpath = direc.append(filename); get_filename(); directory = direc;
		}
		void appendFileToDirectory(std::string directory, std::string &file){
			file = directory.append(file);
		}
		std::string file(){ return fullpath; }

		std::string suffix;
		std::string filename;
		std::string directory;
	private:
		std::string fullpath;
	};
}

#endif