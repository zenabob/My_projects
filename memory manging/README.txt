written by Zenab Mahmoud 212384390

Ex4

==description==
A simulation of memory manging in the computer.


== Structure==
page_descriptor:

fileds:
*valid: valid; if the page in the memory valid=true, else Valid=false.
*dirty: dirty; if the user make store before D=1, else D=0.
*frame: the number of frame if in case it is page_mapped.
*swap_index: where the page is located in the swap file.

==Classes==
sim_mem:

fileds:
*swapfile_fd: swap file fd.
*program_fd: executable file fd.
*text_size:the num of chars that are allocated for text-type.
*data_size:the num of chars that are allocated for data-type.
*bss_size:the num of chars that are allocated for bss-type.
*heap_stack_size:the num of chars that are allocated for heap and stack type.
*num_of_pages:the num of pages in page table.
*page_size:the num of chars in each page.
*num_of_proc:the num of process.
*page_descriptor ** page_table:pointer to the page table.


==constructor==
sim_mem(char exe_file_name[],char swap_file_name[], int text_size,int data_size,int bss_size,int heap_stack_size,int page_size)

fill all fields of the class , and we fill the main memory by 0 and fill the swap file by 0 and put Valid and dirty equal to false to all the pages,and put swap index and frame=-1 of all the pages.


==destructor==
~sim_mem()

close all the files and free all the allocated memory.

==functions==
char load(int address)

 parameter:
  *address: the logical address of the page.

 load the page to the memorey.

void store(int address , char value)

 parameter:
  *address: the logical address of the page.
  *value: the value that need to store to the memory.

 store the value to the memory.

void copyPageFromExe(int in , int offset , int out)

 parameter:
   *offset:the offset of the page.
   *in : the address to the pagetable in the inside.
   *out: the address to the pagetable in the outside.

copy the page from the executable file to the memory.

void write_to_swap(int frame_number , int in , int out , int offset  )
 parameter:
   *offset:the offset of the page.
   *in : the address to the pagetable in the inside.
   *out: the address to the pagetable in the outside.
   *frame_number:the number of the frame
 
write the data of the page to the swap file.

void init_new_page(int in , int out ,int offset) 
 parameter:
   *offset:the offset of the page.
   *in : the address to the pagetable in the inside.
   *out: the address to the pagetable in the outside.
intilaize new page and write zeros in it and write it to the memory.

void bring_page_from_swap(int in , int out , int offset)
  parameter:
   *offset:the offset of the page.
   *in : the address to the pagetable in the inside.
   *out: the address to the pagetable in the outside.
  bring page from swap and add it to main memory 

==Program files==
sim_mem.h
sim_mem.cpp
main.cpp

==How yo compile==
g++ main.cpp sim_mem.cpp -o main