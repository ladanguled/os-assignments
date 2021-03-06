#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "semaphore.h"
#include "stdint.h"
#include "malloc.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif


#define MAX_SLEEP_TIME 3
#define NUM_OF_STUDENTS 4
#define NUM_OF_HELP 2
#define NUM_OF_SEATS 2


pthread_t student_array[NUM_OF_STUDENTS];
pthread_t ta_thread;

pthread_mutex_t mutex_lock;

sem_t students_sem;
sem_t ta_sem;


int waiting_students = 0;

int thread_helped_counter[NUM_OF_STUDENTS];

unsigned int seed = 92;

int sleep_time;

int waiting_queue[NUM_OF_HELP];


//prototype
void * students(void * param);
void * ta();
void create_students();
void create_ta();
void join_students();
void join_ta();


int value;


int main() {
   sem_init(&students_sem,0,1);
   sem_init(&ta_sem,0,1);
   create_students();
   create_ta();
   join_students();
   join_ta();
   sem_destroy(&students_sem);
   sem_destroy(&ta_sem);
}

void join_ta() {
   if(pthread_join(ta_thread, NULL)){
        fprintf(stderr, "Error joining thread\n");
    }
}

void create_ta(){  
   if(pthread_create(&ta_thread, NULL, ta, NULL)){
        fprintf(stderr, "TA thread creation error\n");
   }
}

void join_students() {
   int i;
   for(i = 0; i < NUM_OF_STUDENTS; i++){
        if(pthread_join(student_array[i], NULL)){
           fprintf(stderr, "Error joining thread\n");
        }
   }
}

void create_students(){
   int i;
    
    for(i = 0; i < NUM_OF_STUDENTS; i++) {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        if(pthread_create(&student_array[i], NULL, students, (void *)arg)){
            fprintf(stderr, "Student thread creation error\n");
        }
    }

   if(pthread_create(&ta_thread, NULL, ta, NULL)){
        fprintf(stderr, "TA thread creation error\n");
   }
}


void * students(void * param) {
    int i = *((int *)param);
    int sleeptime = (rand_r(&seed)% MAX_SLEEP_TIME)+1;
    printf("Student %d programming for %d seconds.\n", i, sleeptime);
    sleep(sleeptime);
    pthread_mutex_lock(&mutex_lock);

      if(thread_helped_counter[i] >= NUM_OF_HELP){
        pthread_mutex_unlock(&mutex_lock);
        return NULL;
        }

        pthread_mutex_unlock(&mutex_lock);
        pthread_mutex_lock(&mutex_lock);
        sem_getvalue(&ta_sem, &value);
        pthread_mutex_unlock(&mutex_lock);


        if(value > 0){
            sem_wait(&ta_sem);
            pthread_mutex_lock(&mutex_lock);

            if(waiting_students >= 0 && waiting_students < NUM_OF_SEATS){

               if(waiting_students == 0){
                   waiting_queue[0] = i;
                   printf("\t\t Student %d takes a seat, # of waiting students = %d\n", waiting_queue[0], waiting_students);
                   waiting_students++;
               }

               else if(waiting_students == 1){
                  waiting_queue[1] = i;
                  printf("\t\t Student %d takes a seat, # of waiting students = %d\n", waiting_queue[1], waiting_students);
                  waiting_students++;
               }

               pthread_mutex_unlock(&mutex_lock);
               sem_post(&students_sem);
               sleep_time = (rand_r(&seed)% MAX_SLEEP_TIME)+1;
               sleep(sleep_time);

               }      

        else {
            
            printf("\nwaiting list > 2\n");
            pthread_mutex_unlock(&mutex_lock);
            printf("\t\t\tStudent %d will try later\n", i);
            sleep((rand_r(&seed)% MAX_SLEEP_TIME)+1);
            students(&i);
            }
        }
        else { 
            sem_post(&students_sem);
        }
        students((void*)&i);
}



void * ta() {

        int i;
        int result;

        for(i=0;i< 4; i++){
            result+=thread_helped_counter[i];
        }

        if(result == 8){
          return NULL;
        }

        sem_wait(&students_sem);
        pthread_mutex_lock(&mutex_lock);
        sleep_time = (rand_r(&seed)% MAX_SLEEP_TIME)+1;

        if(waiting_students == 1 && thread_helped_counter[waiting_queue[0]] < 2){
            waiting_students--;
            printf("Helping a student for %d , # of students waiting = %d\n",sleep_time, waiting_students);
            printf("Student %d recieving help\n", waiting_queue[0]);
            thread_helped_counter[waiting_queue[0]]++;
        }
        else if(waiting_students == 2 && thread_helped_counter[waiting_queue[1]] < 2){
            waiting_students--;
            printf("Helping a student for %d , # of students waiting = %d\n",sleep_time, waiting_students);
            printf("Student %d recieving help\n", waiting_queue[1]);
            thread_helped_counter[waiting_queue[1]]++;
        }

        pthread_mutex_unlock(&mutex_lock);
        sleep(sleep_time);
        sem_post(&ta_sem);
        ta();
}