#include <iostream>
#include <vector>
#include <map>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#define DISK_SIZE 512

// Function to convert decimal to binary char
char decToBinary(int n) {
    return static_cast<char>(n);
}

// #define SYS_CALL
// ============================================================================
class fsInode {
    int fileSize;
    int block_in_use;

    int directBlock1;
    int directBlock2;
    int directBlock3;

    int singleInDirect;
    int doubleInDirect;
    int block_size;


public:
    fsInode(int _block_size) {
        fileSize = 0;
        block_in_use = 0;
        block_size = _block_size;
        directBlock1 = -1;
        directBlock2 = -1;
        directBlock3 = -1;
        singleInDirect = -1;
        doubleInDirect = -1;

    }

    // YOUR CODE......
    int getfileSize(){
        return fileSize;
    }int getblock_in_use(){
        return block_in_use;
    }int getblock_size(){
        return block_size;
    }
    int getdirectBlock1(){
        return directBlock1;
    }
    int getdirectBlock2(){
        return directBlock2;
    }
    int getdirectBlock3(){
        return directBlock3;
    }
    int getsingleInDirect(){
        return singleInDirect;
    }
    int getdoubleInDirect(){
        return doubleInDirect;
    }
    void setfile_size(int _file_size){
        fileSize=_file_size;
    }void setblock_in_use(int _block_in_use){
        block_in_use=_block_in_use;
    }
    void setblock_size(int _block_size){
        block_size =_block_size;
    }
    void setdirectBlock1(int _directBlock1){
        directBlock1=_directBlock1;
    }
    void setdirectBlock2(int _directBlock2){
        directBlock2=_directBlock2;
    }
    void setdirectBlock3(int _directBlock3){
        directBlock3=_directBlock3;
    }
    void setsingleInDirect(int _singleInDirect){
        singleInDirect=_singleInDirect;
    }
    void setdoubleInDirect(int _doubleInDirect){
        doubleInDirect=_doubleInDirect;
    }


};

// ============================================================================
class FileDescriptor {
    pair<string, fsInode*> file;
    bool inUse;

public:
    FileDescriptor(string FileName, fsInode* fsi) {
        file.first = FileName;
        file.second = fsi;
        inUse = true;

    }


    string getFileName() {
        return file.first;
    }

    fsInode* getInode() {

        return file.second;

    }

    int GetFileSize() {
        return file.second->getfileSize();
    }
    bool isInUse() {
        return (inUse);
    }
    void setInUse(bool _inUse) {
        inUse = _inUse ;
    }
};

#define DISK_SIM_FILE "DISK_SIM_FILE.txt"
// ============================================================================
class fsDisk {
    FILE *sim_disk_fd;

    bool is_formated;
    // BitVector - "bit" (int) vector, indicate which block in the disk is free
    //              or not.  (i.e. if BitVector[0] == 1 , means that the
    //             first block is occupied.
    int BitVectorSize;
    int *BitVector;
    int blockSize;
    int numOfpointer1 = 0;
    int numOfpointer2 = 0;
    int numOfPointers = 0;
    // Unix directories are lists of association structures,
    // each of which contains one filename and one inode number.
    map<string, fsInode*>  MainDir ;

    // OpenFileDescriptors --  when you open a file,
    // the operating system creates an entry to represent that file
    // This entry number is the file descriptor.
    vector< FileDescriptor > OpenFileDescriptors;


public:
    // ------------------------------------------------------------------------
    fsDisk() {
        sim_disk_fd = fopen( DISK_SIM_FILE , "w+" );
        assert(sim_disk_fd);
        for (int i=0; i < DISK_SIZE ; i++) {
            int ret_val = fseek ( sim_disk_fd , i , SEEK_SET );
            ret_val = fwrite( "\0" ,  1 , 1, sim_disk_fd );
            assert(ret_val == 1);
        }
        fflush(sim_disk_fd);
    }

    ~fsDisk(){
        for (int i=0; i < DISK_SIZE ; i++) {
            fseek ( sim_disk_fd , i , SEEK_SET );
            fwrite( "\0" ,  1 , 1, sim_disk_fd );
        }
        free(BitVector);
        for (int i =0; i<BitVectorSize ; i++){
            BitVector[i]=0;
        }
        for (auto index1= MainDir.begin() ;index1!=MainDir.end();index1++){
            delete(MainDir[index1->first]);
        }
        fclose(sim_disk_fd);
    }

    // ------------------------------------------------------------------------
    void listAll() {
        int i = 0;
        for ( auto it = begin (OpenFileDescriptors); it != end (OpenFileDescriptors); ++it) {
            cout << "index: " << i << ": FileName: " << it->getFileName() <<  " , isInUse: "
                 << it->isInUse() << " file Size: " << it->GetFileSize() << endl;
            i++;
        }
        char bufy;
        cout << "Disk content: '" ;
        for (i=0; i < DISK_SIZE ; i++) {
            int ret_val = fseek ( sim_disk_fd , i , SEEK_SET );
            ret_val = fread(  &bufy , 1 , 1, sim_disk_fd );
            cout << bufy;
        }
        cout << "'" << endl;

    }

    // ------------------------------------------------------------------------
    void fsFormat( int blockSize =4) {
        if(blockSize>DISK_SIZE || blockSize<2){
            printf("Invalid block size ! \n");
        }
        else{
            OpenFileDescriptors.clear();
            for (int i=0; i < DISK_SIZE ; i++) {
                fseek ( sim_disk_fd , i , SEEK_SET );
                fwrite( "\0" ,  1 , 1, sim_disk_fd );
            }
            for (auto index1= MainDir.begin() ;index1!=MainDir.end();index1++){
                delete(MainDir[index1->first]);
            }
            MainDir.clear();
            this->blockSize =blockSize;
            delete [] BitVector;
            int numOfBlocks=DISK_SIZE/blockSize;
            BitVector=new int [numOfBlocks];
            BitVectorSize=numOfBlocks;
            for (int i=0; i < BitVectorSize ; i++) {
                BitVector[i]=0;
            }
            is_formated=true;
            cout<<"FORMAT DISK: number of blocks: "<<numOfBlocks<<endl;
        }

    }

    // ------------------------------------------------------------------------
    int CreateFile(string fileName) {
        for (auto index1= MainDir.begin() ;index1!=MainDir.end();index1++){
            if (index1->first == fileName){
                printf("The name is already used!\n");
                return -1;
            }
        }
        if(is_formated == false  ){
            printf("Is not formated yet!\n");
            return -1;
        }
        if (MainDir.find(fileName)!= MainDir.end()){
            printf("The file already exists!\n");
            return -1;
        }
        fsInode *fs_file=new fsInode(blockSize);
        FileDescriptor fd(fileName,fs_file);
        fd.setInUse(true);
        bool ki= false;
        for ( int i =0 ; i<OpenFileDescriptors.size();i++) {
            if (OpenFileDescriptors[i].isInUse() == false ) {
                ki = true;
                OpenFileDescriptors[i] = fd;
                break;
            }
        }
        if (ki == false){
                OpenFileDescriptors.push_back(fd);

        }
        MainDir.insert(make_pair(fileName,fs_file));
        int index =0;
        for (int i = 0 ; i< OpenFileDescriptors.size();i++){
            if (OpenFileDescriptors[i].getFileName()==fileName){
                index= i;
                break;
            }
        }
        return index ;
    }

    // ------------------------------------------------------------------------
    int OpenFile(string FileName ) {
        bool flag = false;
        for (int i = 0; i < OpenFileDescriptors.size(); i++) {
            if (OpenFileDescriptors[i].getFileName() == FileName) {
                if (OpenFileDescriptors[i].isInUse()==true) {
                    flag = true;
                }
            }
        }
        if(is_formated== false ){
            printf("Is not formated yet!\n");
            return -1;
        }
        if (MainDir.find(FileName)== MainDir.end()){
            printf("The file is not exist!\n");
            return -1;
        }
        if (flag== true){
            printf("The file is already Open!\n");
            return -1;
        }
        bool ki= false;
        FileDescriptor fd (FileName , MainDir[FileName]);
        for ( int i =0 ; i<OpenFileDescriptors.size();i++) {
            if (OpenFileDescriptors[i].isInUse() == false) {
                ki = true;
                OpenFileDescriptors[i] = fd;
                break;
            }
        }
        if (ki == false){
            OpenFileDescriptors.push_back(fd);
        }
        int index=0;
        for (int i = 0 ; i< OpenFileDescriptors.size();i++){
            if (OpenFileDescriptors[i].getFileName() == FileName){
                OpenFileDescriptors[i].setInUse(true);
                index = i;
                break;
            }
        }
        return index;
    }


