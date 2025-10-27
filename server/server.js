// import Papa from 'papaparse';
// import { exec } from "child_process";
// import fs from "fs";
// import http from 'http'
// const data = "bhavesh Gadling"
//This is the demonstration for the  server.
// exec(`./myExecutable ${data}`, (error, stdout, stderr) => console.log(stdout));

// const jsonData = [
//   { name: "Bhavesh", age: 21, city: "Pune" },
//   { name: "Amit", age: 22, city: "Mumbai" }
// ];
// const csv = Papa.unparse(jsonData);
// console.log(csv);

// fs.writeFileSync("student.csv", csv, "utf-8")
const express = require('express');
const app = express();
const PORT = 3000;

app.listen(PORT, (error) =>{
    if(!error)
        console.log("Server is Successfully Running,and App is listening on port "+ PORT);
    else 
        console.log("Error occurred, server can't start", error);
    }
);

app.get('/api/login', (request, response) => {
  response.send([{id : 123, name: "bhavesh", price: 132}])
})

app.post('/api/users', (request, response) => {
  console.log(request.body) 
  return response.send(200)
})