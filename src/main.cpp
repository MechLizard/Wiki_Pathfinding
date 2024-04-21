#include <string>
#include "article.h"
#include "WikiImporter.h"

using namespace std;
using namespace chrono;

/*
 * Data structure possibility. Arrays have an O(1) for fetching an array number.
 * 1. Find the amount of articles in wikipedia.
 * 2. Declare an array of that size.
 * 3. Each number of the array corresponds to an article as it appears in wikipedia. That number in the array has a pointer to the article object.
 * 4. This can be read in from a processed object dump as long as it's in the same order.
 * 5. The links have a number to this array (The object can also have the name of the article and the section it was in, but this won't be used for searching, just the final display)
 * 6. When storing this data to the file each line represents an article with all of its links and sections.
 */

int main() {
    // Function test.

    // Start override. When enabled, doesn't collect any articles until the one in startOverrideTitle is found
    // Used to restart the program if it crashes.
    bool override = false;
    wstring overrideText = L"";

    char proccessedInputfile[] = R"(articles.txt)";


    WikiImporter importer;

    // Load in all the created articles
    unordered_map<wstring, Article *> articles = importer.readInArticles(proccessedInputfile);

    cout << "complete" << endl;
}