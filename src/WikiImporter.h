
#ifndef WIKIPEDIAIMPORTER_WIKIIMPORTER_H
#define WIKIPEDIAIMPORTER_WIKIIMPORTER_H
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <regex>
#include <chrono>
#include <codecvt>
#include <locale>
#include <unordered_set>
#include <unordered_map>
#include "Article.h"
using namespace std;


class WikiImporter {
private:
    static bool ends_with(std::wstring_view str, std::wstring_view suffix);
    static bool starts_with(std::wstring_view str, std::wstring_view prefix);
    static bool longSubtitle(wstring &line);
    static bool colon_check(const wstring& line, wsmatch& match);
    static void articleVectorToFile(vector<Article*>& articles, wofstream& outputFile, wofstream& categoryOutFile);
    static void overrideSkip(wifstream& wikiFile, const wstring& startOverrideTitle);
public:
    static void categoryAssignment(const string& processedFileName, const string& processedFile);
    bool processDump(string inputFilename, string outputFilename, string categoryOutFilename);
    static bool processDump(string inputFilename, string outputFilename, string categoryOutFilename, bool startOverride, wstring startOverrideTitle);
    static unordered_map<wstring, Article*> readInArticles(const string& dataFileName);
};


#endif //WIKIPEDIAIMPORTER_WIKIIMPORTER_H
