import '../assets/styles/css/App.css'
import { React, useState, useEffect } from 'react';
import Data from './data/Data';
import Timer from './timer/Timer';



function App() {


  return (
    <div className="App">
        <Timer />
      <Data />
    </div>
  );
}

export default App;