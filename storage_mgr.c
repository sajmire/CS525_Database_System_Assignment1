#include <stdio.h>
#include <stdlib.h>
#include "storage_mgr.h"
#include "dberror.h"
#include <sys/stat.h>
#include <string.h>


//File header contains total number of pages
#define PAGELEN 4
//We need a global variable to reset values if a file is closed or destroyed
SM_FileHandle *sm_fileHandle;

void initStorageManager (){
//Not being used in this code
}


RC createPageFile (char *fileName){

	//create the file
	FILE *file = fopen(fileName, "w");

	if(file == NULL)
		return RC_WRITE_FAILED;

	//Add a page with the size same as PAGE_SIZE
	char *page = (char *) malloc(PAGE_SIZE);
	memset(page, '\0', PAGE_SIZE);
	
	int pages = 1;
	
	//Write page nos first and then add an empty block having all '\0'
	
	fwrite(&pages, sizeof(int), 1, file);
	
	fwrite(page, sizeof(char), PAGE_SIZE, file);

	//Close file
	fclose(file);
	free(page);

	return RC_OK;
}


RC openPageFile (char *fileName, SM_FileHandle *fHandle){

	//Check if the file exists, the structure help prevent memory faults
	struct stat buffer; 
	if (stat (fileName, &buffer) != 0)
		return RC_FILE_NOT_FOUND;

	//check if the file is already open
	if (fHandle->fileName == fileName)
		return RC_OK ;

	//Open the page file in r+ because we need to perform read+write
	FILE *file = fopen(fileName, "r+");
	
	int pages;
	//read the page nos put at the beginning of the file
	fread(&pages, sizeof(int), 1, file);

	//set the file handle to current file
	fHandle->fileName = fileName;
	fHandle->totalNumPages = pages;
	fHandle->curPagePos = 0;
	fHandle->mgmtInfo = file;

	//We need a global variable to reset values if a file is closed or destroyed
	sm_fileHandle = fHandle;
	
	return RC_OK;
}


RC closePageFile (SM_FileHandle *fHandle){
	//If file is not open
	if (fHandle->fileName == NULL)
		return RC_FILE_NOT_FOUND;
	
	//Close file
	fclose(fHandle->mgmtInfo);
	
	//Reset all the values in the global file handle
	fHandle->fileName = NULL;
	fHandle->totalNumPages = 0;
	fHandle->curPagePos = 0;
	fHandle->mgmtInfo = NULL;

	return RC_OK;
}


RC destroyPageFile (char *fileName){
	//Remove the file if we get a 0 then the file is removed
	int rem=remove(fileName);
	if (rem == 0){
		//reset all the values in the file handle
		sm_fileHandle->fileName = NULL;
		sm_fileHandle->totalNumPages = 0;
		sm_fileHandle->curPagePos = 0;
		sm_fileHandle->mgmtInfo = NULL;
		return 	RC_OK;
	}

	return RC_WRITE_FAILED;
}


RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){

	//If the file is open or not
	if (fHandle->fileName == NULL)
		return RC_FILE_NOT_FOUND;

	//If the page does not exist
	if(pageNum < 0 || pageNum > fHandle->totalNumPages - 1)
		return RC_READ_NON_EXISTING_PAGE;
	
	//Calculate the location of the pointer
	long int point = ((pageNum) * PAGE_SIZE) + PAGELEN;
	fseek(fHandle->mgmtInfo, point, SEEK_SET);

	//Reading the page
	fread(memPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo);
	
	//Updating the file handle
	fHandle->curPagePos = pageNum;

	return RC_OK;
}


int getBlockPos (SM_FileHandle *fHandle){
	//Check if file is open
	if (fHandle->fileName == NULL)
		return -1;

	return fHandle->curPagePos;
}


RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){

	return readBlock(0,fHandle, memPage);
}


RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){

	return readBlock(fHandle->curPagePos - 1, fHandle, memPage);
}


RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){

	return readBlock(fHandle->curPagePos,fHandle, memPage);
}


RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){

	//If current page is the last page
	if ((fHandle->curPagePos) == fHandle->totalNumPages)
		return RC_READ_NON_EXISTING_PAGE;

	return readBlock(fHandle->curPagePos + 1, fHandle, memPage);
}


RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){

	return readBlock(fHandle->totalNumPages - 1, fHandle, memPage);
}


RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){

	//If file is open
	if (fHandle->fileName == NULL)
		return RC_FILE_NOT_FOUND;

	//If memPage is not NULL
	if (memPage == NULL)
		return RC_RM_NO_PRINT_FOR_DATATYPE;

	//If page does not exist in the file
	if(pageNum < 0 || pageNum > fHandle->totalNumPages - 1)
		return RC_READ_NON_EXISTING_PAGE;

	//Calculate location of the pointer from where to write
	long int point = ((pageNum) * PAGE_SIZE) + PAGELEN;
	fseek(fHandle->mgmtInfo, point, SEEK_SET);

	//Write to the file
	fwrite(memPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo);

	//Updating the file handle
	fHandle->curPagePos = pageNum;
	return RC_OK;
}


RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
		
	return writeBlock(fHandle->curPagePos,fHandle, memPage);
}


RC appendEmptyBlock (SM_FileHandle *fHandle){

	//check if file is open
	if (fHandle->fileName == NULL)
		return RC_FILE_NOT_FOUND;

	//add a new page  of the size of PAGE_SIZE
	char *page = (char *) malloc(PAGE_SIZE);
	memset(page, '\0', PAGE_SIZE);
	
	fHandle->totalNumPages++;

	//update no of pages
	fseek (fHandle->mgmtInfo, 0, SEEK_SET);
	fwrite(&fHandle->totalNumPages, sizeof(int), 1, fHandle->mgmtInfo);

	//write an empty page to the file
	fseek (fHandle->mgmtInfo, 0, SEEK_END);
	fwrite(page, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo);
	fHandle->curPagePos = fHandle->totalNumPages-1;
	free(page);
	return RC_OK;
}


RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle){

	//If file is open
	if (fHandle->fileName == NULL)
		return RC_FILE_NOT_FOUND;
	int i;
	//Calculate the required no of pages to be added, if less value is given we ignore
	int change = numberOfPages - fHandle->totalNumPages;
	if (change > 0){
		for ( i = 0; i < change; i++){
			appendEmptyBlock(fHandle);
		}
	}
	printf("File capacity is ensured");
	return RC_OK;
}
