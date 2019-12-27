/*
* Description: CIS 415 Project 1: Pseudo-Shell: a single-threaded UNIX system command-line interface
*
* Author: Nicholas Bonat
*
* Date: 10/13
*
* Notes: 
* 1. Functions for commands appear first, then a function to parse commands is last
*/
/*-------------------------Preprocessor Directives---------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "command.h"
/*---------------------------------------------------------------------------*/
/*-------------------------------Functions-----------------------------------*/

// commands for a basic shell
// ls
void listDir(){
	
	// function vars
	char *buf;
    size_t bufsize = 100;
	char cwd[1000];
	buf = (char *)malloc(bufsize * sizeof(char));

	// get the current directory
	getcwd(cwd, sizeof(cwd));
	struct dirent *d;
  
    // open the dir using opendir() 
    DIR *dr = opendir(cwd); 
  
    if (dr == NULL)
    { 
        printf("Could not open current directory"); 
    }
   
	// while loop to read the dir
	while ((d = readdir(dr)) != NULL){
		printf("%s ", d->d_name);
    	
    } 
    // free allocated memory
    printf("\n");
  	free(buf);
    closedir(dr); 
}

// pwd
void showCurrentDir(){
	char cwd[1000];
	if(getcwd(cwd, sizeof(cwd)) != NULL){
		printf("%s\n", cwd);
	} else{
		perror("Error! Couldn't get working directory. Exiting\n");
		exit(1);
	}
}

// mkdir
void makeDir(char *dirName){
	if(dirName == NULL){
		printf("Parameter: %s one cannot be null", dirName);
	}else{
		mkdir(dirName, 1000);		
	}
}

// cd
void changeDir(char *dirName){
	chdir(dirName);
}

// cp
void copyFile(char *sourcePath, char *destinationPath){
	// function vars
	char newSource[strlen(sourcePath) - 1];
	char cwd[1000];
	getcwd(cwd, sizeof(cwd));

	int check;

	// remove whitespace
	if(sourcePath[0] == '.' && sourcePath[1] == '.'){
		changeDir("..");
		int i;
		for(i = 0; i < strlen(sourcePath); i++){
			newSource[i] = sourcePath[i];
		}
		newSource[i] = '\0';
		memmove(newSource, newSource + 3, strlen(newSource));
		check = 1;
	}
	else{
		int i;
		for(i = 0; i < strlen(sourcePath); i++){
			newSource[i] = sourcePath[i];
		}
		newSource[i] = '\0';
		check = 0;
	}

	FILE * file;
	file = fopen(newSource, "r");
	if(file == NULL){
		printf("Error! File doesn't exist\n");
	}
	else if(strcmp(destinationPath, ".") == 0){
		printf("Error! Having trouble with destinationPath\n");
	}
	else{
		FILE *fDestination;
		fDestination = fopen(destinationPath, "w");
		char input;
		// loop to put the contents into file
		while((input = fgetc(file)) != EOF){
			fputc(input, fDestination);
		}
		fclose(file);
		fclose(fDestination);
	}
	// change directory if needed
	if(check == 0){
		chdir(cwd);
	}
}

// mv
void moveFile(char *sourcePath, char *destinationPath){
	int move = rename(sourcePath, destinationPath);
	if(move == -1){
		printf("Error! %s does not exist\n", sourcePath);
	}
}

// rm
void deleteFile(char *filename){
	int delete = unlink(filename);
	if(delete == -1){
		printf("Error! Could not delete %s\n", filename);
	}
}
// cat
void displayFile(char *filename){
	// function vars
	size_t bs = 300;
	char *b = (char*)calloc(bs, sizeof(char));
	int input = open(filename, O_RDONLY);

	if(input == -1){
		printf("Error! Incorrect Syntax.\n");
	}
	else{
		read(input, b, 300);
    	printf("%s", b);
    	// free allocated memory
	}
	free(b);
}