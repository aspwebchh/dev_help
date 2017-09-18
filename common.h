#pragma once

#include <string>
#include <vector>

using namespace std;

enum TagType {
	script = 1,
	style = 2
};

enum UrlType{
	withVersionNum = 1,
	withParams = 2,
	onlyUrl = 3
};

struct TagStruct
{
	string sourcePath;
	string tagHtml;
	TagType tagType;
	string url;
	string versionNum;
	UrlType urlType;
};

bool exists(string val, string str);
string fileContent(string path);
void getAllFiles(string path, vector<string>& files);
vector<string> getFiles(string cate_dir);
void saveFile(string content, string path);
string int2String(int value);
string replaceAll(std::string str, const std::string from, const std::string to);