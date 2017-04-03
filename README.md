# Linux-C-Shell
Virtual C shell contained purley in memory.

Linux shell impletmentation:
while loop implemented named keks, this shell takes an argument which can be any length, as it calls a method which dynamically allocates the input. The user is allowed to input a string that calls a command and up to three arguments. The input is entered as a string and the space between the command and argument acts as a delimiter that splits the command from the argument. It is stored in up to three separate stings. A “select case” is the used using if else statements to call a command function, and will error if the command does not exist. The second string that holds the argument(s) is the passed into the function if the command takes an argument. The command functions implemented include the following: mkfs, mkdir, rmdir, cd, stat, ls, mkfile, rmfile, exit, open, read, write, seek, and close.
This program is based entirely on a Tree and linked lists written from scratch. Each file directory has a pointer which points to the next directory, it has a pointer that points to the parent directory, it has a linked list of subdirectories, and a linked list of files. 
The files are all stored in each directory node in a linked list. 
File Descriptors are stored in a linked list, with each File descriptor having an FD int value, offset, pointer to the next File Descriptor, and a pointer to the open file.

**mkfs:**

starts by calling the root directory and checks if there are existing subdirectories. Then returns to the parent, checks if the subdirectories have files associated. Checks if the parent files have any files associated with it then a while loop continuously deallocates the memory by calling a file or directory remove function until there are no files and/or subdirectories associated with the parent directory. If there are subdirectories associated with the root it will go to subdirectory then recursively remove them by assigning them to be the parent as the passed argument.

**mkdir:**

first checks if the directory exists by doing a name comparison on the list of subdirectories associated with the current directory. Allocates memory for the different data pointers, then combines the path of the parent directory and previous directories with an allocation of a path of the current directory based on the name string, assigns to the path pointer of the new directory being created.

**rmdir:**

first check is the subdirectory list exists then traverses the list using the next pointer, which is a data member of the struct: dir. Finds the directory and checks if the directory has any files or folders in it. If the directory is empty, it removes it by traversing the subdirectory list using next pointer. 

**cd:**

appends the argument to ensure there are no slashes in the beginning or end. Passes the parent and the subdirectories and the argument into the function which will recursively append the argument based on the first delamination of the slash. Compares the left argument with “..” and assigns a temp pointer to the parent directory and redirects the current directory to the be parent directory. Lastly,  using a while loop checks if the left part of the argument name exists as a name for any of the subdirectories using the next pointer and if so moves on to the right side of the argument. 

**stat:**

Stat command pulls data regarding the specified files or directories. It passes pointers to the current directories files, subdirectories and argument. It will traverse the subdirectory using a while loop, and break if the argument name matches with any of the nodes. It will then Print the nodes, name, and date if the node does not equal null. It traverses the files using a while loop, and breaks if the argument name matches with any of the nodes. It Prints the node's name and date, if the node does not equal null. 

**ls:**

The ls command lists the contents. This function identifies whether the contents are directories or files. Calls an ls function with the passed arguments of current directory’s subdirectories-list-pointer and current directory’s file-list pointer. You traverse the subdirectory using a while loop and the next pointer until the subdirectory does equal null and on each pass print the subdirectory’s name. You traverse the file list using a while loop and the next pointer until the file does equal null and on each pass print the file’s name.

**mkfile:**

first checks if the file exists by doing a name comparison on the list of files associated with the current directory. Allocates memory for the different data pointers, and sets the data pointer to be equal to “”.

**rmfile:**

checks if the file list exists then traverses the list using the next pointer which is a data member of the struct: file. Find the file and remove it by traversing the file list using next pointer. 

**exit:**

“break” the while loop running the keks shell thereby allowing the program to end. 

**open:**

checks if the file exists by searching the files list associated with the current directory. If the file does not exist, call createfile i.e mkfile and then call the function open which passes the parameters of the address of the file descriptor root and the file pointer pointing to the created file, otherwise, call the function open which passes the parameters of the file descriptor root and the result of the search. You create a new file descriptor node and its new file descriptor is the old file descriptor +1, also prints “SUCCESS” and the new file descriptor node’s FD.

**read:**

the shell subdivides the argument into two distinct parts. Calls the function read, which takes in argument 1, 2 and the address of the file descriptor head node as parameters. Checks if the file descriptor is less than 3, if so do nothing. Checks if the file descriptor exists by going through the file descriptor list using a while loop and the next pointer. If it reaches the end of the list and a null pointer is returned, do nothing. Using the current offset as the starting point, and a for loop prints each next character in the data string of the file and if the null character is reached, exits the loop and sets the offset at the end of file.

**write:**

the shell subdivides the argument into two distinct parts. Calls the function read, which takes in argument 1, 2 and the address of the file descriptor head node as parameters. Checks if the file descriptor is less than 3, if so do nothing. Checks if the file descriptor exists by going through the file descriptor list using a while loop and the next pointer. If it reaches the end of the list and a null pointer is returned, do nothing. Combines the necessary parts of the old and the new data strings into a new one. 

**seek:**

the shell subdivides the argument into three distinct parts. Calls the function read, which takes in argument 1, 2 and the address of the file descriptor head node as parameters. Checks if the file descriptor is less than 3, if so do nothing. Checks if the file descriptor exists by going through the file descriptor list using a while loop and the next pointer. Compares argument 3 with either SEEK_END, SEEK_SET or SEEK_CUR, based on which one is used, moves the offset according to argument 2 i.e relative position. 

**close:**

checks if a file descriptor node list exists, if it doesn’t, do nothing. Then traverses the list using the next pointer, which is a data member of the struct: filedescriptor. Finds the filedescriptor and removes it by traversing the filedescriptor list using next pointer.
