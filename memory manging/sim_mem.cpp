#include "sim_mem.h"
#include <bitset>
#include <cmath>
#include <list>


int num_of_txt_pages ;
int num_of_data_pages ;
int num_of_bss_pages;
int num_of_stck_heap_pages;
int swap_size;
char* pageData ;
int** counter;// the order of add
int count=-1;
int* Swap;// empty in swap file
int* empty;// empty in the memory
sim_mem::sim_mem(char exe_file_name[], char swap_file_name[], int text_size, int data_size,int bss_size, int heap_stack_size, int page_size){
    this->text_size=text_size;
    this->data_size=data_size;
    this->bss_size=bss_size;
    this->heap_stack_size=heap_stack_size;
    this->page_size=page_size;
    this->num_of_proc=0;

    // Open executable file
    program_fd = open(exe_file_name, O_RDONLY,0);
    if (program_fd < 0)
    {
        perror ("Failed to open executable file" );
        exit(1);
    }
    // Open swap file
    this->swapfile_fd=open(swap_file_name, O_CREAT|O_RDWR, S_IRWXU|S_IRWXG|S_IRWXO);
    if (swapfile_fd < 0)
    {
        perror("Failed to open swap file");
        exit(1);
    }
    pageData= (char *) malloc(page_size * sizeof(char));
    num_of_pages = MEMORY_SIZE / page_size;
    // Calculate the number of pages
    num_of_data_pages = data_size / page_size;
    num_of_bss_pages = bss_size / page_size;
    num_of_stck_heap_pages = heap_stack_size / page_size;
    num_of_txt_pages = text_size / page_size;


    counter=(int**)malloc(4* sizeof(int*));
    counter[0]=(int*) malloc(num_of_txt_pages* sizeof(int));
    counter[1]=(int*) malloc(num_of_data_pages* sizeof(int));
    counter[2]=(int*) malloc(num_of_bss_pages* sizeof(int));
    counter[3]=(int*) malloc(num_of_stck_heap_pages* sizeof(int));
    for(int j=0 ; j<4;j++){
        if(j==0){
            for(int i=0;i<num_of_txt_pages;i++){
                counter[j][i]=-1;
            }
        }
        else if(j==1){
            for(int i=0;i<num_of_data_pages;i++){
                counter[j][i]=-1;
            }
        }
        else if(j==2){
            for(int i=0;i<num_of_bss_pages;i++){
                counter[j][i]=-1;
            }
        }
        else{
            for(int i=0;i<num_of_stck_heap_pages;i++){
                counter[j][i]=-1;
            }
        }
    }
    // init page table
    this->page_table=(page_descriptor**)malloc(4* sizeof(page_descriptor*));
    this->page_table[0]=(page_descriptor*) malloc(num_of_txt_pages* sizeof(page_descriptor));

    for(int i=0 ; i<num_of_txt_pages;i++) {
        this->page_table[0][i].valid = false;
        this->page_table[0][i].frame = -1;
        this->page_table[0][i].dirty = false;
        this->page_table[0][i].swap_index = -1;
    }
    this->page_table[1]=(page_descriptor*) malloc(num_of_data_pages* sizeof(page_descriptor));
    for(int i=0 ; i<num_of_data_pages;i++) {
        this->page_table[1][i].valid = false;
        this->page_table[1][i].frame = -1;
        this->page_table[1][i].dirty = false;
        this->page_table[1][i].swap_index = -1;
    }
    this->page_table[2]=(page_descriptor*) malloc(num_of_bss_pages* sizeof(page_descriptor));
    for(int i=0 ; i<num_of_bss_pages;i++) {
        this->page_table[2][i].valid = false;
        this->page_table[2][i].frame = -1;
        this->page_table[2][i].dirty = false;
        this->page_table[2][i].swap_index = -1;
    }
    this->page_table[3]=(page_descriptor*) malloc(num_of_stck_heap_pages* sizeof(page_descriptor));
    for(int i=0 ; i<num_of_stck_heap_pages;i++) {
        this->page_table[3][i].valid = false;
        this->page_table[3][i].frame = -1;
        this->page_table[3][i].dirty = false;
        this->page_table[3][i].swap_index = -1;
    }

    for (char& memory : main_memory) {
        memory = '0';
    }


    swap_size=(num_of_data_pages+num_of_stck_heap_pages+num_of_bss_pages)* this->page_size;
    if (ftruncate(swapfile_fd, swap_size) < 0)
    {
        perror("Failed to set the size of the SWAP file");
        exit(1);
    }
    for(int i=0;i<swap_size;i++){
        write(this->swapfile_fd,"0",1);
    }

    Swap=(int*) malloc(sizeof (int )*(swap_size/this->page_size));
    for(int i=0;i<swap_size/this->page_size;i++){
        Swap[i]=-1;
    }
    empty=(int*)malloc(sizeof (int)*(MEMORY_SIZE/this->page_size));
    for(int i=0;i<MEMORY_SIZE/this->page_size;i++){
        empty[i]=-1;
    }

}

