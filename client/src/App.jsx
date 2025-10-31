import React from 'react'
import Navbar from './Components/Navbar'
import Footer from './Components/Footer'
import { Route, Routes } from 'react-router-dom'
import Events from './Components/Events/Events'
import Login from './Components/LoginPage/Login'
import NotFound from './Components/NotFoundPage/NotFoundPage'
import Home from './Components/Home'
const App = () => {
  return (
    <div className='min-h-screen flex flex-col'>
      <Routes>
        <Route path='/' element={<Home/>}/>
        <Route path='/event' element={<Events/>}/>
        <Route path='/login' element={<Login/>}/>

        <Route path='*' element={<NotFound/>}/>
      </Routes>
    </div>
  )
}

export default App
