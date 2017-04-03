#include<stdio.h>
#include<stdlib.h>
#include<time.h>

char * getTime(); //prototype for getting the current date and time method

//directory, contains the name, its path, the date of creation, 
//a pointer to it's parent, a pointer to the next directory in the current directory, 
//a linked list of subdirectories, and a linked list of files
typedef struct dir {
	char *name;
	char *path;
	char *date;
	struct dir *parent;
	struct dir *subdirs;
	struct file *files;
	struct dir *next;
}dirNode;
 
 //a file. contains its name, the date of creation 
 //the data the file contains, and a pointer to the next file in the directory
typedef struct file {
	char *name;
	char *date;
	char *data;
	struct file *next;
}fileNode;

//file descriptor, contains the File Descriptor number, the offset, 
//the file, and a pointer to the next file descriptor
typedef struct fileDescriptor {
	int fD;
	int offset;
	struct file *fdfile;
	struct fileDescriptor *next;
	
}fileDes;

//find a file descriptor in the FD linked list
fileDes *findFD(fileDes *pass, int *fD){
	fileDes *current = pass;
	int lol = *fD;
	int comp;
	
	while(current != NULL)
	{
		comp = current->fD;	
		if(comp == lol)
			break;

		current = current->next;
	}
	return current;
}

//opens a file
void open(fileDes ** head, fileNode ** file) {
	
	int oldFD=(*head)->fD;
	int newFD = oldFD+1;

	fileDes * new_FD; 

	new_FD = malloc(sizeof(fileDes));

	new_FD->fD = newFD;

	new_FD->offset = 0;
	
	new_FD->fdfile = *file;

	new_FD->next = *head;
		
	*head = new_FD;
	printf("SUCCESS, fd=%d\n", (*head)->fD);
}


//reads and prints the file from the specifled offset
//argument 1 is the file descriptor and argument2 is the number of bytes to read
void read(char * arugment1, char * argument2, fileDes **head){
	int fd = atoi(arugment1);

	if(fd < 3){
		printf("invalid input\n");
		return;
	}

	int move = atoi(argument2);
	fileDes * FileDr;
	FileDr = malloc(sizeof(fileDes));
	
	//find the file descriptor
	FileDr = findFD(*head, &fd);
	if(FileDr==NULL){
		printf("File Discriptor does not exitst\n");
		return;
	}
	
	fileNode *tempFile = FileDr->fdfile;

	
	char * p = tempFile->data; //gets the data

	int iter = FileDr->offset; //gets the current position from the offset
	
	move+=iter; //how much to read

	//print the file from the offset position
	for(iter; iter < move; iter++){
		if(p[iter]=='\0')
			break;
		printf("%c", p[iter]);
	}
	printf("\n");

	FileDr->offset = iter; //make the read position the current offset
	
	
	
}
//writes a file
//argument 1 is the file descriptor and argument2 is the number of bytes to write
void write(char * arugment1, char * argument2, fileDes **head){
	int fd = atoi(arugment1);
	if(fd < 3){
		printf("invalid input\n");
		return;
	}
	fileDes * FileDr;
	FileDr = malloc(sizeof(fileDes));
	
	//find the file descriptor
	FileDr = findFD(*head, &fd);
	if(FileDr==NULL){
		printf("File Discriptor does not exitst\n");
		return;
	}

	
	fileNode *tempFile = FileDr->fdfile;

	
	char * p = tempFile->data; 

	
	int size = FileDr->offset;

	char * oldFile;
	int fileSize = snprintf(NULL, 0, "%s", p);
	oldFile=malloc(fileSize+1);

	strncpy(oldFile, p, size);

	int size2 = snprintf(NULL,0, "%s" , argument2);


	int n = snprintf(NULL, 0, "%s%s", oldFile, argument2);
	FileDr->fdfile->data = malloc( n + 1 );

	sprintf(FileDr->fdfile->data, "%s%s", oldFile, argument2);
	int i;
	for(i=0;i<n+1;i++)
		if(FileDr->fdfile->data[i]=='\0')
			break;
	
        FileDr->offset+=size2;


}

//seek through a file and place the offset at the specifed byte location
void seek(char * arugment1, char * argument2, char * argument3, fileDes **head){
	
	int fd = atoi(arugment1);
	if(fd < 3){
		printf("invalid input\n");
		return;
	}

	fileDes * FileDr;
	FileDr = malloc(sizeof(fileDes));
	
	
	//get the File Descriptor
	FileDr = findFD(*head, &fd);
	if(FileDr==NULL){
		printf("File Discriptor does not exitst\n");
		return;
	}

	
	fileNode *tempFile = FileDr->fdfile;

	
	char * p = tempFile->data;
	int fileSize = snprintf(NULL, 0, "%s", p);
	int i;
	int pos = atoi(argument2);
	for(i=0; i<fileSize; i++){
		if(p[i]=='\0')
			break;
	}
	if(strcmp(argument3,"SEEK_END")==0){
		if(pos<=0){
			int seekend = i+pos;
			if(seekend < 0){
				printf("Cannot move offset before beginning of file. Please enter a negative number above current filesize: %d.\n", i);
				return;
			}
			else
				FileDr->offset = seekend;
		}
		else{
			printf("Cannot move offset past end of file.\n");
			return;
		}
	}
	else{
		
		if(strcmp(argument3,"SEEK_SET")==0){
			if(pos<=i && pos >= 0)
				FileDr->offset = pos;
			else{
				printf("the set position is not valid\n");
				return;
			}
		}
		else if(strcmp(argument3,"SEEK_CUR")==0){
			int oldOffset = FileDr->offset;
			int newPos = oldOffset + pos;
			if(newPos<=i && newPos >= 0)
				FileDr->offset = newPos;
			else{
				printf("the set position is not valid\n");
				return;
			}
		}
		else
		{
			printf("the input is not valid\n");
			return;
		}
	}
	printf("the new offset of FD %d is %d\n", FileDr->fD, FileDr->offset);

}


