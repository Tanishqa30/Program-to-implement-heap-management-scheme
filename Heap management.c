#include<stdio.h>
#include<stdlib.h>

#define SIZE_OF_HEAP 100000

typedef struct ID_heap
{
    int block_size;
    int ID;
    int status;
    struct ID_heap *after;
    struct ID_heap *before;
}Heap;

Heap* init(Heap* p)
{
    p->block_size = 4096;
    p->ID = -1;
    p->status = 0;
    p->after = NULL;
    p->before = NULL;

    return p;
}
Heap* ptr_heap; 

//displaying all the actions and current size of the block
void show()
{
    Heap* ptr_curr = ptr_heap;
    printf("\nDisplaying the heap allocations as follows:\n");
    while(ptr_curr!=NULL)
    {
        printf("Size status = %d\t", ptr_curr->block_size);
        ptr_curr = ptr_curr->after;
    }

    printf("\n\n");
}

//merging consecutive free blocks into one block
void merge_adjacent()
{
    Heap* ptr_temp = NULL;
    int final_size;                       
    Heap* ptr_curr = ptr_heap;               

    while(ptr_curr!=NULL)
    {
        if(ptr_curr->status==0)
        {
            final_size = ptr_curr->block_size;
            ptr_temp = ptr_curr->after;

            while(ptr_temp!=NULL && ptr_temp->status==0)
            {
                final_size += ptr_temp->block_size;
                ptr_temp = ptr_temp->after;
            }

            ptr_curr->block_size = final_size;
            ptr_curr->after = ptr_temp;

            if(ptr_temp != NULL)
            {
                ptr_temp->before = ptr_curr;
            }

            ptr_curr = ptr_temp;
        }
        else
        {
            ptr_curr = ptr_curr->after;
        }
    }
}

//removal of blocks
void free_memory(int block_id)
{
    Heap* free_ptr = NULL;
    int avail = 0;
    Heap* ptr_curr = ptr_heap;

    while(ptr_curr != NULL && avail==0)
    {
        if(ptr_curr->ID == block_id)
        {
            avail = 1;
            free_ptr = ptr_curr;
            free_ptr->ID = -1;
            free_ptr->status = 0;
        }

        ptr_curr = ptr_curr->after;
    }

    if(avail==1)
    {
        printf("\nMemory block of size: %d and ID: %d has been freed\n", free_ptr->block_size, block_id);
    }
    else
    {
        printf("\nBlock of ID:  %d not found\n", block_id);
    }
}


//allocation of blocks
void allocate_memory(int space)
{
    Heap* next_mem;
    static int ID = 0;
    int reserve_mem = 0, avail = 1;
    Heap* ptr_curr = ptr_heap;

    if(ptr_curr->block_size<=space || ptr_curr->status==1)
    {
        avail = 0;
        while(ptr_curr->after!=NULL && avail==0)
        {
            ptr_curr = ptr_curr->after;

            if(ptr_curr->block_size>=space  && ptr_curr->status==0)
            {
                avail = 1;
            }
        }
    }

    if(avail==1)
    {
        reserve_mem = ptr_curr->block_size;

        ptr_curr->ID = ID;
        ptr_curr->status = 1;
        ptr_curr->block_size = space;

        next_mem = ptr_curr + space;
        next_mem = init(next_mem);
        next_mem->block_size = reserve_mem - space;

        next_mem->after = ptr_curr->after;
        ptr_curr->after = next_mem;
        next_mem->before = ptr_curr;

        printf("Size of block created = %d\n ID of block = %d\n", ptr_curr->block_size, ptr_curr->ID);

        ID++;
    }
    else
    {
        printf("The block size you have entered exeeds the maximum available space. Please try again.");
    }
}


//displaying all possible choices in front of the user
void choices()
{
    int ch;
    int enter_block;

    printf("Choose one of the following options - \n\n");
    printf("1) Allocate memory\n");
    printf("2) Free Memory\n");
    printf("3) Display Memory Status\n");
    printf("4) Exit Program\n");

    printf("Enter the action you would like to implement: ");
    scanf("%d", &ch);

    switch(ch)
    {
        case 1:
        printf("\nEnter the size of the block to be allocated: ");
        scanf("%d", &enter_block);
        allocate_memory(enter_block);
        show();
        choices();
        break;

        case 2:
        printf("\nEnter the ID of the block to be freed: ");
        scanf("%d", &enter_block);
        free_memory(enter_block);
        merge_adjacent();
        show();
        choices();
        break;

        case 3:
        show();
        choices();
        break;

        case 4:
        printf("Successfully exited from the program");
        exit(0);
        break;

        default:
        printf("The choice you have made does not exist. Please try again.");
        choices();
    }
}

//main function
int main()
{
    ptr_heap = (Heap*)malloc(SIZE_OF_HEAP);
    ptr_heap = init(ptr_heap);
    choices();
    return 0;
}