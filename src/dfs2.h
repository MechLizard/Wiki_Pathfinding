#ifndef DFS_H
#define DFS_H

#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <optional>
#include "Article.h"

// Function to perform Depth-First Search
// Returns true if a path exists from startTitle to targetTitle, false otherwise.
std::optional<bool> DFS(std::unordered_map<std::wstring, Article*>& articles,
                        const std::wstring& startTitle,
                        const std::wstring& targetTitle,
                        std::unordered_set<std::wstring>& visited) {


    if (startTitle == targetTitle) {
        return true;
    }

    visited.insert(startTitle);

    auto it = articles.find(startTitle);
    if (it == articles.end()) {
        return std::nullopt;
    }

    for (auto linkPair : *(it->second)) {
        const std::wstring& linkTitle = linkPair.first;
        if (visited.find(linkTitle) == visited.end()) {
            auto result = DFS(articles, linkTitle, targetTitle, visited);
            if (result.has_value() && *result) {
                return true;
            }
        }
    }

    return false;
}

std::optional<bool> DFS(std::unordered_map<std::wstring, Article*>& articles,
                        const std::wstring& startTitle,
                        const std::wstring& targetTitle) {
    std::unordered_set<std::wstring> visited;
    return DFS(articles, startTitle, targetTitle, visited);
}

#endif // DFS_H