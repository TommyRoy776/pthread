#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
/*Ming Wai Tommy, Chan CSCI340 project2*/
/*
	gcc MChan_thd.c -o MChan_thd.exe -lpthread
	./MChan_thd.exe 1000000 10 10

*/
long int total =0; /*this data is shared by the thread(s) */
void *runner(void *param); /*threads call this function*/
void part2(char* val, char* threads); //part2 function
void part3(char* value, char* threads,char* subthreads); //part3 function
void *createThread(void *param); //function for grandchild


struct SumPart{ //struct for child thread to save information
   int start;
   int end;
   long int partialSum;
   int index;
};

struct subSumPart{ //struct for grandchild thread to save information
   int subthreads;
   int start;
   int end;
   long int partialSum;
   int index;
};


int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4){ //detect invalid input
        printf("invalid input...");
        exit(-1);
    }
    if (atoi(argv[2]) > 50){  //limit the child thread to be maximum 50 threads
        printf("max number of child threads is 50 and max number of grandchildren threads is 50");
        exit(-1);
    }

    if(argc == 3){ //if arg number is 3, execute part2()
        part2(argv[1],argv[2]);
    }
    if (atoi(argv[3]) > 50){  //limit the grandchild thread to be maximum 50 threads
        printf("max number of child threads is 50 and max number of grandchildren threads is 50");
        exit(-1);
    }
    if(argc ==4){ //if arg number is 4, execute part3()
        part3(argv[1],argv[2],argv[3]);
    }
    return 0;
}

void part2(char* val, char* threads){
    printf("Parent: Start of  Execution\n ");
     int numOft = atoi(threads); //convert number of threads to integer
    int value = atoi(val);  //convert the target value to integer
    int divide = (value/numOft)-1; //calculate the difference between threads then - 1 
    int limit =0; //start value
    pthread_t tid[numOft]; /* Array of the thread identifer*/
     pthread_attr_t attr[numOft];   /* Array of the thread attribute*/
     struct SumPart p[numOft]; //array of struct that include all information for each thread
    for(int i=0;i<numOft; i++){ //assign the start value and end value for each thread
       p[i].index = i; // thread number
       limit ++;  //start from 1 and each thread start value is last thread end value + 1
       p[i].start = limit; //start value for thread i
       if(i == numOft-1){  //for the last thread only
          limit += value-limit; //sum up the remain value for not divisible case e.g. 100/8
       }else{
         limit += divide; //calculate the end value
       }
       p[i].end = limit;  //end value
       pthread_attr_init(&attr[i]); /* set the default attributes of the thread */
       pthread_create(&tid[i], &attr[i],runner,&p[i]); /* create the thread */
    }

    /*wait for the thread to exit */
    for(int i =0;i<numOft;i++){
        pthread_join(tid[i],NULL); /* wait for the thread to exit */
        printf("Child thread number %d: \t child start=%d \t end = %d \t sum = %ld \n ", p[i].index, p[i].start, p[i].end,p[i].partialSum);
        total += p[i].partialSum; //sum up value from each thread
    }
    printf("Parent: Total sum is %ld\n",total);
    printf("Parent: End of  Execution\n ")  ;
}

void part3(char* val, char* threads,char* subthreads){
     printf("Parent: Start of  Execution\n ");
    int numOft = atoi(threads); //convert number of threads to integer
    int value = atoi(val); //convert the target value to integer
    int subs = atoi(subthreads);  //convert number of subthreads to integer
    int divide = (value/numOft)-1; //calculate the difference between threads and - 1
    int limit =0;
    pthread_t tid[numOft]; /* Array of the thread identifer*/
     pthread_attr_t attr[numOft];  /* Array of the thread attribute*/
     struct subSumPart p[numOft]; //array of struct that include all information for each thread
    for(int i=0;i<numOft; i++){ //assign the start value and end value for each thread
       p[i].index = i; // thread number
       p[i].subthreads = subs; //assign subthread amount
       p[i].partialSum = 0; //initial grandthread sum
       limit++;
       p[i].start = limit; //start value
       if(i == numOft-1){  //for last thread only
          limit += value-limit; //sum up the remain value for not divisible case
       }else{
         limit += divide;
       }
       p[i].end = limit;  //end value
       pthread_attr_init(&attr[i]);  /* set the default attributes of the thread */
       pthread_create(&tid[i], &attr[i],createThread,&p[i]);
       pthread_join(tid[i],NULL); /* wait for the thread to exit */
       total += p[i].partialSum; //sum up value from each thread
    }
    printf("Parent: Total sum is %ld\n",total);
    printf("Parent: End of  Execution\n ")  ;
}
/* The thread will execute in this function */
void *runner(void *param)
{
    struct SumPart *part = (struct SumPart*) param;

    int i;//, upper = atoi(param);
    long int sum = 0;

    for(i = part->start;i<= part->end; i++) //caculate the sum from start to end
        sum += i;
    part->partialSum = sum; //store the result to the struct
    pthread_exit(0);

}

void *createThread(void *param){
    struct subSumPart *part = (struct subSumPart*) param;
    int numOft = part->subthreads; //get the number of threads
    int value = part->end; //get the end value
    int limit = part->start; //get the start value
    int divide = (value-limit)/numOft-1;  //calculate the difference between threads and - 1
     pthread_t tid[numOft]; /* Array of the thread identifer*/
     pthread_attr_t attr[numOft];   /* Array of the grandchild thread attribute*/
     struct SumPart p[numOft]; //array of struct that include all information for each thread
    limit--;  // -1 for start value
    for(int i=0;i<numOft; i++){
       p[i].index = i;
       limit++;
       p[i].start = limit; //start value
       if(i == numOft-1){  //for last thread only
          limit += value-limit; //sum up the remain value for not divisible case
       }else{
         limit += divide;
       }
       p[i].end = limit;  //end value
       pthread_attr_init(&attr[i]);
       pthread_create(&tid[i], &attr[i],runner,&p[i]);
    }
    /*wait for the thread to exit */
    for(int i =0;i<numOft;i++){
        pthread_join(tid[i],NULL);
        printf("Child thread number %d, GrandChild thread number %d: \t grandchild start=%d \t grandchildend = %d \t grand childsum = %ld \n ", part->index,p[i].index, p[i].start, p[i].end, p[i].partialSum);
        part->partialSum += p[i].partialSum;
    }
    printf("\n --- Child thread number %d: \t child start=%d \t end = %d \t  childsum = %ld ---\n ", part->index, part->start, value, part->partialSum);
}
