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
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>  
#include <map>

using namespace std;

const string SOURCE_VERSION = "source_version";


class FileData {
private:
	static map<string, TagsAndContent> dataSource;

public:

	static void add(string path, TagsAndContent tagsAndContent) {
		dataSource.insert(pair<string, TagsAndContent>(path, tagsAndContent));
	}
	static TagsAndContent findContent(string path) {
		map<string, TagsAndContent>::iterator result = dataSource.find(path);
		if (result != dataSource.end()) {
			return result->second;
		}
	}
};

map<string, TagsAndContent> FileData::dataSource;


string extractUrl(string tag) {
	regex reg(".+?(src|href)=(\"|')(.+?)\\2.+");
	smatch match;
	string result;
	if (regex_match(tag, match, reg)) {
		result = match[match.size() - 1];
	}
	return result;
}

string getCurrWorkPath() {
	char buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	return string(buffer);
}

tuple<string, UrlType> extractVersionNum(string url) {
	regex reg("\\.(js|css)\\?.+");
	string result;
	if (!regex_search(url, reg)) {
		return{ result, onlyUrl };
	}
	regex reg1(".+?(\\?|&)"+ SOURCE_VERSION +"=([^&$]+).*");
	smatch match;
	if (!regex_match(url, match, reg1)) {
		return{ result,withParams };
	}
	return{ match[match.size() - 1] , withVersionNum };
}

string newVersionNum() {
	auto now = time(NULL);
	auto tim = new tm();
	localtime_s(tim, &now);
	auto year = int2String(tim->tm_year + 1900);
	auto month = int2String(tim->tm_mon + 1);
	auto day = int2String(tim->tm_mday);
	auto hour = int2String(tim->tm_hour);
	auto min = int2String(tim->tm_min);
	auto sec = int2String(tim->tm_sec);
	delete tim;
	return year + month + day + hour + min + sec;
}

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

string genUrlWithVersionNum(string url, UrlType urlType) {
	string newUrl;
	regex reg(SOURCE_VERSION + "=[^&$]*");
	auto nvn = newVersionNum();
	switch (urlType)
	{
	case withVersionNum:
		newUrl = regex_replace(url, reg, SOURCE_VERSION + "=" + newVersionNum());
		break;
	case withParams:
		newUrl = url + "&" + SOURCE_VERSION + "=" + nvn;
		break;
	case onlyUrl:
		newUrl = url + "?"+ SOURCE_VERSION +"=" + nvn;
		break;
	default:
		break;
	}
	return newUrl;
}

void updateVersionNum(string filePath, string fileContent, vector<TagStruct> tagDataList) {
	if (tagDataList.size() == 0) {
		return;
	}
	for (int i = 0; i < tagDataList.size(); i++) {
		auto tagItem = tagDataList[i];
		auto newUrl = genUrlWithVersionNum(tagItem.url, tagItem.urlType);
		auto newTag = replaceAll(tagItem.tagHtml, tagItem.url, newUrl);
		fileContent = replaceAll(fileContent, tagItem.tagHtml, newTag);
	}
	saveFile(fileContent, filePath);
	cout << "文件 " << filePath << " 更新成功"  << endl;
}

vector<TagStruct> findTag(string htmlPath, string content, string regexString, TagType tagType) {
	regex reg(regexString, regex_constants::icase);
	vector<TagStruct> newResult;
	for (sregex_iterator it(content.begin(), content.end(), reg), end; it != end; ++it) {
		auto tag = it->str();

		TagStruct tagStruct;
		tagStruct.sourcePath = htmlPath;
		tagStruct.tagHtml = tag;
		tagStruct.tagType = tagType;
		tagStruct.url = extractUrl(tag);

		auto versionNumInfo = extractVersionNum(tagStruct.url);
		tagStruct.versionNum = get<0>(versionNumInfo);
		tagStruct.urlType = get<1>(versionNumInfo);

		newResult.push_back(tagStruct);
	}
	return newResult;
}

vector<TagStruct> mergeTags(vector<TagStruct> v1, vector<TagStruct> v2) {
	vector<TagStruct> v3 = {};
	v3.insert(v3.end(), v1.begin(), v1.end());
	v3.insert(v3.end(), v2.begin(), v2.end());
	return v3;
}

vector<TagStruct> findTag(string path) {
	const auto content = fileContent(path);
	const auto scriptTags = findTag(path, content, "<script.+?<\/script>", script);
 	const auto linkTags = findTag(path, content, "<link[^>]+>", style);
	const auto tags = mergeTags(scriptTags, linkTags);

	TagsAndContent tagsAndContent;
	tagsAndContent.content = content;
	tagsAndContent.tagData = tags;
	FileData::add(path, tagsAndContent);

	return tags;
}


void previewFile( string htmlFilePath, vector<TagStruct> tags ) {
	if (tags.size() == 0) {
		return;
	}
	cout << "html文件：" << htmlFilePath << " 文件中有以下静态资源" << endl;
	for (int i = 0; i < tags.size(); i++) {
		const auto item = tags[i];
		cout << "    " << item.url << endl;
	}
	cout << endl;
}


int main() {
	vector<string> files;
	getAllFiles(getCurrWorkPath(), files);
	const auto htmlPaths = findHtmlFiles(files);
	for (int i = 0; i < htmlPaths.size(); i++) {
		auto path = htmlPaths[i];
		auto tags = findTag(path);
		previewFile(path,tags);
	}
	cout << "是否更新请输入Y/N" << endl;

	string confirm;
	cin >> confirm;
	transform(confirm.begin(), confirm.end(), confirm.begin(), ::toupper);

	if (confirm == "Y") {
		for (int i = 0; i < htmlPaths.size(); i++) {
			const auto path = htmlPaths[i];
			const auto tagsAndContent = FileData::findContent(path);
			updateVersionNum(path, tagsAndContent.content, tagsAndContent.tagData);
		}
		while (true);
	}
	return 0;
}