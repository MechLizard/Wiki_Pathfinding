#include "WikiImporter.h"

// Wiki Markup documentation: https://en.wikipedia.org/wiki/Help:Wikitext

/* File processing notes
 * Title is in <title></title> tags
 * Articles start with <page> and end with </page>
 * Links are formatted with double brackets [[Name Of Article]]. But sometimes files can be inside the double brackets. Example: [[File:Cat.jpg|left|thumb|150px| alt text of the image that might also include [[an article link]] like that]]
 * Double brackets that aren't links to articles: [[Help:...]] [[File:...]] [[media:...]] [[Wikipedia:...]] [[language code:...]] [[wikt:...]] [[Special:...]]
 * However, titles of articles, and thus their links can have colons. Ex: [[Paper Mario: Thousand Year Door]]
 * Double brackets that link to lists of articles: [[Category:NameOfList]] //NOTE these appear BEFORE the title
 * Double brackets that link to the SAME page of a different section start with #, and therefor are invalid [[#...]]
 * Links might have a bar | for an alternate name. [[thermal insulation|insulated]] The first part is the title name, second part is what appears in the text. Can appear at the end ex: [[kingdom (biology)|]]
 * Links to another article at a specific section have a # in the middle. This goes to the history section of United States [[United States#History]]
 * Rules: Double brackets [[]] are links. But don't accept anything with ':' in the rejectList list before the ':'. Don't accept '#' or ':' at the start, but it can be in the middle. Only collect the name before '#' and '|'.
 * Links and sections can only appear between <text></text> tags
 * Unique ID is in <id></id>tags, but there are several ID tags. ID of the page is under the <page> tag, other ids are under <revision> and <contributor>. What is "under" a tag seems to be ruled by tabs
 * There is some HTML encoding sometimes. For example "Drake & Josh" is written as "Drake &amp; Josh"
 * Category: pages are empty, but are populated by a list of pages that have the name of that category in its page.
 * // <nowiki><!-- revealed --></nowiki> makes it so that markup isn't processed.
*/

/*
 * Output file format:
 * 1. Titles are between <t></t> tags
 * 2. Redirects are between <r></r> tags as a 1 or 0 for true or false
 * 3. links appear between <l> tags. But each link appears on their own line.
 * 4. The section that the link is found in appear after the bar '|' character.
 * Example:
 * <t>title text</t>
 * <l>
 * link text1|section text1
 * link text2|section text2
 * </l>
 */

/*
 * Data structure possibility. Arrays have an O(1) for fetching an array number.
 * 1. Find the amount of articles in wikipedia.
 * 2. Declare an array of that size.
 * 3. Each number of the array corresponds to an article as it appears in wikipedia. That number in the array has a pointer to the article object.
 * 4. This can be read in from a processed object dump as long as it's in the same order.
 * 5. The links have a number to this array (The object can also have the name of the article and the section it was in, but this won't be used for searching, just the final display)
 * 6. When storing this data to the file each line represents an article with all of its links and sections.
 */

// Efficiently compares the end of the given string to the suffix. Used for the </text> tag.
bool WikiImporter::ends_with(std::wstring_view str, std::wstring_view suffix){
    return str.size() >= suffix.size() && str.compare(str.size()-suffix.size(), suffix.size(), suffix) == 0;
}

