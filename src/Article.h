// To use this:
// #include "article.h"
// Create the article object:
// Article* anArticle = new Article;
// This can then be used with the functions below, or it can be added to a vector<Article*>

/* Article uses wstring (wide string) rather than string
 * wstring supports unicode characters which wikipedia has a lot of
 * However, all objects wstring interacts with needs to use a unicode supporting method.
 * Usually that just means adding a 'w' at the start of the object type if it doesn't inherently support it.
 * Ex: sregex -> wsregex, cout -> wcout, iostream/ofstream -> wiostream/wofstream, etc.
 *
 * When declaring a wstring an 'L' must be used when writing text to specify that it is unicode text.
 * Ex: string text = "some text";  -->  wstring text = L"some text";
*/


#include <string>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

#ifndef WIKIPEDIAIMPORTER_ARTICLE_H
#define WIKIPEDIAIMPORTER_ARTICLE_H

/* Article uses wstring (wide string) rather than string
 * wstring supports unicode characters which wikipedia has a lot of
 * However, all objects wstring interacts with needs to use a unicode supporting method.
 * Usually that just means adding a 'w' at the start of the object type if it doesn't inherently support it.
 * Ex: sregex -> wsregex, cout -> wcout, iostream/ofstream -> wiostream/wofstream, etc.
 *
 * When declaring a wstring an 'L' must be used when writing text to specify that it is unicode text.
 * Ex: string text = "some text";  -->  wstring text = L"some text";
*/

class Article {
private:
    wstring title; // Title of the article
    int linkCount; // Number of links in the article
    // Whether this page is a redirect to another page. It will contain one link.
    bool redirect = false;

    // Vector of the links on the article. First one is the link, second is the section the link appeared
    // Ex: links.at(5).first = [link name]. links.at(5).second = [section name]
    // Section can be an empty string.
    vector<pair<wstring, wstring>> links;

public:
    // Accessors
    void setTitle(wstring); // Sets article title
    void addLink(const wstring&, const wstring&); // Add a link to the list
    void setRedirect(bool set); // Sets whether this article is a redirect

    // Getters
    wstring getTitle(); // Gets article title
    int getLinkCount() const; // Get the amount of articles
    bool getRedirect(); // Get whether this page is a redirect

    // Iterator functions
    // Can iterate through the links in the article using these.
    // For example here is how to iterate through the links using a for loop:
    // for(auto articleIterator = article.begin(); articleIterator != article.end(); ++articleIterator)
    // articleIterator.first returns the name of the link. articleIterator.second returns the section the link was found.
    std::vector<std::pair<std::wstring, std::wstring>>::iterator begin();
    std::vector<std::pair<std::wstring, std::wstring>>::iterator end();

    //Operators
    bool operator<(const Article& other);

    // Functions
    void deleteDuplicates(); // Deletes duplicate links in an article. Used in file processing.
};


#endif //WIKIPEDIAIMPORTER_ARTICLE_H
