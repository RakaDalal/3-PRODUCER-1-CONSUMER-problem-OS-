
#define BUFFER_SIZE 2

#define RAND_DIVISOR 100000000

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

typedef struct {
    char *buf[BUFFER_SIZE]; // the buffer
    int length; // number of items in the buffer
    pthread_mutex_t mutex; // for locking buffer 
    pthread_cond_t can_produce; // signaling producer when items are removed
    pthread_cond_t can_consume; // signaling consumer when items are produced
} semaphore;

semaphore buffer;

void initiate()
{
	int i;
	
	semaphore buffer = {
        .length = 0,
	};
	
	pthread_mutex_init(&buffer.mutex, NULL);	
	pthread_cond_init(&buffer.can_produce, NULL);		
	pthread_cond_init(&buffer.can_consume, NULL);


	for(i=0;i<BUFFER_SIZE;i++)
	{
		buffer.buf[i]=malloc(1);
	}
	

}




void initializeData() {

   srand ( time(NULL) );
   
   initiate();
}

/* Add an item to the buffer */
void insert_item(char item[]) {
   buffer.buf[buffer.length] = (char *) realloc(buffer.buf[buffer.length], strlen(item) + 1);
   strcpy(buffer.buf[buffer.length],item);
	++buffer.length;
	
   }

/* Remove an item from the buffer */
void remove_item() {
   
	
	//If Buffer has Items
	if(buffer.length>1)
	{
		strcpy(buffer.buf[0],buffer.buf[1]);
	}
		buffer.length--;
			
	
	
	//return item;
}
/* Producer_red Thread */
void *producer_red(void *param) {

struct timeval tv;
char item[25]="";
int current=0;
FILE *fpr;
   while(current<1000) {
      
      int rNum = rand() / RAND_DIVISOR;
      if(rNum%2==0){
      gettimeofday(&tv, NULL);
      sprintf(item,"RED_%li.%li",tv.tv_sec,tv.tv_usec);

	pthread_mutex_lock(&buffer.mutex);// lock critical region

	while(buffer.length == BUFFER_SIZE) // Buffer full so need to wait to produce
	{ 
		pthread_cond_wait(&buffer.can_produce, &buffer.mutex);
	}

	
	insert_item(item);

	fpr = fopen("Producer_RED.txt","a+");
	fprintf(fpr, "%s\n",item);
        fclose(fpr);
	
	// if buffer was empty, then signal the consumer
	if(buffer.length==1)
	{
		pthread_cond_signal(&buffer.can_consume);
	}
	
	//unlock critical region
	pthread_mutex_unlock(&buffer.mutex);
	
     
	
        current++;
      }}
     
}
/* Producer_black Thread */
void *producer_black(void *param) {
   struct timeval tv;
char item[25]="";
int current=0;
FILE *fpb;
   while(current<1000) {
     
      int rNum = rand() / RAND_DIVISOR;
      if(rNum%2==0){
      gettimeofday(&tv, NULL);
      sprintf(item,"BLACK_%li.%li",tv.tv_sec,tv.tv_usec);

	pthread_mutex_lock(&buffer.mutex);//lock critical region

	while(buffer.length == BUFFER_SIZE) // Buffer full so need to wait to produce
	{ 
		pthread_cond_wait(&buffer.can_produce, &buffer.mutex);
	}

	
	insert_item(item);
	fpb = fopen("Producer_BLACK.txt","a+");
	fprintf(fpb, "%s\n",item);
        fclose(fpb);
	
	// if buffer was empty, then signal the consumer
	if(buffer.length==1)
	{
		pthread_cond_signal(&buffer.can_consume);
	}
	
	//unlock critical region
	pthread_mutex_unlock(&buffer.mutex);
	
     
	
        current++;
      }}
     
}
/* Producer_white Thread */
void *producer_white(void *param) {
  struct timeval tv;
char item[25]="";
int current=0;
FILE *fpw;
   while(current<1000) {
     
      int rNum = rand() / RAND_DIVISOR;
      if(rNum%2==0){
      gettimeofday(&tv, NULL);
      sprintf(item,"WHITE_%li.%li",tv.tv_sec,tv.tv_usec);

	pthread_mutex_lock(&buffer.mutex);// lock critical region

	while(buffer.length == BUFFER_SIZE) // Buffer full so need to wait to produce
	{ 
		pthread_cond_wait(&buffer.can_produce, &buffer.mutex);
	}

	
	insert_item(item);
	fpw = fopen("Producer_WHITE.txt","a+");
	fprintf(fpw, "%s\n",item);
        fclose(fpw);
	
	// if buffer was empty, then signal the consumer
	if(buffer.length==1)
	{
		pthread_cond_signal(&buffer.can_consume);
	}
	
	//unlock critical region
	pthread_mutex_unlock(&buffer.mutex);
	
     
	
        current++;
      }}
     
}
/* Consumer Thread */
void *consumer(void *param) {
   char *item="";
   int Consume = 0;
	FILE *fp;
	

   while(Consume<3000) {
      
      pthread_mutex_lock(&buffer.mutex);//lock critical region

	if(buffer.length == 0)     // Buffer empty so need to wait for produce
	{ 
		pthread_cond_wait(&buffer.can_consume, &buffer.mutex);
	}
        fp = fopen("Consumer.txt","a+");
	fprintf(fp, "%s\n",buffer.buf[0]);
	fclose(fp);
	//fetch data from buffer
	remove_item();
	
	
	if(buffer.length==BUFFER_SIZE -1)  // if buffer was full, then broadcast signal to the producers
	{
		pthread_cond_broadcast(&buffer.can_produce);		
	}
	

	//unlock critical region
	pthread_mutex_unlock(&buffer.mutex);

        
        Consume++;
      }
     }






int main() {

remove("Producer_BLACK.txt");
remove("Producer_WHITE.txt");
remove("Producer_RED.txt");
remove("Consumer.txt");
   
   initializeData();

 
pthread_t thread_producer_red, thread_producer_black, thread_producer_white, thread_consumer;
	
        pthread_create( &thread_producer_red, NULL, &producer_red, NULL);
	pthread_create( &thread_producer_black, NULL, &producer_black, NULL);
	pthread_create( &thread_producer_white, NULL, &producer_white, NULL);
	pthread_create( &thread_consumer, NULL, &consumer, NULL);
	
	//Waiting for all the threads to finish before leaving
	pthread_join( thread_producer_red, NULL);
	pthread_join( thread_producer_black, NULL);
	pthread_join( thread_producer_white, NULL);
	pthread_join( thread_consumer, NULL);
   
    
      exit(0);
}
