#ifndef DFS_H
#define DFS_H

#include <unordered_map>
#include <stack>
#include <optional>
#include "article.h"

// Function to perform Depth-First Search
// Returns true if a path exists from startTitle to targetTitle, false otherwise.
std::optional<bool> DFS(std::unordered_map<std::wstring, Article*>& articles, 
                        const std::wstring& startTitle, 
                        const std::wstring& targetTitle) {
    // Check if start and target articles exist in the map
    if (articles.find(startTitle) == articles.end() || articles.find(targetTitle) == articles.end()) {
        return std::nullopt; // Return empty optional if either article is not found
    }

    std::unordered_map<std::wstring, bool> visited;
    std::stack<std::wstring> stack;

    // Mark all articles as not visited
    for (auto& entry : articles) {
        visited[entry.first] = false;
    }

    // Push the start article onto the stack
    stack.push(startTitle);

    while (!stack.empty()) {
        std::wstring currentTitle = stack.top();
        stack.pop();

        // If the current article is the target, return true
        if (currentTitle == targetTitle) {
            return true;
        }

        // If the current article has not been visited yet
        if (!visited[currentTitle]) {
            visited[currentTitle] = true;

            Article* currentArticle = articles[currentTitle];

            // Traverse all links of the current article
            for (auto& link : *currentArticle) {
                // If the linked article has not been visited yet, push it onto the stack
                if (!visited[link.first]) {
                    stack.push(link.first);
                }
            }
        }
    }

    return false; // Return false if no path exists
}

#endif // DFS_H


