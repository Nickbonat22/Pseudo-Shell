/*
* Description: CIS 415 Project 1: Pseudo-Shell: a single-threaded UNIX system command-line interface
*
* Author: Nicholas Bonat
*
* Date: 10/13
*
* Notes: 
* 1. \n\r on line 72 took me 3 hours to figure out
*/
//comment style below taken from lap 3 skeleton code
/*-------------------------Preprocessor Directives---------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "command.h"
#include "command.c"
/*---------------------------------------------------------------------------*/

// function to parse the input token and call the correct command function
void parseCommand(char * token){
	// function vars
	char *command, *para1, *para2;
	command = strtok(token, " ");
	para1 = strtok(NULL, " \n");
	para2 = strtok(NULL, " \n");

	while(command != NULL){
		if(strstr(command, "ls") || strstr(command, "pwd")){
			// error handling
			if(para1 != NULL ){
				printf("Error! Incorrect Syntax.\n");
			}
			else if(strstr(command, "ls")){
				listDir();
			}
			else if(strstr(command, "pwd")){
				showCurrentDir();
			}
		}
		else if(strstr(command, "mkdir") || strstr(command, "rm") || strstr(command, "cd") ||
			strstr(command, "cat")){
			// error handling
			if(para1 == NULL){
				printf("Error! Incorrect Syntax.\n");
			}
			else if(strstr(command, "mkdir")){
				makeDir(para1);
			}
			else if(strstr(command, "rm") ){
				deleteFile(para1);
			}
			else if(strstr(command, "cd")){
				changeDir(para1);
			}
			else if(strstr(command, "cat") ){
				displayFile(para1);
			}
		}else{
			// error handling
			if(para1 == NULL || para2 == NULL){
				printf("Error! Incorrect Syntax.\n");
			}
			else if(strstr(command, "cp") && para1 != NULL && para2 != NULL){
				copyFile(para1, para2);
			}
			else if(strstr(command, "mv") && para1 != NULL && para2 != NULL){
				moveFile(para1, para2);
			}
		}
		command = strtok(NULL, " ");
	}
}
/*-----------------------------Program Main----------------------------------*/
int main(int argc, char *argv[] ) {
	// function vars
	char *token = NULL;
	
	// INTERACTIVE MODE
	if(strcmp(argv[1], "-command") == 0){
		char *cBuffer;
		char *cTemp = NULL;
		size_t bufferSize = 100;
		// Allocate memory to the input buffer.
		cBuffer = (char *)malloc(bufferSize * sizeof(char));
		if(cBuffer == NULL){
			printf("Error! Unable to allocate input buffer. \n");
			exit(1);
		}
		do{
			printf(">>> ");
			getline(&cBuffer, &bufferSize, stdin);
			cTemp = cBuffer;
			while((token = strtok_r(cTemp, ";", &cTemp))){
				// exit program
				if(strcmp(token, "exit\n") == 0 || strcmp(token, "exit") == 0){
					exit(1);
				} 
				parseCommand(token);
			}

		}while(1);
		// free allocated memory, close stdout, exit
		free(cBuffer);
		exit(1);
	}
	// FILE MODE
	else if(strcmp(argv[1], "-f") == 0 && strstr(argv[2], ".txt")){
		// function vars
		FILE *input;
		input = fopen(argv[2], "r");
		char *temp = NULL;
		char *buffer;
		size_t bSize = 300;
		buffer = (char *)malloc(bSize * sizeof(char));
		
		// creates output.txt and redirects console output to the file
		freopen("output.txt", "w", stdout);
		// get each input line and parse each command split by ;
		while((getline(&buffer, &bSize, input)) != -1){
			temp = buffer;
			while((token = strtok_r(temp, ";\n\r", &temp))){
				parseCommand(token);
			}
		}
		fclose(input);
		free(buffer);
	}else{
		printf("Invalid command. Exiting\n");
		exit(1);
	}

	return 0;
}
/*-----------------------------Program End-----------------------------------*/
