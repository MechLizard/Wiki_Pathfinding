import React, { useState } from 'react';
import { FaSearch } from 'react-icons/fa';
import './SearchBar.css';

export const SearchBar = ({ onSearch, placeholder, value }) => {
  // Use the value prop in the input element
  return (
    <div className="input-wrapper">
      <FaSearch id="search-icon" />
      <input
        placeholder={placeholder}
        onChange={(e) => onSearch(e.target.value)}
        value={value} // Use the value prop to reflect changes
      />
    </div>
  );
};