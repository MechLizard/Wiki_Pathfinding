import axios from 'axios'; // Import Axios
import { useState } from 'react';
import './App.css';
import { SearchBar } from './components/SearchBar';

function App() {
  const [origin, setOrigin] = useState('');
  const [destination, setDestination] = useState('');
  const [results, setResults] = useState('');

  const handleSearch = async () => {
    // Trim the values to ensure there's no whitespace-only input
    const trimmedOrigin = origin.trim();
    const trimmedDestination = destination.trim();

    if (!trimmedOrigin || !trimmedDestination) {
      alert("Please enter both origin and destination.");
      return;
    }

    try {
      // Use Axios for making the GET request
      const response = await axios.get(`http://localhost:3000/search`, {
        params: { // Use the params property to send query parameters
          origin: encodeURIComponent(trimmedOrigin),
          destination: encodeURIComponent(trimmedDestination)
        }
      });
      
      setResults(response.data.result);
    } catch (error) {
      console.error("Failed to fetch:", error);
      setResults('Failed to fetch results.');
    }
  };
  
  const handleRandomArticles = async () => {
    try {
      const response = await axios.get('http://localhost:3000/random-articles');
      setOrigin('');
      setDestination('');
      setOrigin(response.data.origin);
      setDestination(response.data.destination);
    } catch (error) {
      console.error("Failed to fetch random articles:", error);
    }
  };
  

  return (
    <div className="App">
      <div className="search-bar-container">
        <img src="wiki.png" height="150" /> {/* Add closing angle bracket */}
        <h1 className="title">Wiki Game Solver</h1> {/* Add class name for styling */}
        <p className="subtext">Find the shortest path between any given wiki article through both DFS and BFS</p>
    <div className="search-label"></div>
        <SearchBar onSearch={(value) => setOrigin(value)} placeholder="Enter origin..." value={origin}/>
        <div className="search-label" style={{ marginTop: "8px" }}></div> {/* Add margin-top style */}
        <SearchBar onSearch={(value) => setDestination(value)} placeholder="Enter destination..." value={destination}/>
        <button onClick={handleSearch}>Search</button>
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.2/css/all.min.css" integrity="sha512-SnH5WK+bZxgPHs44uWIX+LLJAJ9/2PkPKZ5QiAj6Ta86w+fsb2TkcmfRyVX3pBnMFcV7oQPJkl9QevSCWr3W6A==" crossorigin="anonymous" referrerpolicy="no-referrer" />
        <button onClick={handleRandomArticles} className="random-button">
          <i className="fa-solid fa-shuffle"></i>
        </button>
      </div>
      <div className="results-container">
        <pre>{results}</pre>
      </div>
    </div>
  );
}

export default App;

