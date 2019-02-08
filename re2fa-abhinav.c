//"Regular Expression" to "Minimized Finite Automaton" Converter
//Programmer-"Achint Chaudhary"

#include<stdlib.h>
#include<stdio.h>// '\n' = 10; EPSILON==-18; PHI==-19;  BACK==8

#define S 22010  //Average 355 States per alphabet
#define A 62	 //Total possible Alphabet inputs

struct node{ 
		int data;
		struct node *next;		 };

struct stat{
	int al[A];
	struct node *next;
};

struct machine{//Abstraction to Define all NFA in same state table
		int start;
		struct node *finals;
		struct machine *nm;
};

struct machine *utoms=NULL,*ltoms=NULL;//Pointers for top of machine stack

struct stat hash[S];

struct node *state[S][A+1],*state2[S][A];//Initial & final NFAs tables
struct node *alp,*closure[S];//Alphabet read list & E-closure of each state

int dfa[S][A],tab[S][S];
int states,alpha,count,current,f[S],r[S],c[S];
int final,pass=2,chk,r_modify=0,trap,len,indx=-1,radix;

char in[S],post[S],opstk[S],cmplt;

void initialize()
{//Initializes all pointers to NULL & states as Non-final
	int i,j;
	
	for(i=0;i<S;i++)
		for(j=0;j<A;j++)
		{ f[i]=0;
		  state[i][j]=NULL;
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
{//Compares two linked lists, return 0 is same 1 otherwise
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
{//Searches given linked list in hash table
   int i,k;
   
   for(i=0;i<=count;i++)
   {
   	k=comp(list,hash[i].next);
   	if(k==0) {  return i;}
   }
   return -1;
}

int fcheck(struct node *list)
{//Check if there is any final state in linked list
	while(list!=NULL)
	{
		if(f[list->data]==1)
		  return 1;
		  list=list->next;
	}
	return 0;
}

void freeLL(struct node **pointer)
{//Free all the "Heap" memory of the linked list
	struct node *temp,*temp2;
	
	temp=*pointer;
	while(temp!=NULL)
	{
		temp2=temp;
		temp=temp->next;
		free(temp2);
	}
	(*pointer)=NULL;
}

int check(int i, int j,int alp)
{//Check for entries in stair shaped table
	int k,a,b,t;
	
	for(k=0;k<alp;k++)
	{   a=dfa[i][k];
		b=dfa[j][k];
		if(a<b)
		{t=a;a=b;b=t;}
		if(tab[a][b]!=0)
		{return 1;} }//for loop ends
	return 0;//If no such alphabet exist pair remains empty (0)
}

void add(struct node **list,int num)
{//Add at beginning of Linked list
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
{//Reads & returns first data value of linked list
	 int i;
	
	if(list==NULL)
	return -1;
	i=list->data;
	return i;
}

void  dupdel(struct node *list)
{//Deletes duplicate entries from linked list	
	struct node *tmp,*prev;
	
	while(list!=NULL)
	{	prev=tmp=list->next;
		while(tmp!=NULL)
		{  if((tmp->data)==(list->data))
			{tmp=tmp->next; free(prev); prev=tmp;}
			else break;}
	    list->next=tmp;
	    list=list->next;
	}
}

void print(struct node *list)
{//Prints entire linked list
	if(list==NULL){printf("%c\n",-19); return;}
	while(list!=NULL)
	  { printf("S%d ",list->data);
	    list=list->next;
	  }
	   puts("");
	   return;
}
	  
int prcdnc(char c)
{//Returns precedence value of basic operators
	if(c=='*')          return 3;
	if(c=='.')          return 2;
	if(c=='+')          return 1;
	if(c=='(')          return 0;
}

void in_re()
{//Used to input Regular Expression
	int s,i,j,k,tos=-1;
	char c;
	
	printf("\n'+' for Union\n\
'.' for Concatenation\n\
'*' for Kleene Closure\n\
'%%' for %c\n'^' for %c\n",-19,-18);
	puts("\nEnter Regular Expression");
	s=len=0;
	fflush(stdin);
	do{
		while(10!=(c=getchar())) 
		{
			if(c==8)//Backspace is pressed by user
		   {
			if(len!=0)
			len--;
			else{ printf("\a"); }
			printf("\r");
			for(i=0;i<s;i++)
			printf(" ");
			printf("\r");
			for(i=0;i<len;i++)
			printf("%c",in[i]);
			continue;
		   }
		if(c=='^'){in[len]=-18; printf("%c",-18); s=len+=1; continue;}
		if(c=='%'){in[len]=-19; printf("%c",-19); s=len+=1; continue;}
		in[len]=c;
		s=len+=1;
		}
	if(len==0)
	{ printf("\rHey! NO R.E. INPUT\a");
	  getchar();printf("\r                  \r"); }
	  
	}while(len==0);
	
	
    for(i=0,j=-1;i<len;i++)
    {
    	c=in[i];
		if( (c>=48&&c<=57) || (c>=65&&c<=90)
		 || (c>=97&&c<=122) || c==-18/*EPSILON*/ || c==-19/*PHI*/ )
		{//Is a character b/w 0-9, A-Z, a-z\
		   then directly add to POSTFIX  String
		    j+=1;
			post[j]=c;//printf("%c to POSTFIX %d Directly\n",c,j);
		}
		else if(c==')')// ')' is encountered
		{
			while( opstk[tos]!='(' && tos!=-1 )
			{//Pop entire stack until ( is found
				j+=1;
				post[j]=opstk[tos];
				//printf("%c to POSTFIX %d due to ')' \n",c,j);
				tos=tos-1;
			}
			if(tos==-1)
			{ puts("Hey! INVALID R.E. due to more RIGHT parenthesis");
			  getchar(); return; }
			if(tos!=-1){ tos=tos-1; }
		}
		else if(c=='(')
		{//Left parenthesis is encountered
			if(tos==-1)//If operator stack is empty
		  { tos+=1; opstk[tos]=c; }//printf("%c to EMPTY stack\n",c);
		  else
		  {//Operator stack is not empty
		   tos+=1;
		   opstk[tos]=c;
		   //printf("( to OP_STACK      %d\n",tos);
		  }
	    }
		else
		{//Is an operator 
		  if(i==0){puts("1st symbol can't be operator");
		   getchar(); len=j+1; return ;}
		  if(tos==-1)//If operator stack is empty
		  { tos+=1; opstk[tos]=c; }//printf("%c to EMPTY stack\n",c);
		  else
		  {//Operator stack is not empty
			k=prcdnc(c);
			while(k<=prcdnc(opstk[tos])&&tos!=-1)
			{
					//printf("%c at %d index\n",c,i);
					j+=1;
					post[j]=opstk[tos];
					//printf("%c to POSTFIX %d due %c\n",opstk[tos],j,c);
					tos=tos-1;
			}
			tos+=1;
			opstk[tos]=c;
		  }	
		}
    }//Infix readed here completely & all identifiers were processed

    puts("");
    	while(tos!=-1)
    	{ //puts("\nINFIX is readed completely, now pop entire OP_STACK\n");
		   if(opstk[tos]=='(')
		   { puts("Hey! INVALID R.E. due to more LEFT parenthesis");
		    len=j+1; getchar(); return; }
		   j+=1;
		   post[j]=opstk[tos];
		   //printf("%c to POSTFIX index %d\n",opstk[tos],j);
		   tos--; 
		}
		len=j+1;
		//len denotes new length of postfix expression as ( & ) are all removed
	/*puts("POSTFIX regular expression is");
    puts("\t");								  
	for(i=0;i<len;i++)						  
	printf("%c",post[i]);	puts(""); */
	
}

void ermvr()
{//Removes the EPSILON moves from 1st-NFA
    int i,j,k,l,num;
	
	for(i=0;i<states;i++)
		{ add(&state[i][alpha],i);
		  for(j=0;j<=alpha;j++)
		  { sort(state[i][j]); dupdel(state[i][j]); }
	 	}
	/*Printing initial state table
	if(cmplt=='y')
	{
	puts("---------------------------------------------------------");
	printf("\nState table of NFA with %c moves\n\n",-18);
	for(i=0;i<states;i++)
	{ printf("\n  << S%d",i);
	  if(f[i]==1)puts("* >>"); else puts(" >>");
		for(j=0;j<alpha;j++)
		  {printf("'%c' : ",s2char(j)); print(state[i][j]); }
		printf("'%c' : ",-18);
		print(state[i][alpha]);
	}
	}
	*/
	//Upto here state table of NFA is entered by user
	
	for(i=0;i<states;i++)
	{//States without EPSILON moves are unaffected in 2nd state table
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
			dup( &closure[i], state[i][alpha] );
			r[i]=0;
			pointer=closure[i];
			while(pointer!=NULL)
			{
				k=read(pointer);
				//States having a final state in their\
				  E-closure are also final state
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
	/*
	if(cmplt=='y')
	{
	puts("\n---------------------------------------------------------");
	printf("\n%c-closures of states are\n\n",-18);
	for(i=0;i<states;i++)
	{ 
	  printf("S%d ",i);
	  for(j=0;j<(rad(states)-rad(i));j++){printf(" ");}
	  printf("==> ");
	  print(closure[i]);
	}
	}
	*/
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
		 freeLL(&alp);
	    }
	}
	/*
	if(cmplt=='y')
	{
	puts("\n---------------------------------------------------------");
	//Printing Final state table
	printf("\nState table of NFA without %c moves is\n",-18);
	for(i=0;i<states;i++)
	{ printf("\n  << S%d",i);
	  if(f[i]==1)puts("* >>"); else puts(" >>");
		for(j=0;j<alpha;j++)
		  {printf("'%c' : ",s2char(j)); print(state2[i][j]); }
	}
	}
	*/
	for(i=0;i<states;i++)
	  {//Epsilon closure & 1st-NFA are not needed now so Freed up
	  	freeLL(&closure[i]);
		  for(j=0;j<=alpha;j++)
	     	freeLL(&state[i][j]);
	  }
}

void pushmachine(int al)
{//Pushes a machine corresponding to a identifier on TOMS
	if(al>=0&&al<=61)
	{
		indx+=1;
		add(&state[indx][al],indx+1);
		ltoms=utoms;
		utoms=(struct machine *)malloc(sizeof(struct machine));
	    utoms->start=indx;
	    utoms->finals=NULL;
	    add(&utoms->finals,indx+1);
	    utoms->nm=ltoms;
		indx+=1;
		f[indx]=1;
	}
	
	if(al==-18)//machine for EPSILON
	{
		indx+=1;
		ltoms=utoms;
		utoms=(struct machine *)malloc(sizeof(struct machine));
	    utoms->start=indx;
	    utoms->finals=NULL;
		add(&(utoms->finals),indx);
	    utoms->nm=ltoms;
		f[indx]=1;
	}
	
	if(al==-19)//machine for PHI
	{
		indx+=1;
		ltoms=utoms;
		utoms=(struct machine *)malloc(sizeof(struct machine));
	    utoms->start=indx;
	    utoms->finals=NULL;
	    utoms->nm=ltoms;
		f[indx]=0;
	}
}

void kleene(int i)
{//Finds the Kleene * Closure on basis of previous machine
	int k;
	char v;
	struct node *temp;
	
	if(utoms==NULL)
	{ puts("Hey! INVALID R.E. due to more '*' operators"); getchar(); return; }
	
	v=post[i-1];
	
	if( (v>=48&&v<=57) || (v>=65&&v<=90)
	 || (v>=97&&v<=122) || v==-18 /*EPSILON*/ || v==-19 /*PHI*/ )
	{  //'*' is applied to single character machine
		if( (v>=48&&v<=57) || (v>=65&&v<=90)
		 || (v>=97&&v<=122) )
	 	{
	 		f[indx]=0;	//Mark current state as non final
	 		--indx;	  	//Reduce one state that is added by mistake
	 		f[indx]=1;	//Starting state of character machine is final
	 		if(v>=48&&v<=57)  v=v-48;
   			if(v>=65&&v<=90)  v=v-55;
   			if(v>=97&&v<=122) v=v-61;
			freeLL(&state[indx][v]);
	 		add(&state[indx][v],indx);
			freeLL(&(utoms->finals));
	 		add(&(utoms->finals),indx);
	 	}
	 	
	 	else if(v==-18|| v==-19)//'*' on EPSILON / PHI\
	 	So, Resulting machine is also for EPSILON / PHI
	 	{
	 		//Do nothing
	 	}
	}
	
	else
	{ //'*' is not applied to single character machine
	indx+=1;
	temp=utoms->finals;
	if(temp==NULL)v=0;
	while(temp!=NULL)
	{//All final points to new state
		k=read(temp);
		add(&state[k][alpha],indx);
		f[k]=0;
		temp=temp->next;
	}
	add(&state[indx][alpha],utoms->start);
	utoms->start=indx;
	freeLL(&(utoms->finals));
	utoms->finals=NULL;
	if(v!=0)//Initial machine has at least one final state
	{ add(&utoms->finals,indx);	f[indx]=1; }
	}
}

void concatenate()
{//Concatenates two machines
	int k;
	
	if(ltoms==NULL||utoms==NULL)
	{ puts("Hey! INVALID R.E. due to more '.' operators"); getchar(); return; }
	
	struct node *temp1=utoms->finals,*temp2=ltoms->finals;
	
	while(temp2!=NULL)
	{
		k=read(temp2);
		add(&state[k][alpha],utoms->start);
		f[k]=0;
		temp2=temp2->next;
	}
	freeLL(&(ltoms->finals));
	ltoms->finals=utoms->finals;
	free(utoms);
	utoms=ltoms;
	ltoms=ltoms->nm;
}

void plus()
{//Create a new machine for Union of two machines
	int k;
	
	if(ltoms==NULL||utoms==NULL)
	{ puts("Hey! INVALID R.E. due to more '+' operators"); getchar(); return; }
	
	struct node *temp1=utoms->finals,*temp2=ltoms->finals;
	
	indx+=1;
	add(&state[indx][alpha],ltoms->start);
	add(&state[indx][alpha],utoms->start);
	sort(state[indx][alpha]);
	dupdel(state[indx][alpha]);
	dup(&ltoms->finals,utoms->finals);
	sort(ltoms->finals);
	dupdel(ltoms->finals);
	freeLL(&(utoms->finals));
	free(utoms);
	utoms=ltoms;
	utoms->start=indx;
	f[indx]=0;
	ltoms=ltoms->nm;
}

void fillstate()
{//Function responsible for filling state table of 1st-NFA
   int i,v,val;
   
   if(len==0){puts("nothing can be done with RE"); return ;}
   
   for(i=0;i<len;i++)
   { //Read the entire POSTFIX
   	v=post[i];
	if( (v>=48&&v<=57) || (v>=65&&v<=90)
	 || (v>=97&&v<=122) || v==-18/*EPSILON*/ || v==-19/*PHI*/ )
   	{
   		if(v>=48&&v<=57)  pushmachine(v-48);
   		if(v>=65&&v<=90)  pushmachine(v-55);
   		if(v>=97&&v<=122) pushmachine(v-61);
   		if(v==-18)		  pushmachine(-18);
   		if(v==-19)		  pushmachine(-19);
   	}
    else if(v=='*') kleene(i);
    else if(v=='.') concatenate();
    else if(v=='+') plus();
    else	
	{ printf("this Hey! INVALID R.E. due to ILLEGAL SYMBOL\n");
	  getchar();return; }
   }//Complete POSTFIX is processed
}

int s2char(int v)
{//Integer value to character converter
	if(v>=0&&v<=9)   return v+48-0;
	if(v>=10&&v<=35) return v+65-10;
	if(v>=36&&v<=61) return v+97-36;
}

int rad(int v)
{//Returns Biggest 10th place of v
	int temp=0;
	
	while(v!=0)
	{
		v=v/10;
		if(v>0)
		temp++;
	}
	return temp;
}

void replace(int src , int ind)
{//Replaces Src entry with Ind entry in Minimized DFA
	int i,j;
	for(i=0;i<=count;i++)
		for(j=0;j<alpha;j++)
	{
		if(dfa[i][j]==src)
		dfa[i][j]=ind;
	}
}

int main()
{
	int i,j,k,l,num,len=0;
    char more;
	
	puts("Welcome to RE 2 FA converter\n");
	puts("                   -Achint Chaudhary\n");
	
  //puts("If you want NFA also press y (it may take extra time), else any key");
	cmplt='n';//getchar()();
	
	printf("\nEnter the number of alphabets in language ");
	scanf("%d",&alpha);
	
	in_re();
	
	fillstate();
	
	states=indx+1;
	
	//printf("NFA Constructed with %d states\a\n\n",states);
	
	ermvr();
    /*State table of NFA without E-moves is stored in state2 now
	if(cmplt=='y')
	{
	   puts("\n\aPress any key to convert this NFA to DFA\n");
	   getchar()();
	}
	*/
	//Code for conversion to DFA (subset construction method)
	for(i=0;i<alpha;i++)
	  hash[0].al[i]=0;
	  c[0]=0;   //Index 0 is going to be the trap state\
					so all its entries points to itself
	add(&hash[1].next,utoms->start);
	if(f[utoms->start]==1){c[1]=1;}
	else c[1]=0;
	current=count=1;//Working begins from state 1, 0th index for trap state\
			current proceeds by filling the table completely\
			count indiactes the number of subsets in the hash table
	
	struct node *pointer,*subset;
	
	for(i=2;i<S;i++)c[i]=0; //Upcoming subsets non-final for DFA initially
	
	while(current<=count)
	{
	for(j=0;j<alpha;j++)
	 {
	   pointer=hash[current].next;//To read the states corresponding to\
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
	  }//After end of this loop all states reachable by alphabet 'j'\
	     corresponding to current state are in subset
	   	   	   
		 l=search(subset);
     	if(l==-1)//New subset creation
			{
				count+=1;//Index to be used for new subset
				l=count;
				dup(&hash[count].next,subset);
				sort(hash[count].next);
				if(final==1)
				 c[count]=1;	
			}
			
		hash[current].al[j]=l;
		freeLL(&subset);
	 }
	current+=1;;
	}
	
	
	/*
	puts("\aState table of DFA after subset construction\n");
	
	printf("DFA's State");
	for(i=0;i<radix;i++)
	{ printf(" "); }
	printf(" |");
	for(k=0;k<alpha;k++)
	{
		printf("'%c' ",s2char(k));
		for(i=0;i<radix;i++)
		{ printf(" "); }
		printf("|");
	}
	puts("Corresponding Subset from NFA");
	
	len=42+radix+alpha*(5+radix);
	for(i=0;i<len;i++)printf("-");
	puts("");
	
	for(k=0;k<=count;k++)
      {
	    printf("   [C%d]",k);
	    if(c[k]==1)
		{	printf("*  ");
			for(i=0;i<( radix-rad(k) );i++)
			{ printf(" "); }
			printf(": | ");
		}
	    else
		{
			printf("   ");
			for(i=0;i<( radix-rad(k) );i++)
			{ printf(" "); }
			printf(": | ");
		}
		
	   for(l=0;l<alpha;l++)
         {
		  for(i=0;i<( radix-rad( hash[k].al[l] ) );i++)
		  { printf(" "); }
		  printf("C%d | ",hash[k].al[l]);
		 }
	    print(hash[k].next);
	  }
	
	puts("\nDo you want to minimize this DFA\n");*/
	more='y';//getchar()();
	
	if(more=='y')
	{
		i=1;
		for(k=0;k<=count;k++)
		  for(l=0;l<alpha;l++)
		    dfa[k][l]=hash[k].al[l];//Copying from hash table to dfa table
		    	    
		for(k=1;k<=count;k++)
		  { for(l=0;l<alpha;l++)
		   	if(dfa[k][l]==0)
		     { i=0; break;}
		  }//i=0 if trap state is reachable
		
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
    	    {if(tab[j][i]==0)//For empty blocks only
    	     {if(check(i,j,alpha)==1)//If get to a filled block
    	       { tab[j][i]=pass; chk=1;} } } }//Fill that block by pass
	   		pass+=1;//Increment pass as it becomes complete
	  	  }while(chk==1);
	  
	  int max[count],temp=3;
	  
	  for(i=0;i<count;i++)max[i]=0;
	  
	  for(j=0;j<count;j++)
	  {
	  	for(i=j+1;i<=count;i++)
	  	if(tab[i][j]>=max[j])
	  	max[j]=tab[i][j];
	  }
	  
	  /*puts("---------------------------------------------------------\n");
      
	  printf("%d passes occur in minimization\n",pass-1);
       
	   if(cmplt=='y')
	   {
	   puts("\nFinal computed table is\n");      
      for(i=1;i<=count;i++)//Final computed table
    	  { 
		  	temp=radix+4;
			for(j=0;j<i;j++)
				temp+=rad(max[j])+rad(j)+4;
			for(k=0;k<rad(count)+4;k++)printf(" ");
			for(k=rad(count)+4;k<=temp;k++)printf("-");
			puts("");
			printf("[%d] ",i);
    	    for(k=0;k<(radix-rad(i));k++){printf(" ");}
    	    printf("|");
			for(j=0;j<i;j++)
		      {
			  	for(k=0;k<rad(max[j])-rad(tab[i][j])+rad(j);k++)
				  {printf(" ");}
				  if(tab[i][j]!=0)
				  printf(" %d |",tab[i][j]);
				  else
				  printf("   |");
			  }
          	puts("");
		  }
		  for(k=0;k<rad(i-1)+4;k++)printf(" ");
			for(k=rad(i-1)+4;k<=temp;k++)printf("-");
		  puts("");
          printf("     ");
          for(k=0;k<radix;k++)printf(" ");
          for(j=0;j<count;j++)
		      { 
			  	for(k=0;k<(rad(max[j]));k++){printf(" ");}
				  printf("[%d] ",j);
		      }
	   
	   puts("\n\n---------------------------------------------------------");
	   }*/
      for(i=0;i<=count;i++)r[i]=1;
	  
	  for(i=0;i<count;i++)//Modifying same state table to minimize it
      for(j=i+1;j<=count;j++)
       {if(r[i]==1)
       	{if(tab[j][i]==0)
       	 {  r[j]=0;//Make that state unreadable
       	    r_modify=1;
       		for(k=0;k<=count;k++)
       		  for(l=0;l<alpha;l++)
       		   { if(dfa[k][l]==j)
       		      dfa[k][l]=i;//Modify higher states of same block
			     if(c[j]==1)    //With the smallest state
			      c[i]==1;}
       	 } 
		}
       	 else break;
	   }
	/*
       	if( r_modify==0 && trap==0 )
	   { 
	   	 puts("\nMachine is already minimized");
	     puts("\nC0 is trap state while C1 is starting state"); 
	     puts("\nThanks for using my code"); getchar()(); return 0;
	   }
    */
	//puts("\nFinal State table of Minimized DFA is\n");
	
	int ind=0;
	for(i=0;i<=count;i++)
		{//Changing Entries of DFA so that it appears to be Minimized
			if(r[i]==1)
			{
			replace(i,ind);
			ind++;
			}
		}
	j=ind;
	ind=0;
	/*	printf("DFA's State");
	for(i=0;i<radix;i++)
	{ printf(" "); }
	printf("|");
	for(k=0;k<alpha;k++)
	{
		printf("'%c' ",s2char(k));
		for(i=0;i<radix;i++){printf(" ");}
		printf("|");
	}
	puts("");
	
	len=12+radix+alpha*(5+radix);
	for(i=0;i<len;i++)printf("-");
	puts("");
    */
    printf("%d %d\n",j,alpha);
	for(k=0;k<=count;k++)
      {if(r[k]==1) 
	   {printf("C%d",ind);
	    if(c[k]==1)
			printf("*:");
	    else
			printf(" :");
	   for(l=0;l<alpha;l++) 
		    printf(" %d",dfa[k][l]);
       ++ind;
	   }
       if(r[k]==1)puts("");
	  }
	   		
	}//Minimization ends here
	   getchar();
	   return 0;
}
