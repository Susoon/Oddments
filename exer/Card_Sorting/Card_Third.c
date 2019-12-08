#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int num;
	struct node * next;
	struct node * prev;
}Node;

Node * head;
Node * hash_head;

void Add_First(int n)
{
	Node * tmp = (Node*)malloc(sizeof(Node));
	tmp -> num = n;
	tmp -> next = head;
	tmp -> prev = head -> prev;
	head -> prev -> next = tmp;
	head -> prev = tmp;
	head = tmp;
}

void Hash_Add(int n)
{
	Node* tmp = (Node*)malloc(sizeof(Node));
	tmp -> num = n;
	tmp -> next = hash_head;
	tmp -> prev = head;
	hash_head -> next = tmp;
	hash_head = tmp;
}	

void Remove_Cur_Node()
{
	head -> prev -> next = head -> next;
	head -> next -> prev = head -> prev;
	head = head -> next;
}

void Remove_Hash_Node()
{
	Node * tmp = hash_head -> next;
	hash_head -> next = tmp -> next;
	hash_head = tmp -> next;
}

int Hash_Traverse(int move)
{
	while(move > hash_head -> num)
	{
		move -= hash_head -> num;
		hash_head = hash_head -> next;
		while(hash_head -> num == 0) { Remove_Hash_Node();}
	}
	
	return move;
}

int Find_Loca(int move)
{
	int ret = 0;
	
	if(move + hash_head -> num > hash_head -> num)
	{
		move = Hash_Traverse(move);
		head = hash_head -> prev;
	}

	for(int i = 0; i < move; i++)
	{
		head = head -> next;
	}
	ret = head -> num;
	hash_head -> num -= 1;

	Remove_Cur_Node();

	return ret;
}

void sorting(int * arr, int n)
{
	int location = 0;
	for(int i = 1; i <= n; i++)
	{	
		location = Find_Loca(i % (n - i + 1));	

		arr[location] = i;
	}
}

int main(void)
{
	int n, count = 1001;
	int * arr;

	scanf("%d",&n);
	arr = (int*)calloc(n + 1, sizeof(int));
	
	head = (Node*)malloc(sizeof(Node));
	head -> next = head;
	head -> prev = head;
	head -> num = n;

	for(int i = n - 1; i >= 1; i--)
	{
		Add_First(i);
	}
	hash_head = (Node*)malloc(sizeof(Node));
	hash_head -> next = hash_head;
	hash_head -> prev = head;

	if(n > 1000)
		hash_head -> num = 1000;
	else
		hash_head -> num = n;

	for(int i = 1; i <= n; i++)
	{	
		if(i % 1000 == 0)
		{	
			if(n - i < 1000)
			{
				Hash_Add(n - i);
			}
			else
			{
				Hash_Add(1000);
			}
		}
			head = head -> next;
	}
	
	hash_head = hash_head -> next;
	sorting(arr, n);
	
	for(int i = 1; i <= n; i++)
	{
		printf("%d ",arr[i]);
	}
	
	printf("\n");
	
	free(arr);

	return 0;
}
