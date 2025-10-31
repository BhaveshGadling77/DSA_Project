import React from 'react'
import Navbar from './Components/Navbar'
import Footer from './Components/Footer'
import { BrowserRouter, Routes, Route, NavLink } from 'react-router-dom';
const App = () => {
  return (
    <div className='min-h-screen flex flex-col'>
    <Navbar/>
      <Footer/>
    </div>
  )
}

export default App
