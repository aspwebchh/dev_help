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


void saveFile(string content) {
	ofstream outfile("C:\\Users\\宏鸿\\Desktop\\手游社区\\in-game Community (1)\\1.txt", ios::ate);
	if (!outfile)
	{
		cout << "txt文件打开失败!" << endl;
		exit(0);
	}
	outfile << content;
	outfile.close();
}