char sim_mem::load(int address) {
    int offset, in, out;
    bitset<32> binary(address);
    string binaryString = binary.to_string().substr(20, 12);
    // Calculate the offset
    double result = log2(page_size);
    int num_of_bites=static_cast<int>(result);
    string offestt;
    int j=0;
    for (int i=12-num_of_bites;i<12;i++){
        offestt[j]=binaryString[i];
        j++;
    }
    offset=stoi(offestt, nullptr, 2);
     string inn;
    j=0;
    for (int i=2;i<(12-num_of_bites);i++){
        inn[j]=binaryString[i];
        j++;
    }
    in = stoi(inn, nullptr, 2);
    string outt;
    j=0;
    for (int i=0;i<2;i++){
        outt[j]=binaryString[i];
        j++;
    }
    out = stoi(outt, nullptr, 2);
    // check valid address
    int val= 0;
    if (out == 0){
        if (in > num_of_txt_pages){
            perror("Error: Invalid address!");
            val=-1;
            return '\0';
        }
    }
    if (out == 1){
        if (in > num_of_data_pages){
            perror("Error: Invalid address!");
            val=-1;
            return '\0';
        }
    }
    if (out == 2){
        if (in > num_of_bss_pages){
            perror("Error: Invalid address!");
            val=-1;
            return '\0';
        }
    }
    if (out == 3){
        if (in > num_of_stck_heap_pages){
            perror("Error: Invalid address!");
            val=-1;
            return '\0';
        }
    }

    if (val == 0) {
        if (!this->page_table[out][in].valid) {
            if (out == 0) {
                copyPageFromExe(in,offset,out);
                return main_memory[(this->page_table[out][in].frame * this->page_size) + offset];
            }
            else {
                if (!this->page_table[out][in].dirty) {//d=0
                    if (out == 3) {
                        //heap_stack
                        perror("should store first!");
                        return '\0';
                    } else {
                        //data or bss page
                        copyPageFromExe(in,offset,out);
                        return main_memory[(this->page_table[out][in].frame * this->page_size) + offset];
                    }
                }
                else {//d=1
                    bring_page_from_swap(in , out , offset);
                    return main_memory[(this->page_table[out][in].frame * this->page_size) + offset];
                }
            }
        }
        else {//valid=true
            for(int j=0 ; j<4;j++){
                if(j==0){
                    for(int i=0;i<num_of_txt_pages;i++){
                        if (counter[j][i]>-1){
                            counter[j][i]=counter[j][i]-1;
                        }

                    }
                }
                else if(j==1){
                    for(int i=0;i<num_of_data_pages;i++){
                        if (counter[j][i]>-1){
                            counter[j][i]=counter[j][i]-1;
                        }
                    }
                }
                else if(j==2){
                    for(int i=0;i<num_of_bss_pages;i++){
                        if (counter[j][i]>-1){
                            counter[j][i]=counter[j][i]-1;
                        }
                    }
                }
                else{
                    for(int i=0;i<num_of_stck_heap_pages;i++){
                        if (counter[j][i]>-1){
                            counter[j][i]=counter[j][i]-1;
                        }
                    }
                }
            }
            counter[out][in]=count;

            return main_memory[(this->page_table[out][in].frame * this->page_size) + offset];
        }
    }
    return '\0';
}
void sim_mem::copyPageFromExe(int in , int offset , int out) {
    memset(pageData,'\0',page_size);

    // Go to the desired address in the executable file
    if (out ==0) {
        lseek(program_fd, in * page_size, SEEK_SET);
    }
    if (out == 1) {
        int index =((in * page_size)+(page_size*num_of_txt_pages));
        lseek(program_fd, index, SEEK_SET);
    }
    // Read the page data from the executable file
    read(program_fd, pageData, page_size);

    // Find a free frame in the main memory to store the page
    int frame_number=-1;

    for (int i = 0; i <num_of_pages; i++) {
        if(empty[i]==-1){
            frame_number=i;
            break;
        }
    }

    if (frame_number==-1) {
        // No free frame available - perform page replacement algorithm (LRU)
        // Find the least recently used page in the LRU list
        int min = 1;
        for (int j=0 ; j<4 ;j++){
            if (j == 0){
                for (int i = 0; i < num_of_txt_pages; i++){
                   if (counter[j][i] <= min&& counter[j][i]>-1 ){
                       min=counter [j][i];
                       frame_number= page_table[j][i].frame;
                   }
                }
            }
            else if (j == 1) {
                for (int i = 0; i < num_of_data_pages; i++) {
                    if (counter[j][i] <= min && counter[j][i]>-1){
                        min=counter [j][i];
                        frame_number= page_table[j][i].frame;
                    }
                }
            } else if (j == 2) {
                for (int i = 0; i < num_of_bss_pages; i++) {
                    if (counter[j][i] <= min && counter[j][i]>-1){
                        min=counter [j][i];
                        frame_number= page_table[j][i].frame;
                    }
                }
            } else {
                for (int i = 0; i < num_of_stck_heap_pages; i++) {
                    if (counter[j][i] <= min && counter[j][i]>-1){
                        min=counter [j][i];
                        frame_number= page_table[j][i].frame;
                    }
                }
            }
        }

        int find=0;
        for (int j = 0; j < 4; j++) {
            if (j == 0) {
                for (int i = 0; i < num_of_txt_pages; i++) {
                    if (page_table[j][i].frame == frame_number) {
                        this->page_table[j][i].frame = -1;
                        this->page_table[j][i].valid = false;
                        find=1;
                        break;
                    }
                }
            }
            else if (j == 1) {
                for (int i = 0; i < num_of_data_pages; i++) {
                    if (page_table[j][i].frame == frame_number) {
                        this->page_table[j][i].frame = -1;
                        this->page_table[j][i].valid = false;
                        find=1;
                        if (page_table[j][i].dirty) {
                            //if the old page is dirty save it in the swap file
                            write_to_swap(frame_number, i, j, offset);
                        }
                        break;
                    }
                }
            }
            else if (j == 2) {
                for (int i = 0; i < num_of_bss_pages; i++) {
                    if (page_table[j][i].frame == frame_number) {
                        this->page_table[j][i].frame = -1;
                        this->page_table[j][i].valid = false;
                        find=1;
                        if (page_table[j][i].dirty) {
                            //if the old page is dirty save it in the swap file
                            write_to_swap(frame_number, i, j, offset);
                        }
                        break;
                    }
                }
            } else {
                for (int i = 0; i < num_of_stck_heap_pages; i++) {
                    if (page_table[j][i].frame == frame_number) {
                        this->page_table[j][i].frame = -1;
                        this->page_table[j][i].valid = false;
                        find=1;
                        if (page_table[j][i].dirty) {
                            //if the old page is dirty save it in the swap file
                            write_to_swap(frame_number, i, j, offset);
                        }
                        break;
                    }
                }
            }
            if (find ==1){
                empty[frame_number]=-1;
                for(int j=0 ; j<4;j++){
                    if(j==0){
                        for(int i=0;i<num_of_txt_pages;i++){
                            if (counter[j][i]>-1){
                                counter[j][i]=counter[j][i]-1;
                            }

                        }
                    }
                    else if(j==1){
                        for(int i=0;i<num_of_data_pages;i++){
                            if (counter[j][i]>-1){
                                counter[j][i]=counter[j][i]-1;
                            }
                        }
                    }
                    else if(j==2){
                        for(int i=0;i<num_of_bss_pages;i++){
                            if (counter[j][i]>-1){
                                counter[j][i]=counter[j][i]-1;
                            }
                        }
                    }
                    else{
                        for(int i=0;i<num_of_stck_heap_pages;i++){
                            if (counter[j][i]>-1){
                                counter[j][i]=counter[j][i]-1;
                            }
                        }
                    }
                }
                count--;
                break;
            }
        }
    }

    this->page_table[out][in].frame= frame_number;
    // Update the page table
    page_table[out][in].valid = true;
    page_table[out][in].frame = frame_number;
    page_table[out][in].dirty = false;
    page_table[out][in].swap_index = -1;
    // Copy the page data to the main memory
    empty[frame_number]=1;
    count++;
    counter[out][in]=count;
    for(int i=0,j=frame_number*page_size;i<page_size;i++,j++) {
        main_memory[j]=pageData[i];
    }
}

