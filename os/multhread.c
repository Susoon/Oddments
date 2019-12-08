#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0
#define CHART_LENGTH 100

typedef struct chart_node
{
	int process;
	int flag;
	int data;
	int turn;
}node;

node** chart;
int turn = 0, curr = 0;
int* n;
int flag[3] = { 0 };

void init_chart();
void print_chart();
void print(int who);
void lock(int who);
void unlock(int who);
void critical_section(int * num, int who);
void* thread_work0(void * arg);
void* thread_work1(void * arg);
void* thread_work2(void * arg);

void init_chart()
{
	for(int i = 0; i < CHART_LENGTH; i++)
	{
		chart[i] = (node*)malloc(sizeof(node));
		chart[i] -> process = 0;
		chart[i] -> flag = 0;
		chart[i] -> data = 0;
		chart[i] -> turn = 0;
	}
}

void print(int who)
{
	chart[curr] -> process = who;
	chart[curr] -> flag = flag[who];
	chart[curr] -> data = *n;
	chart[curr] -> turn = turn;

	print_chart();

	curr++;
}

void print_chart()
{
	printf("Time : %d -> Process : %d\n", curr, chart[curr] -> process);
	printf("\t\tFlag : %d\n", chart[curr] -> flag);
	printf("\t\tData : %d\n", chart[curr] -> data);
	printf("\t\tTurn : %d\n", chart[curr] -> turn);
}

void lock(int who)
{
	int next = (who + 1) % 3;
	flag[who] = 1;
	turn = next;
	while(flag[next] == 1 && turn == next);
}

void unlock(int who)
{
	flag[who] = 0;
}

void critical_section(int * num, int who)
{

	*num += 10;
	
	print(who);
}

void *thread_work0(void* arg)
{
	int *num = (int*)arg;
	
	while(curr < CHART_LENGTH)
	{
		lock(0);	
		critical_section(num, 0);
		unlock(0);
	}

	pthread_exit(0);
}

void *thread_work1(void* arg)
{
	int *num = (int*)arg;
	
	while(curr < CHART_LENGTH)
	{	
		lock(1);	
		critical_section(num, 1);
		unlock(1);
	}

	pthread_exit(0);
}

void *thread_work2(void* arg)
{
	int *num = (int*)arg;
	
	while(curr < CHART_LENGTH)
	{	
		lock(2);	
		critical_section(num, 2);
		unlock(2);
	}

	pthread_exit(0);
}

int main(void)
{
	n = (int*)malloc(sizeof(int));
	
	chart = (node**)malloc(sizeof(node*) * CHART_LENGTH);
	
	init_chart();
		
	pthread_t th1;
	pthread_t th2;
	pthread_t th3;
	pthread_attr_t attr1, attr2, attr3;

	*n = 1;

	pthread_attr_init(&attr1);
	pthread_attr_init(&attr2);
	pthread_attr_init(&attr3);

	pthread_create(&th1, &attr1, thread_work0, n);
	pthread_create(&th2, &attr2, thread_work1, n);
	pthread_create(&th2, &attr3, thread_work2, n);
	
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);

	
	free(n);
}
		

