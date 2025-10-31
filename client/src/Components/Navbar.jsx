import React from "react";
import { useState, useEffect} from "react";
import { useLocation } from "react-router-dom";
import { FaTimes } from "react-icons/fa";
import { CiMenuFries } from "react-icons/ci";
import { Link } from "react-router-dom";
export default function Navbar() {
    const [menuOpen, setMenuOpen] = useState(false);
    
    const style = { color: "#f3e3b2" };
    return (
        <nav className="bg-[#18212b] w-screen z-50 shadow-sm fixed  border-b-[1px] opacity-80 border-solid border-[#242424] border-t-0 backdrop-blur-2xl">
                <div className="w-full lg:w-4/5 container px-6 py-2 md:py-4 mx-auto ">
                    <div className="flex flex-row justify-between items-center">
                        {/* Logo and Name - always visible */}
                        <div className="flex items-center">
                            
                            <a to="/" className="inline-block p-0">
                                <p
                                    className="font-bold text-2xl text-center text-white p-0"
                                    id="Name"
                                >
                                    Event Management system
                                </p>
                            </a>
                        </div>

                        {/* Desktop Links */}
                         <div className="hidden md:flex font-bold gap-2 px-4 space-x-10 text-teal-50">
                                <a
                                    to="/"
                                    className={`text-lg p-0 ${
                                        location.pathname == "/"
                                            ? "text-[#ffd045]"
                                            : ""
                                    }`}
                                >
                                    Home
                                </a>
                                                                <a
                                    to="/dashboard"
                                    className={`text-lg p-0 ${
                                        location.pathname == "/dashboard"
                                            ? "text-[#ffd045]"
                                            : ""
                                    }`}
                                >
                                    DashBoard
                                </a>
                                
                                <a
                                    to="/about"
                                    className={`text-lg p-0 ${
                                        location.pathname == "/about"
                                            ? "text-[#ffd045]"
                                            : ""
                                    }`}
                                >
                                    About
                                </a>
                                <a
                                    to="dashboard/settings"
                                    className={`text-lg p-0 ${
                                        location.pathname == "/dashboard/settings"
                                            ? "text-[#ffd045]"
                                            : ""
                                    }`}
                                >
                                    Settings
                                </a>
                                                                
                                </div>
                        {/* Hamburger Icon for Mobile */}
                        <div className="md:hidden flex items-center">
                            <button
                                onClick={() => setMenuOpen(!menuOpen)}
                                className="text-white text-3xl focus:outline-none opacity-100"
                            >
                                {menuOpen ? (
                                    <FaTimes style={style} size={25} />
                                ) : (
                                    <CiMenuFries style={style} size={25} />
                                )}
                            </button>
                        </div>
                    </div>

                    {/* Mobile Menu */}
                </div>
            
            {menuOpen && (
                <div className="md:hidden-[800px] p-12 opacity-95  top-6 w-full border-none rounded-lg mt-40 left-0 fixed align-middle text-center bg-navBgC shadow-lg block flex-col items-center z-50 ">
                    <div className="rounded-md bg-[#f4e4b3] mt-2">
                        <a to="/about" className="text-[#284121] block ">
                            About
                        </a>
                    </div>
                    <div className="rounded-md bg-[#f4e4b3] mt-2 ">
                        <a to="/dashboard" className="text-[#0e210e]  block">
                            DashBoard
                        </a>
                    </div>
                    <div className="rounded-md bg-[#f4e4b3] mt-2 ">
                        <a to="/" className="text-[#0e210e]  block">
                            Home
                        </a>
                    </div>
                    <div className="rounded-md bg-[#f4e4b3] mt-2 ">
                        <a to="/loginpage" className="text-[#284121] block">
                            Login / SignUp
                        </a>
                    </div>
                </div>
            )}
            </nav>
    );
}