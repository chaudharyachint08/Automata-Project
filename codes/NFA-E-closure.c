#include<stdlib.h>//e-moves remover from NFA
#include<stdio.h>
#define S 10000  //Programmer-"Achint Chaudhary"
#define A 100
struct node{ 
		int data;
		struct node *next;		 };

struct node *state[S][A+1],*state2[S][A];//initial & final state tables
struct node *alp,*closure[S];//alphabet read list & E-closure of each state
int states,alpha,f[S],r[S],c[S];//final, readable & non- empty closure states

void initialize()
{
	int i,j;
	for(i=0;i<S;i++)
		for(j=0;j<A;j++)
		{ state[i][j]=NULL;
		  state2[i][j]=NULL;
		  closure[i]=NULL; }
}

void sort(struct node *list)
{//Selection sort by swapping data values
	int temp;
	struct node *q;
	while(list!=NULL)
	{	q=list->next;
		while(q!=NULL)
		{if((q->data)<(list->data))
		   {temp=q->data; q->data=list->data; list->data=temp;}
		q=q->next;
		}	list=list->next;
    }
}

void add(struct node **list,int num)
{//add at beginning of Linked list
	struct node *temp;
	temp=(struct node *)malloc(sizeof(struct node));
	temp->data=num;
	temp->next=*list;
	*list=temp;
}

void dup(struct node **list, struct node *p)
{//Generates a separate copy of the linked list in **list
	while(p!=NULL)
	{ add(list,p->data);
	   p=p->next; }
}

int read(struct node *list)
{   int i;
	if(list==NULL)
	return -1;
	i=list->data;
	return i;
}

void  dupdel(struct node *list)
{	struct node *tmp;
	while(list!=NULL)
	{	tmp=list->next;
		while(tmp!=NULL)
		{  if((tmp->data)==(list->data))
			{tmp=tmp->next;}
			else break;}
	    list->next=tmp;
	    list=list->next;
	}
}

void print(struct node *list)
{  if(list==NULL){printf("%c\n",-19); return;}
	while(list!=NULL)
	  { printf("S%d ",list->data);
	    list=list->next;
	  }
	   puts("");
	   return;
}
	  
main()
{   int i,j,k,l,num;
    char more;
	puts("Welcome to E-moves remover for NFA\n");
	puts("                   -Achint Chaudhary\n");
	getch();
	fflush(stdin);
	initialize();
	printf("Enter the number of states in NFA-%c ",-18);
	scanf("%d",&states);
	printf("Enter the number of alphabets in language ");
	scanf("%d",&alpha);
	puts("Enter the enteries in the state table of DFA");
	for(i=0;i<states;i++)
	{    printf("\n  << S%d >> :\n",i);
	    	for(j=0;j<alpha;j++)
        	{ if(j!=0)puts("");
			  printf("\ta%d: \n",j);
			 printf("add move <S%d,a%d> ? ",i,j);
        	  fflush(stdin);scanf("%c",&more);
			  while(more=='y')
			  { printf("Enter the state ");
			    scanf("%d",&num); 
			    add(&state[i][j],num);
				printf("add more move <S%d,a%d> ? ",i,j);
				fflush(stdin);scanf("%c",&more);}}
			    add(&state[i][alpha],i);
			printf("\nadd %c-moves <S%d> ? ",-18,i);
		    fflush(stdin);scanf("%c",&more);
		    if(more=='y')c[i]=1;
		    else c[i]=0;
			while(more=='y')
			{   printf("Enter the state ");
			    scanf("%d",&num); 
			    add(&state[i][alpha],num);
				printf("add more %c-moves <S%d> ? ",-18,i);
				fflush(stdin);scanf("%c",&more); }
    }
    puts("Mark each state as final(1) of non-final(0)");
	for(i=0;i<states;i++) scanf("%d",&f[i]);
	
	for(i=0;i<states;i++)
		for(j=0;j<=alpha;j++)
		{ sort(state[i][j]); dupdel(state[i][j]); }
	
	//printing initial state table
	puts("\n---------------------------------------------------------");
	puts("Input State table");
	for(i=0;i<states;i++)
	{ printf("\n  << S%d",i);
	  if(f[i]==1)puts("* >>"); else puts(" >>");
		for(j=0;j<alpha;j++)
		  {printf("a%d : ",j); print(state[i][j]); }
		printf("E  :");
		print(state[i][alpha]);
	}
	
	//Upto here state table of NFA is entered by user
	
	for(i=0;i<states;i++)
	{//states without E-moves are un affected in state table
		if(c[i]==0)
		{
			for(j=0;j<alpha;j++)
			  { dup(&state2[i][j],state[i][j]);
			    sort(state2[i][j]); }
		}
	}
    struct node *pointer;
	//Finding E-closures of each state
	
	for(i=0;i<states;i++)
	{   for(l=0;l<states;l++)r[l]=1;
		if(c[i]==1)
		{
			dup( &closure[i], state[i][alpha] );
			r[i]=0;
			pointer=closure[i];
			while(pointer!=NULL)
			{
				k=read(pointer);
				//States having a final state in their 
				//E-closure are also final state
				if(f[k]==1)f[i]=1;
				if(r[k]==0)       
				{ pointer=pointer->next; continue; }
				else
				{ k=read(pointer);
				  dup( &closure[i], state[k][alpha] );
				  sort(closure[i]);
				  dupdel(closure[i]);
				  r[k]=0;
				}
			pointer=closure[i];
			}
		}
		else add(&closure[i],i);
	}
	puts("\n---------------------------------------------------------");
	printf("%c-closures of states are\n",-18);
	for(i=0;i<states;i++){ printf("S%d ==> ",i); print(closure[i]); }
	
	for(i=0;i<states;i++)
	{
		for(j=0;j<alpha;j++)
		{
			alp=NULL;
			pointer=closure[i];
			while(pointer!=NULL)
			{
				k=read(pointer);
				dup(&alp,state[k][j]);
				sort(alp);
				dupdel(alp);
				pointer=pointer->next;
			}
		 pointer=alp;
		 while(pointer!=NULL)
		 {
		 	k=read(pointer);
		 	dup(&state2[i][j],closure[k]);
		 	sort(state2[i][j]);
		 	dupdel(state2[i][j]);
		 	pointer=pointer->next;
		 }
	    }
	}
	puts("---------------------------------------------------------");
	//printing Final state table
	printf("\nState table of NFA without %c moves is\n",-18);
	for(i=0;i<states;i++)
	{ printf("\n  << S%d",i);
	  if(f[i]==1)puts("* >>"); else puts(" >>");
		for(j=0;j<alpha;j++)
		  {printf("a%d :",j); print(state2[i][j]); }
	}
	   puts("\nThanks for using my code");
	   getch();
}
