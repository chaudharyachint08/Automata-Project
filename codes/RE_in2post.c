#include<stdio.h>// C Program to convert given INFIX regular expression
#define L 10000  // '\n' = 13; EPSILON==-18; PHI==-19;  BACK==8
                //to its POSTFIX form for evaluation
int prcdnc(char c)
{
	if(c=='*')          return 3;
	if(c=='.')          return 2;
	if(c=='+')          return 1;
	if(c=='(')          return 0;
}

in_re()
{
	int len,s,i,j,k,tos=-1;
	char c,in[L],post[L],opstk[L];
	puts("Welcome to IN 2 POST FIX Converter for Regular Expressions\n");
	puts("                   -Achint Chaudhary");
	getch();
	puts("Use '+' for Union, '.' for Concatenation & '*' for Kleene Closure\n");
	puts("Enter INFIX Regular Expression");
	s=len=0;
	fflush(stdin);
	while(13!=(c=getch())) 
	{
		if(c==8)//backspace is pressed by user
		{
			if(len!=0)
			len--;
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
		putch(c);
		s=len+=1;
	}
	puts("\n\nInput Regular Expression is");
	for(i=0;i<len;i++)
	printf("%c",in[i]);
	puts("");
	
    for(i=0,j=0;i<len;i++)
    {
    	c=in[i];
		if( (c>=48&&c<=57) || (c>=65&&c<=90) || (c>=97&&c<=122) || c==-18/*EPSILON*/ || c==-19/*PHI*/ )
		{//is a character b/w 0-9, A-Z, a-z
		 //then directly add to POSTFIX  String
			post[j]=c;printf("%c to POSTFIX %d Directly\n",c,j);
			j=j+1;
		}
		else if(c==')')// ')' is encountered
		{
			while( opstk[tos]!='(' && tos!=-1 )
			{//post entrie stack until ( is found
				post[j]=opstk[tos];printf("%c to POSTFIX %d due to ')' \n",c,j);
				tos=tos-1;
				j+=1;
			}
			if(tos!=-1){ tos=tos-1; }
		}
		else if(c=='(')
		{//left parenthesis is encountered
			if(tos==-1)//if operator stack is empty
		  { tos+=1; opstk[tos]=c;printf("%c to EMPTY stack\n",c); }
		  else
		  {//opearator stack is not empty
		   tos+=1;
		   opstk[tos]=c;printf("( to OP_STACK      %d\n",tos);
		  }
	    }
		else
		{//is an operator 
		  if(tos==-1)//if operator stack is empty
		  { tos+=1; opstk[tos]=c;printf("%c to EMPTY stack\n",c); }
		  else
		  {//opearator stack is not empty
			k=prcdnc(c);
			while(k<=prcdnc(opstk[tos])&&tos!=-1)
			{
					//printf("%c at %d index\n",c,i);
					post[j]=opstk[tos];printf("%c to POSTFIX %d due %c\n",opstk[tos],j,c);
					tos=tos-1;
					j+=1;
			}
			tos+=1;
			opstk[tos]=c;
		  }	
		}
    }//infix readed here completely
    if(tos!=-1)
    puts("\nINFIX is readed completely, now pop entire OP_STACK\n");
    	while(tos!=-1)
    	{ 
		   post[j]=opstk[tos];printf("%c to POSTFIX index %d\n",opstk[tos],j);
		   j+=1;
		   tos--; 
		}
	  puts("\nPOSTFIX regular expression is");
    puts("\t");
    len=j;//j denotes new length of postfix expression as ( & ) are all removed
	for(i=0;i<len;i++)
	printf("%c",post[i]);
	puts("\n\nThanks for Using my code");
	getch();

}
main()
{
	in_re();
}