    // ------------------------------------------------------------------------
    string CloseFile(int fd) {
        bool flag = false ;
        if (fd>OpenFileDescriptors.size()){
            printf("This file does not exist !\n");
            return "-1" ;
        }
        for (int i =0 ; i< OpenFileDescriptors.size(); i++){
            if (OpenFileDescriptors[fd].getFileName() == OpenFileDescriptors[i].getFileName()){
                if (OpenFileDescriptors[fd].isInUse()== true) {
                    flag = true;
                }
            }
        }
        if(is_formated== false ){
            printf("Is not formated yet!\n");
            return "-1";
        }
        if (flag == false){
            printf("The file is already close!\n");
            return "-1";
        }
        string fileName=OpenFileDescriptors[fd].getFileName();
        OpenFileDescriptors[fd].setInUse(false);
        return fileName;
    }
    // ------------------------------------------------------------------------
    int WriteToFile(int fd, char *buf, int len ) {
        bool flag = false;
        if (fd>OpenFileDescriptors.size()){
            printf("This file does not exist !\n");
            return -1 ;
        }
        for (int i = 0; i < OpenFileDescriptors.size(); i++) {
            if (OpenFileDescriptors[fd].getFileName() == OpenFileDescriptors[i].getFileName()) {
                if (OpenFileDescriptors[fd].isInUse() == true) {
                    flag = true;
                }
            }
        }
        int maxFileSize = (3 + blockSize  + blockSize * blockSize) * blockSize;
        if (is_formated == false) {
            printf("The file is not formated! \n");
            return -1;
        }
        if (flag == false) {
            printf("You cant write to closed file! \n");
            return -1;
        }
        if (OpenFileDescriptors[fd].getInode()->getfileSize() >= maxFileSize) {
            printf("The size of the file is the maxFileSize! \n");
            return -1;
        }
        if (len == 0) {
            printf("There is no something to write! \n");
            return -1;
        }
        FileDescriptor fileDes = OpenFileDescriptors[fd];
        if (len > (maxFileSize - fileDes.getInode()->getfileSize())) {
            len = maxFileSize - fileDes.getInode()->getfileSize();
        }
        int numOfNeededBlocks = len / blockSize;
        int numOfEmptyBlocks = 0;
        bool flagg = false;
        for (int i = 0; i < BitVectorSize; i++) {
            if (BitVector[i] == 0) {
                numOfEmptyBlocks++;
            }
        }

        if (numOfEmptyBlocks == 0) {
            return -1;
        }
        if (fileDes.getInode()->getfileSize() == 0) {//first
            if (len % blockSize != 0 && numOfNeededBlocks != 0) {
                numOfNeededBlocks++;
                flagg = true;
            }
            if (numOfNeededBlocks < 4 || numOfEmptyBlocks < 4) {
                if (numOfNeededBlocks == 0) { // len <blockesize
                    for (int i = 0; i < BitVectorSize; i++) {
                        if (BitVector[i] == 0) {
                            BitVector[i] = 1;
                            int m=0;
                            while(m<blockSize){
                                fseek(sim_disk_fd,(i*blockSize)+m,SEEK_SET);
                                fprintf(sim_disk_fd ,"%c",'\0');
                                m++;

                            }
                            fseek(sim_disk_fd, i * blockSize, SEEK_SET);
                            fwrite(buf, sizeof(char), len, sim_disk_fd);
                            numOfEmptyBlocks--;
                            int fileSize = fileDes.getInode()->getfileSize();
                            fileDes.getInode()->setfile_size(fileSize + len);
                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                            numOfBlocks++;
                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                            OpenFileDescriptors[fd].getInode()->setdirectBlock1(i);
                            return 1;
                            break;
                        }
                    }
                } else {
                    int j=0;
                    int num =0;
                    while (numOfNeededBlocks>0 && numOfEmptyBlocks>0){
                        for (int i = 0; i < BitVectorSize; i++) {
                            if (BitVector[i] == 0) {
                                BitVector[i] = 1;
                                int m=0;
                                while(m<blockSize){
                                    fseek(sim_disk_fd,(i*blockSize)+m,SEEK_SET);
                                    fprintf(sim_disk_fd ,"%c",'\0');
                                    m++;
                                }
                                if ((num == 0) && numOfEmptyBlocks > 0) {
                                    OpenFileDescriptors[fd].getInode()->setdirectBlock1(i);
                                }
                                if ((num == 1) && numOfEmptyBlocks > 0){
                                    OpenFileDescriptors[fd].getInode()->setdirectBlock2(i);
                                }
                                if ((num == 2 ) && numOfEmptyBlocks > 0){
                                    OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                }
                                num++;
                                fseek(sim_disk_fd, (i*blockSize), SEEK_SET);
                                FileDescriptor fileDes = OpenFileDescriptors[fd];
                                if (flagg == true && numOfNeededBlocks == 1) {
                                    for (int l = 0; l < len; l++) {
                                        fseek(sim_disk_fd, (i*blockSize)+l , SEEK_SET);
                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                    }
                                    int fileSize = fileDes.getInode()->getfileSize();
                                    fileDes.getInode()->setfile_size(fileSize + len);
                                } else {
                                    for (int l = 0; l < blockSize; l++) {
                                        fseek(sim_disk_fd, (i*blockSize)+l , SEEK_SET);
                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                    }
                                    len-=blockSize;
                                    int fileSize = fileDes.getInode()->getfileSize();
                                    fileDes.getInode()->setfile_size(fileSize + blockSize);
                                }
                                j += blockSize;
                                numOfNeededBlocks--;
                                numOfEmptyBlocks--;
                                int numOfBlocks =OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                numOfBlocks++;
                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                break;
                            }
                        }
                    }
                    return 1;
                }
            }
            if (numOfNeededBlocks > 3 && numOfNeededBlocks < (3 + blockSize + 1)) {
                int ind = numOfNeededBlocks - 3;
                int j = 0;
                int num =0;
                while (numOfNeededBlocks > ind && numOfEmptyBlocks > 0) {
                    for (int i = 0; i < BitVectorSize; i++) {
                        if (BitVector[i] == 0) {
                            BitVector[i] = 1;
                            int m=0;
                            while(m<blockSize){
                                fseek(sim_disk_fd,(i*blockSize)+m,SEEK_SET);
                                fprintf(sim_disk_fd ,"%c",'\0');
                                m++;
                            }
                            if ((num == 0) && numOfEmptyBlocks > 0) {
                                OpenFileDescriptors[fd].getInode()->setdirectBlock1(i);
                            }
                            if ((num == 1) && numOfEmptyBlocks > 0){
                                OpenFileDescriptors[fd].getInode()->setdirectBlock2(i);
                            }
                            if ((num == 2 ) && numOfEmptyBlocks > 0){
                                OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                            }
                            num++;
                            fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                            FileDescriptor fileDes = OpenFileDescriptors[fd];
                            if (flagg == true && numOfNeededBlocks == 1) {
                                for (int l = 0; l < len; l++) {
                                    fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                }
                                int fileSize = fileDes.getInode()->getfileSize();
                                fileDes.getInode()->setfile_size(fileSize + len);
                            } else {
                                for (int l = 0; l < blockSize; l++) {
                                    fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                }
                                len -= blockSize;
                                int fileSize = fileDes.getInode()->getfileSize();
                                fileDes.getInode()->setfile_size(fileSize + blockSize);
                            }
                            j += blockSize;
                            numOfNeededBlocks--;
                            numOfEmptyBlocks--;
                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                            numOfBlocks++;
                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                            break;
                        }
                    }
                }
                if (numOfEmptyBlocks > 1 && numOfNeededBlocks > 0) {
                    int numOfPointers = 0;
                    for (int i = 0; i < BitVectorSize; i++) {
                        if (BitVector[i] == 0) {
                            BitVector[i] = 1;
                            int m=0;
                            while(m<blockSize){
                                fseek(sim_disk_fd,(i*blockSize)+m,SEEK_SET);
                                fprintf(sim_disk_fd ,"%c",'\0');
                                m++;
                            }
                            numOfEmptyBlocks--;
                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                            numOfBlocks++;
                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                            OpenFileDescriptors[fd].getInode()->setsingleInDirect(i);
                            int z = 0;
                            while (numOfNeededBlocks > 0 && numOfPointers < blockSize && numOfEmptyBlocks > 0) {
                                for (int k = 0; k < BitVectorSize; k++) {
                                    if (BitVector[k] == 0) {
                                        BitVector[k] = 1;
                                        int m=0;
                                        while(m<blockSize){
                                            fseek(sim_disk_fd,(k*blockSize)+m,SEEK_SET);
                                            fprintf(sim_disk_fd ,"%c",'\0');
                                            m++;
                                        }
                                        char address = decToBinary(k);
                                        fseek(sim_disk_fd, (i * blockSize) + numOfPointers, SEEK_SET);
                                        fprintf(sim_disk_fd, "%c", address);
                                        numOfPointers++;
                                        numOfEmptyBlocks--;
                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                        numOfBlocks++;
                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                        FileDescriptor fileDes = OpenFileDescriptors[fd];
                                        if (flagg == true && numOfNeededBlocks == 1) {
                                            for (int l = 0; l < len; l++) {
                                                fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                            }
                                            int fileSize = fileDes.getInode()->getfileSize();
                                            fileDes.getInode()->setfile_size(fileSize + len);
                                        } else {
                                            for (int l = 0; l < blockSize; l++) {
                                                fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                            }
                                            len -= blockSize;
                                            int fileSize = fileDes.getInode()->getfileSize();
                                            fileDes.getInode()->setfile_size(fileSize + blockSize);
                                        }
                                        numOfNeededBlocks--;
                                        j += blockSize;
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                    }
                }
            }
            if (numOfNeededBlocks > 3 + blockSize &&
                numOfNeededBlocks < (3 + blockSize + (blockSize * blockSize) + 1)) {
                int ind = numOfNeededBlocks - 3;
                int j = 0;
                int num =0;
                while (numOfNeededBlocks > ind && numOfEmptyBlocks > 0) {
                    for (int i = 0; i < BitVectorSize; i++) {
                        if (BitVector[i] == 0) {
                            BitVector[i] = 1;
                            int m=0;
                            while(m<blockSize){
                                fseek(sim_disk_fd,(i*blockSize)+m,SEEK_SET);
                                fprintf(sim_disk_fd ,"%c",'\0');
                                m++;
                            }
                            if ((num == 0) && numOfEmptyBlocks > 0) {
                                OpenFileDescriptors[fd].getInode()->setdirectBlock1(i);
                            }
                            if ((num == 1) && numOfEmptyBlocks > 0){
                                OpenFileDescriptors[fd].getInode()->setdirectBlock2(i);
                            }
                            if ((num == 2 ) && numOfEmptyBlocks > 0){
                                OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                            }
                            num++;
                            fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                            FileDescriptor fileDes = OpenFileDescriptors[fd];
                            if (flagg == true && numOfNeededBlocks == 1) {
                                for (int l = 0; l < len; l++) {
                                    fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                }
                                int fileSize = fileDes.getInode()->getfileSize();
                                fileDes.getInode()->setfile_size(fileSize + len);
                            } else {
                                for (int l = 0; l < blockSize; l++) {
                                    fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                }
                                len -= blockSize;
                                int fileSize = fileDes.getInode()->getfileSize();
                                fileDes.getInode()->setfile_size(fileSize + blockSize);
                            }
                            j += blockSize;
                            numOfNeededBlocks--;
                            numOfEmptyBlocks--;
                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                            numOfBlocks++;
                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                            break;
                        }
                    }
                }
                if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 1) {
                    int numOfPointers = 0;
                    for (int i = 0; i < BitVectorSize; i++) {
                        if (BitVector[i] == 0) {
                            BitVector[i] = 1;
                            int z=0;
                            while(z<blockSize){
                                fseek(sim_disk_fd,(i*blockSize)+z,SEEK_SET);
                                fprintf(sim_disk_fd ,"%c",'\0');
                                z++;
                            }
                            numOfEmptyBlocks--;
                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                            numOfBlocks++;
                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                            OpenFileDescriptors[fd].getInode()->setsingleInDirect(i);
                            int m=0;
                            while (numOfNeededBlocks > 0 && numOfPointers < blockSize && numOfEmptyBlocks > 0) {
                                for (int k = 0; k < BitVectorSize; k++) {
                                    if (BitVector[k] == 0) {
                                        BitVector[k] = 1;
                                        while(m<blockSize){
                                            fseek(sim_disk_fd,(k*blockSize)+m,SEEK_SET);
                                            fprintf(sim_disk_fd ,"%c",'\0');
                                            m++;
                                        }
                                        char address = decToBinary(k);
                                        fseek(sim_disk_fd, (i * blockSize) + numOfPointers, SEEK_SET);
                                        fprintf(sim_disk_fd, "%c", address);
                                        numOfPointers++;
                                        numOfEmptyBlocks--;
                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                        numOfBlocks++;
                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                        FileDescriptor fileDes = OpenFileDescriptors[fd];
                                        if (flagg == true && numOfNeededBlocks == 1) {
                                            for (int l = 0; l < len; l++) {
                                                fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                            }
                                            int fileSize = fileDes.getInode()->getfileSize();
                                            fileDes.getInode()->setfile_size(fileSize + len);
                                        } else {
                                            for (int l = 0; l < blockSize; l++) {
                                                fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                            }
                                            len -= blockSize;
                                            int fileSize = fileDes.getInode()->getfileSize();
                                            fileDes.getInode()->setfile_size(fileSize + blockSize);
                                        }
                                        numOfNeededBlocks--;
                                        j += blockSize;
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                    }
                }
                if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 2) {
                    int numOfPointer1 = 0;
                    int numOfPointer2 = 0;
                    int r=0;
                    for (int i = 0; i < BitVectorSize; i++) {
                        if (BitVector[i] == 0) {
                            BitVector[i] = 1;
                            while(r<blockSize){
                                fseek(sim_disk_fd,(i*blockSize)+r,SEEK_SET);
                                fprintf(sim_disk_fd ,"%c",'\0');
                                r++;
                            }
                            numOfEmptyBlocks--;
                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                            numOfBlocks++;
                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                            OpenFileDescriptors[fd].getInode()->setdoubleInDirect(i);
                            int m=0;
                            while (numOfNeededBlocks > 0 && numOfPointer1 < blockSize && numOfEmptyBlocks > 0) {
                                for (int k = 0; k < BitVectorSize; k++) {
                                    if (BitVector[k] == 0) {
                                        numOfPointer2 = 0;
                                        BitVector[k] = 1;
                                        while(m<blockSize){
                                            fseek(sim_disk_fd,(k*blockSize)+m,SEEK_SET);
                                            fprintf(sim_disk_fd ,"%c",'\0');
                                            m++;
                                        }
                                        char address = decToBinary(k);
                                        fseek(sim_disk_fd, (i * blockSize) + numOfPointer1, SEEK_SET);
                                        fprintf(sim_disk_fd, "%c", address);
                                        numOfPointer1++;
                                        numOfEmptyBlocks--;
                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                        numOfBlocks++;
                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                        int w=0;
                                        while (numOfNeededBlocks > 0 && numOfPointer2 < blockSize && numOfEmptyBlocks > 0) {
                                            for (int q = 0; q < BitVectorSize; q++) {
                                                if (BitVector[q] == 0) {
                                                    BitVector[q] = 1;
                                                    while(w<blockSize){
                                                        fseek(sim_disk_fd,(q*blockSize)+w,SEEK_SET);
                                                        fprintf(sim_disk_fd ,"%c",'\0');
                                                        w++;
                                                    }
                                                    char address = decToBinary(q);
                                                    fseek(sim_disk_fd, (k * blockSize) + numOfPointer2, SEEK_SET);
                                                    fprintf(sim_disk_fd, "%c", address);
                                                    numOfPointer2++;
                                                    numOfEmptyBlocks--;
                                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                    numOfBlocks++;
                                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                    FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                    if (flagg == true && numOfNeededBlocks == 1) {
                                                        for (int l = 0; l < len; l++) {
                                                            fseek(sim_disk_fd, (q * blockSize) + l, SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                        }
                                                        int fileSize = fileDes.getInode()->getfileSize();
                                                        fileDes.getInode()->setfile_size(fileSize + len);
                                                    } else {
                                                        for (int l = 0; l < blockSize; l++) {
                                                            fseek(sim_disk_fd, (q * blockSize) + l, SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                        }
                                                        len -= blockSize;
                                                        int fileSize = fileDes.getInode()->getfileSize();
                                                        fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                    }
                                                    numOfNeededBlocks--;
                                                    j += blockSize;
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                    }

                }
            }
        } else {
            int direct1 = OpenFileDescriptors[fd].getInode()->getdirectBlock1();
            int ind = 0;
            bool check = false;
            while (ind < blockSize) {
                fseek(sim_disk_fd, (((direct1) * blockSize)) + ind, SEEK_SET);
                char character = fgetc(sim_disk_fd);
                int j = 0;
                if (character == ' ' || character == '\0') {
                    check = true;
                    while (len > 0 && ind < blockSize) {
                        fseek(sim_disk_fd, (direct1 * blockSize) + ind, SEEK_SET);
                        fprintf(sim_disk_fd, "%c", buf[j]);
                        j++;
                        ind++;
                        len--;
                        int fileSize = fileDes.getInode()->getfileSize();
                        fileSize++;
                        fileDes.getInode()->setfile_size(fileSize);
                    }
                    if (len > 0) {
                        int numOfNeededBlocks = len / blockSize;
                        int numOfEmptyBlocks = 0;
                        bool flagg = false;
                        for (int i = 0; i < BitVectorSize; i++) {
                            if (BitVector[i] == 0) {
                                numOfEmptyBlocks++;
                            }
                        }
                        if (len % blockSize != 0 && numOfNeededBlocks != 0) {
                            numOfNeededBlocks++;
                            flagg = true;
                        }
                        if (numOfNeededBlocks < 3 || numOfEmptyBlocks < 3) {
                            if (numOfNeededBlocks == 0) { // len <blockesize
                                for (int i = 0; i < BitVectorSize; i++) {
                                    if (BitVector[i] == 0) {
                                        BitVector[i] = 1;
                                        int m=0;
                                        int w=0;
                                        while(w<blockSize){
                                            fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                            fprintf(sim_disk_fd ,"%c",'\0');
                                            w++;
                                        }
                                        while( len >0 && m< blockSize) {
                                            fseek(sim_disk_fd, (i * blockSize)+m, SEEK_SET);
                                            fprintf(sim_disk_fd , "%c" , buf [j]);
                                            j++;
                                            m++;
                                            len--;
                                            int fileSize = fileDes.getInode()->getfileSize();
                                            fileSize++;
                                            fileDes.getInode()->setfile_size(fileSize);
                                        }
                                        numOfEmptyBlocks--;
                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                        numOfBlocks++;
                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                        OpenFileDescriptors[fd].getInode()->setdirectBlock2(i);
                                        return 1;
                                        break;
                                    }
                                }
                            } else {
                                int num =0;
                                int w =0;
                                while (numOfNeededBlocks > 0 && numOfEmptyBlocks > 0) {
                                    for (int i = 0; i < BitVectorSize; i++) {
                                        if (BitVector[i] == 0) {
                                            BitVector[i] = 1;
                                            while(w<blockSize){
                                                fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                w++;
                                            }
                                            if ((num == 0) && numOfEmptyBlocks > 0){
                                                OpenFileDescriptors[fd].getInode()->setdirectBlock2(i);
                                            }
                                            if ((num == 1 ) && numOfEmptyBlocks > 0){
                                                OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                            }
                                            num++;
                                            fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                                            FileDescriptor fileDes = OpenFileDescriptors[fd];
                                            int k = 0;
                                            if (flagg == true && numOfNeededBlocks == 1) {
                                                for (int l = 0; l < len; l++) {
                                                    fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                    fileSize++;
                                                    fileDes.getInode()->setfile_size(fileSize);
                                                }
                                            } else {
                                                for (int l = 0; l < blockSize; l++) {
                                                    fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                    fileSize++;
                                                    fileDes.getInode()->setfile_size(fileSize);
                                                }
                                                len -= blockSize;
                                                k++;
                                            }
                                            j +=  blockSize;
                                            numOfNeededBlocks--;
                                            numOfEmptyBlocks--;
                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                            numOfBlocks++;
                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                            break;
                                        }
                                    }
                                }
                                return 1;
                            }
                        }
                        if (numOfNeededBlocks > 2 && numOfNeededBlocks < (3 + blockSize)) {
                            int ind = numOfNeededBlocks - 2;
                            int num =0;
                            int w=0;
                            while (numOfNeededBlocks > ind && numOfEmptyBlocks > 0) {
                                for (int i = 0; i < BitVectorSize; i++) {
                                    if (BitVector[i] == 0) {
                                        BitVector[i] = 1;
                                        while(w<blockSize){
                                            fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                            fprintf(sim_disk_fd ,"%c",'\0');
                                            w++;
                                        }
                                        if ((num ==0) && numOfEmptyBlocks > 0) {
                                            OpenFileDescriptors[fd].getInode()->setdirectBlock2(i);
                                        }
                                        if ((num ==1) && numOfEmptyBlocks > 0) {
                                            OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                        }
                                        num++;
                                        fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                                        FileDescriptor fileDes = OpenFileDescriptors[fd];
                                        int k = 0;
                                        if (flagg == true && numOfNeededBlocks == 1) {
                                            for (int l = 0; l < len; l++) {
                                                fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                            }
                                            int fileSize = fileDes.getInode()->getfileSize();
                                            fileDes.getInode()->setfile_size(fileSize + len);
                                        } else {
                                            for (int l = 0; l < blockSize; l++) {
                                                fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                            }
                                            len -= blockSize;
                                            k++;
                                            int fileSize = fileDes.getInode()->getfileSize();
                                            fileDes.getInode()->setfile_size(fileSize + blockSize);
                                        }
                                        j += blockSize;
                                        numOfNeededBlocks--;
                                        numOfEmptyBlocks--;
                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                        numOfBlocks++;
                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                        break;
                                    }
                                }
                            }
                            if (numOfEmptyBlocks > 1 && numOfNeededBlocks > 0) {
                                int numOfPointers = 0;
                                int w=0;
                                for (int i = 0; i < BitVectorSize; i++) {
                                    if (BitVector[i] == 0) {
                                        BitVector[i] = 1;
                                        while(w<blockSize){
                                            fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                            fprintf(sim_disk_fd ,"%c",'\0');
                                            w++;
                                        }
                                        numOfEmptyBlocks--;
                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                        numOfBlocks++;
                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                        OpenFileDescriptors[fd].getInode()->setsingleInDirect(i);
                                        int z = 0;
                                        int b=0;
                                        while (numOfNeededBlocks > 0 && numOfPointers < blockSize &&
                                               numOfEmptyBlocks > 0) {
                                            for (int k = 0; k < BitVectorSize; k++) {
                                                if (BitVector[k] == 0) {
                                                    BitVector[k] = 1;
                                                    while(b<blockSize){
                                                        fseek(sim_disk_fd,(k  * blockSize)+ b,SEEK_SET);
                                                        fprintf(sim_disk_fd ,"%c",'\0');
                                                        b++;
                                                    }
                                                    char address = decToBinary(k);
                                                    fseek(sim_disk_fd, (i * blockSize) + numOfPointers, SEEK_SET);
                                                    fprintf(sim_disk_fd, "%c", address);
                                                    numOfPointers++;
                                                    numOfEmptyBlocks--;
                                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                    numOfBlocks++;
                                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                    FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                    if (flagg == true && numOfNeededBlocks == 1) {
                                                        for (int l = 0; l < len; l++) {
                                                            fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                        }
                                                        int fileSize = fileDes.getInode()->getfileSize();
                                                        fileDes.getInode()->setfile_size(fileSize + len);
                                                    } else {
                                                        for (int l = 0; l < blockSize; l++) {
                                                            fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                        }
                                                        z++;
                                                        len -= blockSize;
                                                        int fileSize = fileDes.getInode()->getfileSize();
                                                        fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                    }
                                                    numOfNeededBlocks--;
                                                    j += blockSize;
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                        if (numOfNeededBlocks > 2 + blockSize &&
                            numOfNeededBlocks < (3 + blockSize + (blockSize * blockSize))) {
                            int ind = numOfNeededBlocks - 2;
                            int num =0;
                            int w=0;
                            while (numOfNeededBlocks > ind && numOfEmptyBlocks > 0) {
                                for (int i = 0; i < BitVectorSize; i++) {
                                    if (BitVector[i] == 0) {
                                        BitVector[i] = 1;
                                        while(w<blockSize){
                                            fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                            fprintf(sim_disk_fd ,"%c",'\0');
                                            w++;
                                        }
                                        if ((num==0) &&numOfEmptyBlocks > 0) {
                                            OpenFileDescriptors[fd].getInode()->setdirectBlock2(i);
                                        }
                                        if ((num == 1) &&
                                            numOfEmptyBlocks > 0) {
                                            OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                        }
                                        num++;
                                        fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                                        FileDescriptor fileDes = OpenFileDescriptors[fd];
                                        if (flagg == true && numOfNeededBlocks == 1) {
                                            for (int l = 0; l < len; l++) {
                                                fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                            }
                                            int fileSize = fileDes.getInode()->getfileSize();
                                            fileDes.getInode()->setfile_size(fileSize + len);
                                        } else {
                                            for (int l = 0; l < blockSize; l++) {
                                                fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                            }
                                            len -= blockSize;
                                            int fileSize = fileDes.getInode()->getfileSize();
                                            fileDes.getInode()->setfile_size(fileSize + blockSize);
                                        }
                                        j += blockSize;
                                        numOfNeededBlocks--;
                                        numOfEmptyBlocks--;
                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                        numOfBlocks++;
                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                        break;
                                    }
                                }
                            }
                            if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 1) {
                                int numOfPointers = 0;
                                for (int i = 0; i < BitVectorSize; i++) {
                                    if (BitVector[i] == 0) {
                                        BitVector[i] = 1;
                                        int w=0;
                                        while(w<blockSize){
                                            fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                            fprintf(sim_disk_fd ,"%c",'\0');
                                            w++;
                                        }
                                        numOfEmptyBlocks--;
                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                        numOfBlocks++;
                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                        OpenFileDescriptors[fd].getInode()->setsingleInDirect(i);
                                        int b=0;
                                        while (numOfNeededBlocks > 0 && numOfPointers < blockSize && numOfEmptyBlocks > 0) {
                                            for (int k = 0; k < BitVectorSize; k++) {
                                                if (BitVector[k] == 0) {
                                                    BitVector[k] = 1;
                                                    while(b<blockSize){
                                                        fseek(sim_disk_fd,(k  * blockSize)+ b,SEEK_SET);
                                                        fprintf(sim_disk_fd ,"%c",'\0');
                                                        b++;
                                                    }
                                                    char address = decToBinary(k);
                                                    fseek(sim_disk_fd, (i * blockSize) + numOfPointers, SEEK_SET);
                                                    fprintf(sim_disk_fd, "%c", address);
                                                    numOfPointers++;
                                                    numOfEmptyBlocks--;
                                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                    numOfBlocks++;
                                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                    FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                    if (flagg == true && numOfNeededBlocks == 1) {
                                                        for (int l = 0; l < len; l++) {
                                                            fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                        }
                                                        int fileSize = fileDes.getInode()->getfileSize();
                                                        fileDes.getInode()->setfile_size(fileSize + len);
                                                    } else {
                                                        for (int l = 0; l < blockSize; l++) {
                                                            fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                        }
                                                        len -= blockSize;
                                                        int fileSize = fileDes.getInode()->getfileSize();
                                                        fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                    }
                                                    numOfNeededBlocks--;
                                                    j += blockSize;
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                            if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 2) {
                                int numOfPointer1 = 0;
                                int numOfPointer2 = 0;
                                int w=0;
                                for (int i = 0; i < BitVectorSize; i++) {
                                    if (BitVector[i] == 0) {
                                        BitVector[i] = 1;
                                        while(w<blockSize){
                                            fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                            fprintf(sim_disk_fd ,"%c",'\0');
                                            w++;
                                        }
                                        numOfEmptyBlocks--;
                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                        numOfBlocks++;
                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                        OpenFileDescriptors[fd].getInode()->setdoubleInDirect(i);
                                        int b=0;
                                        while (numOfNeededBlocks > 0 && numOfPointer1 < blockSize && numOfEmptyBlocks > 0) {
                                            for (int k = 0; k < BitVectorSize; k++) {
                                                if (BitVector[k] == 0) {
                                                    numOfPointer2 = 0;
                                                    BitVector[k] = 1;
                                                    while(b<blockSize){
                                                        fseek(sim_disk_fd,(k * blockSize)+ b,SEEK_SET);
                                                        fprintf(sim_disk_fd ,"%c",'\0');
                                                        b++;
                                                    }
                                                    char address = decToBinary(k);
                                                    fseek(sim_disk_fd, (i * blockSize) + numOfPointer1, SEEK_SET);
                                                    fprintf(sim_disk_fd, "%c", address);
                                                    numOfPointer1++;
                                                    numOfEmptyBlocks--;
                                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                    numOfBlocks++;
                                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                    int x=0;
                                                    while (numOfNeededBlocks > 0 && numOfPointer2 < blockSize && numOfEmptyBlocks > 0) {
                                                        for (int q = 0; q < BitVectorSize; q++) {
                                                            if (BitVector[q] == 0) {
                                                                BitVector[q] = 1;
                                                                while(x<blockSize){
                                                                    fseek(sim_disk_fd,(q  * blockSize)+ x,SEEK_SET);
                                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                                    x++;
                                                                }
                                                                char address = decToBinary(q);
                                                                fseek(sim_disk_fd, (k * blockSize) + numOfPointer2,SEEK_SET);
                                                                fprintf(sim_disk_fd, "%c", address);
                                                                numOfPointer2++;
                                                                numOfEmptyBlocks--;
                                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                                numOfBlocks++;
                                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                                FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                                if (flagg == true && numOfNeededBlocks == 1) {
                                                                    for (int l = 0; l < len; l++) {
                                                                        fseek(sim_disk_fd, (q * blockSize) + l,SEEK_SET);
                                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                    }
                                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                                    fileDes.getInode()->setfile_size(fileSize + len);
                                                                } else {
                                                                    for (int l = 0; l < blockSize; l++) {
                                                                        fseek(sim_disk_fd, (q * blockSize) + l,SEEK_SET);
                                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                    }
                                                                    len -= blockSize;
                                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                                    fileDes.getInode()->setfile_size(
                                                                            fileSize + blockSize);
                                                                }
                                                                numOfNeededBlocks--;
                                                                j += blockSize;
                                                                break;
                                                            }
                                                        }
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
                ind++;
            }
            if (check == false) {
                int direct2 = OpenFileDescriptors[fd].getInode()->getdirectBlock2();
                if (direct2 != -1) {
                    ind = 0;
                    while (ind < blockSize) {
                        fseek(sim_disk_fd, ((direct2 * blockSize)) + ind, SEEK_SET);
                        char character = fgetc(sim_disk_fd);
                        int j = 0;
                        if (character == ' ' || character == '\0') {
                            check = true;
                            while (len > 0 && ind < blockSize) {
                                fseek(sim_disk_fd, ((direct2 * blockSize)) + ind, SEEK_SET);
                                fprintf(sim_disk_fd, "%c", buf[j]);
                                j++;
                                ind++;
                                len--;
                                int fileSize = fileDes.getInode()->getfileSize();
                                fileSize++;
                                fileDes.getInode()->setfile_size(fileSize);
                            }
                            if (len > 0) {
                                int numOfNeededBlocks = len / blockSize;
                                int numOfEmptyBlocks = 0;
                                bool flagg = false;
                                for (int i = 0; i < BitVectorSize; i++) {
                                    if (BitVector[i] == 0) {
                                        numOfEmptyBlocks++;
                                    }
                                }
                                if (len % blockSize != 0 && numOfNeededBlocks != 0) {
                                    numOfNeededBlocks++;
                                    flagg = true;
                                }
                                if (numOfNeededBlocks < 2 || numOfEmptyBlocks < 2) {
                                    if (numOfNeededBlocks == 0) { // len <blockesize
                                        for (int i = 0; i < BitVectorSize; i++) {
                                            if (BitVector[i] == 0) {
                                                BitVector[i] = 1;
                                                int w=0;
                                                while(w<blockSize){
                                                    fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                    w++;
                                                }
                                                int m=0;
                                                while( len >0 && m < blockSize) {
                                                    fseek(sim_disk_fd, (i * blockSize)+m, SEEK_SET);
                                                    fprintf(sim_disk_fd , "%c" , buf [j]);
                                                    j++;
                                                    m++;
                                                    len--;
                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                    fileSize++;
                                                    fileDes.getInode()->setfile_size(fileSize);
                                                }
                                                numOfEmptyBlocks--;
                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                numOfBlocks++;
                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                                return 1;
                                                break;
                                            }
                                        }
                                    }
                                    else {
                                        if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 0) {
                                            for (int i = 0; i < BitVectorSize; i++) {
                                                if (BitVector[i] == 0) {
                                                    BitVector[i] = 1;
                                                    int w =0;
                                                    while(w<blockSize){
                                                        fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                        fprintf(sim_disk_fd ,"%c",'\0');
                                                        w++;
                                                    }
                                                    OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                                    fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                                                    FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                    int k = 0;
                                                    if (flagg == true && numOfNeededBlocks == 1) {
                                                        for (int l = 0; l < len; l++) {
                                                            fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                        }
                                                        int fileSize = fileDes.getInode()->getfileSize();
                                                        fileDes.getInode()->setfile_size(fileSize + len);
                                                    } else {
                                                        for (int l = 0; l < blockSize; l++) {
                                                            fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                        }
                                                        len -= blockSize;
                                                        k++;
                                                        int fileSize = fileDes.getInode()->getfileSize();
                                                        fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                    }
                                                    j += blockSize;
                                                    numOfNeededBlocks--;
                                                    numOfEmptyBlocks--;
                                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                    numOfBlocks++;
                                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                    break;
                                                }
                                            }
                                        }
                                        return 1;
                                    }
                                }
                                if (numOfNeededBlocks > 2 && numOfNeededBlocks < (2 + blockSize)) {
                                    int ind = numOfNeededBlocks - 1;
                                    if (numOfNeededBlocks > ind && numOfEmptyBlocks > 0) {
                                        for (int i = 0; i < BitVectorSize; i++) {
                                            if (BitVector[i] == 0) {
                                                BitVector[i] = 1;
                                                int w=0;
                                                while(w<blockSize){
                                                    fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                    w++;
                                                }
                                                OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                                fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                                                FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                int k = 0;
                                                if (flagg == true && numOfNeededBlocks == 1) {
                                                    for (int l = 0; l < len; l++) {
                                                        fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                    }
                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                    fileDes.getInode()->setfile_size(fileSize + len);
                                                } else {
                                                    for (int l = 0; l < blockSize; l++) {
                                                        fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                    }
                                                    len -= blockSize;
                                                    k++;
                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                    fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                }
                                                j +=  blockSize;
                                                numOfNeededBlocks--;
                                                numOfEmptyBlocks--;
                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                numOfBlocks++;
                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                break;
                                            }
                                        }
                                    }
                                    if (numOfEmptyBlocks > 1 && numOfNeededBlocks > 0) {
                                        int numOfPointers = 0;
                                        for (int i = 0; i < BitVectorSize; i++) {
                                            if (BitVector[i] == 0) {
                                                BitVector[i] = 1;
                                                int w=0;
                                                while(w<blockSize){
                                                    fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                    w++;
                                                }
                                                numOfEmptyBlocks--;
                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                numOfBlocks++;
                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                OpenFileDescriptors[fd].getInode()->setsingleInDirect(i);
                                                int z = 0;
                                                int b=0;
                                                while (numOfNeededBlocks > 0 && numOfPointers < blockSize &&
                                                       numOfEmptyBlocks > 0) {
                                                    for (int k = 0; k < BitVectorSize; k++) {
                                                        if (BitVector[k] == 0) {
                                                            BitVector[k] = 1;
                                                            while(b<blockSize){
                                                                fseek(sim_disk_fd,(i  * blockSize)+ b,SEEK_SET);
                                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                                b++;
                                                            }
                                                            char address = decToBinary(k);
                                                            fseek(sim_disk_fd, (i * blockSize) + numOfPointers,SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", address);
                                                            numOfPointers++;
                                                            numOfEmptyBlocks--;
                                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                            numOfBlocks++;
                                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                            FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                            if (flagg == true && numOfNeededBlocks == 1) {
                                                                for (int l = 0; l < len; l++) {
                                                                    fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                }
                                                                int fileSize = fileDes.getInode()->getfileSize();
                                                                fileDes.getInode()->setfile_size(fileSize + len);
                                                            } else {
                                                                for (int l = 0; l < blockSize; l++) {
                                                                    fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                }
                                                                z++;
                                                                len -= blockSize;
                                                                int fileSize = fileDes.getInode()->getfileSize();
                                                                fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                            }
                                                            numOfNeededBlocks--;
                                                            j +=  blockSize;
                                                            break;
                                                        }
                                                    }
                                                }
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (numOfNeededBlocks > 1 + blockSize && numOfNeededBlocks < (1 + blockSize + (blockSize * blockSize)+1)) {
                                    int ind = numOfNeededBlocks - 1;
                                    if (numOfNeededBlocks > ind && numOfEmptyBlocks > 0) {
                                        for (int i = 0; i < BitVectorSize; i++) {
                                            if (BitVector[i] == 0) {
                                                BitVector[i] = 1;
                                                int w=0;
                                                while(w<blockSize){
                                                    fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                    w++;
                                                }
                                                OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                                fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                                                FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                if (flagg == true && numOfNeededBlocks == 1) {
                                                    for (int l = 0; l < len; l++) {
                                                        fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                    }
                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                    fileDes.getInode()->setfile_size(fileSize + len);
                                                } else {
                                                    for (int l = 0; l < blockSize; l++) {
                                                        fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                    }
                                                    len -= blockSize;
                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                    fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                }
                                                j += blockSize;
                                                numOfNeededBlocks--;
                                                numOfEmptyBlocks--;
                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                numOfBlocks++;
                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                break;
                                            }
                                        }
                                    }
                                    if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 1) {
                                        int numOfPointers = 0;
                                        for (int i = 0; i < BitVectorSize; i++) {
                                            if (BitVector[i] == 0) {
                                                BitVector[i] = 1;
                                                int w=0;
                                                while(w<blockSize){
                                                    fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                    w++;
                                                }
                                                numOfEmptyBlocks--;
                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                numOfBlocks++;
                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                OpenFileDescriptors[fd].getInode()->setsingleInDirect(i);
                                                int b=0;
                                                while (numOfNeededBlocks > 0 && numOfPointers < blockSize && numOfEmptyBlocks > 0) {
                                                    for (int k = 0; k < BitVectorSize; k++) {
                                                        if (BitVector[k] == 0) {
                                                            BitVector[k] = 1;
                                                            while(w<blockSize){
                                                                fseek(sim_disk_fd,(k  * blockSize)+ w,SEEK_SET);
                                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                                w++;
                                                            }
                                                            char address = decToBinary(k);
                                                            fseek(sim_disk_fd, (i * blockSize) + numOfPointers,SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", address);
                                                            numOfPointers++;
                                                            numOfEmptyBlocks--;
                                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                            numOfBlocks++;
                                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                            FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                            if (flagg == true && numOfNeededBlocks == 1) {
                                                                for (int l = 0; l < len; l++) {
                                                                    fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                }
                                                                int fileSize = fileDes.getInode()->getfileSize();
                                                                fileDes.getInode()->setfile_size(fileSize + len);
                                                            } else {
                                                                for (int l = 0; l < blockSize; l++) {
                                                                    fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                }
                                                                len -= blockSize;
                                                                int fileSize = fileDes.getInode()->getfileSize();
                                                                fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                            }
                                                            numOfNeededBlocks--;
                                                            j += blockSize;
                                                            break;
                                                        }
                                                    }
                                                }
                                                break;
                                            }
                                        }
                                    }
                                    if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 2) {
                                        int numOfPointer1 = 0;
                                        int numOfPointer2 = 0;
                                        for (int i = 0; i < BitVectorSize; i++) {
                                            if (BitVector[i] == 0) {
                                                BitVector[i] = 1;
                                                int w=0;
                                                while(w<blockSize){
                                                    fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                    w++;
                                                }
                                                numOfEmptyBlocks--;
                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                numOfBlocks++;
                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                OpenFileDescriptors[fd].getInode()->setdoubleInDirect(i);
                                                int b=0;
                                                while (numOfNeededBlocks > 0 && numOfPointer1 < blockSize && numOfEmptyBlocks > 0) {
                                                    for (int k = 0; k < BitVectorSize; k++) {
                                                        if (BitVector[k] == 0) {
                                                            numOfPointer2 = 0;
                                                            BitVector[k] = 1;
                                                            while(b<blockSize){
                                                                fseek(sim_disk_fd,(k  * blockSize)+ b,SEEK_SET);
                                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                                b++;
                                                            }
                                                            char address = decToBinary(k);
                                                            fseek(sim_disk_fd, (i * blockSize) + numOfPointer1,SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", address);
                                                            numOfPointer1++;
                                                            numOfEmptyBlocks--;
                                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                            numOfBlocks++;
                                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                            int x=0;
                                                            while (numOfNeededBlocks > 0 && numOfPointer2 < blockSize && numOfEmptyBlocks > 0) {
                                                                for (int q = 0; q < BitVectorSize; q++) {
                                                                    if (BitVector[q] == 0) {
                                                                        BitVector[q] = 1;
                                                                        while(x<blockSize){
                                                                            fseek(sim_disk_fd,(q * blockSize)+ x,SEEK_SET);
                                                                            fprintf(sim_disk_fd ,"%c",'\0');
                                                                            x++;
                                                                        }
                                                                        char address = decToBinary(q);
                                                                        fseek(sim_disk_fd,(k * blockSize) + numOfPointer2,SEEK_SET);
                                                                        fprintf(sim_disk_fd, "%c", address);
                                                                        numOfPointer2++;
                                                                        numOfEmptyBlocks--;
                                                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                                        numOfBlocks++;
                                                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                                        FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                                        if (flagg == true && numOfNeededBlocks == 1) {
                                                                            for (int l = 0; l < len; l++) {
                                                                                fseek(sim_disk_fd, (q * blockSize) + l,SEEK_SET);
                                                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                            }
                                                                            int fileSize = fileDes.getInode()->getfileSize();
                                                                            fileDes.getInode()->setfile_size(fileSize + len);
                                                                        } else {
                                                                            for (int l = 0; l < blockSize; l++) {
                                                                                fseek(sim_disk_fd, (q * blockSize) + l,SEEK_SET);
                                                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                            }
                                                                            len -= blockSize;
                                                                            int fileSize = fileDes.getInode()->getfileSize();
                                                                            fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                                        }
                                                                        numOfNeededBlocks--;
                                                                        j += blockSize;
                                                                        break;
                                                                    }
                                                                }
                                                            }
                                                            break;
                                                        }
                                                    }
                                                }
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        ind++;
                    }
                    if (check == false){
                        int direct3 = OpenFileDescriptors[fd].getInode()->getdirectBlock3();
                        if (direct3 != -1){
                            ind = 0;
                            while (ind < blockSize) {
                                fseek(sim_disk_fd, ((direct3 * blockSize)) + ind, SEEK_SET);
                                char character = fgetc(sim_disk_fd);
                                int j = 0;
                                if (character == ' ' || character == '\0') {
                                    check = true;
                                    while (len > 0 && ind < blockSize) {
                                        fseek(sim_disk_fd, ((direct3 * blockSize)) + ind, SEEK_SET);
                                        fprintf(sim_disk_fd, "%c", buf[j]);
                                        j++;
                                        ind++;
                                        len--;
                                        int fileSize = fileDes.getInode()->getfileSize();
                                        fileSize++;
                                        fileDes.getInode()->setfile_size(fileSize);
                                    }
                                    if (len > 0) {
                                        int numOfNeededBlocks = len / blockSize;
                                        int numOfEmptyBlocks = 0;
                                        bool flagg = false;
                                        for (int i = 0; i < BitVectorSize; i++) {
                                            if (BitVector[i] == 0) {
                                                numOfEmptyBlocks++;
                                            }
                                        }
                                        if (len % blockSize != 0 ) {
                                            numOfNeededBlocks++;
                                            flagg = true;
                                        }
                                        if (numOfEmptyBlocks > 1 && numOfNeededBlocks > 0) {
                                            int numOfPointers = 0;
                                            int w=0;
                                            for (int i = 0; i < BitVectorSize; i++) {
                                                if (BitVector[i] == 0) {
                                                    BitVector[i] = 1;
                                                    while(w<blockSize){
                                                        fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                        fprintf(sim_disk_fd ,"%c",'\0');
                                                        w++;
                                                    }
                                                    numOfEmptyBlocks--;
                                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                    numOfBlocks++;
                                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                    OpenFileDescriptors[fd].getInode()->setsingleInDirect(i);
                                                    int z = 0;
                                                    int b=0;
                                                    while (numOfNeededBlocks > 0 && numOfPointers < blockSize && numOfEmptyBlocks > 0) {
                                                        for (int k = 0; k < BitVectorSize; k++) {
                                                            if (BitVector[k] == 0) {
                                                                BitVector[k] = 1;
                                                                while(b<blockSize){
                                                                    fseek(sim_disk_fd,(k  * blockSize)+ b,SEEK_SET);
                                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                                    b++;
                                                                }
                                                                char address = decToBinary(k);
                                                                fseek(sim_disk_fd, (i * blockSize) + numOfPointers,SEEK_SET);
                                                                fprintf(sim_disk_fd, "%c", address);
                                                                numOfPointers++;
                                                                numOfEmptyBlocks--;
                                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                                numOfBlocks++;
                                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                                FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                                if (flagg == true && numOfNeededBlocks == 1) {
                                                                    for (int l = 0; l < len; l++) {
                                                                        fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                    }
                                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                                    fileDes.getInode()->setfile_size(fileSize + len);
                                                                } else {
                                                                    for (int l = 0; l < blockSize; l++) {
                                                                        fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                    }
                                                                    len -= blockSize;
                                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                                    fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                                }
                                                                numOfNeededBlocks--;
                                                                j += blockSize;
                                                                break;
                                                            }
                                                        }
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                        if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 2) {
                                            int numOfPointer1 = 0;
                                            int numOfPointer2 = 0;
                                            int w=0;
                                            for (int i = 0; i < BitVectorSize; i++) {
                                                if (BitVector[i] == 0) {
                                                    BitVector[i] = 1;
                                                    while(w<blockSize){
                                                        fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                        fprintf(sim_disk_fd ,"%c",'\0');
                                                        w++;
                                                    }
                                                    numOfEmptyBlocks--;
                                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                    numOfBlocks++;
                                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                    OpenFileDescriptors[fd].getInode()->setdoubleInDirect(i);
                                                    int b=0;
                                                    while (numOfNeededBlocks > 0 && numOfPointer1 < blockSize && numOfEmptyBlocks > 0) {
                                                        for (int k = 0; k < BitVectorSize; k++) {
                                                            if (BitVector[k] == 0) {
                                                                numOfPointer2 = 0;
                                                                BitVector[k] = 1;
                                                                while(b<blockSize){
                                                                    fseek(sim_disk_fd,(k  * blockSize)+ b,SEEK_SET);
                                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                                    b++;
                                                                }
                                                                char address = decToBinary(k);
                                                                fseek(sim_disk_fd, (i * blockSize) + numOfPointer1,SEEK_SET);
                                                                fprintf(sim_disk_fd, "%c", address);
                                                                numOfPointer1++;
                                                                numOfEmptyBlocks--;
                                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                                numOfBlocks++;
                                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                                int x=0;
                                                                while (numOfNeededBlocks > 0 && numOfPointer2 < blockSize && numOfEmptyBlocks > 0) {
                                                                    for (int q = 0; q < BitVectorSize; q++) {
                                                                        if (BitVector[q] == 0) {
                                                                            BitVector[q] = 1;
                                                                            while(x<blockSize){
                                                                                fseek(sim_disk_fd,(q  * blockSize)+ x,SEEK_SET);
                                                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                                                x++;
                                                                            }
                                                                            char address = decToBinary(q);
                                                                            fseek(sim_disk_fd,(k * blockSize) + numOfPointer2,SEEK_SET);
                                                                            fprintf(sim_disk_fd, "%c", address);
                                                                            numOfPointer2++;
                                                                            numOfEmptyBlocks--;
                                                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                                            numOfBlocks++;
                                                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                                            FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                                            if (flagg == true && numOfNeededBlocks == 1) {
                                                                                for (int l = 0; l < len; l++) {
                                                                                    fseek(sim_disk_fd, (q * blockSize) + l,SEEK_SET);
                                                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                                }
                                                                                int fileSize = fileDes.getInode()->getfileSize();
                                                                                fileDes.getInode()->setfile_size(fileSize + len);
                                                                            } else {
                                                                                for (int l = 0; l < blockSize; l++) {
                                                                                    fseek(sim_disk_fd, (q * blockSize) + l,SEEK_SET);
                                                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                                }
                                                                                len -= blockSize;
                                                                                int fileSize = fileDes.getInode()->getfileSize();
                                                                                fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                                            }
                                                                            numOfNeededBlocks--;
                                                                            j += blockSize;
                                                                            break;
                                                                        }
                                                                    }
                                                                }
                                                                break;
                                                            }
                                                        }
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    break;
                                }
                                ind++;
                            }
                            if(check == false){
                                int single = OpenFileDescriptors[fd].getInode()->getsingleInDirect();
                                if (single != -1){
                                   ind=0;
                                   int j=0;
                                   while (ind < blockSize && len >0) {
                                       numOfNeededBlocks =len/blockSize;
                                       if (len % blockSize !=0 || numOfNeededBlocks ==0){
                                           numOfNeededBlocks++;
                                           flagg=true;
                                       }
                                       fseek(sim_disk_fd, (single * blockSize) + ind, SEEK_SET);
                                       char character = fgetc(sim_disk_fd);
                                       int value = (int)character;
                                       if (character == ' ' || character == '\0') {
                                           check = true;
                                           while (len > 0 && ind < blockSize) {
                                               for (int k = 0; k < BitVectorSize; k++) {
                                                   if (BitVector[k] == 0) {
                                                       BitVector[k] = 1;
                                                       char address = decToBinary(k);
                                                       fseek(sim_disk_fd, (single * blockSize) + ind, SEEK_SET);
                                                       fprintf(sim_disk_fd, "%c", address);
                                                       ind++;
                                                       numOfEmptyBlocks--;
                                                       int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                       numOfBlocks++;
                                                       OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                       FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                       if (flagg == true && numOfNeededBlocks == 1) {
                                                           for (int l = 0; l < len; l++) {
                                                               fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                               fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                           }

                                                           int fileSize = fileDes.getInode()->getfileSize();
                                                           fileDes.getInode()->setfile_size(fileSize + len);
                                                           len = 0;
                                                       } else {
                                                           for (int l = 0; l < blockSize; l++) {
                                                               fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                               fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                           }
                                                           len -= blockSize;
                                                           int fileSize = fileDes.getInode()->getfileSize();
                                                           fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                       }
                                                       numOfNeededBlocks--;
                                                       j += blockSize;
                                                       break;
                                                   }
                                               }
                                           }
                                           break;
                                       } else {
                                           int m = 0;
                                           while (m < blockSize && len>0) {
                                               fseek(sim_disk_fd, (value * blockSize) + m, SEEK_SET);
                                               character = fgetc(sim_disk_fd);
                                               if (character == ' ' || character == '\0') {
                                                   check = true;
                                                   while (m < blockSize && len > 0) {
                                                       fseek(sim_disk_fd, (value * blockSize) + m, SEEK_SET);
                                                       fprintf(sim_disk_fd, "%c", buf[j]);
                                                       j++;
                                                       len--;
                                                       m++;
                                                       int fileSize = fileDes.getInode()->getfileSize();
                                                       fileSize++;
                                                       fileDes.getInode()->setfile_size(fileSize);
                                                   }
                                                   break;
                                               }
                                               m++;
                                           }
                                       }
                                       ind++;
                                   }
                                   if (check == true) {
                                       if (len > 0 && numOfEmptyBlocks > 0) {
                                           int numOfNeededBlocks = len / blockSize;
                                           int numOfEmptyBlocks = 0;
                                           bool flagg = false;
                                           for (int i = 0; i < BitVectorSize; i++) {
                                               if (BitVector[i] == 0) {
                                                   numOfEmptyBlocks++;
                                               }
                                           }
                                           if (len % blockSize != 0) {
                                               numOfNeededBlocks++;
                                               flagg = true;
                                           }
                                           if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 2) {
                                               int numOfPointer1 = 0;
                                               int numOfPointer2 = 0;
                                               int w=0;
                                               for (int i = 0; i < BitVectorSize; i++) {
                                                   if (BitVector[i] == 0) {
                                                       BitVector[i] = 1;
                                                       while(w<blockSize){
                                                           fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                           fprintf(sim_disk_fd ,"%c",'\0');
                                                           w++;
                                                       }
                                                       numOfEmptyBlocks--;
                                                       int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                       numOfBlocks++;
                                                       OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                       OpenFileDescriptors[fd].getInode()->setdoubleInDirect(i);
                                                       int b =0;
                                                       while (numOfNeededBlocks > 0 && numOfPointer1 < blockSize && numOfEmptyBlocks > 0) {
                                                           for (int k = 0; k < BitVectorSize; k++) {
                                                               if (BitVector[k] == 0) {
                                                                   numOfPointer2 = 0;
                                                                   BitVector[k] = 1;
                                                                   while(b < blockSize){
                                                                       fseek(sim_disk_fd,(k  * blockSize)+ b,SEEK_SET);
                                                                       fprintf(sim_disk_fd ,"%c",'\0');
                                                                       b++;
                                                                   }
                                                                   char address = decToBinary(k);
                                                                   fseek(sim_disk_fd,(i * blockSize) + numOfPointer1, SEEK_SET);
                                                                   fprintf(sim_disk_fd, "%c", address);
                                                                   numOfPointer1++;
                                                                   numOfEmptyBlocks--;
                                                                   int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                                   numOfBlocks++;
                                                                   OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                                   int x=0;
                                                                   while (len > 0 && numOfPointer2 < blockSize && numOfEmptyBlocks > 0) {
                                                                       for (int q = 0; q < BitVectorSize; q++) {
                                                                           if (BitVector[q] == 0) {
                                                                               BitVector[q] = 1;
                                                                               while(x<blockSize){
                                                                                   fseek(sim_disk_fd,(q  * blockSize)+ x,SEEK_SET);
                                                                                   fprintf(sim_disk_fd ,"%c",'\0');
                                                                                   x++;
                                                                               }
                                                                               char address = decToBinary(q);
                                                                               fseek(sim_disk_fd, (k * blockSize) +numOfPointer2,SEEK_SET);
                                                                               fprintf(sim_disk_fd, "%c", address);
                                                                               numOfPointer2++;
                                                                               numOfEmptyBlocks--;
                                                                               int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                                               numOfBlocks++;
                                                                               OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                                               FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                                               if (flagg == true && numOfNeededBlocks == 1) {
                                                                                   for (int l = 0; l < len; l++) {
                                                                                       fseek(sim_disk_fd,(q * blockSize) + l,SEEK_SET);
                                                                                       fprintf(sim_disk_fd, "%c",buf[l + j]);
                                                                                   }
                                                                                   int fileSize = fileDes.getInode()->getfileSize();
                                                                                   fileDes.getInode()->setfile_size(fileSize + len);
                                                                                   len =0;
                                                                               } else {
                                                                                   for (int l = 0;l < blockSize; l++) {
                                                                                       fseek(sim_disk_fd,(q * blockSize) + l,SEEK_SET);
                                                                                       fprintf(sim_disk_fd, "%c",buf[l + j]);
                                                                                   }
                                                                                   len -= blockSize;
                                                                                   int fileSize = fileDes.getInode()->getfileSize();
                                                                                   fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                                               }
                                                                               numOfNeededBlocks--;
                                                                               j += blockSize;
                                                                               break;
                                                                           }
                                                                       }
                                                                   }
                                                                   break;
                                                               }
                                                           }
                                                       }
                                                       break;
                                                   }
                                               }
                                           }
                                       }
                                   }
                                   else{
                                       int douIndirect = OpenFileDescriptors[fd].getInode()->getdoubleInDirect();
                                       int j=0;
                                       if(douIndirect != -1){
                                           numOfNeededBlocks=len/blockSize;
                                           if(len%blockSize!=0){
                                               numOfNeededBlocks++;
                                               flagg=true;
                                           }
                                           ind=0;
                                           while (ind < blockSize && len >0){
                                               fseek(sim_disk_fd, (douIndirect * blockSize) + ind, SEEK_SET);
                                               char character = fgetc(sim_disk_fd);
                                               int value = (int)character;
                                               if (character == '\0' || character == ' ') {
                                                   check = true;
                                                   while (len > 0 && ind < blockSize) {
                                                       for (int k = 0; k < BitVectorSize; k++) {
                                                           if (BitVector[k] == 0) {
                                                               BitVector[k] = 1;
                                                               char address = decToBinary(k);
                                                               fseek(sim_disk_fd, (douIndirect * blockSize) + ind,SEEK_SET);
                                                               fprintf(sim_disk_fd, "%c", address);
                                                               ind++;
                                                               numOfEmptyBlocks--;
                                                               int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                               numOfBlocks++;
                                                               OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                               int g = 0;
                                                               while (g < blockSize && len>0) {
                                                                   for (int q = 0; q < BitVectorSize; q++) {
                                                                       if (BitVector[q] == 0) {
                                                                           BitVector[q] = 1;
                                                                           char address = decToBinary(q);
                                                                           fseek(sim_disk_fd, (k * blockSize) + g,SEEK_SET);
                                                                           fprintf(sim_disk_fd, "%c", address);
                                                                           g++;
                                                                           numOfEmptyBlocks--;
                                                                           int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                                           numOfBlocks++;
                                                                           OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                                           FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                                           if (flagg == true && numOfNeededBlocks == 1) {
                                                                               for (int l = 0; l < len; l++) {
                                                                                   fseek(sim_disk_fd,(q * blockSize) + l, SEEK_SET);
                                                                                   fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                               }
                                                                               int fileSize = fileDes.getInode()->getfileSize();
                                                                               fileDes.getInode()->setfile_size(fileSize + len);
                                                                               len = 0;
                                                                           } else {
                                                                               for (int l = 0; l < blockSize; l++) {
                                                                                   fseek(sim_disk_fd,(q * blockSize) + l,SEEK_SET);
                                                                                   fprintf(sim_disk_fd, "%c",buf[l + j]);
                                                                               }
                                                                               len -= blockSize;
                                                                               int fileSize = fileDes.getInode()->getfileSize();
                                                                               fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                                           }
                                                                           numOfNeededBlocks--;
                                                                           j += blockSize;
                                                                           break;
                                                                       }
                                                                   }
                                                               }
                                                               break;
                                                           }
                                                       }
                                                   }
                                                   break;
                                               }
                                               else {
                                                   int m = 0;
                                                   while (m < blockSize && len >0) {
                                                       fseek(sim_disk_fd, (value * blockSize) + m, SEEK_SET);
                                                       character = fgetc(sim_disk_fd);
                                                       int intValue = (int)character;
                                                       if (character == '\0') {
                                                           check = true;
                                                           for (int q =0 ; q<BitVectorSize ; q++){
                                                               if (BitVector[q]==0) {
                                                                   BitVector[q] = 1;
                                                                   numOfEmptyBlocks--;
                                                                   numOfNeededBlocks--;
                                                                   char address = decToBinary(q);
                                                                   fseek(sim_disk_fd, (value * blockSize) + m,SEEK_SET);
                                                                   fprintf(sim_disk_fd, "%c", address);
                                                                   int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                                   numOfBlocks++;
                                                                   OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                                   int g = 0;
                                                                   while (g < blockSize && len > 0) {
                                                                       fseek(sim_disk_fd, (q * blockSize) + g,SEEK_SET);
                                                                       fprintf(sim_disk_fd, "%c", buf[j]);
                                                                       j++;
                                                                       len--;
                                                                       m++;
                                                                       g++;
                                                                       int fileSize = fileDes.getInode()->getfileSize();
                                                                       fileSize++;
                                                                       fileDes.getInode()->setfile_size(fileSize);
                                                                   }
                                                                   break;
                                                               }
                                                           }
                                                       }
                                                       else{
                                                           int a=0;
                                                           while(a < blockSize && len>0){
                                                               fseek(sim_disk_fd , (intValue*blockSize)+a ,SEEK_SET);
                                                               character = fgetc(sim_disk_fd);
                                                               if (character == ' ' || character == '\0'){
                                                                   check=true;
                                                                   while(a<blockSize && len>0){
                                                                       fseek(sim_disk_fd , (intValue*blockSize)+a ,SEEK_SET);
                                                                       fprintf(sim_disk_fd , "%c",buf[j]);
                                                                       a++;
                                                                       j++;
                                                                       len--;
                                                                       int fileSize = fileDes.getInode()->getfileSize();
                                                                       fileSize++;
                                                                       fileDes.getInode()->setfile_size(fileSize);
                                                                   }
                                                                   numOfNeededBlocks--;
                                                                   break;
                                                               }
                                                               a++;
                                                           }
                                                       }
                                                       m++;
                                                   }
                                               }
                                               ind++;
                                           }
                                           if(check == false){
                                               printf("Cant write to the file! \n");
                                               return -1;
                                           }
                                       }
                                       else {
                                           int j = 0;
                                           numOfNeededBlocks=len/blockSize;
                                           if (numOfNeededBlocks == 0){
                                               numOfNeededBlocks++;
                                               flagg = true;
                                           }
                                           if (len > 0 && numOfEmptyBlocks > 2) {
                                               int numOfPointer1 = 0;
                                               int numOfPointer2 = 0;
                                               for (int i = 0; i < BitVectorSize; i++) {
                                                   if (BitVector[i] == 0) {
                                                       BitVector[i] = 1;
                                                       int w=0;
                                                       while(w<blockSize){
                                                           fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                           fprintf(sim_disk_fd ,"%c",'\0');
                                                           w++;
                                                       }
                                                       numOfEmptyBlocks--;
                                                       int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use(); numOfBlocks++;
                                                       OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                       OpenFileDescriptors[fd].getInode()->setdoubleInDirect(i);
                                                       int b=0;
                                                       while (numOfNeededBlocks > 0 && numOfPointer1 < blockSize && numOfEmptyBlocks > 0) {
                                                           for (int k = 0; k < BitVectorSize; k++) {
                                                               if (BitVector[k] == 0) {
                                                                   numOfPointer2 = 0;
                                                                   BitVector[k] = 1;
                                                                   while(b<blockSize){
                                                                       fseek(sim_disk_fd,(k  * blockSize)+ b,SEEK_SET);
                                                                       fprintf(sim_disk_fd ,"%c",'\0');
                                                                       b++;
                                                                   }
                                                                   char address = decToBinary(k);
                                                                   fseek(sim_disk_fd,(i * blockSize) + numOfPointer1, SEEK_SET);
                                                                   fprintf(sim_disk_fd, "%c", address);
                                                                   numOfPointer1++;
                                                                   numOfEmptyBlocks--;
                                                                   int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                                   numOfBlocks++;
                                                                   OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                                   int x=0;
                                                                   while (numOfNeededBlocks > 0 && numOfPointer2 < blockSize && numOfEmptyBlocks > 0) {
                                                                       for (int q = 0; q < BitVectorSize; q++) {
                                                                           if (BitVector[q] == 0) {
                                                                               BitVector[q] = 1;
                                                                               while(x<blockSize){
                                                                                   fseek(sim_disk_fd,(q  * blockSize)+ x,SEEK_SET);
                                                                                   fprintf(sim_disk_fd ,"%c",'\0');
                                                                                   x++;
                                                                               }
                                                                               char address = decToBinary(q);
                                                                               fseek(sim_disk_fd, (k * blockSize) +numOfPointer2,SEEK_SET);
                                                                               fprintf(sim_disk_fd, "%c", address);
                                                                               numOfPointer2++;
                                                                               numOfEmptyBlocks--;
                                                                               int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                                               numOfBlocks++;
                                                                               OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                                               FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                                               if (flagg == true &&
                                                                                   numOfNeededBlocks == 1) {
                                                                                   for (int l = 0; l < len; l++) {
                                                                                       fseek(sim_disk_fd,
                                                                                             (q * blockSize) + l,
                                                                                             SEEK_SET);
                                                                                       fprintf(sim_disk_fd, "%c",
                                                                                               buf[l + j]);
                                                                                   }
                                                                                   int fileSize = fileDes.getInode()->getfileSize();
                                                                                   fileDes.getInode()->setfile_size(
                                                                                           fileSize + len);
                                                                               } else {
                                                                                   for (int l = 0;
                                                                                        l < blockSize; l++) {
                                                                                       fseek(sim_disk_fd,
                                                                                             (q * blockSize) + l,
                                                                                             SEEK_SET);
                                                                                       fprintf(sim_disk_fd, "%c",
                                                                                               buf[l + j]);
                                                                                   }
                                                                                   len -= blockSize;
                                                                                   int fileSize = fileDes.getInode()->getfileSize();
                                                                                   fileDes.getInode()->setfile_size(
                                                                                           fileSize + blockSize);
                                                                               }
                                                                               numOfNeededBlocks--;
                                                                               j += blockSize;
                                                                               break;
                                                                           }
                                                                       }
                                                                   }
                                                                   break;
                                                               }
                                                           }
                                                       }
                                                       break;
                                                   }
                                               }
                                           }
                                       }
                                   }
                                }
                                else{
                                    if (len % blockSize != 0 ) {
                                        numOfNeededBlocks++;
                                        flagg = true;
                                    }
                                    int j=0;
                                    if (numOfEmptyBlocks > 1 && numOfNeededBlocks > 0) {
                                        int numOfPointers = 0;
                                        for (int i = 0; i < BitVectorSize; i++) {
                                            if (BitVector[i] == 0) {
                                                BitVector[i] = 1;
                                                int w=0;
                                                while(w<blockSize){
                                                    fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                    w++;
                                                }
                                                numOfEmptyBlocks--;
                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                numOfBlocks++;
                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                OpenFileDescriptors[fd].getInode()->setsingleInDirect(i);
                                                int z = 0;
                                                int b=0;
                                                while (numOfNeededBlocks > 0 && numOfPointers < blockSize && numOfEmptyBlocks > 0) {
                                                    for (int k = 0; k < BitVectorSize; k++) {
                                                        if (BitVector[k] == 0) {
                                                            BitVector[k] = 1;
                                                            while(b<blockSize){
                                                                fseek(sim_disk_fd,(k  * blockSize)+ b,SEEK_SET);
                                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                                b++;
                                                            }
                                                            char address = decToBinary(k);
                                                            fseek(sim_disk_fd, (i * blockSize) + numOfPointers,SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", address);
                                                            numOfPointers++;
                                                            numOfEmptyBlocks--;
                                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                            numOfBlocks++;
                                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                            FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                            if (flagg == true && numOfNeededBlocks == 1) {
                                                                for (int l = 0; l < len; l++) {
                                                                    fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                }
                                                                int fileSize = fileDes.getInode()->getfileSize();
                                                                fileDes.getInode()->setfile_size(fileSize + len);
                                                            } else {
                                                                for (int l = 0; l < blockSize; l++) {
                                                                    fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                }
                                                                z++;
                                                                len -= blockSize;
                                                                int fileSize = fileDes.getInode()->getfileSize();
                                                                fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                            }
                                                            numOfNeededBlocks--;
                                                            j += blockSize;
                                                            break;
                                                        }
                                                    }
                                                }
                                                break;
                                            }
                                        }
                                    }
                                    if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 2) {
                                        int numOfPointer1 = 0;
                                        int numOfPointer2 = 0;
                                        for (int i = 0; i < BitVectorSize; i++) {
                                            if (BitVector[i] == 0) {
                                                BitVector[i] = 1;
                                                int w=0;
                                                while(w<blockSize){
                                                    fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                    w++;
                                                }
                                                numOfEmptyBlocks--;
                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                numOfBlocks++;
                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                OpenFileDescriptors[fd].getInode()->setdoubleInDirect(i);
                                                int b=0;
                                                while (numOfNeededBlocks > 0 && numOfPointer1 < blockSize && numOfEmptyBlocks > 0) {
                                                    for (int k = 0; k < BitVectorSize; k++) {
                                                        if (BitVector[k] == 0) {
                                                            numOfPointer2 = 0;
                                                            BitVector[k] = 1;
                                                            while(b <blockSize){
                                                                fseek(sim_disk_fd,(k  * blockSize)+b,SEEK_SET);
                                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                                b++;
                                                            }
                                                            char address = decToBinary(k);
                                                            fseek(sim_disk_fd, (i * blockSize) + numOfPointer1,SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", address);
                                                            numOfPointer1++;
                                                            numOfEmptyBlocks--;
                                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                            numOfBlocks++;
                                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                            int x=0;
                                                            while (numOfNeededBlocks > 0 && numOfPointer2 < blockSize && numOfEmptyBlocks > 0) {
                                                                for (int q = 0; q < BitVectorSize; q++) {
                                                                    if (BitVector[q] == 0) {
                                                                        BitVector[q] = 1;
                                                                        while(x<blockSize){
                                                                            fseek(sim_disk_fd,(q  * blockSize)+ x,SEEK_SET);
                                                                            fprintf(sim_disk_fd ,"%c",'\0');
                                                                            x++;
                                                                        }
                                                                        char address = decToBinary(q);
                                                                        fseek(sim_disk_fd,(k * blockSize) + numOfPointer2,SEEK_SET);
                                                                        fprintf(sim_disk_fd, "%c", address);
                                                                        numOfPointer2++;
                                                                        numOfEmptyBlocks--;
                                                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                                        numOfBlocks++;
                                                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                                        FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                                        if (flagg == true && numOfNeededBlocks == 1) {
                                                                            for (int l = 0; l < len; l++) {
                                                                                fseek(sim_disk_fd, (q * blockSize) + l,SEEK_SET);
                                                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                            }
                                                                            int fileSize = fileDes.getInode()->getfileSize();
                                                                            fileDes.getInode()->setfile_size(fileSize + len);
                                                                        } else {
                                                                            for (int l = 0; l < blockSize; l++) {
                                                                                fseek(sim_disk_fd, (q * blockSize) + l,SEEK_SET);
                                                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                            }
                                                                            len -= blockSize;
                                                                            int fileSize = fileDes.getInode()->getfileSize();
                                                                            fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                                        }
                                                                        numOfNeededBlocks--;
                                                                        j += blockSize;
                                                                        break;
                                                                    }
                                                                }
                                                            }
                                                            break;
                                                        }
                                                    }
                                                }
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else{
                            int j=0;
                            if (len % blockSize != 0 && numOfNeededBlocks != 0) {
                                numOfNeededBlocks++;
                                flagg = true;
                            }
                            if (numOfNeededBlocks < 2 || numOfEmptyBlocks < 2) {
                                if (numOfNeededBlocks == 0) { // len <blockesize
                                    for (int i = 0; i < BitVectorSize; i++) {
                                        if (BitVector[i] == 0) {
                                            BitVector[i] = 1;
                                            int w=0;
                                            while(w<blockSize){
                                                fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                w++;
                                            }
                                            int m=0;
                                            while( len >0) {
                                                fseek(sim_disk_fd, (i * blockSize)+m, SEEK_SET);
                                                fprintf(sim_disk_fd , "%c" , buf [j]);
                                                j++;
                                                m++;
                                                len--;
                                                int fileSize = fileDes.getInode()->getfileSize();
                                                fileSize++;
                                                fileDes.getInode()->setfile_size(fileSize);
                                            }
                                            numOfEmptyBlocks--;
                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                            numOfBlocks++;
                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                            OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                            return 1;
                                            break;
                                        }
                                    }
                                }
                                else {
                                    if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 0) {
                                        for (int i = 0; i < BitVectorSize; i++) {
                                            if (BitVector[i] == 0) {
                                                BitVector[i] = 1;
                                                int w=0;
                                                while(w<blockSize){
                                                    fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                    w++;
                                                }
                                                OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                                fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                                                FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                int k = 0;
                                                if (flagg == true && numOfNeededBlocks == 1) {
                                                    for (int l = 0; l < len; l++) {
                                                        fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                    }
                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                    fileDes.getInode()->setfile_size(fileSize + len);
                                                } else {
                                                    for (int l = 0; l < blockSize; l++) {
                                                        fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                    }
                                                    len -= blockSize;
                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                    fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                }
                                                j += blockSize;
                                                numOfNeededBlocks--;
                                                numOfEmptyBlocks--;
                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                numOfBlocks++;
                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                break;
                                            }
                                        }
                                    }
                                    return 1;
                                }
                            }
                            if (numOfNeededBlocks > 2 && numOfNeededBlocks < (2 + blockSize)) {
                                int ind = numOfNeededBlocks - 1;
                                while (numOfNeededBlocks > ind && numOfEmptyBlocks > 0) {
                                    for (int i = 0; i < BitVectorSize; i++) {
                                        if (BitVector[i] == 0) {
                                            BitVector[i] = 1;
                                            int w=0;
                                            while(w < blockSize){
                                                fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                w++;
                                            }
                                            OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                            fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                                            FileDescriptor fileDes = OpenFileDescriptors[fd];
                                            int k = 0;
                                            if (flagg == true && numOfNeededBlocks == 1) {
                                                for (int l = 0; l < len; l++) {
                                                    fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                }
                                                int fileSize = fileDes.getInode()->getfileSize();
                                                fileDes.getInode()->setfile_size(fileSize + len);
                                            } else {
                                                for (int l = 0; l < blockSize; l++) {
                                                    fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                }
                                                len -= blockSize;
                                                int fileSize = fileDes.getInode()->getfileSize();
                                                fileDes.getInode()->setfile_size(fileSize + blockSize);
                                            }
                                            j +=  blockSize;
                                            numOfNeededBlocks--;
                                            numOfEmptyBlocks--;
                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                            numOfBlocks++;
                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                            break;
                                        }
                                    }
                                }
                                if (numOfEmptyBlocks > 1 && numOfNeededBlocks > 0) {
                                    int numOfPointers = 0;
                                    for (int i = 0; i < BitVectorSize; i++) {
                                        if (BitVector[i] == 0) {
                                            BitVector[i] = 1;
                                            int w=0;
                                            while(w<blockSize){
                                                fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                w++;
                                            }
                                            numOfEmptyBlocks--;
                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                            numOfBlocks++;
                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                            OpenFileDescriptors[fd].getInode()->setsingleInDirect(i);
                                            int z = 0;
                                            int b=0;
                                            while (numOfNeededBlocks > 0 && numOfPointers < blockSize && numOfEmptyBlocks > 0) {
                                                for (int k = 0; k < BitVectorSize; k++) {
                                                    if (BitVector[k] == 0) {
                                                        BitVector[k] = 1;
                                                        while(b <blockSize){
                                                            fseek(sim_disk_fd,(k * blockSize)+ b,SEEK_SET);
                                                            fprintf(sim_disk_fd ,"%c",'\0');
                                                            b++;
                                                        }
                                                        char address = decToBinary(k);
                                                        fseek(sim_disk_fd, (i * blockSize) + numOfPointers,SEEK_SET);
                                                        fprintf(sim_disk_fd, "%c", address);
                                                        numOfPointers++;
                                                        numOfEmptyBlocks--;
                                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                        numOfBlocks++;
                                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                        FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                        if (flagg == true && numOfNeededBlocks == 1) {
                                                            for (int l = 0; l < len; l++) {
                                                                fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                            }
                                                            int fileSize = fileDes.getInode()->getfileSize();
                                                            fileDes.getInode()->setfile_size(fileSize + len);
                                                        } else {
                                                            for (int l = 0; l < blockSize; l++) {
                                                                fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                            }
                                                            z++;
                                                            len -= blockSize;
                                                            int fileSize = fileDes.getInode()->getfileSize();
                                                            fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                        }
                                                        numOfNeededBlocks--;
                                                        j += blockSize;
                                                        break;
                                                    }
                                                }
                                            }
                                            break;
                                        }
                                    }
                                }
                            }
                            if (numOfNeededBlocks > 1 + blockSize && numOfNeededBlocks < (1 + blockSize + (blockSize * blockSize)+1)) {
                                int ind = numOfNeededBlocks - 1;
                                if (numOfNeededBlocks > ind && numOfEmptyBlocks > 0) {
                                    for (int i = 0; i < BitVectorSize; i++) {
                                        if (BitVector[i] == 0) {
                                            BitVector[i] = 1;
                                            int w=0;
                                            while(w<blockSize){
                                                fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                w++;
                                            }
                                            OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                            fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                                            FileDescriptor fileDes = OpenFileDescriptors[fd];
                                            if (flagg == true && numOfNeededBlocks == 1) {
                                                for (int l = 0; l < len; l++) {
                                                    fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                }
                                                int fileSize = fileDes.getInode()->getfileSize();
                                                fileDes.getInode()->setfile_size(fileSize + len);
                                            } else {
                                                for (int l = 0; l < blockSize; l++) {
                                                    fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                }
                                                len -= blockSize;
                                                int fileSize = fileDes.getInode()->getfileSize();
                                                fileDes.getInode()->setfile_size(fileSize + blockSize);
                                            }
                                            j += blockSize;
                                            numOfNeededBlocks--;
                                            numOfEmptyBlocks--;
                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                            numOfBlocks++;
                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                            break;
                                        }
                                    }
                                }
                                if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 1) {
                                    int numOfPointers = 0;
                                    for (int i = 0; i < BitVectorSize; i++) {
                                        if (BitVector[i] == 0) {
                                            BitVector[i] = 1;
                                            int w=0;
                                            while(w<blockSize){
                                                fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                w++;
                                            }
                                            numOfEmptyBlocks--;
                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                            numOfBlocks++;
                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                            OpenFileDescriptors[fd].getInode()->setsingleInDirect(i);
                                            int b=0;
                                            while (numOfNeededBlocks > 0 && numOfPointers < blockSize && numOfEmptyBlocks > 0) {
                                                for (int k = 0; k < BitVectorSize; k++) {
                                                    if (BitVector[k] == 0) {
                                                        BitVector[k] = 1;
                                                        while(b<blockSize){
                                                            fseek(sim_disk_fd,(k  * blockSize)+ b,SEEK_SET);
                                                            fprintf(sim_disk_fd ,"%c",'\0');
                                                            b++;
                                                        }
                                                        char address = decToBinary(k);
                                                        fseek(sim_disk_fd, (i * blockSize) + numOfPointers,SEEK_SET);
                                                        fprintf(sim_disk_fd, "%c", address);
                                                        numOfPointers++;
                                                        numOfEmptyBlocks--;
                                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                        numOfBlocks++;
                                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                        FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                        if (flagg == true && numOfNeededBlocks == 1) {
                                                            for (int l = 0; l < len; l++) {
                                                                fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                            }
                                                            int fileSize = fileDes.getInode()->getfileSize();
                                                            fileDes.getInode()->setfile_size(fileSize + len);
                                                        } else {
                                                            for (int l = 0; l < blockSize; l++) {
                                                                fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                            }
                                                            len -= blockSize;
                                                            int fileSize = fileDes.getInode()->getfileSize();
                                                            fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                        }
                                                        numOfNeededBlocks--;
                                                        j += blockSize;
                                                        break;
                                                    }
                                                }
                                            }
                                            break;
                                        }
                                    }
                                }
                                if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 2) {
                                    int numOfPointer1 = 0;
                                    int numOfPointer2 = 0;
                                    for (int i = 0; i < BitVectorSize; i++) {
                                        if (BitVector[i] == 0) {
                                            BitVector[i] = 1;
                                            int w=0;
                                            while(w<blockSize){
                                                fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                w++;
                                            }
                                            numOfEmptyBlocks--;
                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                            numOfBlocks++;
                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                            OpenFileDescriptors[fd].getInode()->setdoubleInDirect(i);
                                            int b=0;
                                            while (numOfNeededBlocks > 0 && numOfPointer1 < blockSize && numOfEmptyBlocks > 0) {
                                                for (int k = 0; k < BitVectorSize; k++) {
                                                    if (BitVector[k] == 0) {
                                                        numOfPointer2 = 0;
                                                        BitVector[k] = 1;
                                                        while(b<blockSize){
                                                            fseek(sim_disk_fd,(k  * blockSize)+ b,SEEK_SET);
                                                            fprintf(sim_disk_fd ,"%c",'\0');
                                                            b++;
                                                        }
                                                        char address = decToBinary(k);
                                                        fseek(sim_disk_fd, (i * blockSize) + numOfPointer1,SEEK_SET);
                                                        fprintf(sim_disk_fd, "%c", address);
                                                        numOfPointer1++;
                                                        numOfEmptyBlocks--;
                                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                        numOfBlocks++;
                                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                        int x=0;
                                                        while (numOfNeededBlocks > 0 && numOfPointer2 < blockSize && numOfEmptyBlocks > 0) {
                                                            for (int q = 0; q < BitVectorSize; q++) {
                                                                if (BitVector[q] == 0) {
                                                                    BitVector[q] = 1;
                                                                    while(x<blockSize){
                                                                        fseek(sim_disk_fd,(q  * blockSize)+ x,SEEK_SET);
                                                                        fprintf(sim_disk_fd ,"%c",'\0');
                                                                        x++;
                                                                    }
                                                                    char address = decToBinary(q);
                                                                    fseek(sim_disk_fd,(k * blockSize) + numOfPointer2,SEEK_SET);
                                                                    fprintf(sim_disk_fd, "%c", address);
                                                                    numOfPointer2++;
                                                                    numOfEmptyBlocks--;
                                                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                                    numOfBlocks++;
                                                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                                    FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                                    if (flagg == true && numOfNeededBlocks == 1) {
                                                                        for (int l = 0; l < len; l++) {
                                                                            fseek(sim_disk_fd, (q * blockSize) + l,SEEK_SET);
                                                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                        }
                                                                        int fileSize = fileDes.getInode()->getfileSize();
                                                                        fileDes.getInode()->setfile_size(fileSize + len);
                                                                    } else {
                                                                        for (int l = 0; l < blockSize; l++) {
                                                                            fseek(sim_disk_fd, (q * blockSize) + l,SEEK_SET);
                                                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                        }
                                                                        len -= blockSize;
                                                                        int fileSize = fileDes.getInode()->getfileSize();
                                                                        fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                                    }
                                                                    numOfNeededBlocks--;
                                                                    j += blockSize;
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else{
                    if (len % blockSize != 0 && numOfNeededBlocks != 0) {
                        numOfNeededBlocks++;
                        flagg = true;
                    }
                    int j=0;
                    if (numOfNeededBlocks < 3 || numOfEmptyBlocks < 3) {
                        if (numOfNeededBlocks == 0) { // len <blockesize
                            for (int i = 0; i < BitVectorSize; i++) {
                                if (BitVector[i] == 0) {
                                    BitVector[i] = 1;
                                    int w=0;
                                    while(w<blockSize){
                                        fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                        fprintf(sim_disk_fd ,"%c",'\0');
                                        w++;
                                    }
                                    int m=0;
                                    while( len >0) {
                                        fseek(sim_disk_fd, (i * blockSize)+m, SEEK_SET);
                                        fprintf(sim_disk_fd , "%c" , buf [j]);
                                        j++;
                                        m++;
                                        len--;
                                        int fileSize = fileDes.getInode()->getfileSize();
                                        fileSize++;
                                        fileDes.getInode()->setfile_size(fileSize);
                                    }
                                    numOfEmptyBlocks--;
                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                    numOfBlocks++;
                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                    OpenFileDescriptors[fd].getInode()->setdirectBlock2(i);
                                    return 1;
                                    break;
                                }
                            }
                        } else {
                            int num =0;
                            while (numOfNeededBlocks > 0 && numOfEmptyBlocks > 0) {
                                for (int i = 0; i < BitVectorSize; i++) {
                                    if (BitVector[i] == 0) {
                                        BitVector[i] = 1;
                                        int w=0;
                                        while(w<blockSize){
                                            fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                            fprintf(sim_disk_fd ,"%c",'\0');
                                            w++;
                                        }
                                        if ((num == 0) && numOfEmptyBlocks > 0){
                                            OpenFileDescriptors[fd].getInode()->setdirectBlock2(i);
                                        }
                                        if ((num == 1 ) && numOfEmptyBlocks > 0){
                                            OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                        }
                                        num++;
                                        fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                                        FileDescriptor fileDes = OpenFileDescriptors[fd];
                                        int k = 0;
                                        if (flagg == true && numOfNeededBlocks == 1) {
                                            for (int l = 0; l < len; l++) {
                                                fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                            }
                                            int fileSize = fileDes.getInode()->getfileSize();
                                            fileDes.getInode()->setfile_size(fileSize + len);
                                        } else {
                                            for (int l = 0; l < blockSize; l++) {
                                                fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                                fprintf(sim_disk_fd, "%c", buf[l + j]);
                                            }
                                            len -= blockSize;
                                            k++;
                                            int fileSize = fileDes.getInode()->getfileSize();
                                            fileDes.getInode()->setfile_size(fileSize + blockSize);
                                        }
                                        j +=  blockSize;
                                        numOfNeededBlocks--;
                                        numOfEmptyBlocks--;
                                        int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                        numOfBlocks++;
                                        OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                        break;
                                    }
                                }
                            }
                            return 1;
                        }
                    }
                    if (numOfNeededBlocks > 2 && numOfNeededBlocks < (3 + blockSize)) {
                        int ind = numOfNeededBlocks - 2;
                        int num=0;
                        while (numOfNeededBlocks > ind && numOfEmptyBlocks > 0) {
                            for (int i = 0; i < BitVectorSize; i++) {
                                if (BitVector[i] == 0) {
                                    BitVector[i] = 1;
                                    int w=0;
                                    while(w<blockSize){
                                        fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                        fprintf(sim_disk_fd ,"%c",'\0');
                                        w++;
                                    }
                                    if (num == 0 && numOfEmptyBlocks > 0) {
                                        OpenFileDescriptors[fd].getInode()->setdirectBlock2(i);
                                    }
                                    if (num ==1 && numOfEmptyBlocks > 0) {
                                        OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                    }
                                    num++;
                                    fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                                    FileDescriptor fileDes = OpenFileDescriptors[fd];
                                    int k = 0;
                                    if (flagg == true && numOfNeededBlocks == 1) {
                                        for (int l = 0; l < len; l++) {
                                            fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                        }
                                        int fileSize = fileDes.getInode()->getfileSize();
                                        fileDes.getInode()->setfile_size(fileSize + len);
                                    } else {
                                        for (int l = 0; l < blockSize; l++) {
                                            fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                        }
                                        len -= blockSize;
                                        int fileSize = fileDes.getInode()->getfileSize();
                                        fileDes.getInode()->setfile_size(fileSize + blockSize);
                                    }
                                    j += blockSize;
                                    numOfNeededBlocks--;
                                    numOfEmptyBlocks--;
                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                    numOfBlocks++;
                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                    break;
                                }
                            }
                        }
                        if (numOfEmptyBlocks > 1 && numOfNeededBlocks > 0) {
                            int numOfPointers = 0;
                            for (int i = 0; i < BitVectorSize; i++) {
                                if (BitVector[i] == 0) {
                                    BitVector[i] = 1;
                                    int w=0;
                                    while(w<blockSize){
                                        fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                        fprintf(sim_disk_fd ,"%c",'\0');
                                        w++;
                                    }
                                    numOfEmptyBlocks--;
                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                    numOfBlocks++;
                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                    OpenFileDescriptors[fd].getInode()->setsingleInDirect(i);
                                    int z = 0;
                                    int b=0;
                                    while (numOfNeededBlocks > 0 && numOfPointers < blockSize &&
                                           numOfEmptyBlocks > 0) {
                                        for (int k = 0; k < BitVectorSize; k++) {
                                            if (BitVector[k] == 0) {
                                                BitVector[k] = 1;
                                                while(b<blockSize){
                                                    fseek(sim_disk_fd,(k  * blockSize)+ b,SEEK_SET);
                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                    b++;
                                                }
                                                char address = decToBinary(k);
                                                fseek(sim_disk_fd, (i * blockSize) + numOfPointers, SEEK_SET);
                                                fprintf(sim_disk_fd, "%c", address);
                                                numOfPointers++;
                                                numOfEmptyBlocks--;
                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                numOfBlocks++;
                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                if (flagg == true && numOfNeededBlocks == 1) {
                                                    for (int l = 0; l < len; l++) {
                                                        fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                    }
                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                    fileDes.getInode()->setfile_size(fileSize + len);
                                                } else {
                                                    for (int l = 0; l < blockSize; l++) {
                                                        fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                    }
                                                    len -= blockSize;
                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                    fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                }
                                                numOfNeededBlocks--;
                                                j +=  blockSize;
                                                break;
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    if (numOfNeededBlocks > 2 + blockSize &&
                        numOfNeededBlocks < (3 + blockSize + (blockSize * blockSize))) {
                        int ind = numOfNeededBlocks - 2;
                        int num=0;
                        int w=0;
                        while (numOfNeededBlocks > ind && numOfEmptyBlocks > 0) {
                            for (int i = 0; i < BitVectorSize; i++) {
                                if (BitVector[i] == 0) {
                                    BitVector[i] = 1;
                                    while(w<blockSize){
                                        fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                        fprintf(sim_disk_fd ,"%c",'\0');
                                        w++;
                                    }
                                    if (num ==0 && numOfEmptyBlocks > 0) {
                                        OpenFileDescriptors[fd].getInode()->setdirectBlock2(i);
                                    }
                                    if (num ==1 && numOfEmptyBlocks > 0) {
                                        OpenFileDescriptors[fd].getInode()->setdirectBlock3(i);
                                    }
                                    num++;
                                    fseek(sim_disk_fd, (i * blockSize), SEEK_SET);
                                    FileDescriptor fileDes = OpenFileDescriptors[fd];
                                    if (flagg == true && numOfNeededBlocks == 1) {
                                        for (int l = 0; l < len; l++) {
                                            fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                        }
                                        int fileSize = fileDes.getInode()->getfileSize();
                                        fileDes.getInode()->setfile_size(fileSize + len);
                                    } else {
                                        for (int l = 0; l < blockSize; l++) {
                                            fseek(sim_disk_fd, (i * blockSize) + l, SEEK_SET);
                                            fprintf(sim_disk_fd, "%c", buf[l + j]);
                                        }
                                        len -= blockSize;
                                        int fileSize = fileDes.getInode()->getfileSize();
                                        fileDes.getInode()->setfile_size(fileSize + blockSize);
                                    }
                                    j += blockSize;
                                    numOfNeededBlocks--;
                                    numOfEmptyBlocks--;
                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                    numOfBlocks++;
                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                    break;
                                }
                            }
                        }
                        if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 1) {
                            int numOfPointers = 0;
                            int w=0;
                            for (int i = 0; i < BitVectorSize; i++) {
                                if (BitVector[i] == 0) {
                                    BitVector[i] = 1;
                                    while(w<blockSize){
                                        fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                        fprintf(sim_disk_fd ,"%c",'\0');
                                        w++;
                                    }
                                    numOfEmptyBlocks--;
                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                    numOfBlocks++;
                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                    OpenFileDescriptors[fd].getInode()->setsingleInDirect(i);
                                    int b=0;
                                    while (numOfNeededBlocks > 0 && numOfPointers < blockSize && numOfEmptyBlocks > 0) {
                                        for (int k = 0; k < BitVectorSize; k++) {
                                            if (BitVector[k] == 0) {
                                                BitVector[k] = 1;
                                                while(b<blockSize){
                                                    fseek(sim_disk_fd,(k  * blockSize)+ b,SEEK_SET);
                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                    b++;
                                                }
                                                char address = decToBinary(k);
                                                fseek(sim_disk_fd, (i * blockSize) + numOfPointers, SEEK_SET);
                                                fprintf(sim_disk_fd, "%c", address);
                                                numOfPointers++;
                                                numOfEmptyBlocks--;
                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                numOfBlocks++;
                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                if (flagg == true && numOfNeededBlocks == 1) {
                                                    for (int l = 0; l < len; l++) {
                                                        fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                    }
                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                    fileDes.getInode()->setfile_size(fileSize + len);
                                                } else {
                                                    for (int l = 0; l < blockSize; l++) {
                                                        fseek(sim_disk_fd, (k * blockSize) + l, SEEK_SET);
                                                        fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                    }
                                                    len -= blockSize;
                                                    int fileSize = fileDes.getInode()->getfileSize();
                                                    fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                }
                                                numOfNeededBlocks--;
                                                j += blockSize;
                                                break;
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                        if (numOfNeededBlocks > 0 && numOfEmptyBlocks > 2) {
                            int numOfPointer1 = 0;
                            int numOfPointer2 = 0;
                            for (int i = 0; i < BitVectorSize; i++) {
                                if (BitVector[i] == 0) {
                                    BitVector[i] = 1;
                                    int w=0;
                                    while(w<blockSize){
                                        fseek(sim_disk_fd,(i  * blockSize)+ w,SEEK_SET);
                                        fprintf(sim_disk_fd ,"%c",'\0');
                                        w++;
                                    }
                                    numOfEmptyBlocks--;
                                    int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                    numOfBlocks++;
                                    OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                    OpenFileDescriptors[fd].getInode()->setdoubleInDirect(i);
                                    int b=0;
                                    while (numOfNeededBlocks > 0 && numOfPointer1 < blockSize && numOfEmptyBlocks > 0) {
                                        for (int k = 0; k < BitVectorSize; k++) {
                                            if (BitVector[k] == 0) {
                                                numOfPointer2 = 0;
                                                BitVector[k] = 1;
                                                while(b<blockSize){
                                                    fseek(sim_disk_fd,(k * blockSize)+ b,SEEK_SET);
                                                    fprintf(sim_disk_fd ,"%c",'\0');
                                                    b++;
                                                }
                                                char address = decToBinary(k);
                                                fseek(sim_disk_fd, (i * blockSize) + numOfPointer1, SEEK_SET);
                                                fprintf(sim_disk_fd, "%c", address);
                                                numOfPointer1++;
                                                numOfEmptyBlocks--;
                                                int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                numOfBlocks++;
                                                OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                int x=0;
                                                while (numOfNeededBlocks > 0 && numOfPointer2 < blockSize && numOfEmptyBlocks > 0) {
                                                    for (int q = 0; q < BitVectorSize; q++) {
                                                        if (BitVector[q] == 0) {
                                                            BitVector[q] = 1;
                                                            while(x<blockSize){
                                                                fseek(sim_disk_fd,(q  * blockSize)+ x,SEEK_SET);
                                                                fprintf(sim_disk_fd ,"%c",'\0');
                                                                x++;
                                                            }
                                                            char address = decToBinary(q);
                                                            fseek(sim_disk_fd, (k * blockSize) + numOfPointer2,SEEK_SET);
                                                            fprintf(sim_disk_fd, "%c", address);
                                                            numOfPointer2++;
                                                            numOfEmptyBlocks--;
                                                            int numOfBlocks = OpenFileDescriptors[fd].getInode()->getblock_in_use();
                                                            numOfBlocks++;
                                                            OpenFileDescriptors[fd].getInode()->setblock_in_use(numOfBlocks);
                                                            FileDescriptor fileDes = OpenFileDescriptors[fd];
                                                            if (flagg == true && numOfNeededBlocks == 1) {
                                                                for (int l = 0; l < len; l++) {
                                                                    fseek(sim_disk_fd, (q * blockSize) + l,SEEK_SET);
                                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                }
                                                                int fileSize = fileDes.getInode()->getfileSize();
                                                                fileDes.getInode()->setfile_size(fileSize + len);
                                                            } else {
                                                                for (int l = 0; l < blockSize; l++) {
                                                                    fseek(sim_disk_fd, (q * blockSize) + l,SEEK_SET);
                                                                    fprintf(sim_disk_fd, "%c", buf[l + j]);
                                                                }
                                                                len -= blockSize;
                                                                int fileSize = fileDes.getInode()->getfileSize();
                                                                fileDes.getInode()->setfile_size(fileSize + blockSize);
                                                            }
                                                            numOfNeededBlocks--;
                                                            j += blockSize;
                                                            break;
                                                        }
                                                    }
                                                }
                                                break;
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        return 1;
    }
    // ------------------------------------------------------------------------
    int DelFile( string FileName ) {
        bool flag = false;
        int fd ;
        for (int i = 0; i < OpenFileDescriptors.size(); i++) {
            if (OpenFileDescriptors[i].getFileName() == FileName) {
                fd=i;
                if(OpenFileDescriptors[i].isInUse()==true) {
                    flag = true;
                }
            }
        }
        if (MainDir.find(FileName)== MainDir.end()){
            printf("The file is not exist!\n");
            return -1;
        }
        if ( is_formated == false ){
                printf("The file is not formated yet!\n");
                return -1 ;
        }
        if(flag==true){
            printf("The file is open !\n");
            return -1 ;
        }
        if (MainDir[FileName]->getdirectBlock1() != -1) {
            int direct1 = MainDir[FileName]->getdirectBlock1();
            BitVector[direct1] = 0;
        }
        if (MainDir[FileName]->getdirectBlock2() != -1) {
            int direct2 = MainDir[FileName]->getdirectBlock2();
            BitVector[direct2]=0;
        }
        if (MainDir[FileName]->getdirectBlock2() != -1 ) {
            int direct3 = MainDir[FileName]->getdirectBlock3();
            BitVector[direct3] = 0;
        }
        if (MainDir[FileName]->getsingleInDirect() != -1) {
            int single = MainDir[FileName]->getsingleInDirect();
            BitVector[single] = 0;
            int p = 0;
            while (p < blockSize) {
                int in = 0;
                fseek(sim_disk_fd, (single * blockSize) + p, SEEK_SET);
                char character = fgetc(sim_disk_fd);
                in = (int)character ;
                BitVector[in] = 0;
                p++;
            }
        }
        if (MainDir[FileName]->getdoubleInDirect() != -1) {
            int doublle = MainDir[FileName]->getdoubleInDirect();
            BitVector[doublle] = 0;
            int l = 0;
            while (l < blockSize) {
                int in = 0;
                fseek(sim_disk_fd, (doublle * blockSize) + l, SEEK_SET);
                char character = fgetc(sim_disk_fd);
                in = (int)character;
                BitVector[in] = 0;
                l++;
                int m = 0;
                while (m < blockSize) {
                    int yi = 0;
                    fseek(sim_disk_fd, (in * blockSize) + m, SEEK_SET);
                    character = fgetc(sim_disk_fd);
                    yi = (int)character;
                    BitVector[yi] = 0;
                    m++;
                }
            }
        }
        FileDescriptor fid (" ",OpenFileDescriptors[fd].getInode());
        bool check=false;
        for (int i =0 ;i<OpenFileDescriptors.size();i++){
            if(OpenFileDescriptors[i].getFileName() == FileName){
                if(i<OpenFileDescriptors.size()) {
                    check= true;
                    OpenFileDescriptors[i] = fid;
                    OpenFileDescriptors[i].getInode()->setfile_size(0);
                    OpenFileDescriptors[i].setInUse(false);
                    OpenFileDescriptors[i].getInode()->setblock_size(0);
                    OpenFileDescriptors[i].getInode()->setblock_in_use(0);
                    OpenFileDescriptors[i].getInode()->setdirectBlock1(-1);
                    OpenFileDescriptors[i].getInode()->setdirectBlock2(-1);
                    OpenFileDescriptors[i].getInode()->setdirectBlock3(-1);
                    OpenFileDescriptors[i].getInode()->setsingleInDirect(-1);
                    OpenFileDescriptors[i].getInode()->setdoubleInDirect(-1);
                }
                break;
            }
        }
        if (check== false){
            MainDir[FileName]->setfile_size(0);
            MainDir[FileName]->setblock_size(0);
            MainDir[FileName]->setblock_in_use(0);
            MainDir[FileName]->setdirectBlock1(-1);
            MainDir[FileName]->setdirectBlock2(-1);
            MainDir[FileName]->setdirectBlock3(-1);
            MainDir[FileName]->setsingleInDirect(-1);
            MainDir[FileName]->setdoubleInDirect(-1);
            delete(MainDir[FileName]);
        }
        MainDir.erase(FileName);
        return 1;
    }
    // ------------------------------------------------------------------------
    int ReadFromFile(int fd, char *buf, int len ) {
        for(int i =0;i<512;i++){
            buf[i]='\0';
        }
        bool flag = false;
        if (fd>OpenFileDescriptors.size()){
            printf("This file does not exist !\n");
            return -1 ;
        }
        for (int i = 0; i < OpenFileDescriptors.size(); i++) {
            if (OpenFileDescriptors[fd].getFileName() == OpenFileDescriptors[i].getFileName()) {
                if(OpenFileDescriptors[fd].isInUse() == true) {
                    flag = true;
                }
            }
        }
        if (is_formated == false ) {
            printf("The file is not formated yet! \n");
            return -1;
        }
        if (flag == false){
            printf("The file is close! \n");
            return -1;
        }
        if(len==0){
            for(int i =0;i<512;i++){
                buf[i]='\0';
            }
        }
        if (len > 0) {
            int numOfneededblocks = len / blockSize;
            int r = 0;
            bool flagg = false;
            int numOfblockInuse=OpenFileDescriptors[fd].getInode()->getblock_in_use();
            if (OpenFileDescriptors[fd].getInode()->getblock_in_use()<4){
                numOfblockInuse=OpenFileDescriptors[fd].getInode()->getblock_in_use();
            }
            if(OpenFileDescriptors[fd].getInode()->getblock_in_use()>3 && OpenFileDescriptors[fd].getInode()->getblock_in_use()<3+blockSize+2){
                numOfblockInuse--;
            }
            if(OpenFileDescriptors[fd].getInode()->getblock_in_use()>3+blockSize+1 && OpenFileDescriptors[fd].getInode()->getblock_in_use()<3+blockSize+1+(blockSize*blockSize)+2+blockSize) {
                int dublle = OpenFileDescriptors[fd].getInode()->getdoubleInDirect();
                char character='c';
                int m=0;
                while((character!=' '||character!='\0')&& m<blockSize) {
                    fseek(sim_disk_fd, (dublle * blockSize)+m ,SEEK_SET);
                    char character= fgetc(sim_disk_fd);
                    if(character!=' ' && character!='\0'){
                        numOfblockInuse--;
                    }
                    m++;
                }
                numOfblockInuse-=2;
            }
            if (len % blockSize != 0 && numOfneededblocks<numOfblockInuse) {
                numOfneededblocks++;
                if (len < blockSize) {
                    r = len;
                } else {
                    int y = len;
                    while (y > blockSize) {
                        y -= blockSize;
                    }
                    r = y;
                }
                flagg = true;
            }
            if (numOfneededblocks > numOfblockInuse-1) {
                numOfneededblocks = numOfblockInuse;
                len = numOfneededblocks * blockSize;
                if (len % blockSize != 0 ) {
                    numOfneededblocks++;
                    if (len < blockSize) {
                        r = len;
                    } else {
                        int y = len;
                        while (y > blockSize) {
                            y -= blockSize;
                        }
                        r = y;
                    }
                    flagg = true;
                }
                if (flagg == true) {
                    numOfneededblocks--;
                    len = numOfneededblocks * blockSize;
                    len += r;
                }
            }
            int ind = 0;
            while (len > 0) {
                if (OpenFileDescriptors[fd].getInode()->getdirectBlock1() != -1) {
                    int direct1 = OpenFileDescriptors[fd].getInode()->getdirectBlock1();
                    char character = 'c';
                    int h = 0;
                    while ((character != ' ' || character != '\0') && h < blockSize && len > 0) {
                        fseek(sim_disk_fd, (direct1 * blockSize) + h, SEEK_SET);
                        char character = fgetc(sim_disk_fd);
                        buf[ind] = character;
                        ind++;
                        len--;
                        h++;
                    }
                    if (len == 0) {
                        break;
                    } else {
                        if (OpenFileDescriptors[fd].getInode()->getdirectBlock2() != -1) {
                            int direct2 = OpenFileDescriptors[fd].getInode()->getdirectBlock2();
                            char character = 'c';
                            int h = 0;
                            while ((character != ' ' || character != '\0') && h < blockSize && len > 0) {
                                fseek(sim_disk_fd, (direct2 * blockSize) + h, SEEK_SET);
                                char character = fgetc(sim_disk_fd);
                                buf[ind] = character;
                                len--;
                                ind++;
                                h++;
                            }
                            if (len == 0) {
                                break;
                            } else {
                                if (OpenFileDescriptors[fd].getInode()->getdirectBlock3() != -1) {
                                    int direct3 = OpenFileDescriptors[fd].getInode()->getdirectBlock3();
                                    int h = 0;
                                    while ((character != ' ' || character != '\0') && h < blockSize && len > 0) {
                                        fseek(sim_disk_fd, (direct3 * blockSize) + h, SEEK_SET);
                                        char character = fgetc(sim_disk_fd);
                                        len--;
                                        buf[ind] =(char) character;
                                        ind++;
                                        h++;
                                    }
                                }
                            }
                            if (len == 0) {
                                break;
                            } else {
                                int single = OpenFileDescriptors[fd].getInode()->getsingleInDirect();
                                int q = 0;
                                char character = 'c';
                                while ((character != ' ' || character != '\0') && q < blockSize && len > 0) {
                                    fseek(sim_disk_fd, (single * blockSize) + q, SEEK_SET);
                                    char character = fgetc(sim_disk_fd);
                                    int intValue = (int) character;
                                    q++;
                                    int w = 0;
                                    while ((character != ' ' || character != '\0') && w < blockSize && len > 0) {
                                        fseek(sim_disk_fd, (intValue * blockSize) + w, SEEK_SET);
                                        char character = fgetc(sim_disk_fd);
                                        buf[ind] = (char)character;
                                        ind++;
                                        w++;
                                        len--;
                                    }
                                    if (len == 0) {
                                        break;
                                    }
                                }
                                if (len == 0) {
                                    break;
                                } else {
                                    int doublle = OpenFileDescriptors[fd].getInode()->getdoubleInDirect();
                                    char character = 'c';
                                    int q = 0;
                                    while ((character != ' ' || character != '\0') && q < blockSize && len > 0) {
                                        fseek(sim_disk_fd, (doublle * blockSize) + q, SEEK_SET);
                                        char character = fgetc(sim_disk_fd);
                                        int intValue = (int) character;
                                        int w = 0;
                                        while ((character != ' ' || character != '\0') && w < blockSize && len > 0) {
                                            fseek(sim_disk_fd, (intValue * blockSize) + w, SEEK_SET);
                                            char character = fgetc(sim_disk_fd);
                                            int in = (int) character;
                                            int z = 0;
                                            while ((character != ' ' || character != '\0') && z < blockSize && len > 0) {
                                                fseek(sim_disk_fd, (in * blockSize) + z, SEEK_SET);
                                                char character = fgetc(sim_disk_fd);
                                                buf[ind] = (char)character;
                                                ind++;
                                                z++;
                                                len--;
                                            }
                                            w++;
                                        }
                                        if (len == 0) {
                                            break;
                                        }
                                        q++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
          return len;
    }

    // ------------------------------------------------------------------------
    int GetFileSize(int fd) {
        bool flagg = false;
        if (fd>OpenFileDescriptors.size()){
            printf("This file does not exist !\n");
            return -1 ;
        }
        for(int i = 0; i <OpenFileDescriptors.size();i++){
            if (OpenFileDescriptors[fd].getFileName()==OpenFileDescriptors[i].getFileName()){
                if(OpenFileDescriptors[fd].isInUse()==true){
                    flagg= true;
                }
            }
        }
        if (flagg == false){
            perror("The file is close! \n");
            return -1;
        }
        return OpenFileDescriptors[fd].getInode()->getfileSize();
    }

    // ------------------------------------------------------------------------
    int CopyFile(string srcFileName, string destFileName) {
        bool flag = false;
        int fd1 ;
        int fd2;
        for (int i = 0; i < OpenFileDescriptors.size(); i++) {
            if (OpenFileDescriptors[i].getFileName() == srcFileName) {
                fd1 = i;
                if(OpenFileDescriptors[i].isInUse()==true) {
                    flag = true;
                }
                break;
            }
        }
        if (is_formated == false ) {
            printf("the file is not formated !\n");
            return -1;
        }
        if (MainDir.find(srcFileName)== MainDir.end()){
            printf("the srcFile is not exist!\n");
            return -1;
        }
        if(flag == true){
            printf("The srcFile is open !\n");
            return -1;
        }
        else{
            fd1=OpenFile(srcFileName);
        }
        if (MainDir.find(destFileName)== MainDir.end()){
            fd2=CreateFile(destFileName);
            if (fd2 == -1){
                printf("Cant creat this file!\n");
                CloseFile(fd1);
                return -1;
            }
        }
        else{
            bool flagg = false;
            for (int i = 0; i < OpenFileDescriptors.size(); i++) {
                if (OpenFileDescriptors[i].getFileName() == destFileName) {
                    fd2 = i;
                    if(OpenFileDescriptors[i].isInUse()==true) {
                        printf("The destFile is already open!\n");
                        CloseFile(fd1);
                        flagg=true;
                        return -1;
                    }
                  break;
                }
            }
            if(flagg==false) {
                if (MainDir[destFileName]->getdirectBlock1() != -1) {
                    int direct1 = MainDir[destFileName]->getdirectBlock1();
                    BitVector[direct1] = 0;
                }
                if (MainDir[destFileName]->getdirectBlock2() != -1) {
                    int direct2 = MainDir[destFileName]->getdirectBlock2();
                    BitVector[direct2] = 0;
                }
                if (MainDir[destFileName]->getdirectBlock2() != -1) {
                    int direct3 = MainDir[destFileName]->getdirectBlock3();
                    BitVector[direct3] = 0;
                }
                if (MainDir[destFileName]->getsingleInDirect() != -1) {
                    int single = MainDir[destFileName]->getsingleInDirect();
                    BitVector[single] = 0;
                    int p = 0;
                    while (p < blockSize) {
                        int in = 0;
                        fseek(sim_disk_fd, (single * blockSize) + p, SEEK_SET);
                        char character = fgetc(sim_disk_fd);
                        in = (int) character;
                        if(in ==0 ){
                            break;
                        }
                        BitVector[in] = 0;
                        p++;
                    }
                }
                if (MainDir[destFileName]->getdoubleInDirect() != -1) {
                    int doublle = MainDir[destFileName]->getdoubleInDirect();
                    BitVector[doublle] = 0;
                    int l = 0;
                    while (l < blockSize) {
                        int in = 0;
                        fseek(sim_disk_fd, (doublle * blockSize) + l, SEEK_SET);
                        char character = fgetc(sim_disk_fd);
                        in = (int) character;
                        if(in ==0 ){
                            break;
                        }
                        BitVector[in] = 0;
                        l++;
                        int m = 0;
                        while (m < blockSize) {
                            int yi = 0;
                            fseek(sim_disk_fd, (in * blockSize) + m, SEEK_SET);
                            character = fgetc(sim_disk_fd);
                            yi = (int) character;
                            if(yi ==0 ){
                                break;
                            }
                            BitVector[yi] = 0;
                            m++;
                        }
                    }
                }
                fd2 = OpenFile(destFileName);
            }

        }
        char buff[DISK_SIZE];
        ReadFromFile(fd1 , buff ,OpenFileDescriptors[fd1].getInode()->getfileSize());
        MainDir[destFileName]->setfile_size(0);
        MainDir[destFileName]->setblock_size(0);
        MainDir[destFileName]->setblock_in_use(0);
        MainDir[destFileName]->setdirectBlock1(-1);
        MainDir[destFileName]->setdirectBlock2(-1);
        MainDir[destFileName]->setdirectBlock3(-1);
        MainDir[destFileName]->setsingleInDirect(-1);
        MainDir[destFileName]->setdoubleInDirect(-1);
        WriteToFile(fd2,buff,OpenFileDescriptors[fd1].GetFileSize());
        CloseFile(fd1);
        CloseFile(fd2);
        return 1;
    }

    // ------------------------------------------------------------------------
    int RenameFile(string oldFileName, string newFileName) {
        bool flag = false;
        int fd;
        for (int i = 0; i < OpenFileDescriptors.size(); i++) {
            if (OpenFileDescriptors[i].getFileName() == oldFileName) {
                fd=i;
                if(OpenFileDescriptors[i].isInUse()==true) {
                    flag = true;
                }
            }
        }
        if (MainDir.find(oldFileName)== MainDir.end()){
            printf("The oldFileName is not existed!\n");
            return -1;
        }
        if (MainDir.find(newFileName)!= MainDir.end()){
            printf("The new name is already used!\n");
            return -1;
        }
        if (is_formated == false ) {
            printf("Is not formated it \n");
            return -1;
        }
        if(flag == true){
            printf("The file is already open! \n");
            return -1;
        }
        fsInode* fs_file = OpenFileDescriptors[fd].getInode();
        FileDescriptor fdi (newFileName , fs_file);
        MainDir.erase(oldFileName);
        MainDir.insert({newFileName ,OpenFileDescriptors[fd].getInode()});
        OpenFileDescriptors[fd]=fdi;
        OpenFileDescriptors[fd].setInUse(false);
        return 1;
    }
};

int main() {
    int blockSize;
    int direct_entries;
    string fileName;
    string fileName2;
    char str_to_write[DISK_SIZE];
    char str_to_read[DISK_SIZE];
    int size_to_read;
    int _fd;

    fsDisk *fs = new fsDisk();
    int cmd_;
    while(1) {
        cin >> cmd_;
        switch (cmd_)
        {
            case 0:   // exit
                delete fs;
                exit(0);
                break;

            case 1:  // list-file
                fs->listAll();
                break;

            case 2:    // format
                cin >> blockSize;
                fs->fsFormat(blockSize);
                break;

            case 3:    // creat-file
                cin >> fileName;
                _fd = fs->CreateFile(fileName);
                cout << "CreateFile: " << fileName << " with File Descriptor #: " << _fd << endl;
                break;

            case 4:  // open-file
                cin >> fileName;
                _fd = fs->OpenFile(fileName);
                cout << "OpenFile: " << fileName << " with File Descriptor #: " << _fd << endl;
                break;

            case 5:  // close-file
                cin >> _fd;
                fileName = fs->CloseFile(_fd);
                cout << "CloseFile: " << fileName << " with File Descriptor #: " << _fd << endl;
                break;

            case 6:   // write-file
                cin >> _fd;
                cin >> str_to_write;
                fs->WriteToFile( _fd , str_to_write , strlen(str_to_write) );
                break;

            case 7:    // read-file
                cin >> _fd;
                cin >> size_to_read ;
                fs->ReadFromFile( _fd , str_to_read , size_to_read );
                cout << "ReadFromFile: " << str_to_read << endl;
                break;

            case 8:   // delete file
                cin >> fileName;
                _fd = fs->DelFile(fileName);
                cout << "DeletedFile: " << fileName << " with File Descriptor #: " << _fd << endl;
                break;

            case 9:   // copy file
                cin >> fileName;
                cin >> fileName2;
                fs->CopyFile(fileName, fileName2);
                break;

            case 10:  // rename file
                cin >> fileName;
                cin >> fileName2;
                fs->RenameFile(fileName, fileName2);
                break;

            default:
                break;
        }
    }

}