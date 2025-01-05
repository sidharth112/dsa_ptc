//
//  main.cpp
//  Malloc_Program_DELL
//  Created by Sidharth on 29/07/18.
//  Copyright © 2018 Sidharth. All rights reserved.
//  Malloc_Project_DELL
//  Email: sidharth.priyadarshan@gmail.com/sidharth112@gmail.com
//  This program is created with intent to impliment
//  Malloc, Free and re-alloc library functionality.
//

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// I am assuming to allocate on a global space as heap allocation not mentioned
char mallocArray[60000] = {'\0'};
// *base is a base pointer to the 0 initialized arrary
char *base = mallocArray;

// Assumed 2 flags 'f' for free, 'a' for allocated
char free_flag      = 'f';
char allocateflag   = 'a';
/*
 char* dell_Example(size_t)
 Return Type        :       charector pointer
 Arguments-
 size_t size        :       Size of the Block to be allocated
 Upon Success returns a charector pointer
 Upon failure returns NULL
 */
char* dell_Example_Find_Block(size_t size){
    
    //mover is a walking pointer on the different sized blocks
    char* mover;
    //Initially mover is at base
    mover = base;
    int num = 0;
    
    while(1){
        //calculate the size of mover and place
        num = *(int *)(mover+1);
        //check the metadata if the mover has the free_flag set and available size is greater than requested size+metadata
        if(*mover == free_flag && num >= size+5){
            return (mover);
        }else if(!mover){
            //mover failed returned NULL
                return NULL;}
              else{
                  //if requested size is greater than 60000
                if((mover + num + size) >= (mallocArray + 59999))
                {
                    printf(" OUT OF MEMORY ERROR\n");
                    return NULL;
                }
                  //make the moover walk to next
                mover = mover+num;
            }
    }
    //Success and returned the mover
    return(mover);
}
/*
 If the size available is greater than requested sizefragment it
 Use the requested size and leave the rest for free
 void dell_Example_Split_block(char*, size_t)
 Return             :           Nothing
 Arguments-
 char* b            :           base size required
 size_t s           :           Size Requested
 On success splits the bigger block in to required size
 */
void dell_Example_Split_block(char *b, size_t s){
    char* temp;
    int b_size          =       *(int *)(b+1);
    b_size              =       (int)(b_size - s - 5);
    temp                =       b+s+5;
    *temp               =       free_flag;
    *(int *)(temp+1)    =       b_size;
    *(int *)(b+1)       =       (int)(s+5);
    *b                  =       allocateflag;
}
/*
 Main Malloc function to allocate memory
 char* dell_Example_Malloc(size_t)
 Return         :       Number of bytes allocated
 Arguments-
 size_t size    :       Size of the block required to be allocated
 Upon Success allocate momory
 Upon Failure return null
 */
char* dell_Example_Malloc(size_t size){
    //Memory allocation can't be negative
    if (size<=0){
        return NULL;}
    /*if base is not NULL
    Initially all blocks are free
    so give it a free_flag and provide allocation after Meta data of 5
    hence 59999-5 to base+1*/
    if(!*base){
        *base   =   free_flag;
        *(int *)(base + 1) = 59999 - 5;
    }
    size_t s = size;
    //Find appropriatly sized block for allocation
    char *b = dell_Example_Find_Block(s);
    if(b){
        if(*(int *)(b+1) >= size){
            //Since size found is bigger than requested, split it
            dell_Example_Split_block(b , s);
        }else {
            //If you cant find return NULL
            return NULL;
        }
    }
    //Success and found memory
    return (b);
}
/*
 Free function for allocated memory
 char* dell_Example_Free(char*)
 Return         :       Nothing
 Arguments-
 char*          :       pointer to allocated address
 Upon Success allocate momory
 Upon Failure return null
 */