dirNode *findNode(dirNode * parent, dirNode *subdirs, char *name){
	
	
	char * checker;
	checker = strchr(name,'/');
	
	if (checker!=NULL){
		char *leftArg;
		char *rightArg;
		leftArg = name;
		
		strtok_r(leftArg, "/", &rightArg);
		
		dirNode *current=subdirs;
		char *path = parent->path;

		if(strcmp("..", leftArg)==0 && path!=NULL){
			current=parent;
		}

		else
			current = findNode(parent, subdirs, leftArg);

		if(current!=NULL){
			dirNode *NewSubdirs=current;

			current=findNode(NewSubdirs->parent, NewSubdirs->subdirs, rightArg);
			if(current==NULL){

				return NULL;

			}
			else
				return current;
		}


	}

	dirNode * current = subdirs;
	char* path=parent->path;
	if(strcmp("..", name)==0 && path!=NULL)
		current = parent;
	else{

	while(current != NULL)
	{
		if(strcmp(current->name, name) == 0)
			break;

		current = current->next;
	}
	}
	return current;
}

dirNode *findDir(dirNode * parent, dirNode *subdirs, char *name){
	dirNode *current = subdirs;
	while(current != NULL)
	{	
		if(strcmp(current->name, name) == 0)
			break;

		current = current->next;
	}
	return current;
}

fileNode *findFile(fileNode *subdirs, char *name){
	fileNode * current = subdirs;

	while(current != NULL)
	{	
		if(strcmp(current->name, name) == 0)
			break;

		current = current->next;
	}
	return current;
}

//you must pass the file stuctre into this method
void createFile(fileNode ** head, char *name) {
	if(head!=NULL){
		fileNode *temp = findFile(*head, name);
		if(temp != NULL){
			printf("File already exists\n");
			return;
		}
	}
	char * lel = "";
	

	fileNode * new_file; //creates a new file

	

	new_file = malloc(sizeof(fileNode));

	new_file->data = lel;

	new_file->name = name;

	new_file->date = getTime();

	new_file->next = *head;
		

	*head = new_file;
	printf("OK, %s successfully created at %s\n", name, new_file->date);
}

//pass the current directory node into this method
void createDir(dirNode ** pwd, dirNode **subdirRoot, char *name){
	if(pwd!=NULL){
		if(subdirRoot!=NULL){
			dirNode *temp = findDir(*pwd, *subdirRoot, name);
			if(temp != NULL){
				printf("directory already exists\n");
				return;
			}
		}
	}

	dirNode * new_dir;
	new_dir = malloc(sizeof(dirNode));
	dirNode * empty;
	empty = malloc(sizeof(dirNode));

	new_dir->name = name;
	new_dir->date = getTime();
	new_dir->subdirs = NULL;
	new_dir->files = NULL;
	
	if(pwd!=NULL)
	{
	//sets the parent node
	new_dir->parent = *pwd;

	char * oldPath = (*pwd)->path;

	//creates an integer to determine the size of 
	//the memory needed to allocate for the path string
	int n = snprintf(NULL, 0, "%s%s/", (*pwd)->path, name );
	char * p = malloc( n + 1 );
	
	sprintf(p, "%s%s/", (*pwd)->path, name);	
	new_dir->path = p;
	}
	else{
		//parent is sent to null
		new_dir->parent = empty;
		new_dir->path = name;
		
	}

	new_dir->next = *subdirRoot;
	*subdirRoot = new_dir;
	if(strcmp(name,"/")!=0)
		printf("OK, %s successfully created at %s\n", name, new_dir->date);
}

