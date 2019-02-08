#include<stdio.h>//Tabulation method code for DFA minimization
#define S 10000  //Programmer-"Achint Chaudhary"
#define A 100
int tab[S][S],state[S][A];

int check(int i, int j,int alp)
{//This function is used to check for each empty block in Table
	int k,a,b,t;
	for(k=0;k<alp;k++)
	{   a=state[i][k];
		b=state[j][k];
		if(a<b)
		{t=a;a=b;b=t;}
		if(tab[a][b]!=0)
		{return 1;} }//for loop ends
	return 0;//if no such alphabet exist pair remains empty (0)
}

main()
{   
    int i,j,k,l,states,alpha,pass=2,chk,r_modify=0;
	puts("Welcome to DFA minimization program by Tabulation method\n");
	puts("                -Achint Chaudhary");
	getch();
	puts("Enter the number of states in DFA");
	scanf("%d",&states);
	int f[states],r[states];
	puts("Enter the number of alphabets in language");
	scanf("%d",&alpha);
	
	puts("Enter the enteries in the state table of DFA");
	for(i=0;i<states;i++)
    	for(j=0;j<alpha;j++)
        	{ scanf("%d",&state[i][j]); r[i]=1; }
    
	puts("Mark each state as final(1) of non-final(0)");
	for(i=0;i<states;i++) scanf("%d",&f[i]);
	
	for(i=0;i<(states-1);i++)//First pass (final/non-final)
    	for(j=i+1;j<states;j++)
        	if((f[i]+f[j])==1)
        	 tab[j][i]=1;
    
	do{//Further passes in this do-while loop
    	chk=0;
    	for(i=0;i<(states-1);i++)
    	  {for(j=i+1;j<states;j++)
    	    {if(tab[j][i]==0)//for empty blocks only
    	    {if(check(i,j,alpha)==1)//if get to a filled block
    	    { tab[j][i]=pass; chk=1;}}}}//fill that block by pass
	   pass+=1;//increment pass as it becomes complete
	  }while(chk==1);
	  
	  puts("---------------------------------------------------------");
      
	  printf("%d passes occur in minimization\n",pass-1);
       puts("\nFinal computed table is");
      for(i=1;i<states;i++)//Final computed table
    	  { printf("[%d] ",i);
		    for(j=0;j<i;j++)
		      printf(" %d ",tab[i][j]);
          puts("");}
          printf("    ");
          for(j=0;j<(states-1);j++)
		      printf("[%d]",j);
	  
	   puts("\n---------------------------------------------------------");
      for(i=0;i<(states-1);i++)//modifying state table to minimize it
      for(j=i+1;j<states;j++)
       {if(r[i]==1)
       	{if(tab[j][i]==0)
       	 {  r[j]=0;//make that state unreadable
       	    r_modify=1;
       		for(k=0;k<states;k++)
       		  for(l=0;l<alpha;l++)
       		   { if(state[k][l]==j)
       		      state[k][l]=i;//modify higher states of same block
			     if(f[j]==1)    //with the smallest state
			      f[i]==1; }   //if any large is final,block is final	   
       	 } }
       	 else break; }
       	 
       	 if(r_modify==0)
	   { puts("\nMachine is already minimized"); 
	     puts("\nThanks for using my code"); getch(); return 0; }
    
	puts("\nFinal State table of Minimized DFA is");
    for(k=0;k<states;k++)
      {if(r[k]==1) 
	   {printf("[S%d]",k);
	    if(f[k]==1)printf("* : | ");
	    else printf("  : | ");
	   for(l=0;l<alpha;l++)
          printf("%d | ",state[k][l]);
       }
       if(r[k]==1)puts("");
	  }
	   puts("\nThanks for using my code");
	   getch();
}