void sim_mem:: write_to_swap(int frame_number , int in , int out , int offset ){

    int swapIndex=-1;
    char* copy=(char*)malloc(sizeof(char )*page_size);
    for(int i=0;i<page_size;i++){
        copy[i]=main_memory[(frame_number*page_size)+i];
    }


    for (int i = 0; i < (swap_size / this->page_size); i++) {
        if (Swap[i] == -1) {
            swapIndex = i ;
            Swap[i]=1;
            break;
        }
    }
    this->page_table[out][in].swap_index=swapIndex;
    lseek(swapfile_fd, swapIndex*page_size , SEEK_SET);
    write(swapfile_fd, copy , page_size);
    free(copy);
}

void sim_mem::init_new_page(int in , int out ,int offset) {
    memset(pageData, '\0', page_size);
    for (int i=0; i<page_size;i++){
        pageData[i]='0';
    }
    // Find a free frame in the main memory to store the page
    int frame_number=-1;
    for (int i = 0; i <num_of_pages; i++) {
        if(empty[i]==-1){
            frame_number=i;
            break;
        }
    }

    if (frame_number==-1) {
        // No free frame available - perform page replacement algorithm (LRU)
        // Find the least recently used page in the LRU list
        int min = 1;
        for (int j=0 ; j<4 ;j++){
            if (j == 0){
                for (int i = 0; i < num_of_txt_pages; i++){
                    if (counter[j][i] <= min && counter[j][i]>-1){
                        min=counter [j][i];
                        frame_number= page_table[j][i].frame;
                    }
                }
            }
            else if (j == 1) {
                for (int i = 0; i < num_of_data_pages; i++) {
                    if (counter[j][i] <= min && counter[j][i]>-1){
                        min=counter [j][i];
                        frame_number= page_table[j][i].frame;
                    }
                }
            } else if (j == 2) {
                for (int i = 0; i < num_of_bss_pages; i++) {
                    if (counter[j][i] <= min && counter[j][i]>-1){
                        min=counter [j][i];
                        frame_number= page_table[j][i].frame;
                    }
                }
            } else {
                for (int i = 0; i < num_of_stck_heap_pages; i++) {
                    if (counter[j][i] <= min && counter[j][i]>-1){
                        min=counter [j][i];
                        frame_number= page_table[j][i].frame;
                    }
                }
            }
        }

        int find=0;
        for (int j = 0; j < 4; j++) {
            if (j == 0) {
                for (int i = 0; i < num_of_txt_pages; i++) {
                    if (page_table[j][i].frame == frame_number) {
                        this->page_table[j][i].frame = -1;
                        this->page_table[j][i].valid = false;
                        find=1;
                        break;
                    }
                }
            }
            else if (j == 1) {
                for (int i = 0; i < num_of_data_pages; i++) {
                    if (page_table[j][i].frame == frame_number) {
                        this->page_table[j][i].frame = -1;
                        this->page_table[j][i].valid = false;
                        find=1;
                        if (page_table[j][i].dirty) {
                            //if the old page is dirty save it in the swap file
                            write_to_swap(frame_number, i, j, offset);
                        }
                        break;
                    }
                }
            }
            else if (j == 2) {
                for (int i = 0; i < num_of_bss_pages; i++) {
                    if (page_table[j][i].frame == frame_number) {
                        this->page_table[j][i].frame = -1;
                        this->page_table[j][i].valid = false;
                        find=1;
                        if (page_table[j][i].dirty) {
                            //if the old page is dirty save it in the swap file
                            write_to_swap(frame_number, i, j, offset);
                        }
                        break;
                    }
                }
            } else {
                for (int i = 0; i < num_of_stck_heap_pages; i++) {
                    if (page_table[j][i].frame == frame_number) {
                        this->page_table[j][i].frame = -1;
                        this->page_table[j][i].valid = false;
                        find=1;
                        if (page_table[j][i].dirty) {
                            //if the old page is dirty save it in the swap file
                            write_to_swap(frame_number, i, j, offset);
                        }
                        break;
                    }
                }
            }
            if (find ==1){
                empty[frame_number]=-1;
                for(int j=0 ; j<4;j++){
                    if(j==0){
                        for(int i=0;i<num_of_txt_pages;i++){
                            if (counter[j][i]>-1){
                                counter[j][i]=counter[j][i]-1;
                            }

                        }
                    }
                    else if(j==1){
                        for(int i=0;i<num_of_data_pages;i++){
                            if (counter[j][i]>-1){
                                counter[j][i]=counter[j][i]-1;
                            }
                        }
                    }
                    else if(j==2){
                        for(int i=0;i<num_of_bss_pages;i++){
                            if (counter[j][i]>-1){
                                counter[j][i]=counter[j][i]-1;
                            }
                        }
                    }
                    else{
                        for(int i=0;i<num_of_stck_heap_pages;i++){
                            if (counter[j][i]>-1){
                                counter[j][i]=counter[j][i]-1;
                            }
                        }
                    }
                }
                count --;
                break;
            }
        }
    }

    this->page_table[out][in].frame= frame_number;
    // Update the page table
    page_table[out][in].valid = true;
    page_table[out][in].frame = frame_number;
    page_table[out][in].dirty = false;
    page_table[out][in].swap_index = -1;
    empty[frame_number]=1;
    count++;
    counter[out][in]=count;
    // Copy the page data to the main memory
    for(int j=frame_number*page_size;j<(frame_number*page_size)+page_size;j++) {
        main_memory[j]='0';
    }
}
void sim_mem::bring_page_from_swap(int in , int out , int offset){
    memset(pageData, '\0', page_size);

    // Find a free frame in the main memory to store the page
    int frame_number=-1;
    for (int i = 0; i <num_of_pages; i++) {
        if(empty[i]==-1){
            frame_number=i;
            break;
        }
    }

    if (frame_number==-1) {
        // No free frame available - perform page replacement algorithm (LRU)
        // Find the least recently used page in the LRU list
        int min = 1;
        for (int j=0 ; j<4 ;j++){
            if (j == 0){
                for (int i = 0; i < num_of_txt_pages; i++){
                    if (counter[j][i] <= min && counter[j][i]>-1){
                        min=counter [j][i];
                        frame_number= page_table[j][i].frame;
                    }
                }
            }
            else if (j == 1) {
                for (int i = 0; i < num_of_data_pages; i++) {
                    if (counter[j][i] <= min && counter[j][i]>-1){
                        min=counter [j][i];
                        frame_number= page_table[j][i].frame;
                    }
                }
            } else if (j == 2) {
                for (int i = 0; i < num_of_bss_pages; i++) {
                    if (counter[j][i] <= min && counter[j][i]>-1){
                        min=counter [j][i];
                        frame_number= page_table[j][i].frame;
                    }
                }
            } else {
                for (int i = 0; i < num_of_stck_heap_pages; i++) {
                    if (counter[j][i] <= min && counter[j][i]>-1){
                        min=counter [j][i];
                        frame_number= page_table[j][i].frame;
                    }
                }
            }
        }

        int find=0;
        for (int j = 0; j < 4; j++) {
            if (j == 0) {
                for (int i = 0; i < num_of_txt_pages; i++) {
                    if (page_table[j][i].frame == frame_number) {
                        this->page_table[j][i].frame = -1;
                        this->page_table[j][i].valid = false;
                        find=1;
                        break;
                    }
                }
            }
            else if (j == 1) {
                for (int i = 0; i < num_of_data_pages; i++) {
                    if (page_table[j][i].frame == frame_number) {
                        this->page_table[j][i].frame = -1;
                        this->page_table[j][i].valid = false;
                        find=1;
                        if (page_table[j][i].dirty) {
                            //if the old page is dirty save it in the swap file
                            write_to_swap(frame_number, i, j, offset);
                        }
                        break;
                    }
                }
            }
            else if (j == 2) {
                for (int i = 0; i < num_of_bss_pages; i++) {
                    if (page_table[j][i].frame == frame_number) {
                        this->page_table[j][i].frame = -1;
                        this->page_table[j][i].valid = false;
                        find=1;
                        if (page_table[j][i].dirty) {
                            //if the old page is dirty save it in the swap file
                            write_to_swap(frame_number, i, j, offset);
                        }
                        break;
                    }
                }
            } else {
                for (int i = 0; i < num_of_stck_heap_pages; i++) {
                    if (page_table[j][i].frame == frame_number) {
                        this->page_table[j][i].frame = -1;
                        this->page_table[j][i].valid = false;
                        find=1;
                        if (page_table[j][i].dirty) {
                            //if the old page is dirty save it in the swap file
                            write_to_swap(frame_number, i, j, offset);
                        }
                        break;
                    }
                }
            }
            if (find ==1){
                empty[frame_number]=-1;
                for(int j=0 ; j<4;j++){
                    if(j==0){
                        for(int i=0;i<num_of_txt_pages;i++){
                            if (counter[j][i]>-1){
                                counter[j][i]=counter[j][i]-1;
                            }

                        }
                    }
                    else if(j==1){
                        for(int i=0;i<num_of_data_pages;i++){
                            if (counter[j][i]>-1){
                                counter[j][i]=counter[j][i]-1;
                            }
                        }
                    }
                    else if(j==2){
                        for(int i=0;i<num_of_bss_pages;i++){
                            if (counter[j][i]>-1){
                                counter[j][i]=counter[j][i]-1;
                            }
                        }
                    }
                    else{
                        for(int i=0;i<num_of_stck_heap_pages;i++){
                            if (counter[j][i]>-1){
                                counter[j][i]=counter[j][i]-1;
                            }
                        }
                    }
                }
                count --;
                break;
            }
        }
    }

    //write the newPage in the main memory
    int swapIndex=this->page_table[out][in].swap_index;
    Swap[swapIndex/page_size]=-1;
    lseek(swapfile_fd, swapIndex*page_size, SEEK_SET); // go to the address in the swap file
    read(swapfile_fd,pageData,page_size);//read from swap file
    for(int i=0;i<page_size;i++){
        lseek(swapfile_fd, (swapIndex*page_size)+i, SEEK_SET); // go to the address in the swap file
        write(swapfile_fd,"0",1);//write 0 in the place
    }
    this->page_table[out][in].frame= frame_number;
    // Update the page table
    page_table[out][in].valid = true;
    page_table[out][in].frame = frame_number;
    page_table[out][in].dirty = false;
    page_table[out][in].swap_index = -1;
    empty[frame_number]=1;
    count++;
    counter[out][in]=count;
    // Copy the page data to the main memory
    for(int i=0,j=frame_number*page_size;i<page_size;i++,j++) {
        main_memory[j]=pageData[i];
    }
}
void sim_mem:: store(int address, char value) {
    int offset, in, out;
    bitset<32> binary(address);
    string binaryString = binary.to_string().substr(20, 12);
    // Calculate the offset
    double result = log2(page_size);
    int num_of_bites=static_cast<int>(result);
    string offestt;
    int j=0;
    for (int i=12-num_of_bites;i<12;i++){
        offestt[j]=binaryString[i];
        j++;
    }
    offset=stoi(offestt, nullptr, 2);
    string inn;
    j=0;
    for (int i=2;i<(12-num_of_bites);i++){
        inn[j]=binaryString[i];
        j++;
    }
    in = stoi(inn, nullptr, 2);
    string outt;
    j=0;
    for (int i=0;i<2;i++){
        outt[j]=binaryString[i];
        j++;
    }
    out = stoi(outt, nullptr, 2);
    // check valid address
    int val= 0;
    if (out == 0){
        if (in > num_of_txt_pages){
            perror("Error: Invalid address!");
            val=-1;
        }
    }
    if (out == 1){
        if (in > num_of_data_pages){
            perror("Error: Invalid address!");
            val=-1;

        }
    }
    if (out == 2){
        if (in > num_of_bss_pages){
            perror("Error: Invalid address!");
            val=-1;
        }
    }
    if (out == 3){
        if (in > num_of_stck_heap_pages){
            perror("Error: Invalid address!");
            val=-1;
        }
    }
    if(val==0) {
        if (!this->page_table[out][in].valid) {
            if (out == 0) {
                perror("can't write in a text page!");
            } else {
                if (!this->page_table[out][in].dirty) {//d=0
                    if (out == 2 || out == 3) {
                        //bss,heapStack page
                        init_new_page(in, out, offset);
                        main_memory[(this->page_table[out][in].frame * this->page_size) + offset] = value;
                        this->page_table[out][in].dirty = true;
                    }
                    if (out == 1) {
                        //data page
                        copyPageFromExe(in, offset, out);
                        main_memory[(this->page_table[out][in].frame * this->page_size) + offset] = value;
                        this->page_table[out][in].dirty = true;
                    }
                } else {//d=1
                    //bring page from swap
                    bring_page_from_swap(in, out, offset);
                    main_memory[(this->page_table[out][in].frame * this->page_size) + offset] = value;
                    this->page_table[out][in].dirty = true;
                }
            }
        } else {//V=1
            if (out == 0){
                perror("can't write in a text page!");
            }
            else {
                main_memory[(this->page_table[out][in].frame * this->page_size) + offset] = value;
                this->page_table[out][in].dirty = true;
            }
        }
    }
}
/**************************************************************************************/
void sim_mem::print_memory() {
    int i;
    printf("\n Physical memory\n");
    for(i = 0; i < MEMORY_SIZE; i++) {
        printf("[%c]\n", main_memory[i]);
    }
}

