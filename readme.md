### COP3530 - Project 3

Group Members:
- [Izzy Jones](https://github.com/MechLizard) @MechLizard
- [Balal Mansour](https://github.com/bmansour1) @bmansour1
- [Mohamed Mirghani](https://github.com/InventorMaxworks) @InventorMaxworks

Project repo: https://github.com/MechLizard/Wiki_Pathfinding

Project Video: https://www.youtube.com/watch?v=FqtkWeINUo0

# Overview
It is said that on wikipedia any two articles are connected through eachothers links and it is possible to get to the other article without having to use a search bar. This web app is an implmentation of a Wiki Game Solver that uses BFS and DFS to search for paths between two articles on wikipedia. It uses Vite + React on the front-end, and on the back-end it uses Nodejs and C++ Addon to run the c++ code for the searches.

# How To Set up Wiki Pathfinding App

## Getting the files and code
- STart by moving over to the create-react-app branch and clone it in Visual Studio Code
- Unzip article.zip in Wiki_Pathfinding/backend 
- The create-react-app branch contains the Vite and Nodejs server directories while the master contains the c++ source files

## Installing Nodejs and npm
- Start by checking out the nodejs download website to download nodejs and npm. https://nodejs.org/en/download
- Open the terminal into the Wiki_Pathfinding folder you cloned and then cd into the backend folder like this: "cd .\wiki_pathfinding_app\"
- Then run "npm install"

### Running it
- The project can be ran by going to the wiki_pathfinding_app with "cd wiki_pathfinding_app"
- Then run "npm run start:dev"
- This will load the project in and you should see the project running at http://localhost:5173/
- Keep in mind that the search and random buttons wont work until all the files are loaded in, which takes around 20 seconds after starting up the server.

