/*
				VISVSESVARAYA NATIONAL INSTITUTE OF TECHNOLOGY,NAGPUR
					COMPUTER SCIENCE AND ENGINEERING DEPARTMENT
						CONCEPTS IN PROGRAMMING LANGUAGES
							ASSIGNMENT-2(HEAP MANAGEMENT)
						CODE PREPARED BY:ABDUL SATTAR MAPARA
										 BT16CSE053
							   GUIDED BY:RESPECTED U.A.DESHPANDE SIR
										 HoD
										 CSE
										 VNIT,NAGPUR
						DT.10th March,2018 and 11th March,2018

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#define HEAP_SIMULATION_SIZE 1000	//in BYTES
#define NAME_SIZE 10
#define MAX_MALLOC_CALLS 100
#define MALLOC 1
#define FREE 2
#define DISPLAY 3
#define EXIT 0
typedef enum{FAILURE,SUCCESS} Status;
typedef enum{FALSE,TRUE} Bool;

//ALLOCATION LIST NODE STORES START ADDRESS AND END ADDRESS
typedef struct AllocatedNodeTag
{
	void* start;
	void* end;
	struct AllocatedNodeTag* next;
}AllocatedNode;
//FREE LIST NODE STORES START ADDRESS AS WELL AS SIZE WHICH IS FREE
typedef struct FreeNodeTag
{
	void* start;
	size_t size;
	struct FreeNodeTag* next;
}FreeNode;

AllocatedNode* MakeAllocatedNode(void* start,void* end)
{
	//CREATES A NODE OF TYPE ALLOCATEDNODE AND RETURNS IT AFTER SETTING UP PROPER VALUES
	AllocatedNode* ptr=(AllocatedNode*)malloc(sizeof(AllocatedNode));
	if(ptr == NULL)
	{
		printf("ORIGINAL (SYSTEM) HEAP FULL ! SYSTEM CAN'T ALLOCATE MEMORY\n");
	}
	else
	{
		ptr->next=NULL;
		ptr->start=start;
		ptr->end=end;
	}
	return ptr;
}
FreeNode* MakeFreeNode(void* start,size_t size)
{
	/*
		CREATES A FREE LIST NODE SETS VALUES AND RETURNS IT
	*/
	FreeNode* ptr=(FreeNode*)malloc(sizeof(FreeNode));
	if(ptr != NULL)
	{
		ptr->start=start;
		ptr->size=size;
		ptr->next=NULL;
	}
	return ptr;
}
void display_mem(AllocatedNode* head_allocated,FreeNode* head_free)
{
	printf("\nALLOCATED MEMORY\n");
	AllocatedNode*ptr=head_allocated;
	FreeNode* fptr=head_free;
	while(ptr != NULL)
	{
		printf("START:%"PRIuPTR,(uintptr_t) ptr->start);
		printf("\tEND:%"PRIuPTR,(uintptr_t)ptr->end);
		printf("\n");
		ptr=ptr->next;
	}
	printf("\nFREE MEMORY\n");
	while(fptr != NULL)
	{
		printf("START:%"PRIuPTR,(uintptr_t)fptr->start);
		printf("\tSIZE:%u\n",fptr->size);
		fptr=fptr->next;
	}
	return;
}
void AddToAllocatedList(AllocatedNode** head_allocated,void* start,void* end)
{	
	//UPDATES THE ALLOCATED LIST
	AllocatedNode* ptr=MakeAllocatedNode(start,end);
	if(ptr != NULL)
	{
		if(*head_allocated == NULL)
		{
			*head_allocated=ptr;
		}
		else
		{
			ptr->next=*head_allocated;
			*head_allocated=ptr;//INSERT AT START
		}
	}
	else
	{
		printf("SYSTEM (ORIGINAL)MEMORY FULL\n");
	}
	
	return;
}
void* alloc_mem(size_t size,AllocatedNode** head_allocated,FreeNode** head_free)
{
	void* retval=NULL;
	Bool found=FALSE;
	FreeNode* prev=NULL;
	FreeNode* temp;
	FreeNode* trv=*head_free;
	if(size!=0)
	{
		while( trv != NULL && !found)
		{
			if(trv->size >= size)			//FIRST FIT ALGORITHM
			{

				retval=trv->start;
				found=TRUE;
				trv->size=trv->size-size;
				if(trv->size <= 3)
				{
					size+=trv->size;
					trv->size=0;
					if(prev == NULL)
					{
						*head_free=trv->next;
					}
					else
					{
						prev->next=trv->next;
					}
					free(trv);
					trv=NULL;
				}
				if(trv != NULL)
				{
					trv->start=trv->start+size;
				}
				AddToAllocatedList(head_allocated,retval,retval+size-1);

			}
			else
			{
				prev=trv;
				trv=trv->next;
			}
		}
	}
	else
	{
		printf("OOPS! YOU ASKED FOR 0 BYTE i.e you didn't ask any byte.So we return you NULL\n");
	}
	return retval;
}
void AddToFreeList(void* address,size_t size,FreeNode** head_free)
{
	FreeNode* fptr=*head_free;
	FreeNode* prev=NULL;
	FreeNode* newNode=NULL;
	FreeNode* newCompact=NULL;
	FreeNode* q;
	while(fptr!=NULL && (fptr->start + fptr->size) < address)
	{
		prev=fptr;
		fptr=fptr->next;
	}
	if(fptr == NULL)
	{
		//either all fptr->start+size < address and hence insert after prev w/o compaction
		//or linked list is empty(i.e prev is also NULL)
		newNode=MakeFreeNode(address,size);
		if(prev == NULL)
		{
			*head_free=newNode;
		}
		else
		{
			//insert after prev without compaction
			prev->next=newNode;
		}
	}
	else	//fptr not NULL but prev may be NULL--if prev==NULL insert at start and check for compaction on front side only
	{		//if prev not NULL insert after prev and check for compaction on both sides
		if(prev==NULL)
		{
			if(address+size==fptr->start)
			{
				fptr->start=address;
				fptr->size=fptr->size+size;
			}
			else if(fptr->size+fptr->start==address)
			{
				fptr->size+=size;
				if(fptr->next!=NULL && fptr->start+fptr->size==(fptr->next)->start)
				{
					fptr->size+=(fptr->next)->size;
					q=fptr->next;
					fptr->next=(fptr->next)->next;
					free(q);
					q=NULL;
				}
			}
			else
			{
				newNode=MakeFreeNode(address,size);
				newNode->next=fptr;
			   *head_free=newNode;
			}
		}
		else
		{
					if(fptr->start+fptr->size == address)
					{
						fptr->size+=size;
						if(fptr->next!=NULL && fptr->start+fptr->size==(fptr->next)->start)
						{
							fptr->size+=(fptr->next)->size;
							q=fptr->next;
							fptr->next=(fptr->next)->next;
							free(q);
							q=NULL;
						}
					}
					else
					{
						if(prev->start+prev->size == address)
						{
							//UNDERGOING COMPACTION
							prev->size+=size;
						}
						
						else
						{
							//COMAPCTION FROM BACK NOT POSSIBLE
							newNode=MakeFreeNode(address,size);
							prev->next=newNode;
							newNode->next=fptr;
							prev=newNode;//prev means prev to fptr
						}
						if(prev->start+prev->size==fptr->start)
						{
							//CHECKING COMPACTION AT FRONT
							prev->size+=fptr->size;
							prev->next=fptr->next;
							free(fptr);
							fptr=NULL;
						}
					}
		}

	}
	return;
}
void free_mem(void* address,AllocatedNode** head_allocated,FreeNode** head_free)
{
	AllocatedNode* ptr=*head_allocated;
	AllocatedNode* prev=NULL;
	AllocatedNode* del=NULL;
	size_t size=0;
					
	while(ptr!= NULL && ptr->start != address)
	{
		prev=ptr;
		ptr=ptr->next;
	}
	if(ptr == NULL)
	{
		printf("ERROR CONDITION!ADDRESS TO BE FREED NOT FOUND\n");
	}
	else
	{
		if(prev == NULL)
		{
			*head_allocated=ptr->next;
		}
		else
		{
			prev->next=ptr->next;
		}
		size=ptr->end-ptr->start+1;
		free(ptr);
		ptr=NULL;
		AddToFreeList(address,size,head_free);
		printf("MEMORY FREED SUCCESSFULLY\n");
	}
	return;
}
void Simulate(AllocatedNode* head_allocated,FreeNode* head_free,void* heap)
{
	int choice;
	int i,j,free_choice;
	Status sc;
	Bool done;
	done=FALSE;
	size_t size;
	void** ptr[MAX_MALLOC_CALLS];
	for(i=0;i<MAX_MALLOC_CALLS;i++)
	{
		ptr[i]=NULL;

	}
	printf("MENU\n1]MALLOC\n2]FREE\n3]DISPLAY\n0]EXIT\n");
	i=0;
	while(!done && i<MAX_MALLOC_CALLS)
	{	

		printf("ENTER YOUR CHOICE\n");
		scanf("%d",&choice);	
		switch(choice)
		{
			case MALLOC:
						printf("YOU HAVE CHOSEN MALLOC\n");
						printf("ENTER THE SIZE IN BYTES\n");
						scanf("%u",&size);
						
							ptr[i]=alloc_mem(size,&head_allocated,&head_free);//similar to malloc
							if(ptr[i] != NULL)
							{
								printf("MALLOC SUCCESSFUL\nADDRESS ASSIGNED:%"PRIuPTR,(uintptr_t)ptr[i]);
								printf("\n");
								i++;
							}
							else
							{
								printf("MALLOC FAILED!POSSIBLE CAUSE(s) OF FAILURE:\n1)HEAP RAN OUT OF MEMORY\n2)YOU ASKED FOR NON-POSITIVE BYTES OF MEMORY");
							}
						
						break;
			case FREE:
						printf("YOU HAVE OPTED TO FREE SOME PART OF THE MEMORY\nFIRST WE SHOW YOU ALL THE POINTERS THAT HAVE BEEN GIVEN VALID ADDRESSES FROM THE HEAP.FREE ONE OF THEM\n");

						printf("CHOICE\t\tADDRESSES\n");
						for(j=0;j<i;j++)
						{
																if(ptr[j]!=NULL)
							printf("%d\t\t%"PRIuPTR,j+1,(uintptr_t)ptr[j]);
							printf("\n");
						}
						printf("YOUR CHOICE\n");
						scanf("%d",&free_choice);
						free_choice--;
						while(free_choice>=i || free_choice < 0)
						{
							printf("ENTER A VALID CHOICE\n");
							scanf("%d",&free_choice);
							free_choice--;
						}
						free_mem(ptr[free_choice],&head_allocated,&head_free);
												ptr[free_choice]=NULL;
						break;
			case DISPLAY:
						printf("YOU HAVE OPTED TO VIEW THE CURRENT SITUATION OF THE HEAP\n\n");
						display_mem(head_allocated,head_free);
						break;
			case EXIT:
						done=TRUE;
						break;
			default:
						printf("ENTER A VALID CHOICE\n");
						break;
		}
	}
	return;
}
void driver()
{
	void* heap=malloc(HEAP_SIMULATION_SIZE);
	int i;
	FreeNode* head_free=NULL;
	AllocatedNode* head_allocated=NULL;
	head_free=MakeFreeNode(heap,HEAP_SIMULATION_SIZE);
	head_free->next=NULL;
	Simulate(head_allocated,head_free,heap);
	printf("THANK YOU FOR USING THE PROGRAM\nPLAESE PUT DOWN YOUR COMMENTS/SUGGESTIONS ABOUT THIS CODE at www.codeingzone.in");
	return;
}
int main()
{
	driver();
	return 0;
}
/*
	IF YOU FIND ANYTHING INCORRECT PLAESE REPORT TO THE AUTHOR OF THIS CODE.
*/
