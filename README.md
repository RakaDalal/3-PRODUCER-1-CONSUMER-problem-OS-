# 3-PRODUCER-1-CONSUMER-problem-OS-

Info about 3 PRODUCER-1 CONSUMER problem:

4 POSIX threads are created, 3 for 3 different coloured producers(RED, BLACK and WHITE) and 1 for 1 consumer.
Each producer produces 1000 items each (“COLOR timestamp”) example: RED_1476292622.317513 and then write the
item into its log file (Producer_COLOR.txt) example : Producer_RED.txt
The consumer consumes an item, and write its contents to Consumer.txt file. 
A shared Buffer is used for the problem. And only one thread executes in the critical section at a time.


How to run the program:

1.Open a terminal

2.Change the working directory to the folder containing the program.

	1. Type "make compile" to compile the program
	
	2. Type "make run" to run the program
	
