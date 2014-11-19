#include "../headers/file-properties.h"

void FileProperties::get_filename(){
	int pos = fullpath.rfind("/");
	++pos;									// go right beyond location of from
	filename = fullpath.substr(pos);
}

void FileProperties::get_directory(){
	int pos = fullpath.find(filename);
	directory = fullpath.substr(0, pos);
}

void FileProperties::copyfile(){

}