#include "stdafx.h"
#include "common.h"
#include <io.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <regex>
#include <tuple>

using namespace std;

bool exists(string val, string str) {
	string::size_type loc = str.find(val, 0);
	return loc != string::npos;
}

string fileContent(string path) {
	ifstream f(path);
	stringstream buffer;
	buffer << f.rdbuf();
	string result(buffer.str());
	return result;
}

void getAllFiles(string path, vector<string>& files) {
	long  hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		do {
			if ((fileinfo.attrib &  _A_SUBDIR)) {
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else {
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

vector<string> getFiles(string cate_dir) {
	string path = cate_dir;
	cate_dir = cate_dir + "\\*";
	vector<string> files;//存放文件名  
	_finddata_t file;
	long lf;
	//输入文件夹路径  
	if ((lf = _findfirst(cate_dir.c_str(), &file)) == -1) {
		cout << cate_dir << " not found!!!" << endl;
	}
	else {
		while (_findnext(lf, &file) == 0) {
			//输出文件名  
			//cout<<file.name<<endl;  
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			files.push_back(path + "\\" + file.name);
		}
	}
	_findclose(lf);
	return files;
}


void saveFile(string content) {
	string path = "C:\\Users\\宏鸿\\Desktop\\test.txt";
	ofstream outfile;
	outfile.open(path);
	if (!outfile)
	{
		cout << "txt文件打开失败!" << endl;
		exit(0);
	}
	outfile << content;
	outfile.close();
}


string int2String(int value) {
	stringstream ss;
	ss << value;
	return ss.str();
}

string replaceAll(std::string str, const std::string from, const std::string to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
	return str;
}