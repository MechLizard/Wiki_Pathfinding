import React, { useState } from 'react';
import { FaSearch } from 'react-icons/fa';
import './SearchBar.css';

export const SearchBar = ({ onSearch, placeholder }) => {
  // Call the onSearch prop function directly on change
  const handleChange = (e) => {
      onSearch(e.target.value); // Update the parent component's state on change
  };

  return (
      <div className="input-wrapper">
        <FaSearch id="search-icon" />
        <input
          placeholder={placeholder}
          onChange={handleChange} // Passes the changed value up to the App component
          // ... rest of the attributes
        />
      </div>
  );
};