Compilation instructions: 
1.	gcc MChan_thd.c -o MChan_thd.exe -lpthread
2.	./MChan_thd.exe 1000000 10 10 
 or ./MChan_thd.exe 1000000 10 (for part 2)  
 
Program design:
This is a program for summation from 1 to n that accepts 2 or 3 arguments to select execute function part2 or function part3. In the main function, there are some conditional determination to prevent invalid input. If the input is correct, the corresponding function would be invoked. 

For the part 2, I have to create child threads for the main thread. As the number of threads is more than 1, I need an array to store the thread identifier and thread attribute. The array of SumPart struct is to store other information of each thread includes start value, end value, thread summation and thread number. Then, I use a for loop to pass different values to the thread struct, set the default attributes and create thread with the pointer of the runner function that calculate the partial summation. Finally, I use another for loop the wait for thread termination and add the value to the total sum. 

For the part 3, this is similar to part 2 but we have to create grandchild thread for each child thread. The part to create child thread is the same as part 2 but with little changes that the subSumPart struct is used to store one more information, the number of grandchild thread. And for the pthread_create(), the pointer of createThread() is passed instead of runner() to create grandchild thread. The createThread() does the same thing as part 2, execute the runner() to calculate the sum.     

