import Papa from 'papaparse';
import { exec } from "child_process";
import fs from "fs";
import http from 'http'
const data = "bhavesh Gadling"
//This is the demonstration for the  server.
exec(`./myExecutable ${data}`, (error, stdout, stderr) => console.log(stdout));

const jsonData = [
  { name: "Bhavesh", age: 21, city: "Pune" },
  { name: "Amit", age: 22, city: "Mumbai" }
];
const csv = Papa.unparse(jsonData);
console.log(csv);

fs.writeFileSync("student.csv", csv, "utf-8")


const server = http.createServer((req, res) => {
    // Sending the response
    res.write("This is the response from the server")
    res.end();
})

server.listen((3000), () => {
    console.log("Server is Running");
})

