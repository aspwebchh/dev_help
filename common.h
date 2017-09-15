#pragma once

#include <string>
#include <vector>

using namespace std;

enum TagType
{
	script = 1,
	style = 2
};

bool exists(string val, string str);
string fileContent(string path);
void getAllFiles(string path, vector<string>& files);
void saveFile(string content);