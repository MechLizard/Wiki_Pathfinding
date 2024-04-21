#include <iostream>
#include <napi.h>
#include <codecvt>
#include <locale>
#include "../src/Article.h"
#include "../src/WikiImporter.h"
#include "../src/bfs.h"
#include "../src/dfs.h"
#include <chrono>
#include <sstream>
#include <vector>

using namespace chrono; 
using namespace std;

// Global instance of WikiImporter to maintain state across calls
WikiImporter importer;
unordered_map<wstring, Article *> articles;

// Function to load articles from a file
void LoadArticles(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Path to articles.txt expected as a string").ThrowAsJavaScriptException();
        return;
    }

    std::string filePath = info[0].As<Napi::String>();
    articles = importer.readInArticles(filePath.c_str()); // Ensure this modifies the global `articles`
}

wstring outputArticles(vector<Article*> &path, time_point<high_resolution_clock> start) {
    wstring output;
    wstringstream ss;
    int articleCount = 0;

    for (size_t i = 0; i < path.size(); ++i) {
        output += path[i]->getTitle();

        if (path[i]->getRedirect() && i + 1 < path.size()) {
            ss << L" (redirects to " << path[i + 1]->getTitle() << L")";
            output += ss.str();
            ss.str(L""); // Clears the stringstream
            ++i; // Skip the next article since it's a redirect target
        }

        if (i + 1 < path.size()) {
            output += L" -> ";
        }

        ++articleCount;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    output += L"\n";
    ss << articleCount << L" hops in " << duration.count() << L" milliseconds\n";
    output += ss.str();

    return output;
}

// Function to perform search and return path as string
Napi::String PerformBothSearches(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    std::string origin = info[0].As<Napi::String>();
    std::string destination = info[1].As<Napi::String>();
    
    if (info.Length() < 2 || !origin.IsString() || destination.IsString()) {
        Napi::TypeError::New(env, "Expected two string arguments: origin and destination").ThrowAsJavaScriptException();
        return Napi::String::New(env, "");
    }


    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring originW = converter.from_bytes(origin);
    std::wstring destinationW = converter.from_bytes(destination);

    wstring output;

    // Perform BFS
    auto start = chrono::high_resolution_clock::now();
    auto bfsPath = BFS(articles, originW, destinationW);
    if (bfsPath) {
        output += L"BFS Path:\n";
        output += outputArticles(*bfsPath, start);
    } else {
        output += L"BFS: Failed to find a connecting path\n";
    }

    // Perform DFS
    auto start2 = chrono::high_resolution_clock::now();
    auto dfsPath = DFS(articles, originW, destinationW);
    if (dfsPath) {
        output += L"\nDFS Path:\n";
        output += outputArticles(*dfsPath, start2);
    } else {
        output += L"DFS: Failed to find a connecting path\n";
    }

    // Convert wstring output to string
    std::string outputStr = converter.to_bytes(output);
    return Napi::String::New(env, outputStr);
}


Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("loadArticles", Napi::Function::New(env, LoadArticles));
    exports.Set("performBothSearches", Napi::Function::New(env, PerformBothSearches));
    return exports;
}

NODE_API_MODULE(addon, Init)
