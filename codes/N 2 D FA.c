#include<stdlib.h>//N 2 D FA converter
#include<stdio.h>
#define S 10000  //Programmer-"Achint Chaudhary"
#define A 100
struct node{ 
		int data;
		struct node *next;		 };

struct stat{
	int al[A];
	struct node *next;
};

struct stat hash[S];

struct node *state[S][A+1],*state2[S][A];//initial & final state tables
struct node *alp,*closure[S];//alphabet read list & E-closure of each state

int dfa[S][A],tab[S][S];
int states,alpha,count,current,f[S],r[S],c[S],final,pass=2,chk,r_modify=0,trap;

void initialize()
{
	int i,j;
	for(i=0;i<S;i++)
		for(j=0;j<A;j++)
		{ state[i][j]=NULL;
		  state2[i][j]=NULL;
		  closure[i]=NULL;
		  hash[i].next=NULL; }
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

int comp(struct node *list,struct node *list2)
{
	while(list!=NULL&&list2!=NULL)
	{
		if( (list->data)==(list2->data) )
		  {
		  	list=list->next;
		  	list2=list2->next;
		  }
		else
		  return 1;
	}
	if(list==NULL&&list2==NULL)
	  return 0;
	return 1;
}

int search(struct node *list)
{
   int i,k;
   for(i=0;i<=count;i++)
   {
   	k=comp(list,hash[i].next);
   	if(k==0) {  return i;}
   }
   return -1;
}

int fcheck(struct node *list)
{
	while(list!=NULL)
	{
		if(f[list->data]==1)
		  return 1;
		  list=list->next;
	}
	return 0;
}

int check(int i, int j,int alp)
{//This function is used to check for each empty block in Table
	int k,a,b,t;
	for(k=0;k<alp;k++)
	{   a=dfa[i][k];
		b=dfa[j][k];
		if(a<b)
		{t=a;a=b;b=t;}
		if(tab[a][b]!=0)
		{return 1;} }//for loop ends
	return 0;//if no such alphabet exist pair remains empty (0)
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
	  
void ermvr()
{   
    int i,j,k,l,num;
    char more;
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
	
}

main()
{
	int i,j,k,l,num,start;
    char more;
	puts("Welcome to N 2 D FA converter\n");
	puts("                   -Achint Chaudhary\n");
	getch();
	
	printf("Does NFA contains %c moves\n",-18);
	more=getch();
	if(more=='y')
	ermvr();
	else
	{
	initialize();
	printf("Enter the number of states in NFA ");
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
			    add(&state2[i][j],num);
				printf("add more move <S%d,a%d> ? ",i,j);
				fflush(stdin);scanf("%c",&more);}}
    }
    puts("Mark each state as final(1) of non-final(0)");
	for(i=0;i<states;i++) scanf("%d",&f[i]);
	
	for(i=0;i<states;i++)
		for(j=0;j<=alpha;j++)
		{ sort(state2[i][j]); dupdel(state2[i][j]); }
	
	//printing initial state table
	puts("\n---------------------------------------------------------");
	puts("State table for NFA to be converted to DFA is");
	for(i=0;i<states;i++)
	{ printf("\n  << S%d",i);
	  if(f[i]==1)puts("* >>"); else puts(" >>");
		for(j=0;j<alpha;j++)
		  {printf("a%d : ",j); print(state2[i][j]); }
	}

	}//state table of NFA without E-moves is stored in state2 now
	   puts("Enter the start state");
	   scanf("%d",&start);
	   puts("\nPress any key to convert this NFA to DFA\n");
	   getch();
	//code for conversion to DFA (subset contruction method)
	for(i=0;i<alpha;i++)
	  hash[0].al[i]=0;
	  c[0]=0;   //index 0 is going to be the trap state\
	so all its entries points to itself
	add(&hash[1].next,start);
	if(f[start]==1){c[1]=1;}
	else c[1]=0;
	current=count=1;//working begins from state 1, 0th index for trap state\
	current proceeds by filling the table completely\
	count indiactes the number of subsets in the hash table
	
	struct node *pointer,*subset;
	
	for(i=2;i<S;i++)c[i]=0; //upcoming subsets non-final for DFA initially
	while(current<=count)
	{
	for(j=0;j<alpha;j++)
	 {
	   pointer=hash[current].next;// to read the states corresponding to\
	  state of current index (count)
	  subset=NULL; final=0;
	  while(pointer!=NULL)
	  {   
			k=read(pointer);
			dup(&subset,state2[k][j]);
			sort(subset);
			dupdel(subset);
	        final=fcheck(subset);
			pointer=pointer->next;
	  }//after end of this loop all states reachable by alphabet 'j'\
	   corresponding to current state are in subset
	   	   	   
		 l=search(subset);
     	if(l==-1)//new subset creation
			{
				count+=1;//index to be used for new subset
				l=count;
				dup(&hash[count].next,subset);
				sort(hash[count].next);
				if(final==1)
				 c[count]=1;		
			}
			
		hash[current].al[j]=l;
	 }
	current+=1;;
	}
	
	puts("State table of DFA after subset construction");
	for(k=0;k<=count;k++)
      {
	    printf("[S%d]",k);
	    if(c[k]==1)printf("* : | ");
	    else printf("  : | ");
	   for(l=0;l<alpha;l++)
          printf("%d | ",hash[k].al[l]); print(hash[k].next);
	  }
	
	puts("Do you want to minimize this DFA");
	more=getch();
	if(more=='y')
	{
		i=1;
		for(k=0;k<=count;k++)
		  for(l=0;l<alpha;l++)
		    dfa[k][l]=hash[k].al[l];//copying from hash table to dfa table
		    	    
		for(k=1;k<=count;k++)
		  {for(l=0;l<alpha;l++)
		   if(dfa[k][l]==0)
		   { i=0; break;}}//i=0 if trap state is reachable
		trap=i;
		if(i==1)
		{//Our trap state is unreachable
		   for(k=0;k<=count;k++)
		     for(l=0;l<alpha;l++)
		     {
		       dfa[k][l]=dfa[k+1][l]-1;
			   c[k]=c[k+1];	
		     }
		     count=count-1;
		}
		else
		{//Trap state is reachable
			for(k=0;k<count;k++)
		     for(l=0;l<alpha;l++)
		     {
		       if(dfa[k+1][l]-1!=-1)
			   dfa[k][l]=dfa[k+1][l]-1;
			   else
			   dfa[k][l]=count;
			   c[k]=c[k+1];	
		     }
		     for(l=0;l<alpha;l++)
		       { dfa[count][l]=count; c[count]=0; }
		}
				
		for(i=0;i<count;i++)//First pass (final/non-final)
    	for(j=i+1;j<=count;j++)
        	if((c[i]+c[j])==1)
        	 tab[j][i]=1;
    
		do{//Further passes in this do-while loop
    	chk=0;
    	for(i=0;i<count;i++)
    	  {for(j=i+1;j<=count;j++)
    	    {if(tab[j][i]==0)//for empty blocks only
    	    {if(check(i,j,alpha)==1)//if get to a filled block
    	    { tab[j][i]=pass; chk=1;}}}}//fill that block by pass
	   pass+=1;//increment pass as it becomes complete
	  }while(chk==1);
	  
	  puts("---------------------------------------------------------");
      
	  printf("%d passes occur in minimization\n",pass-1);
       puts("\nFinal computed table is");
      for(i=1;i<=count;i++)//Final computed table
    	  { printf("[%d] ",i);
		    for(j=0;j<i;j++)
		      printf(" %d ",tab[i][j]);
          puts("");}
          printf("    ");
          for(j=0;j<count;j++)
		      printf("[%d]",j);
	  
	   puts("\n---------------------------------------------------------");
      for(i=0;i<=count;i++)r[i]=1;
	  for(i=0;i<count;i++)//modifying state table to minimize it
      for(j=i+1;j<=count;j++)
       {if(r[i]==1)
       	{if(tab[j][i]==0)
       	 {  r[j]=0;//make that state unreadable
       	    r_modify=1;
       		for(k=0;k<=count;k++)
       		  for(l=0;l<alpha;l++)
       		   { if(dfa[k][l]==j)
       		      dfa[k][l]=i;//modify higher states of same block
			     if(c[j]==1)    //with the smallest state
			      c[i]==1;}
       	 } }
       	 else break; }

       	 if(r_modify==0 && trap==0 )
	   { puts("\nMachine is already minimized");
	     puts("S0 is trap state while S1 is starting state"); 
	     puts("\nThanks for using my code"); getch(); return 0; }
    
	puts("\nFinal State table of Minimized DFA is");
    for(k=0;k<=count;k++)
      {if(r[k]==1) 
	   {printf("[S%d]",k);
	    if(c[k]==1)printf("* : | ");
	    else printf("  : | ");
	   for(l=0;l<alpha;l++)
          printf("%d | ",dfa[k][l]);
       }
       if(r[k]==1)puts("");
	  }
	   puts("S0 is starting state");
	   		
	}//minimization ends here
	  puts("\nThanks for using my code");
	   getch();
}
