#include "Article.h"

// Set article title
void Article::setTitle(wstring newTitle) {
    this->title = std::move(newTitle);
}

// Add a link to the list
// arguments:
// wstring&: name of the link
// wstring&: name of the section the link was found in (This can be an empty string)
void Article::addLink(const wstring& link, const wstring& section) {
    this->links.emplace_back(link, section);
    linkCount++;
}

void Article::setRedirect(bool set){
    this->redirect = set;
}

// Returns the title of the article
wstring Article::getTitle() {
    return this->title;
}

// Returns the number of links in the article
int Article::getLinkCount() const {
    return this->linkCount;
}

bool Article::getRedirect(){
    return this->redirect;
}

// Iterator functions
// Can iterate through the links in the article using these.
// For example here is how to iterate through the links using a for loop: for(auto articleIterator = article.begin(); articleIterator != article.end(); ++articleIterator)
// articleIterator.first returns the name of the link. articleIterator.second returns the section the link was found.
// Returns the beginning iterator of the links vector. A vector<pair<string>>
std::vector<std::pair<std::wstring, std::wstring>>::iterator Article::begin() {
    return this->links.begin();
}

// Returns the ending iterator of the links vector. A vector<pair<string>>
std::vector<std::pair<std::wstring, std::wstring>>::iterator Article::end() {
    return this->links.end();
}

// Returns whether this article's title is less than the other article's title alphabetically. Used for sorting
bool Article::operator<(const Article &other) {
    return this->title < other.title;
}

// Sorts the string vector and then removes duplicates
void Article::deleteDuplicates() {
    sort(this->links.begin(), this->links.end());

    // Sorting must be done before uniques can be removed. Only adjacent duplicates can be removed with this function
    auto ip = unique(this->links.begin(), this->links.end());

    // Resizing the vector, otherwise a traversal of the vector returns empty results
    this->links.resize(distance(this->links.begin(), ip)); //Resize the vector to remove blank objects

    // Adjust the link count
    this->linkCount = this->links.size();
}
