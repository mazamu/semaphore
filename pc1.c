#define __LIBRARY__
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>

#define Total 500
#define PNUM 5
#define BUFFERSIZE 10

sem_t *empty,*full,*mutex;
int main()
{
	int i,j,k;
	int costnum;
	int Outpos = 0;
	int Inpos = 0;
	FILE *fp = NULL;
	int pp;
//	sem_init(&empty,0,10);
//	sem_init(&full,0,0);
//	sem_init(&mutex,1,1);
	empty = sem_open("empty",O_CREAT|O_EXCL,0064,10);
	full = sem_open("full",O_CREAT|O_EXCL,0064,0);
	mutex = sem_open("mutex",O_CREAT|O_EXCL,0064,1);
	fp = fopen("FileBuffer.txt","wb++");
	
	fseek(fp,10 * sizeof(int),SEEK_SET);
	fwrite(&Outpos,sizeof(int),1,fp);
	fflush(fp);

	if(!fork()) {
		printf("in producer %d\n",getpid());
		for(i = 0; i < Total; i++) {
			sem_wait(empty);
			sem_wait(mutex);
			printf("p%d in lock\n",getpid());
			fseek(fp,Inpos * sizeof(int),SEEK_SET);
			fwrite(&i,sizeof(int),1,fp);
			fflush(fp);
			printf("%d was writed \n",i);
			fflush(stdout);
			Inpos = (Inpos + 1) % BUFFERSIZE;
		
			sem_post(mutex);
			sem_post(full);
			sem_getvalue(full,&pp);	
			printf("p%d out lock\n",getpid());
			printf("set full = %d\n",pp);
		}
		exit(0);
	}
	sleep(1);
	for(k = 0; k < PNUM; k++) {
		if(!fork()) {
			printf("in consumer %d \n",getpid());
			for(j = 0; j < Total / PNUM; j++) {
				 sem_getvalue(full,&pp);		
				 printf("full = %d\n",pp);
				 sem_wait(full);
				 sem_wait(mutex);
				 printf("c%d in lock\n",getpid());
				 fflush(stdout);
				 
				 fseek(fp,10 * sizeof(int),SEEK_SET);
				 fread(&Outpos,sizeof(int),1,fp);
				
				 fseek(fp,Outpos * sizeof(int),SEEK_SET);
				 fread(&costnum,sizeof(int),1,fp);
				 
				 printf("%d:	%d\n",getpid(),costnum);
				 fflush(stdout);
				 Outpos = (Outpos + 1) % BUFFERSIZE;
				
				 fseek(fp,10 * sizeof(int),SEEK_SET);
			 	 fwrite(&Outpos,sizeof(int),1,fp);
				 fflush(fp);
	
				 sem_post(mutex);
				 sem_post(empty);	
				 printf("c%d out lock\n",getpid());
				
			}
			printf("c%d exit\n",getpid());
			exit(0);
		}
	
	}
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	sem_unlink("empty");
	sem_unlink("full");
	sem_unlink("mutex");
	
	fclose(fp);
	return 0;
}
