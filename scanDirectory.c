//this program for scan directory with infinit max-depth on windows and linux/unix

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

//to check OPERATING SYSTEM
#if defined(WIN32) || defined(_WIN32)
	#define separator '\\'
	#define stateFolder 16384
	#define stateFile 16384*2
	#define clearConsole "cls"

#elif defined(__unix__) || defined(linux)
	#define separator '/'
	#define stateFolder 4
	#define stateFile 4*2
	#define clearConsole "clear"

#endif

//all functions used
short unsigned int scanDirectory(char*);
void print(char*);

//principal function
int main(int argc, char** argv){
	scanDirectory(".");

	return 0;
}

short unsigned int scanDirectory(char *path){
	//this variable used to check the last character of the path if is a '/' or not
	//if yes (exist) we just a copy the path to this variable
	//else on copy this path on the variable and add the '/' in last character of this path and close path with '\0'
	char* work_path = NULL;

	work_path = (char*)malloc(strlen(path)+1);
	for(int i=0;i<(int)strlen(path);i++)	*(work_path + i) = *(path + i);
	*(work_path + strlen(path)) = '\0';

	if(path[strlen(path) - 1] != '/'){
		work_path = (char*)realloc(work_path,strlen(path)+2);
		*(work_path + strlen(path)) = separator;
		*(work_path + (strlen(path) + 1)) = '\0';
	}

	//check directory if it's exist or not
	DIR* dir = NULL;
	if((dir = opendir(work_path)) == NULL){
		printf("[Failed] ==> Cannot open this directory : %s\n",work_path);
		return -1;
	}

	//read directory
	struct dirent* liste;
	while((liste = readdir(dir)) != NULL){
		if(strcmp(liste->d_name,".") && strcmp(liste->d_name,"..")){ //to remove (. and ..) directory

			char* query =(char*)malloc(sizeof(work_path) + sizeof(liste->d_name));
			strcpy(query,work_path);
			strcat(query,liste->d_name);

			//check the result is a directory
			if(liste->d_type == stateFolder){
				//scan another time this directory (recursion)
				//and check the permission
				//if current user don't have permission on the folder the ransomware will ignore it and continue to another folder
				if(scanDirectory(query) == -1){
					continue;
				}
			}

			//check the result is a file
			if(liste->d_type == stateFile){
				print(query);
			}
			free(query);
		}
	}

	//free all pointers used
	free(work_path);

	//close directory
	closedir(dir);
	return 0;
}

void print(char* path){
	printf("%s\n",path);
}

