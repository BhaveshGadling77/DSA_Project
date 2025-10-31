// src/pages/NotFound.jsx
import { useNavigate } from "react-router-dom";
import astronautImg from "../../assets/astronaut.png";

function NotFound() {
  const navigate = useNavigate();

  return (
    <div className="min-h-screen bg-black flex items-center justify-center px-4 py-16 relative overflow-hidden">
      <div className="absolute top-1/2 left-1/2 transform -translate-x-1/2 -translate-y-1/2 w-[600px] h-[600px] bg-gradient-to-br from-blue-600/30 via-purple-600/30 to-blue-800/30 rounded-full blur-3xl" />

      {/* Main content */}
      <div className="relative z-10 max-w-6xl mx-auto grid md:grid-cols-2 gap-16 items-center">
        {/* Left side text */}
        <div className="text-left order-2 md:order-1">
          <h1 className="text-5xl md:text-6xl font-light text-white mb-3 tracking-wide">
            404-Error
          </h1>

          <h2 className="text-3xl md:text-4xl font-bold text-white mb-6 tracking-wider">
            PAGE NOT FOUND
          </h2>

          <p className="text-gray-400 text-lg mb-10 max-w-md leading-relaxed">
            Oops! This page is lost in space. It doesn’t exist or has moved.
            Let’s get you back home.
          </p>

          <button
            onClick={() => navigate("/")}
            className="px-10 py-4 bg-transparent border-2 border-white text-white font-medium text-lg rounded-lg hover:bg-white hover:text-black transition-all duration-300"
          >
            Back To Home
          </button>
        </div>

        {/* image part */}
        <div className="relative order-1 md:order-2 flex justify-center">
          <div className="relative w-full max-w-lg">
            {/* Astronaut*/}
            <img
              src={astronautImg}
              alt="Lost astronaut"
              className="relative w-full h-full object-contain animate-float"
            />
          </div>
        </div>
      </div>
      
      {/* Float animation */}
      <style>{`
        @keyframes float {
          0%, 100% { transform: translateY(0px); }
          50% { transform: translateY(-20px); }
        }
        .animate-float {
          animation: float 3s ease-in-out infinite;
        }
      `}</style>
    </div>
  );
}

export default NotFound;