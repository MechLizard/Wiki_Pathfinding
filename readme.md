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

### Features
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

### The Data
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

### Algorithms Implemented
TODO: Expand on this

Two algorithms are used to traverse the Wikipedia pages. Depth-first search and breadth-first search. 
Both are usable for the user to compare the time it takes for each to work.

### Additional Data Structures/Algorithms used
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

### Distribution of responsibility and Roles
TODO: Flesh out Mohamad and Belal's contributions.

- Izzy: Created file handling and data structure; created WikiImporter and Article classes. 
Administrative work for deliverables
- Mohamad: Front-end interface with React. Work with Belal to implement the API with the C++ backend. 
Implemented the BFS and DFS algorithms.
- Belal: Back-end for C++. Connecting the different pieces together and working with Mohamad to implement an API for the React front-end.

## Analysis
TODO: Any changes the group made after the proposal? The rationale behind the changes.
TODO: Big O worst case time complexity analysis of the major functions/features you implemented

## Reflection
TODO: As a group, how was the overall experience for the project?
TODO: Did you have any challenges? If so, describe.
TODO: If you were to start once again as a group, any changes you would make to the project and/or workflow?
TODO: Comment on what each of the members learned through this process.

## References