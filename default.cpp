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


vector<string> findHtmlFiles(vector<string> allFiles) {
	regex reg1(".+html$", regex_constants::icase);
	vector<string> htmlFiles;
	for (int i = 0; i < allFiles.size(); i++) {
		string path = allFiles[i];
		if (regex_match(path, reg1)) {
			htmlFiles.push_back(path);
		}
	}
	return htmlFiles;
}

vector<tuple<string, TagType>> findTag( string htmlPath, string regexString, TagType tagType ) {
	vector<tuple<string, TagType>> results;
	string content = fileContent(htmlPath);
	regex reg(regexString, regex_constants::icase);
	for (sregex_iterator it(content.begin(), content.end(), reg), end; it != end; ++it) {
		results.push_back({ it->str(), tagType });
	}
	return results;
}

vector<tuple<string, TagType>> mergeTags(vector<tuple<string, TagType>> v1, vector<tuple<string, TagType>> v2) {
	vector<tuple<string, TagType>> v3 = {};
	v3.insert(v3.end(), v1.begin(), v1.end());
	v3.insert(v3.end(), v2.begin(), v2.end());
	return v3;
}

vector<tuple<string,TagType>> findTag(string path) {
	auto scriptTags = findTag(path, "<script.+?<\/script>", script);
	auto linkTags = findTag(path, "<link[^>]+>", style);
	auto tags = mergeTags(scriptTags, linkTags);
	return tags;
}

int main() {
	vector<string> files;
	getAllFiles("C:\\Users\\���\\Desktop\\��������\\in-game Community (1)", files);
	vector<string> htmlPaths = findHtmlFiles(files);
	for (int i = 0; i < htmlPaths.size(); i++) {
		string path = htmlPaths[i];
		if (exists("test.html",path)){
			auto tags = findTag(path);
			for (int i = 0; i < tags.size(); i++) {
				auto tag = get<0>(tags[i]);
				auto tagType = get<1>(tags[i]);
				cout << tag << endl;
			}
		}
	}
	cin.get();
	return 0;
}