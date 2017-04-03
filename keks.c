//keks.c version 0.95
//Neal Wolff
//12,05,2015


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

char *allocateString(FILE *FilePointer, size_t size); //prototype of string allocation method
char * removeSlash(char *input);

int main(int argc, char *argv[]) {
	int length = 150;
	//This is the user command for the shell
	char *command; 
	//The command can take up to three arguments
	char *argument;
	char *argument2;
	char *argument3;
	char *search = " ";
	int startFD =2;
	
	
	//creates root directory.
	dirNode *root= NULL;
	root = malloc(sizeof(dirNode));
	createDir(NULL, &root, "/");

	dirNode *pwd = root; //make the working directory the root
	
	//creates a file descriptor
	fileDes *FD= NULL;
	FD = malloc(sizeof(fileDes));
	FD->fD=startFD;
	FD->fdfile=NULL;
	FD->next=NULL;

	//shell loop
	while (1)
	{

		printf("Keks:~ %s $ ", pwd->path); //shell prompt
		command = allocateString(stdin, 10);
		
		//if nothing is entered, go back to the prompt.
		if(command[0]=='\0')
			continue;

		strtok_r(command, " ", &argument); //split the argument and the command

		if(strcmp(command, "read")==0 || strcmp(command, "write")==0 || strcmp(command, "seek")==0){
			strtok_r(argument, " ", &argument2);
			if(strcmp(command, "seek")==0)
				strtok_r(argument2, " ", &argument3);
		}



		//calls the approperate method for the command
		//the argument is passed to the command function
		if (strcmp(command, "exit") == 0)  //exits the shell
		{
			printf("Exiting kek shell\n");
			break;
		}
		else if (strcmp(command, "open") == 0) //opens a file
		{
			fileNode *pass = findFile(pwd->files, argument);
			if(pass == NULL){
				printf("File does not exist. Creating file %s\n", argument);
				createFile(&pwd->files, argument);
				pass = findFile(pwd->files, argument);
				open(&FD,&pass);
			}
			else
				open(&FD, &pass);
		}
		else if (strcmp(command, "read") == 0)//read a file
		{
			read(argument, argument2, &FD);
		}
		else if (strcmp(command, "write") == 0)  //write to the file
		{
			write(argument, argument2, &FD);
		}
		else if (strcmp(command, "seek") == 0) //seek through a file
		{
			seek(argument, argument2, argument3, &FD);
		}
		else if (strcmp(command, "close") == 0) //close a file
		{
			removeFD(FD,&FD, argument);
		}
		else if (strcmp(command, "mkdir") == 0) //create a directory
		{
			if(strcmp(argument, "") == 0){
				printf("Not a valid input\n");
				continue;
			}
			createDir(&pwd, &pwd->subdirs, argument);
		}
		else if (strcmp(command, "rmdir") == 0) //remove a directory 
		{
			removeDir(pwd->subdirs, &pwd->subdirs, argument);
		}
		else if (strcmp(command, "ls") == 0) //list the files and directorys in the current working directory
		{
			ls(pwd->subdirs,pwd->files);
		}
		else if (strcmp(command, "mkfile") == 0) //make a file
		{
			if(strcmp(argument, "") == 0){
				printf("Not a valid input\n");
				continue;
			}
			createFile(&pwd->files, argument);
		}
		else if (strcmp(command, "rmfile") == 0) //remove a file
		{
			removeFile(pwd->files,&pwd->files,argument);
		}
		else if (strcmp(command, "clear") == 0) //clear the terminal
		{
			system("clear");
		}
		else if (strcmp(command, "stat") == 0) //stats
		{
			stat(pwd->files, pwd->subdirs, argument);
		}
		else if (strcmp(command, "mkfs") == 0) //format the filesystem
		{
			removeAll(root);
			removeAllFDS(FD);
			pwd = root;
			
		}
		else if (strcmp(command, "cd") == 0) //change the directory
		{
			char * lel = removeSlash(argument);
		
			dirNode *temp=pwd;
			temp = findNode(pwd->parent, pwd->subdirs, lel);
			if(temp!=NULL)
				pwd=temp;
			else
				printf("Directory does not exist\n");
		}
		else
		{
			printf("the command %s is not valid\n", command);
		}
		
	}

  return 0;
}



//this method allows for the user to input a string of any size.
char *allocateString(FILE* FilePointer, size_t size){
    char *input;
    int pos;
    size_t length = 0;

    input = realloc(NULL, sizeof(char)*size);//size is start size
    if(!input)return input;
    while(EOF!=(pos=fgetc(FilePointer)) && pos != '\n'){
        input[length++]=pos;
        if(length==size){
            input = realloc(input, sizeof(char)*(size+=16));
            if(!input)return input;
        }
    }
    input[length++]='\0';

    return realloc(input, sizeof(char)*length);
}


//this method removes the "/" from the front and back.
char * removeSlash(char *input)
{
  int len = strlen(input); 
  if(input[0]=='/'){
  	
 	 if(len > 0)
    		input++;//Go past the first char
  }
  if(input[len-2]== '/'){
  	if(len > 1)
    		input[len - 2] = '\0';//Replace the last char with a null termination
}
  return input;
}

