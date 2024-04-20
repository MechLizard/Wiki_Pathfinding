#ifndef BFS_H
#define BFS_H

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>
#include <optional>
#include <iostream>
#include "article.h"

// Function to perform Breadth-First Search
// Returns an optional vector representing the shortest path from startTitle to targetTitle,
// or an empty optional if no path exists.
std::optional<std::vector<Article*>> BFS(std::unordered_map<std::wstring, Article*>& articles,
                                             const std::wstring& startTitle, 
                                             const std::wstring& targetTitle) {
    int depthLimit = 10;
    int depth = 0;
    std::vector<Article*> path;

    // Check if start and target articles exist in the map
    if (articles.find(startTitle) == articles.end() || articles.find(targetTitle) == articles.end()) {
        std::wcout << "Either start or end article doesn't exist" << std::endl;
        return std::nullopt; // Return empty optional if either article is not found
    }

    // Queues for BFS traversal
    std::queue<std::wstring> q1;
    std::queue<std::wstring> q2;

    // Map to store parent of each article in the BFS traversal

    std::unordered_map<Article*, Article*> parent;
    //std::unordered_map<std::wstring*, std::wstring*> parent;

    // Mark the starting article as visited and enqueue it
    parent[articles[startTitle]] = nullptr;
    q1.push(startTitle);

    while (!q1.empty() && depth <= depthLimit) {
        std::wstring currentTitle = q1.front();
        q1.pop();

        // If the target article is found, reconstruct and return the path
        if (currentTitle == targetTitle) {
            // Reconstruct path by tracing back from targetTitle to startTitle using parent map
            while (currentTitle != startTitle) {
                path.push_back(articles[currentTitle]);
                currentTitle = parent[articles[currentTitle]]->getTitle();
            }
            path.push_back(articles[startTitle]);
            std::reverse(path.begin(), path.end()); // Reverse path to get correct order
            return path; // Return the shortest path
        }

        // Traverse all links of the current article
        Article* currentArticle = articles[currentTitle];
        if(currentArticle != nullptr) {
            for (auto &link: *currentArticle) {
                if (parent.find(articles[link.first]) == parent.end()) {
                    parent[articles[link.first]] = articles[currentTitle]; // Store parent of linked article
                    q2.push(link.first);
                }
            }
        }

        // If the queue is empty it indicates that a new layer is reached. move the queue over and update the depth.
        if (q1.empty()){
            // Move queue2 into queue1. q2 is now empty.
            swap(q1, q2);
            depth += 1;
        }
    }

    std::wcout << L"No path exists" << endl;
    return std::nullopt; // Return empty optional if no path exists
}

#endif // BFS_H
