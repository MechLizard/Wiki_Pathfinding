#ifndef BFS_H
#define BFS_H

#include <unordered_map>
#include <queue>
#include <vector>
#include <optional>
#include "Article.h"
#include <unordered_set>

// Function to perform Breadth-First Search
// Returns an optional vector representing the shortest path from startTitle to targetTitle,
// or an empty optional if no path exists.
std::optional<std::vector<std::wstring>> BFS(std::unordered_map<std::wstring, Article*>& articles,
                                             const std::wstring& startTitle,
                                             const std::wstring& targetTitle) {
    std::unordered_set<std::wstring> visited;
    std::queue<std::wstring> queue;
    std::unordered_map<std::wstring, std::wstring> parent;

    queue.push(startTitle);
    visited.insert(startTitle);

    while (!queue.empty()) {
        std::wstring current = queue.front();
        queue.pop();

        if (current == targetTitle) {
            std::vector<std::wstring> path;
            while (current != startTitle) {
                path.push_back(current);
                current = parent[current];
            }
            path.push_back(startTitle);
            std::reverse(path.begin(), path.end());
            return path;
        }

        auto it = articles.find(current);
        if (it != articles.end()) {
            for (auto linkPair = it->second->begin(); linkPair != it->second->end(); ++linkPair) {
                const std::wstring& linkTitle = linkPair->first;
                if (visited.find(linkTitle) == visited.end()) {
                    queue.push(linkTitle);
                    visited.insert(linkTitle);
                    parent[linkTitle] = current;
                }
            }
        }
    }

    return std::nullopt;
}

#endif // BFS_H
