import { exec } from "child_process";
const data = "bhavesh Gadling"
exec(`./myExecutable ${data}`, (error, stdout, stderr) => console.log(stdout));