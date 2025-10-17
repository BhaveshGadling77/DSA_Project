import { exec } from "child_process";
const data = "Working on dsa project";
exec(`myExecutable.exe "${data}"`, (error, stdout, stderr) => {
  if (error) {
    console.error(`Error: ${error}`);
    return;
  }
  if (stderr) {
    console.error(`stderr: ${stderr}`);
    return;
  }
  console.log(stdout);
});
