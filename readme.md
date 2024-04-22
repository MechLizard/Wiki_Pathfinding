### COP3530 - Project 3

Group Members:
- [Izzy Jones](https://github.com/MechLizard) @MechLizard
- [Balal Mansour](https://github.com/bmansour1) @bmansour1
- [Mohamed Mirghani](https://github.com/InventorMaxworks) @InventorMaxworks

Project repo: https://github.com/MechLizard/Wiki_Pathfinding

TODO: Link to video demo

# Wiki Pathfinding

## Extended and refined Proposal

### Problem
It is said that any web page on Wikipedia can be reached using only the links on the pages. We seek to find the shortest
path between any given wiki article.

### Motivation
There is a popular game called “Wiki Game” that allows players to compete against each other to get from one wiki page
to another using only links. This can seem impossible, but there is always some way to navigate between these two points.
Finding the shortest path would bring insights into how efficiently these searches can be done.

## Features
TODO: Revise this list with what we end up with

This project allows the user to enter two Wikipedia article titles, which will show the articles that need to be visited to reach the second entered article.
Along with the main function, features include:
- Pick two random articles. Pick two random articles and enter them
  into the search fields.
- Display the time it took to find the connection between the two
  articles.
- Compare depth-first to breadth-first search methodologies. To determine
  which one is the quickest to find the target article.
- Display the section that the links were found in.
- Display whether a link is a redirect to another article.

## The Data
TODO: Include the number of articles

A dump of the English Wikipedia was provided by Wikipedia, which included every article (without revisions, images, or
files) that was available at the time of the dump (January 20, 2024). ~91GB worth of text uncompressed.

A dump of the simplified English Wikipedia was used for testing as it was significantly smaller (~1GB).

Dumps can be accessed here: https://dumps.wikimedia.org/

The WikiImporter class processes the Wikipedia dump and outputs a processed file that only includes the article title,
what pages the article links to, what section each link was found in, and whether the article is a redirect. This new
file can be imported significantly faster and is significantly smaller at ~16GB uncompressed.

### Tools
TODO: Fill this out more. Tools/Languages/APIs/Libraries

C++ was used as the backend to handle the article objects and run the pathfinding algorithm.
React is used on the front-end to interact with the user and display the results.

## Algorithms Implemented

Two algorithms are used to traverse the Wikipedia pages. Depth-first search and breadth-first search.

### Breadth First
Explores all the links in the current article before searching for all the links in the next layer of articles.

#### Methodology:
1. Starting from the given starting article. Adds all the links in the article to a queue.
2. Searches all the articles in the queue for the ending article.
3. Keep track of the articles visited to reduce double work and remove loops.
   Also, keep track of which article is the parent of the one searched.
4. When the ending article is found, use the list of articles and parents to reconstruct the path back to the starting article.

### Depth first:
Explores the first link in the current article, then the first link in the next article. Recursively repeat until the end article is found.

#### Methodology:
1. Starting from the given starting article. Recursively calls `dfs` on each link in the article.
2. Keeps track of the path that the recursive function has gone down.
3. Keeps calling `dfs` until the target article is found or reaches the depth limit.
4. If the target article is found, return the path that it was found in.

Both methodologies are displayed after the search. Breadth-first search tends to find the shortest path, but it takes longer.
Depth-first search tends to find the article faster but often doesn't find the shortest path.

## Additional Data Structures/Algorithms used
TODO: Expand on this

Each article is stored in an Article object as defined by the class Article.h. The class contains the article title,
the amount of links it contains, whether the article is a redirect, and a `vector<pair<wstring, wstring>>` containing a list of
the links and their corresponding section that they appeared in. Article objects also have the ability to be sorted when
in a list by implementing the `operator<` function. The ability to iterate through the object's links by implementing
begin() and end() functions. And functions to get and set the variables.

The processed data file uses tags to delimit variables in the file. The title is in `<t></t>` tags, whether the
article is a redirect in `<r></r>` tags, and the links are in a block between `<l></l>` tags. Each link is on its own
line, separated by a new line. Each link is followed by a `'|'` character and then the section it was found in.

Here is an example article:

```
<t>title text</t>
<r>0</r>
<l>
link text1|section text1
link text2|section text2
</l>
```

When loaded into our program, the Article objects are stored in an `unordered_map<wstring, Article*>`.
With the title of the article as the key. We chose this due to unordered_map's O(1) lookup time complexity.
We considered having each link be a pointer to another Article object, but that would increase the complexity of the
project and initial processing time significantly. Although this would likely make the program faster.

## Distribution of responsibility and Roles
TODO: Flesh out Mohamad and Belal's contributions.

- Izzy: Created file handling and data structure; created WikiImporter and Article classes.
  Administrative work for deliverables
- Mohamad: Worked with Belal to implement the styling of teh front-end.
  Implemented the BFS and DFS algorithms.
- Belal: Front-end with vite and react and Back-end with Nodejs with C++ Addon. Connecting the different pieces together and working with Mohamad to style teh front-end.

## Analysis
TODO: Any changes the group made after the proposal? The rationale behind the changes.

#### Worst Case Big O of Major Functions:
- **WikiImporter::processDump:** Processes the raw dump of Wikipedia and outputs it into a processed file.
    - **O((n * m) + (l*log(l) + l) + a)** where:
        - n is the number of lines in the Wikipedia dump. (Each line of the dump is processed individually.)
        - m is the number of characters per line. (Regex is used to search for links in the body of the articles.)
        - l is the number of links in the Wikipedia dump. (Links are sorted in each article, and duplicates are removed.)
        - a is the number of articles in the Wiki dump. (Each article is processed into an Article file and written to the disk.)


- **WikiImporter::categoryAssignment:** Fills in the links for the "category" articles and adds them to the processed file.
    - **O(n)** where:
        - n is the number of lines in the processed file.

- **WikiImporter::articleVectorToFile:** Saves a bundle of Article objects to a processed file
    - O(a * l) where:
        - a is the number of articles being saved.
        - l is the number of links in the article.

- **WikiImporter::readInArticles:** Reads in articles from a processed file and creates Article objects.
    - **O(n)** where
        - n is the number of lines in the processed file.


- **Article::deleteDuplicates():** Sorts and then removes duplicates from the article object.
    - **O(l*log(l) + l)** Where:
        - l is the number of links in the article object before duplicates are removed.
          (Links are sorted in each article for l*log(l) comparisons, and duplicates are removed for l comparisons.)


- **dfs:** Searches and returns a path between two Article objects using depth-first methodology.
    - O(a) where:
        - a is the number of articles in the processed dump.

- **bfs:** Searches and returns a path between two Article objects using breadth-first methodology.
    - O(a) where:
        - a is the number of articles in the processed dump.


## Reflection
TODO: As a group, how was the overall experience for the project?
TODO: Did you have any challenges? If so, describe.
TODO: If you were to start once again as a group, any changes you would make to the project and/or workflow?
- After the near completion of this project, we found that there were pre-processed files of the Wikipedia dump that were
  optimized for this kind of project on GitHub. That would reduce the needed code base for the WikiImporter module.


- The sheer size of the Wikipedia dump reduced the number of tools and libraries that could be used significantly.
  Since we weren't able to use some of those libraries, we essentially had to reinvent the wheel to process the dump.
  If we were to do this again, we would split the dump up into many smaller (~100MB) files.
  There was a way to do this through the command prompt, but it would split articles in half, causing errors.
  A custom solution to splitting the files between articles would be needed, but not difficult to achieve.


- Using a database (such as SQL) likely would have improved the memory requirements of this project significantly.
  However, we did not implement this as it was outside the scope of the project, but if we were to do this project
  again, it would be worthwhile to explore this option.
  Currently, it loads all the Article objects into memory before it is able to function, which takes about 16GB of memory.
  This also limits the ability to deploy this project as a live service.


## Lessons learned for each group member
TODO: Comment on what each of the members learned through this process.

Izzy:
- The limitations of libraries and tools when it comes to incredibly large files and how to circumvent those limitations.
- Identified many different tools and the benefits of them in relation to optimization.
- Identified how small optimizations can allow for significant changes in time spent on large problems.
- Familiarized myself with Regex to identify and extract important items from text.

Belal:
- The more langauges and libraries you are using will require more configurations and it is easy to lose track of what frameworks/languages are running on so it is best to keep a small amount of languages used.
- How to run a backend and front-end simultaneously
- Familiarized myself with React, Nodejs, express api, and C++ Addon

Mohamad:

## References
TODO: Fill this out
