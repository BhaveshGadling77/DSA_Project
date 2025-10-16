import { exec } from "child_process";
const data = "bhavesh Gadling"
exec(`./myExecutable 100 2000`, (error, stdout, stderr) => p(stdout));

function p(data) {
    console.log(data.split(" "));
}