# CS525_Database_System_Assignment1
/********************************************************/
                   Team Information:
/********************************************************/
Group No 12
Members:
1) Tanmaya Bhatt (A20381345)
2) Shushupti Ajmire (A20385389)
3) Vikram Gavisiddeshwara (A20356257)
4) Saisruthi Kolanupaka (A20388341)


/********************************************************/
                   Storage Manager: 
/********************************************************/

Storage manager is a interface to the systems storage service.
 
It is used to read blocks from files on the local disk and write it to memory and viceversa.


/********************************************************/
                      How to Run:
/********************************************************/

make assignment1  (to run default testcases)

make Add_testcases (to run additional testcases)


/********************************************************/
      Functionalitys provided in the Storage manager:
/********************************************************/

1. CreatePageFile: 
It takes input from the user for the file name to be created and creates the file.
It creates an empty block in begining of the file.
To start writing the data, append a block to the file.

2. OpenPageFile: 
It asks the user to enter the filename to be opend and opens the file if filename is valid and exists.

3. ClosePageFile: 
It will close the specified file if the entered filename is valid.

4. DestroyPageFile:
It will destroy the file specified.

5. ReadBlock:
It will read the block/page specified by the user inside a file.

6. GetBlockPosition: 
It is used to get the current block/page number inside the file.

7. ReadFirstBlock: 
It is used to read the first block of the file already opened by the user.It will throw and error if there is no file to read.

8. ReadPreviousBlock:
It is used to read the previous block/page from the file opened by the user.It will throw and error if there is no file to read.

9. ReadCurrentBlock: 
It will read the current block/page in the file opened by the user.It will throw and error if there is no file to read.

10. ReadNextBlock: 
It will read the next block/page in the file opened, it will throw and error if there is no file to read.

11. ReadLastBlock: 
It will read the last block accessed by the user inside the file.It will throw and error if there is no file to read.

12. WriteBlock: 
It will take the input charecter from the user and will write it to the file alraedy opened by the user.

13. write to current block: 
It will take input charecter from the user and will write it to the current block/page in the opened file.

14. Append Empty block: 
It will append the empty block/page in the end of the file opened by user.

15. Ensure Capacity of the File: 
It is used to set the maximum number of blocks/pages which can be accomaodated by the file. 

16. Exit:
It will exit the file.



/********************************************************/
               Files and their purpose:
/********************************************************/

1. testcase.c: 
It provides the user interface displayed to the user,which gives the option to perform the different file operations for the users like read,write and delete the files.

2. Storage_mgr.c: 
This file contains the main logic of the storage manager.

3. storage_mgr.h:
It provides the  storage manager interface.

4. dberror.h, dberror.c : 
They provide the error handling strategies for the storage manager application.

5. test_assign1_1.c: 
It provides test cases to test the storage manager application.


