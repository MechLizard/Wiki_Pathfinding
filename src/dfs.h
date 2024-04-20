#ifndef DFS_H
#define DFS_H

#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <optional>
#include <iostream>
#include <vector>
#include "article.h"

// Function to perform Depth-First Search
// Returns true if a path exists from startArticle to targetArticle, false otherwise.
bool recurseDFS(std::unordered_map<std::wstring, Article*>& articles,
                        Article* startArticle,
                        Article* targetArticle,
                        vector<Article*> *path,
                        unordered_set<Article*> *visited,
                        int depth) {

    int depthLimit = 8;

    // Check if the start article exists
    if (startArticle == nullptr)
        return false;

    // If the current article is the target, return the objects
    if (startArticle->getTitle() == targetArticle->getTitle()) {
        path->push_back(startArticle);
        return true;
    }

    // Check if the depth limit has been reached
    if (depth == depthLimit)
        return false;

    // Check if this article has been visited. If not, mark it visited
    if(visited->find(startArticle) == visited->end())
        visited->insert(startArticle);
    else
        return false;

    // Add this article to the current path
    path->push_back(startArticle);

    // Traverse all links of the current article
    for (auto& link : *startArticle) {
        if (recurseDFS(articles, articles[link.first], targetArticle, path, visited, depth + 1))
            return true;
    }

    // Pulling out of this path, so remove the last element from the vector
    path->pop_back();
    return false;
}

std::optional<std::vector<Article*>> DFS(std::unordered_map<std::wstring, Article*>& articles,
                                         const std::wstring& startTitle,
                                         const std::wstring& targetTitle) {
    auto *path = new vector<Article*>;
    auto *visited = new unordered_set<Article*>;
    if (recurseDFS(articles, articles[startTitle], articles[targetTitle], path, visited, 0))
        return *path;

    return std::nullopt; // Return empty optional if no path exists
}

#endif // DFS_H


