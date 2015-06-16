#include "FileProperties.h"

namespace dirc
{
	FileProperties::FileProperties(std::string &FullPath) : fullpath(FullPath) { setnewpath(FullPath);	}
	FileProperties::~FileProperties(){}

	std::string FileProperties::get_before(std::string item, std::string of){
		int pos = of.rfind(item);
		return of.substr(0, pos);
	}

	std::string FileProperties::get_after(std::string item, std::string of){
		int pos = of.rfind(item);
		++pos;									// go right beyond location
		return of.substr(pos);
	}
	std::string FileProperties::get_after_with(std::string item, std::string of){
		int pos = of.rfind(item);
		return of.substr(pos);
	}

	void FileProperties::get_filename(){
		filename = get_after("/", fullpath);
	}
	void FileProperties::get_suffix(){
		suffix = get_after_with(".", filename);
	}
	std::string FileProperties::get_filename_without_suffix(){
		return get_before(suffix, filename);
	}
	std::string FileProperties::get_path_without_suffix(){
		return get_before(suffix, fullpath);
	}

	void FileProperties::get_directory(){
		int pos = fullpath.find(filename);
		directory = fullpath.substr(0, pos);
	}

	void FileProperties::copyfile(){
	}

}