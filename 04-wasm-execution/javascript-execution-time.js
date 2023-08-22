// Calculate the execution time of counting from 1 to 100M in ms
// Create a variable to store the start time
console.log("JAVACRIPT TIME $###############################################");
let startTime = Date.now();

// Create a loop to count from 1 to 100 million
let count = 0;
for (let i = 1; i <= 1000000000; i++) {
  count++;
}

// Create a variable to store the end time
let endTime = Date.now();

// Calculate the difference between the end time and the start time in seconds
let diff = (endTime - startTime) / 1000;

// Print the result in the format 0.000s
console.log(diff.toFixed(3) + "s");
console.log("JAVACRIPT FINISHED $###############################################");

