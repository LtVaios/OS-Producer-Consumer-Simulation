# OS Server-Client Simulation
🔨This producer-consumer problem has 1 parent and n (input) children forked and by using semaphores and process synchronization the parent can give requested lines of text from a text file to all the children while all the children ask for a line simultaneously. All children are synchronized so one else is served while someone is in his critical section of code. After each child is finished you can also see the average time of each transaction it made with the parent process

## Installation / Build / Run
📂 Download the files in a local folder or put them in a IDE. Run:
 ```
  $ make
  $ ./server text.txt 10 5
  ```
The first argument is the name of the text file you want to use. You can add you own file in the txt_files folder and run the programm with that but be careful each line of text must not exceed 100 characters. The second argument is the number of forked processes you want the programm to create. The third argument is the number of transactions you want each child to make with the parent process.

## Built with
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/1/18/C_Programming_Language.svg/380px-C_Programming_Language.svg.png" style="height: 200px; width:180px;"/>