//list the contents of the work directory 
void ls(dirNode *pwd, fileNode *fn){
	dirNode * cDir = pwd;
	fileNode * cfile = fn;
	printf("______\n|     |_____________________________________\n|                                          |\n|  ");
		
		
		
	printf("directories:\n|  ");
	while (cDir != NULL)
	{
		printf("[%s] ", cDir->name);
		cDir = cDir->next;
	}
	printf("\n|  files:\n|  ");
	while (cfile != NULL)
	{
		printf("%s, ", cfile->name);
		cfile = cfile->next;
	}
	printf("\n|                                          |");
	printf("\n|                                          |");
	printf("\n|__________________________________________|\n");
	
}
//remove a file descriptor
void removeFD(fileDes * subdirRoot, fileDes ** subdirRoot2, char * argument) {
	int fD = atoi(argument);
	if(subdirRoot==NULL){
		printf("File Descriptor does not exist\n");
		return;
	}

	fileDes * current = subdirRoot;
	fileDes * prev;
	prev = malloc(sizeof(fileDes));
	
	//if the first directory is the one to be removed,
	//change the root to the next directory
	if(current->fD == fD){
		prev = (*subdirRoot2)->next;
		free(*subdirRoot2);
		*subdirRoot2=prev;
		
	}
	else{ //if it is not the root, find the directory and remove it.
	while(current != NULL)
	{
		
		if(current->fD == fD)
			break;
		prev = current;
		current = current->next;
	}
	//if the directory is not found
	if(current==NULL){
		printf("not found");
		return;
	}
	prev->next=current->next;

	free(current);
	}
	

}

//remove a directory
void removeDir(dirNode * subdirRoot, dirNode ** subdirRoot2, char * name) {
	if(subdirRoot==NULL){
		printf("directory does not exist\n");
		return;
	}
	if(subdirRoot->subdirs!=NULL || subdirRoot->files !=NULL){
		printf("Directory is not empty, cannot remove\n");
		return;
	}

	dirNode * current = subdirRoot;
	dirNode * prev;
	prev = malloc(sizeof(dirNode));
	
	//if the first directory is the one to be removed,
	//change the root to the next directory
	if(strcmp(current->name, name) == 0){
		prev = (*subdirRoot2)->next;
		free(*subdirRoot2);
		*subdirRoot2=prev;
		
	}
	else{ //if it is not the root, find the directory and remove it.
	while(current != NULL)
	{
		
		if(strcmp(current->name, name) == 0)
			break;
		prev = current;
		current = current->next;
	}
	//if the directory is not found
	if(current==NULL){
		printf("Directory not found");
		return;
	}
	prev->next=current->next;

	free(current);
	}
	

}

//remove a file
void removeFile(fileNode * subdirRoot, fileNode ** subdirRoot2, char * name) {
	if(subdirRoot==NULL){
		printf("File does not exist\n");
		return;
	}

	fileNode * current = subdirRoot;
	fileNode * prev;
	prev = malloc(sizeof(fileNode));
	
	//if the first directory is the one to be removed,
	//change the root to the next directory
	if(strcmp(current->name, name) == 0){
		prev = (*subdirRoot2)->next;
		free(*subdirRoot2);
		*subdirRoot2=prev;
		
	}
	else{ //if it is not the root, find the directory and remove it.
	while(current != NULL)
	{
		
		if(strcmp(current->name, name) == 0)
			break;
		prev = current;
		current = current->next;
	}
	if(current==NULL){
		printf("File not found");
		return;
	}
	prev->next=current->next;

	free(current);
	}
}

//recursively remove all files and directories
void removeAll(dirNode * root){
	if (root == NULL)
		return;

	dirNode *current = root;
	
	
		
	while(current->subdirs!=NULL)
		current=current->subdirs;
	if(strcmp(current->name, "/") != 0)
		current = current->parent;
	
	

	//remove all files

	while(current->files != NULL){
		removeFile(current->files, &current->files, current->files->name);
	}
	while(current->subdirs->files != NULL){
		removeFile(current->subdirs->files, &current->subdirs->files, current->subdirs->files->name);
	}
	
	//saves the parent to be passed in
	
	while(current->subdirs != NULL){
		if(current->subdirs->subdirs!=NULL)
			removeAll(current->subdirs);
		

		if(current->subdirs!=NULL){
			removeDir(current->subdirs, &current->subdirs, current->subdirs->name);
		}
		else
			return;
		
	}
	if(strcmp(current->name, "/") != 0){
		removeAll(current->parent);
		
	}
	else{
		printf("Filesystem has been wiped clean\n");
	}
		
	
}
//remove all the file descriptors
void removeAllFDS(fileDes *head){
	if (head == NULL)
		return;
	fileDes *current = head;

	//close all FDs
	while(current!=NULL){
		char * str = malloc(sizeof(int));
		sprintf(str, "%d", current->fD);
		removeFD(current,&current,str);
	}
}

//stats for the file or directory specifled
void stat(fileNode * files, dirNode * subdirs, char * name){
	dirNode * current = subdirs;
	fileNode * current2 = files;
	while(current != NULL)
	{
		if(strcmp(current->name, name) == 0)
			break;

		current = current->next;
	}
	if(current == NULL){
		while(current2 != NULL)
		{
			if(strcmp(current2->name, name) == 0)
				break;

			current2 = current2->next;
		}
		if(current2==NULL)
			printf("Item not found");
		else
			printf("the stats for the file is: %s\n", current2->date);
	}
	else
		printf("the stats for the directory is: %s\n", current->date);

	
		
	
}

//returns the date and time in a string.
char * getTime(){
	time_t timer;
	char *t =malloc(26);

	struct tm* tm_info;

	time(&timer);
	tm_info = localtime(&timer);

	strftime(t, 26, "%Y:%m:%d %H:%M:%S", tm_info);
	return t;

}
