#include<sys/syscall.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
typedef int buffer_item;
#define BUFFER_SIZE 5
buffer_item buffer[BUFFER_SIZE];
int i,count;
//pthread_mutex_t mutex;
sem_t empty,full;//in the begin,empty = 5,full = 5
sem_t mutex;
inline int insert_item(buffer_item item) {
	
	sem_wait(&empty);
	sem_wait(&mutex);
	//pthread_mutex_lock(&mutex);
		buffer[i] = item;
		i = (i + 1) % BUFFER_SIZE;
	//pthread_mutex_unlock(&mutex);
	sem_post(&mutex);
	sem_post(&full);
	return 0;
}

inline int remove_item(buffer_item *item) {
	int ans;	
	sem_wait(&full);
	sem_wait(&mutex);
	//pthread_mutex_lock(&mutex);
		ans = buffer[*item];
		buffer[*item] = 0;
	//pthread_mutex_unlock(&mutex);
	sem_post(&mutex);
	sem_post(&empty);
	return ans;
}

void *producer(void * param){
	buffer_item rand = 1;
	while(rand < 500){
		if(insert_item(rand)) {
			printf("report producer error condition\n");	
		}
		rand++;
	}
}

void *consumer(void * param){

	buffer_item rand = 0;
	int ans;
	while(1){
		ans = remove_item(&rand);
		if(!ans) {;
			//printf("report consumer %d,error condition\n",syscall(SYS_gettid));
		}else {
			printf("consumer %d consumed %d\n",syscall(SYS_gettid),ans);	
		}
		
		rand = (rand + 1) % BUFFER_SIZE;
	}
}


int main()
{
	pthread_t pro,consm1,consm2,consm3;
	pthread_attr_t attr;
	int p = 1;
	pthread_attr_init(&attr);
	//pthread_mutex_init(&mutex,NULL);
	sem_init(&empty,0,5);
	sem_init(&full,0,0);
	sem_init(&mutex,0,1);
	pthread_create(&pro,&attr,producer,&p);
	pthread_create(&consm1,&attr,consumer,&consm1);
	pthread_create(&consm2,&attr,consumer,&consm2);
	pthread_create(&consm3,&attr,consumer,&consm2);

	pthread_join(pro,NULL);
	pthread_join(consm1,NULL);	
	pthread_join(consm2,NULL);	
	pthread_join(consm3,NULL);	

	return 0;
}
