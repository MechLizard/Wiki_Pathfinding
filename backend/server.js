const express = require('express');
const cors = require('cors');
const app = express();
const port = 3000;

app.use(express.json());
app.use(cors());
app.use((err, req, res, next) => {
  console.error(err.stack);
  res.status(500).send('Something broke!');
});

// Require the addon
const addon = require('./build/Release/addon.node');

// Function to initialize the application
function initializeApp() {
    const articlesPath = "articles.txt";
    addon.loadArticles(articlesPath);

    console.log("Articles have been loaded.");
}

// Call initializeApp to load articles at app startup
initializeApp();

// Endpoint to handle search requests
app.get('/search', (req, res) => {
    const { origin, destination } = req.query;

    // Check if origin and destination are provided and are strings
    if (typeof origin !== 'string' || typeof destination !== 'string') {
        console.error("Expected two string arguments: origin and destination");
        res.status(400).json({ success: false, message: "Expected two string arguments: origin and destination" });
        return;
    }

    try {
        const searchResults = addon.performBothSearches(origin, destination);
        res.json({ success: true, result: searchResults });
    } catch (error) {
        console.error("Error during search:", error);
        res.status(500).json({ success: false, message: "An error occurred during the search." });
    }
});


app.use(express.static('../wiki_pathfinding_app'));

// Start the server
app.listen(port, () => {
    console.log(`Server listening at http://localhost:${port}`);
});
