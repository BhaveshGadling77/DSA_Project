import React from "react";
import Navbar from "./Navbar";
import { Outlet } from "react-router-dom";

function Layout() {
    return(<>
        <Navbar/>
        <Outlet/>
        <Footer/>
    </>)
}

export default Layout;