bool WikiImporter::starts_with(std::wstring_view str, std::wstring_view prefix){
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

// Handles titles/links with colons in it.
// As special pages have links like [[Help:users]] in it that aren't valid articles, but titles can still have colons
// If it's a valid article link it extracts the relevant title and strips any other extra markers.
// Input 1: a wstring that represents the internals of a link or title tag. Ex: "Commons:stuff#Subsection"
// Input 2: a match object where the results of the function will be placed.
// Output: bool indicating whether the function found a valid name or link.
// Note: assumes that a colon a colon is included
bool WikiImporter::colon_check(const wstring& line, wsmatch& match){
    const wregex colonRegex(L"^([^:]+):"); // Checks if there is a colon and returns what is before it
    const wregex getSeparatorRegex(L"(.*?)(?:#|\\||$)"); // Gets everything before a pound '#' or '|' Ex: "United States#History" returns "United States"
    // List keywords that we don't accept from link tagged items
    unordered_set<wstring> rejectList = {L"File", L"Media", L"Help", L"Special", L"Language Code",
                                         L"MediaWiki", L"commons", L"", L"User", L"m", L"Wikipedia talk",
                                         L"sourceforge", L"meta", L"Template"};

    // Find colon. If it doesn't exist, return with false
    if(!regex_search(line, match, colonRegex)){
        return false;
    }

    // If the tag before the colon is in the reject list
    if(rejectList.find(match[1]) != rejectList.end()){
        return false; // Return an empty match
    }

    // returns everything up to a separator character  '#' or '|'
    if(regex_search(line, match, getSeparatorRegex)){
        return true;
    }

    return false;
}

// Some subtitles are extremely long [[File:...]] tags that crash the regex. Returns true if this is one of them.
// Manually navigates through the line and returns true if the opening [[ is 500 characters before ]]
bool WikiImporter::longSubtitle (wstring &line){
    if (!starts_with(line, L"[[File"))
        return false;

    // if the end of the tag is found within 500 characters then the tag isn't too long to process
    for(int i = 6; i < 500; i++){
        if (line[i] == L']' && line[i + 1] == L']'){
            return false;
        }
    }
    return true;
}

// Write articles to file
// Accepts a vector of articles, a wofstream output file, and a wofstream for if the article is a category
void WikiImporter::articleVectorToFile(vector<Article*>& articles, wofstream& outputFile, wofstream& categoryOutFile){
    outputFile.flush();
    categoryOutFile.flush();
    wofstream* outFile;

    // Write the titles to the file
    for(Article* itr : articles){
        wstring title = itr->getTitle();
        if (starts_with(title, L"Category:"))
            outFile = &categoryOutFile;
        else
            outFile = &outputFile;

        *outFile << L"<t>" << itr->getTitle() << L"</t>" << endl;
        *outFile << L"<r>" << itr->getRedirect() << L"</r>" << endl;
        *outFile << "<l>" << endl;
        for(auto & it : *itr){
            *outFile << it.first << L"|" << it.second << endl;
        }
        *outFile << "</l>" << endl;
    }

    outputFile.flush();
    categoryOutFile.flush();

    // Delete the articles already written to file
    for (auto & article : articles)
    {
        delete article;
    }
    articles.clear();
}

// Used to continue a processing a file that stopped working.
// Finds the article in startOverrideTitle and continues to the NEXT article
void WikiImporter::overrideSkip(wifstream& wikiFile, const wstring& startOverrideTitle){
    int overrideCount = 0;
    const wregex pageRegex(L"^\\s*<page>");
    const wregex titleContentRegx(L"^\\s*<title>(.*?)</title>");
    wsmatch match;
    wstring line;

    // When restarting the dump enter the pos from pos.txt
    // Pos.txt is a known location in the last 1000 articles processed so that the entire file doesn't have to be searched.
    //wikiFile.seekg(92176978839);

    while(getline(wikiFile, line)){
        // Find <title></title> tag
        if(regex_search(line, match, titleContentRegx)){
            overrideCount++;
            if (overrideCount % 1000 == 0){
                cout << "Articles Skipped: " << overrideCount << endl;
            }
            // If the contents match the target article
            if(match[1] == startOverrideTitle){
                getline(wikiFile, line);
                cout << overrideCount << " articles skipped" << endl;
                break;
            }
        }
    }
    // find the next </page> tag to allow the program to trigger a new article
    long long int pos = wikiFile.tellg();
    while(getline(wikiFile, line)){
        if(regex_search(line, match, pageRegex)){
            wikiFile.seekg(pos);
            break;
        }
        pos = wikiFile.tellg();
    }
    // Output the next 1000 lines to a debug file to find any anomalous lines
    wofstream debugFile("debug.txt");
    for(int i = 0; i < 1000; i++){
        getline(wikiFile, line);
        debugFile << line << "\n";
    }
    wikiFile.seekg(pos);
    debugFile.close();
}

// Reads in a categories-only article file and matches it up with the links in the main file.
// Then adds the filled categories to the main file.
void WikiImporter::categoryAssignment(const string& processedFileName, const string& categoriesFileName) {
    wifstream processedFile(processedFileName);
    wifstream categoriesFile(categoriesFileName);
    wstring line;
    bool linkbody = false;
    unordered_map<wstring, Article*> articleMap;
    Article* currentArticle;
    int counter = 0;
    int consoleUpdateFrequency = 10000;

    // regex objects
    wregex tTagRegex(L"^<t>(.*?)<\\/t>"); // Gets everything inside the <t></t> tag
    wregex rTagRegex(L"^<r>(.*?)<\\/r>");
    wregex lTagOpenRegex(L"^<l>");
    wregex lTagCloseRegex(L"^<\\/l>");
    wregex linkSectionRegex(L"^(.*?)\\|(.*?)$"); //group 1 will have the left side, group 2 will have the right side.

    wsmatch match;

    // Read in all the category articles
    while(getline(categoriesFile, line)){
        if (!linkbody) {
            // Check for the title tag. If found, read it in and create a new article
            if (regex_search(line, match, tTagRegex)) {
                currentArticle = new Article;
                currentArticle->setTitle(match[1]);
                counter++;
                if (counter % consoleUpdateFrequency == 0) {
                    wcout << counter << L" categories loaded. Current: " << currentArticle->getTitle() << endl;
                }
                // Check for redirect tag. Apply it to the article
            } else if (regex_search(line, match, rTagRegex)) {
                if(match[1] == L"1") {
                    currentArticle->setRedirect(true);
                } else {
                    currentArticle->setRedirect(false);
                }
                // If the link section is found, switch to link processing mode
            } else if (regex_search(line, match, lTagOpenRegex)) {
                linkbody = true;
                continue;
            }
        }

        if (linkbody){
            // If the closing link tag is found, add the article to the map and exit the body section
            if(regex_search(line, match, lTagCloseRegex)){
                articleMap.insert({currentArticle->getTitle(), currentArticle});
                linkbody = false;
                continue;
            }

            // Add the link to the article
            regex_search(line, match, linkSectionRegex);
            currentArticle->addLink(match[1], match[2]);
        }
    }
    categoriesFile.close();

    wstring currentTitle;
    linkbody = false;
    counter = 0;
    // Go through the processed file and find any link starting with "Category:", find its respective category article and add it to its link list.
    while(getline(processedFile, line)){
        if (!linkbody) {
            if (regex_search(line, match, tTagRegex)) {
                currentTitle = match[1];
            }
            else if (regex_search(line, match, lTagOpenRegex)) {
                linkbody = true;
                continue;
            }
        }

        if (linkbody){
            if(regex_search(line, match, lTagCloseRegex)){
                linkbody = false;
                continue;
            }

            regex_search(line, match, linkSectionRegex);
            if (starts_with(match[1].str(), L"Category:")){
                auto itr = articleMap.find(match[1]);
                if (itr == articleMap.end())
                    wcout << "Category Not Found: " << match[1] << endl;
                else {
                    itr->second->addLink(currentTitle, L"");
                    counter++;
                    if (counter % consoleUpdateFrequency == 0) {
                        wcout << counter << L" links loaded. Current: " << currentTitle << endl;
                    }
                }
            }
        }
    }
    processedFile.close();


    cout << "Finished collecting links. Writing to file..." << endl;

    // Open process file with append and add the newly filled categories to the end.
    wofstream outProcessedFile(processedFileName, ios_base::app);
    for (auto & itr : articleMap){

        outProcessedFile << L"<t>" << itr.first << L"</t>" << endl;
        outProcessedFile << L"<r>" << itr.second->getRedirect() << L"</r>" << endl;
        outProcessedFile << "<l>" << endl;
        for(auto & it : *itr.second){
            outProcessedFile << it.first << L"|" << it.second << endl;
        }
        outProcessedFile << "</l>" << endl;
    }

    outProcessedFile.close();
}

bool WikiImporter::processDump(string inputFilename, string outputFilename, string categoryOutFilename) {
    return processDump(inputFilename, outputFilename, categoryOutFilename, false, L"");
}

// Processes a raw dump of wikipedia. Takes the titles and links and turns them in to Article objects.
// Outputs the article objects in to a more usable format that can be read in with
// arg1: wifstream of the wiki dump.
// arg2: wofstream for the output file, do not open it
bool
WikiImporter::processDump(string inputFilename, string outputFilename, string categoryOutFilename, bool startOverride, wstring startOverrideTitle) {
    // How often should the console update. 100 updates after 100 articles
    int consoleUpdateFrequency = 1000;
    // How often should the program write the output to file. "100" updates the output after 100 articles
    int writeFrequency = 1;

    string posFileName = "pos.txt";

    // Open files
    wifstream wikiFile(inputFilename, ios::binary);

    // open the output file. If continuing from previous run, append the file.
    wofstream outputFile;
    wofstream categoryOutFile;
    wofstream pos; // Stores the last known position of outputFile to make starting back up faster
    if (startOverride) {
        outputFile.open(outputFilename, ios_base::app);
        categoryOutFile.open(categoryOutFilename, ios_base::app);
    }
    else{
        outputFile.open(outputFilename);
        categoryOutFile.open(categoryOutFilename);
    }

    // File error handling
    if(wikiFile.bad() || wikiFile.fail()){
        cerr << "!Error opening " << inputFilename << endl;
        return false;
    }
    if(outputFile.bad() || outputFile.fail()){
        cerr << "!Error opening " << outputFilename << endl;
        return false;
    }
    if(categoryOutFile.bad() || categoryOutFile.fail()){
        cerr << "!Error opening " << categoryOutFilename << endl;
        return false;
    }


    // Regex
    const wregex pageRegex(L"^\\s*<page>"); // Used to quickly find the start of a page
    const wregex titleContentRegx(L"^\\s*<title>(.*?)</title>"); // Used to collect the data from the title tag
    const wregex linkRegex(L"\\[\\[(.*?)\\]\\]"); // Gets everything between double brackets Ex: [[Earth]] returns "Earth"
    const wregex colonRegex(L":"); // Checks if there is a colon denoting a special wikipedia page
    const wregex getSeparatorRegex(L".*?(?=#|\\|)"); // Gets everything before a pound '#' or '|' Ex: "United States#History" returns "United States"
    const wregex sectionRegex(L"^={2,3} (.*?) ={2,3}"); // Matches a section title in double or triple '='. Ex: ==section==, ===subsection===
    const wregex startTextRegex(L"^\\s*<text"); // Matches the "<text" tag at the start of the line with any amount of white space
    const wregex redirectTag(L"#redirect", regex_constants::icase); // Matches #redirect. Case insensitive
    const wregex coordinateRegex1(L"^.{0,70}\\{\\s?&quot;type&quot;:\\s?&quot;Feature"); // Finds long blocks of coordinates that confuse the link logic
    const wregex coordinateRegex2(L"^.{0,70}\\{\\s?&quot;features&quot;:");
    const wregex coordinateRegex3(L"^.{0,70}\\&lt;noinclude&gt"); // Finds long blocks of coordinates that confuse the link logic

    // Regex match objects
    wsmatch match;
    wsmatch subMatch;
    vector<Article*> articles;

    // Miscellaneous variables
    Article* currentArticle = nullptr;
    wstring currentSection = L"";
    wstring line;
    int counter = 0;
    bool insideBody = false;
    wstring consoleOutput;

    auto start = chrono::high_resolution_clock::now();

    // Used to continue a processing a file that stopped working.
    // Finds the article in startOverrideTitle and continues to the NEXT article
    if(startOverride){
        overrideSkip(wikiFile, startOverrideTitle);
    }
    wikiFile.clear();
    // Search all the lines for important tags and apply them to Article objects appropriately.
    while(getline(wikiFile, line)) {
        // Throw away empty lines, abnormally long lines, and map coordinate lines,
        // or that one stupid long caption on some DNA article, or abnormally long.
        if(line.empty()
        || regex_search(line, match, coordinateRegex1)
        || regex_search(line, match, coordinateRegex2)
        || regex_search(line, match, coordinateRegex3)
        || longSubtitle(line)
        || line.length() > 20000){
            continue;
        }

        // If not in the body. Look for tags
        if (!insideBody) {
            // Check if there is a new article by checking for <page>
            if (regex_search(line, match, pageRegex)) {
                // update article count
                counter++;

                // Output console update every consoleUpdateFrequency articles
                if (counter % consoleUpdateFrequency == 0) {
                    consoleOutput = to_wstring(counter) + L" articles. Current: " + currentArticle->getTitle();
                    wcout << consoleOutput << endl;

                    // Save current position
                    pos.open(posFileName);
                    pos << wikiFile.tellg();
                    pos.close();
                }

                if (counter % writeFrequency == 0){
                    articleVectorToFile(articles, outputFile, categoryOutFile);
                }

                // If the article isn't a valid article, remove it.
                if (currentArticle != nullptr){
                    wstring title = currentArticle->getTitle();
                    if (regex_search(title, match, colonRegex) && !colon_check(title, match)) {
                        delete currentArticle;
                        currentArticle = nullptr;
                    }
                }


                // Save current article object
                if (currentArticle != nullptr) {

                    // Sort and delete duplicates of the articles of the current article
                    currentArticle->deleteDuplicates();

                    if (currentArticle->getLinkCount() == 0){
                        wcout <<  L"Dead end article: " << currentArticle->getTitle() << endl;
                    }

                    articles.push_back(currentArticle);
                }

                // Create a new article object
                currentArticle = new Article;
                currentSection = L"";
            }
                // check if <title></title> tag appears on this line
            else if (regex_search(line, match, titleContentRegx)) {
                currentArticle->setTitle(match[1].str());
            }
                // Check for "<text" tag to mark the start of the body of the article. This is where links and sections are found
            else if (regex_search(line, match, startTextRegex)) {
                insideBody = true;

                if (regex_search(line, match, redirectTag)){
                    currentArticle->setRedirect(true);
                }
            }
        }

        // If inside of body look for links and sections
        if (insideBody) {
            wstring::const_iterator searchStart(line.cbegin()); //used for link matcher below
            // check if this line contains a new section. Appears at the start of a line
            if (regex_search(line, match, sectionRegex)) {
                currentSection = match[1].str();
            }
                // Get all instances of double brackets from the line
            else while (regex_search(searchStart, line.cend(), match, linkRegex)) {
                wstring bracketText = match[1].str();
                wstring currentLinkText;

                // Check for '#' or ':' at the start (A link to the current page) and don't accept them
                // # at start links to the current page while : at start is a special page that's not relevant
                if (starts_with(bracketText, L"#") || starts_with(bracketText, L":")){
                    searchStart = match.suffix().first; // Updates where to start searching for the next match.
                    continue;
                }

                // Check for a colon. ":" may represent a special unimportant wiki admin page. But can also be included in regular titles.
                if(regex_search(bracketText, subMatch, colonRegex)) {
                    if (colon_check(bracketText, subMatch)) {
                        currentLinkText = subMatch[1];
                    } else {
                        searchStart = match.suffix().first; // Updates where to start searching for the next match.
                        continue;
                    }
                }

                // Get everything before | and # separators
                if (regex_search(bracketText, subMatch, getSeparatorRegex)) {
                    currentLinkText = subMatch[0].str();
                } else {
                    // If it passes all the other checks then the whole string is a valid link
                    currentLinkText = bracketText;
                }

                // Create a new link if there was an appropriate one found.
                if (!currentLinkText.empty()) {
                    currentArticle->addLink(currentLinkText, currentSection);
                }

                searchStart = match.suffix().first; // Updates where to start searching for the next match.
            }

            // Check for </text> tag. To signal to stop searching for sections and links.
            if (ends_with(line, L"</text>")) {
                insideBody = false;
            }
        }
    }

    //Write whatever articles are left to the file
    articleVectorToFile(articles, outputFile, categoryOutFile);

    outputFile.close();
    categoryOutFile.close();

    // Find all the category tags in the file and assign them to their proper categories

    auto stop = chrono::high_resolution_clock::now();
    wcout << "Article extraction time: " << duration_cast<chrono::seconds>(stop - start) << endl;
    return true;
}

// Uses a processed data file output from processDump, then had the articles filled using categoryAssignment
// Reads in all the articles in the data file and returns an unordered_map of Article Objects.
// Note: There can be HTML encoding in the titles. For example "Drake & Josh" is written as "Drake &amp; Josh". Prepare to adjust for that.
unordered_map<wstring, Article *> WikiImporter::readInArticles(const string& dataFileName) {
    wifstream dataFile(dataFileName);
    wstring line;
    bool linkbody = false;
    unordered_map<wstring, Article*> articleMap;
    Article* currentArticle;
    int counter = 0;
    int consoleUpdateFrequency = 10000; // How often is the user notified of progress. after X amount of articles

    // regex objects
    wregex tTagRegex(L"^<t>(.*?)<\\/t>"); // Gets everything inside the <t></t> tag
    wregex rTagRegex(L"^<r>(.*?)<\\/r>");
    wregex lTagOpenRegex(L"^<l>");
    wregex lTagCloseRegex(L"^<\\/l>");
    wregex linkSectionRegex(L"^(.*?)\\|(.*?)$"); //group 1 will have the left side, group 2 will have the right side.

    wsmatch match;

    auto start = chrono::high_resolution_clock::now();

    // Read in all the articles and place them in the unordered_map articleMap;
    while(getline(dataFile, line)){
        if (!linkbody) {
            // Check for the title tag. If found, read it in and create a new article,
            if (regex_search(line, match, tTagRegex)) {
                currentArticle = new Article;
                currentArticle->setTitle(match[1]);

                // notify user if consoleUpdateFrequency articles have been processed
                counter++;
                if (counter % consoleUpdateFrequency == 0)
                    wcout << counter << L" articles loaded. Current: " << currentArticle->getTitle() << endl;

                // Check for redirect tag. Apply it to the article
            } else if (regex_search(line, match, rTagRegex)) {
                if(match[1] == L"1") {
                    currentArticle->setRedirect(true);
                } else {
                    currentArticle->setRedirect(false);
                }
                // If the link section is found, switch to link processing mode
            } else if (regex_search(line, match, lTagOpenRegex)) {
                linkbody = true;
                continue;
            }
        }


        if (linkbody){
            // If the closing link tag is found, add the article to the map and exit the body section
            if(regex_search(line, match, lTagCloseRegex)){
                articleMap.insert({currentArticle->getTitle(), currentArticle});
                linkbody = false;
                continue;
            }

            // Add the link to the article
            regex_search(line, match, linkSectionRegex);
            currentArticle->addLink(match[1], match[2]);
        }
    }
    dataFile.close();

    auto stop = chrono::high_resolution_clock::now();
    auto duration = stop - start;
    wcout << "Article loading time: " << duration.count() / 1000000000 << " seconds" << endl;
    wcout << "Articles Loaded: " << counter << endl;


    return articleMap;
}



