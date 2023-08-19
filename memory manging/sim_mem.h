#ifndef EX4_SIM_MEM_H
#define EX4_SIM_MEM_H
#define MEMORY_SIZE 200
#include <iostream>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <list>
using namespace std;
extern char main_memory[MEMORY_SIZE];

typedef struct page_descriptor
{
    bool valid;
    int frame;
    bool dirty;
    int swap_index;
} page_descriptor;

class sim_mem {
    int swapfile_fd; //swap file fd
    int program_fd; //executable file fd
    int text_size;
    int data_size;
    int bss_size;
    int heap_stack_size;
    int num_of_pages;
    int page_size;
    int num_of_proc;
    page_descriptor **page_table; //pointer to page table
public:
    sim_mem(char exe_file_name[], char swap_file_name[], int text_size, int data_size,int bss_size, int heap_stack_size, int page_size);

    ~sim_mem();

    char load(int address);

    void store(int address, char value);

    void print_memory();

    void print_swap();

    void print_page_table();
    void copyPageFromExe(int in , int offset , int out );
    void write_to_swap(int frame_number , int in , int out , int offset );
    void init_new_page(int in , int out , int offset);
    void bring_page_from_swap (int in , int out ,int offset) ;
};
#endif //EX4_SIM_MEM_H
