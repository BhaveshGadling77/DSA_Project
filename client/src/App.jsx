import React from 'react'
import Navbar from './Components/Navbar'
import { createBrowserRouter, Router } from 'react-router-dom'
import { Outlet } from 'react-router-dom'

const App = () => {
  const router = createBrowserRouter([
    {
      path:'/',
      element:<Layout/>,
      
    }
  ])
  return (
    <Router>
      <Navbar/>
      <Outlet/>
    </Router>
  )
}

export default App