void dell_Example_Free(char *address_allocated){
    //walker pointer, previous pointer of allocated memory, next pointer for allocated memory
    char *mover, *previous = NULL, *next = NULL;
    //Make walker point to base.
    mover = base;
    //get the place and value
    int num = *(int *)(mover+1);
    //look for the allocated memory
    while(1){
        //get the place and value again inside the true while
        num = *(int *)(mover+1);
        //when you find the allocated address break
        if(mover == address_allocated){
            break;
        }else if(!*mover){
            break;
        }else{
            //continue to move along
            previous = mover;
            mover = mover + num;
            next = mover + *(int *)(mover+1);
        }
    }
    //if the next block to the request block is free, add it with the requested block, this will reduce fragmentation and free all
    if(*next == free_flag){
        *mover = free_flag;
        *(int *)(mover +1) = *(int *)(mover+1) + *(int *)(next +1);
    }
    //if the previous block to the request block is free, add it with the requested block, this will reduce fragmentation and free all
    if(*previous == free_flag){
        *mover = free_flag;
        *(int *)(previous + 1) = *(int *)(mover +1) + *(int *)(previous + 1);
    }
    else{
        //if it is the base and sole block mark it free
        *address_allocated = free_flag;
    }
}

/*
 Re Alloc function for allocated memory
 char* dell_Example_Realloc(char*, size_t)
 Return         :       number of reallocated bytes
 Arguments-
 char* base_ptr :       pointer to allocated address
 size_t size    :       New size requested
 Upon Success Re-allocate momory
 Upon Failure return null
 */


char* dell_Example_Realloc(char *base_ptr, size_t size){
    char *temp;
    if(base_ptr == NULL){
        return dell_Example_Malloc(size);
    }
    if(size == 0){
        dell_Example_Free(base_ptr);
        return NULL;
    }
    temp = dell_Example_Malloc(size);
    if(temp == NULL){
        return NULL;
    }
    memcpy(temp, base_ptr, size);
    dell_Example_Free(base_ptr);
    return temp;
}



int main(){
    /*
     Some positive allocation cases for malloc
     */
    char* test1 = dell_Example_Malloc(200);
    char* test2 = dell_Example_Malloc(400);
    char* test3 = dell_Example_Malloc(100);
    
    printf(" Test1 for allocation 8 %p \n", test1);
    printf(" Test2 for Allocation 400 %p \n", test2);
    printf(" Test3 for allocation 100 %p \n", test3);
    if(test1 != NULL||test2!=NULL||test3!=NULL){
        printf("               TEST PASSED1\n");
    }else{
        printf("               TEST FAILED1\n");
    }
    
    /*
     Some positive Free cases for Free function
     */
    dell_Example_Free(test3);
    dell_Example_Free(test2);
    
    /*
     After you free 400+100 = 500, try to allocate again
     and check the base address of the test 2 allocated 
     memory address, It should be re-assigned to newly requested memory
     you can swap the free test allocated memory but still it'll take base address 
     of test 2 allocated
     */
    char* test4 = dell_Example_Malloc(500);
    printf(" Test4 for allocation 500, Should be equal to Test2 Address %p \n", test4);
    if(test2 == test4){
        printf("               TEST PASSED2\n");
    }else{
        printf("               TEST FAILED2\n");
    }
    
    /*
     Lets try to re-allocate the previously allocated memory
     for test 4 and check the addresses, if they are same 
     our test for re-allocation is passed.
    */
    char* test5 = dell_Example_Realloc(test4, 1000);
    printf(" Test5 for allocation 1000 %p \n", test5);
    if(test5 != NULL){
        printf("               TEST PASSED3\n");
    }else{
        printf("               TEST FAILED3\n");
    }
    
    //Boundary value -ve case
    char* test6 = dell_Example_Malloc(1000000000);
    if (test6 == NULL){
        
        printf("               TEST PASSED4\n");
        
    }else{
        printf("               TEST FAILED4\n");
    }
    // Use re-alloc as free function
    char* test7 = dell_Example_Realloc(test5, 0);
    printf(" Test7 for allocation 0 %p \n", test7);
    if (test7 == NULL){
        
        printf("               TEST PASSED5\n");
        
    }else{
        printf("               TEST FAILED5\n");
    }
    //Check if the re-allocated memory is re-assigned to newly created block
    char* test8 = dell_Example_Malloc(1000);
    printf(" Test8 for allocation 1000 %p \n", test8);
    if(test2 == test8){
        
        printf("               TEST PASSED6\n");
        
    }else{
        printf("               TEST FAILED6\n");
    }

    
    return 0;
}