/************************************************************************************/
void sim_mem::print_swap() {
    char* str = (char*)malloc(this->page_size *sizeof(char));
    int i;
    printf("\n Swap memory\n");
    lseek(swapfile_fd, 0, SEEK_SET); // go to the start of the file
    while(read(swapfile_fd, str, this->page_size) == this->page_size) {
        for(i = 0; i < page_size; i++) {
            printf("%d - [%c]\t", i, str[i]);
        }
        printf("\n");
    }
}
/***************************************************************************************/
void sim_mem::print_page_table() {
    int i;
    // Calculate the number of pages
    num_of_data_pages = data_size / page_size;
    num_of_bss_pages = bss_size / page_size;
    num_of_stck_heap_pages = heap_stack_size / page_size;
    num_of_txt_pages = text_size / page_size;
    printf("Valid\t Dirty\t Frame\t Swap index\n");
    for (i = 0; i < num_of_txt_pages; i++) {
        printf("[%d]\t[%d]\t[%d]\t[%d]\n",
               page_table[0][i].valid,
               page_table[0][i].dirty,
               page_table[0][i].frame,
               page_table[0][i].swap_index);

    }
    printf("Valid\t Dirty\t Frame\t Swap index\n");
    for (i = 0; i < num_of_data_pages; i++) {
        printf("[%d]\t[%d]\t[%d]\t[%d]\n",
               page_table[1][i].valid,
               page_table[1][i].dirty,
               page_table[1][i].frame,
               page_table[1][i].swap_index);

    }
    printf("Valid\t Dirty\t Frame\t Swap index\n");
    for (i = 0; i < num_of_bss_pages; i++) {
        printf("[%d]\t[%d]\t[%d]\t[%d]\n",
               page_table[2][i].valid,
               page_table[2][i].dirty,
               page_table[2][i].frame,
               page_table[2][i].swap_index);

    }
    printf("Valid\t Dirty\t Frame\t Swap index\n");
    for (i = 0; i < num_of_stck_heap_pages; i++) {
        printf("[%d]\t[%d]\t[%d]\t[%d]\n",
               page_table[3][i].valid,
               page_table[3][i].dirty,
               page_table[3][i].frame,
               page_table[3][i].swap_index);


    }
}
sim_mem::~sim_mem(){
    //close all the files and free all the allocated memory
    close(swapfile_fd);
    close(this->program_fd);
    for(int i=0;i<4;i++){
        free(this->page_table[i]);

    }
    free(this->page_table);
    for(int i=0;i<4;i++){
        free(counter[i]);

    }
    free(counter);
    free(Swap);
    free(pageData);
    free(empty);
}

