/*
					CONCEPTS IN PROGRAMMING LANGUAGES
				 				ASSIGNMENT 1
					--REPRESENTATION OF 1024-BIT NUMBER
					--PERFORMING ADDITION,SUBTRACTION,MULTIPLICATION OF THE NUMBERs
CODE PREPARED and SUBMITTED BY:	
					ABDUL SATTAR MAPARA
					BT16CSE053
					Computer Science and Engineering Department,
					VISVESVARAYA NATIONAL INSTITUTE of TECHNOLOGY,
					NAGPUR
GUIDED BY:			
					RESPECTED U.A.DESHPANDE SIR
					Head of the Department
					VISVESVARAYA NATIONAL INSTITUTE of TECHNOLOGY,
					NAGPUR
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ADD 1			//SWITCH CASE ADD
#define SUBTRACT 2		//SWITCH CASE SUBTRACT
#define MULTIPLY 3		//SWITCH CASE MULTIPLY
#define INPUT_SIZE 309	//1024 bits = 309 digits
typedef struct NodeTag
{
	//structure for node of linked list
	int data;
	struct NodeTag* next;
}node;					
node* MakeNode(int data)
{
	//creates a node with input value inside=data
	node* nptr=(node* )malloc(sizeof(node));
	if(nptr!=NULL)
	{
		nptr->data=data;
		nptr->next=NULL;
	}
	return nptr;
}						
node* InsertAtEnd(node* tail,int data,node** head)
{
	//CREATES a node AND PUTS IN data in its data field,PLACES the node at end of the linked list Returns tail to the caling function to help subsequent calls
	node* ptr,*lptr;
	ptr=MakeNode(data);
	if(*head==NULL)
	{
		*head=ptr;
		(*head)->next=NULL;
		tail=*head;
	}
	else
	{
		
		tail->next=ptr;
		tail=ptr;
		tail->next=NULL;
	}
	return tail;
}
node* ConvertToNum(char* str,int* len)
{
	//Takes in character string and convert to number representation in linked list.Also finds length of the string.Returns head pointer to the linked list representing the number
	//LSB at starting node and hence MSB at the end
	int i;
	*len=strlen(str);
	i=(*len)-1;
	node* tail=NULL,*head=NULL;
	while(i>=0)
	{
		tail=InsertAtEnd(tail,(str[i]-'0'),&head);
		i--;
		
	}
	return head;
}

node* add(node* lptr1,node* lptr2)
{
	//adds the numbers in the two linked lists
	node* ret=NULL,*tail=NULL,*p,*q;//ret is head
	p=lptr1;
	q=lptr2;
	int sum=0;
	while(p!=NULL&&q!=NULL)
	{
		sum=p->data+q->data+sum/10;
		//sum/10 for carry from previous addition
		tail=InsertAtEnd(tail,sum%10,&ret);
		p=p->next;
		q=q->next;
	}
	if(sum/10&&p==NULL&&q==NULL)
	{
		
		tail=InsertAtEnd(tail,sum/10,&ret);
	}
	while(p!=NULL)
	{
		sum=p->data+sum/10;
		tail=InsertAtEnd(tail,sum,&ret);
		p=p->next;
	}

	while(q!=NULL)
	{
		sum=q->data+sum/10;
		tail=InsertAtEnd(tail,sum,&ret);
		q=q->next;
	}
	return ret;
}
node* subtractNum(node* lptr1,node* lptr2)
{
	//subtraction function
	//lptr1-lptr2
	//it assumes that number in lptr1 is greater than lptr2
	node* p=lptr1;
	node* q=lptr2;
	node* tail=NULL,*head=NULL;
	int carry,subResult;
	if(p==NULL||q==NULL)
	{
		printf("ERROR\n");
	}
	else
	{
		carry=0;
		do
		{

			subResult=(p->data-q->data)+carry;
			carry=0;
			if(subResult<0)
			{
				subResult+=10;
				carry=-1;
			}
			tail=InsertAtEnd(tail,subResult,&head);
			p=p->next;
			q=q->next;
		}while(q!=NULL);
		while(p!=NULL)
		{
			subResult=p->data+carry;
			carry=0;
			if(subResult<0)
			{
				subResult+=10;
				carry=-1;
			}
			tail=InsertAtEnd(tail,subResult,&head);

			p=p->next;
		}
	}
	return head;
}
node* multiply(node* lptr1,node* lptr2)
{
	//multiplies the two numbers represented by lptr1 and lptr2
	node* p,*q;
	int numzero=0,i;
	node* head=NULL,*tail,*tail2,*head2;
	head2=MakeNode(0);
	tail2=head2;
	int mult,carry=0;
	for(p=lptr1;p!=NULL;p=p->next)
	{
		mult=0;
		carry=0;
		tail=head=NULL;
		for(i=0;i<numzero;i++)
		{
			tail=InsertAtEnd(tail,0,&head);
		}
		numzero++;
		for(q=lptr2;q!=NULL;q=q->next)
		{
			
		
			mult=(p->data)*(q->data)+carry;
			if(mult/10>0)
			{
				carry=mult/10;
			}
			else
			{
				carry=0;
			}
			//printf("%d\n",mult);
			tail=InsertAtEnd(tail,mult%10,&head);
		}
		tail=InsertAtEnd(tail,carry,&head);
		head2=add(head,head2);		
	}
	return head2;
}
void PrintNum(node* head)
{
	//takes in head pointer to the linked list and prints the number represented by the linked list
	node* h,*t,*mid,*trv;//t=tail h=head mid=middle trv=traverser
	h=head;
	mid=NULL;
	while(h!=NULL)
	{
		t=mid;
		mid=h;
		h=h->next;
		mid->next=t;

	}
	trv=mid;
	while(trv!=NULL)
	{
		printf("%d",trv->data);
		trv=trv->next;
	}
	return;
}
int CheckNeg(char str[])
{
	//check if the string holds negative number or not
	//if yes returns 1
	//else returns 0
	int retval=0;
	if(str[0]=='-')
	{
		retval=1;
	}
	return retval;
}
void Shift(char str[])
{
	//removes"-"sign if present
	int i;
	for(i=0;str[i]!='\0';i++)
	{
		str[i]=str[i+1];
	}
	return;
}
void CompareNum(char str1[],char str2[],int* neg1,int* neg2,int* swap)
{
	//compares the two numbers represented by string1 and string2(magnitude wise only) and makes sure that str1 contains greater number than in str2 wrt  magnitude
	//also finds if swapping took place or not(using swap)
	//finds if the number is -ve or not(using neg1/neg2)
	
	*swap=0;
	int tempVar;
	(*neg1)=CheckNeg(str1);
	(*neg2)=CheckNeg(str2);
	if((*neg1))
	{
		Shift(str1);
	}
	if((*neg2))
	{
		Shift(str2);
	}
	int len1=strlen(str1),len2=strlen(str2);
	int flag=0,j,i;			//flag used to decide if num1>=num2 flag=1 else flag=-1
	j=0,i=0;
	char temp;
	while(!flag)
	{
		if(len1>len2)
		{
			flag=1;
		}
		else if(len2>len1)
		{
			flag=-1;
		}
		else
		{
			if(str1[i]>str2[j])
			{
				flag=1;
			}
			else if(str1[i]<str2[j])
			{
				flag=-1;
			}
			else
			{
				while(i<len1&&j<len2&&str1[i]==str2[j])
				{
					i++;
					j++;
				}
				if(i==len1&&j==len2)
				{
					flag=1;
				}

			}
			
		}
	}

	if(flag==-1)
	{
		*swap=1;
		tempVar=(*neg1);
		(*neg1)=(*neg2);
		(*neg2)=tempVar;
		//num2 is greater than num1
		for(i=0;str1[i]!='\0';i++)
		{
			temp=str2[i];
			str2[i]=str1[i];
			str1[i]=temp;
		}
		j=i;
		while(str2[i]!='\0')
		{
			str1[i]=str2[i];
			i++;
		}
		str1[i]='\0';
		str2[j]='\0';
		
	}
	else
	{
		;
	}
	//printf("%s\n%d\n",str1,*neg1);
	//printf("%s\n%d\n",str2,*neg2);
	return;
}
int main()
{
	//Driver function begins
	int len1,len2,swap=0;
	int neg1,neg2,great1;
	neg1=0,neg2=0;
	char numStr1[INPUT_SIZE],numStr2[INPUT_SIZE];
	node* lptr1,*lptr2,*addRes,*subRes,*multRes;
	printf("ENTER TWO NUMBERS\n");
	scanf("%s",numStr1);
	scanf("%s",numStr2);
	CompareNum(numStr1,numStr2,&neg1,&neg2,&swap);
	lptr1=ConvertToNum(numStr1,&len1);
	lptr2=ConvertToNum(numStr2,&len2);
//magnitude wise lptr1>=lptr2
	int choose;
	printf("\tMENU\n1) ADD THE TWO NUMBERS\n2) SUBTRACT THE TWO NUMBERS\n3) MULTIPLY THE TWO NUMBERS\n");
	scanf("%d",&choose);
	switch(choose)
	{
		case ADD:
			printf("ADDITION IS\t");
			if(neg1)
			{
				printf("-");
			}
			if(neg1==neg2)
			{
				addRes=add(lptr1,lptr2);
			}
			else
			{
				addRes=subtractNum(lptr1,lptr2);
			}
			
			PrintNum(addRes);
			
			break;
		case SUBTRACT:
			printf("SUBTRACTION IS\t");
			if(neg1!=neg2)
			{
				subRes=add(lptr1,lptr2);
				if(swap)
				{
					if(neg1==0)
					{
						printf("-");
					}
				}
				else
				{
					if(neg2==0)
					{
						printf("-");
					}
				}
			}
			else if(neg1==0&&neg2==0)
			{
				if(swap)
				{
					printf("-");
				}
				subRes=subtractNum(lptr1,lptr2);
			}
			else //else if(neg1==1 && neg2==1)
			{	
				if(!swap)
				{
					printf("-");
				}
				subRes=subtractNum(lptr1,lptr2);
			}
			PrintNum(subRes);
			break;
		case MULTIPLY:
			printf("MULTIPLICATION IS\t");
			if(neg1!=neg2)
			{
				printf("-");
			}
			multRes=multiply(lptr1,lptr2);
			PrintNum(multRes);
			break;
		default:
			printf("SORRY! YOU ENTERED A CHOICE NOT AVAILABLE.TRY TO RERUN THE PROGRAM\n");
	}
	
	return 0;

}
