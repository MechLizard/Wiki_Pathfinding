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

// Helper function to shuffle array
function shuffleArray(array) {
    for (let i = array.length - 1; i > 0; i--) {
        const j = Math.floor(Math.random() * (i + 1));
        [array[i], array[j]] = [array[j], array[i]];
    }
    return array;
}

// Call initializeApp to load articles at app startup
initializeApp();


// Endpoint to handle search requests
app.get('/search', (req, res) => {
    let { origin, destination } = req.query;

    // Manually decode URI components if necessary
    origin = decodeURIComponent(origin);
    destination = decodeURIComponent(destination);

    const trimmedOrigin = origin.trim();
    const trimmedDestination = destination.trim();

    console.log(trimmedOrigin, trimmedDestination);
    // Check if origin and destination are provided and are strings
    if (typeof trimmedOrigin !== 'string' || typeof trimmedDestination !== 'string') {
        console.error("Expected two string arguments: origin and destination");
        res.status(400).json({ success: false, message: "Expected two string arguments: origin and destination" });
        return;
    }

    try {
        const searchResults = addon.performBothSearches(trimmedOrigin, trimmedDestination);
        res.json({ success: true, result: searchResults });
    } catch (error) {
        console.error("Error during search:", error);
        res.status(500).json({ success: false, message: "An error occurred during the search." });
    }
});


// Endpoint to get random articles
app.get('/random-articles', (req, res) => {
    try {
        let titles = addon.getArticleTitles(); // This calls your C++ addon function
        let randomTitles = shuffleArray(titles).slice(0, 2);
        
        // Return two random article titles
        res.json({ origin: randomTitles[0], destination: randomTitles[1] });
    } catch (error) {
        console.error("Error during random article selection:", error);
        res.status(500).json({ success: false, message: "An error occurred during random article selection." });
    }
});


app.use(express.static('../wiki_pathfinding_app'));

// Start the server
app.listen(port, () => {
    console.log(`Server listening at http://localhost:${5173}`);
});
