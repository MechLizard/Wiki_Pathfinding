#ifndef BFS_H
#define BFS_H

#include <unordered_map>
#include <queue>
#include <vector>
#include <optional>
#include "article.h"

// Function to perform Breadth-First Search
// Returns an optional vector representing the shortest path from startTitle to targetTitle,
// or an empty optional if no path exists.
std::optional<std::vector<std::wstring>> BFS(std::unordered_map<std::wstring, Article*>& articles, 
                                             const std::wstring& startTitle, 
                                             const std::wstring& targetTitle) {
    // Check if start and target articles exist in the map
    if (articles.find(startTitle) == articles.end() || articles.find(targetTitle) == articles.end()) {
        return std::nullopt; // Return empty optional if either article is not found
    }

    // Mark all articles as not visited
    std::unordered_map<std::wstring, bool> visited;
    for (auto& entry : articles) {
        visited[entry.first] = false;
    }

    // Queue for BFS traversal
    std::queue<std::wstring> q;

    // Map to store parent of each article in the BFS traversal
    std::unordered_map<std::wstring, std::wstring> parent;

    // Mark the starting article as visited and enqueue it
    visited[startTitle] = true;
    q.push(startTitle);

    while (!q.empty()) {
        std::wstring currentTitle = q.front();
        q.pop();

        // If the target article is found, reconstruct and return the path
        if (currentTitle == targetTitle) {
            std::vector<std::wstring> path;
            // Reconstruct path by tracing back from targetTitle to startTitle using parent map
            while (currentTitle != startTitle) {
                path.push_back(currentTitle);
                currentTitle = parent[currentTitle];
            }
            path.push_back(startTitle); // Add startTitle to the path
            std::reverse(path.begin(), path.end()); // Reverse path to get correct order
            return path; // Return the shortest path
        }

        // Traverse all links of the current article
        Article* currentArticle = articles[currentTitle];
        for (auto& link : *currentArticle) {
            if (!visited[link.first]) {
                visited[link.first] = true;
                parent[link.first] = currentTitle; // Store parent of linked article
                q.push(link.first);
            }
        }
    }

    return std::nullopt; // Return empty optional if no path exists
}

#endif // BFS_H
