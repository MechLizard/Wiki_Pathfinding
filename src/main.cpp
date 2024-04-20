#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "article.h"
#include "WikiImporter.h"
#include "bfs.h"
#include "dfs.h"

using namespace std;
using namespace chrono;

wstring outputArticles(vector<Article*> &path, time_point<std::chrono::high_resolution_clock> start) {
    // Iterate through the articles and print the path.
    wstring output;
    wstringstream ss;
    int articleCount = 0;
    for (int i = 0; i < path.size(); i++) {
        //for (auto &article: *path) {
        output += path.at(i)->getTitle();

        if (path.at(i)->getRedirect()) {
            i++;
            if(path.at(i) != *path.end()) {
                ss << L" (redirects to " << path.at(i)->getTitle() << L")";
                output += ss.str();
                ss.clear();
            }
        }
        output += L" -> ";
        articleCount += 1;
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = stop - start;
    output.erase(output.end() - 4, output.end());
    output += L"\n";
    ss << articleCount << L" hops in " << duration.count() / 1000000 << L" milliseconds\n";
    output += ss.str();
    ss.clear();

    return output;
}

int main() {

    string searchType = "DFS";
    char proccessedInputfile[] = R"(articles.txt)";

    WikiImporter importer;

    // Load in all the created articles
    unordered_map<wstring, Article *> articles = importer.readInArticles(proccessedInputfile);
    wstring origin;
    wstring destination;

    while (true) {
        cout << "Enter the origin article title" << endl;
        getline(wcin, origin);
        if (articles.find(origin) == articles.end()) {
            wcout << origin << L" does not exist. Please try again" << endl;
            continue;
        }

        cout << "Enter the destination article title" << endl;
        wstring output;
        getline(wcin, destination);
        if (articles.find(destination) == articles.end()){
            wcout << destination << L" does not exist. Please try again" << endl;
            continue;
        }

        auto start = chrono::high_resolution_clock::now();
        if (searchType == "BFS")
            if (auto path = BFS(articles, origin, destination))
                output = outputArticles(*path, start);
            else
                output = L"Failed to find a connecting path\n";

        if (searchType == "DFS")
            if (auto path = DFS(articles, origin, destination))
                output = outputArticles(*path, start);
            else
                output = L"Failed to find a connecting path\n";

        wcout << output;
    }
}