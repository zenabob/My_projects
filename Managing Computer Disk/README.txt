written by Zenab Mahmoud 212384390

Ex5(final project)

==description==
A disk manging simulation.

The program contains a 3 class that called FsInode, FileDescriptor, fsDisk.

FsInode class:
  To save the numbers of the blocks in which the file is sorted.

  The fields of the class are:
    int fileSize: The size of file.
    int block_in_use:The num of blocks that the file uses.
    int directBlock1:The index of the first direct block.
    int directBlock2:The index of the second direct block.
    int directBlock3:The index of the third direct block.
    int singleInDirect:The index of the pointer to the blocks that used.
    int doubleInDirect:The index of the pointer tothe pointers to the blocks that used.
    int block_size: The size of block.
  
  The constructor of the class: fsInode(int _block_size).
  Fill the fields of the class:
        fileSize = 0;
        block_in_use = 0;
        block_size = _block_size;
        directBlock1 = -1;
        directBlock2 = -1;
        directBlock3 = -1;
        singleInDirect = -1;
        doubleInDirect = -1;

 Functions:
       set/get for all the fields.


FileDescriptor class:
   stores the file name and points to the inode of the file.

   The fields of the class are:
      pair<string, fsInode*> file : pair to the name of the file and his inode.
      bool inUse : If equal to true the file is open , else false.


  The constructor of the class:FileDescriptor(string FileName, fsInode* fsi).
  Fill the fields of the class:
        file.first = FileName;
        file.second = fsi;
        inUse = true;

  Functions:
       get for all the fields.
       set for inUse.


 fsDisk class:
   The disk itself,saves all disk data.

   The fields of the class are:
      FILE *sim_disk_fd : The stream of the disk file.
      bool is_formated : True if the disk formatted, false otherwise.
      int BitVectorSize : The size of BitVector
      int *BitVector : Indicate which block in the disk is free or not (bitVector[the i-th block] =1 if the block in use, 0 otherwise).
      int blockSize : The block size in the disk.
      map<string, fsInode*>  MainDir : A map that contains all the files that is written in the disk, the key is the file name, the value is a pointer of the file descriptor.
      vector< FileDescriptor > OpenFileDescriptors : A map that contains all the files that is open, the key is the fd num, the value is a pointer of the file descriptor.
   
  The constructor of the class: fsDisk():
        open the file disk and fill the fields.


  Functions:
     void listAll():
            print all the name of the crated files and theirs status.
            print all the disk file.


    void fsFormat( int blockSize =4, int direct_Enteris_ = 3 ):
            delete every thing in the disk and set the block size in the disk.
            initialize the BitVector array and count the num of blocks in the disk.
            the default size of the block is 4.

    int CreateFile(string fileName):
            creat a new file, and open it (inUse= true).
            update the mainDir,OpenFileDes.
            return the fd in success, -1 in failed.

    int OpenFile(string fileName):
            open file that is already exist.
            update the OpenFileDes.
            return the fd in success, -1 in failed.

   string CloseFile(int fd):
            close existed and opened file.
            update the OpenFileDes.
            return file name in success, -1 in failed.

   int WriteToFile(int fd, char *buf, int len ):
            if it is the first write: find an empty block of the block's index, and blocks to write the file.
            else: find the index block of the file than find a blocks to write in and update the block index.
            return 1 in success, return -1 in failed.


   int DelFile( string FileName ):
            delete the file from the disk.
            update the mainDir, OpenFileDes, BitVictor.
            return 1 in success, return -1 in failed.
  
  int ReadFromFile(int fd, char *buf, int len ):
             read from the file.
             fd: the file desc, buf: the read section will be written here, len: the length of the section that will be read.
             return len in success, return -1 in failed.

 int GetFileSize(int fd):
            Get the size of file.
            return the file size in success, return -1 in failed.

  int CopyFile(string srcFileName, string destFileName):
            copy from scrFileName to destFileName.
            read from scrFileName and write to the destFileName.
            return 1 in success, return -1 in failed.

  int RenameFile(string oldFileName, string newFileName): 
             rename the file to the newFileName.         
            return 1 in success, return -1 in failed.
 to compile:
    g++ stub_code.cpp -o stub_code
to run:
    ./stub_code