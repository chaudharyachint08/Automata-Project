#include<stdio.h>
#include<stdlib.h>

struct node{ 
		int data;
		struct node *next;		 };
		
void add(struct node **list,int num)
{//add at beginning of Linked list
	struct node *temp;
	temp=(struct node *)malloc(sizeof(struct node));
	temp->data=num;
	temp->next=*list;
	*list=temp;
}

main()
{
	int i;
	struct node *temp=NULL;
	for(;;)
	{
		add(&temp,0);
	}
}
