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

  return (
    <div className="App">
      <div className="search-bar-container">
        <SearchBar onSearch={(value) => setOrigin(value)} placeholder="Enter origin..." />
        <SearchBar onSearch={(value) => setDestination(value)} placeholder="Enter destination..." />
        <button onClick={handleSearch}>Search</button>
        <div>
          <pre>{results}</pre>
        </div>
      </div>
    </div>
  );
}

export default App;
