#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>




int main(int argc, char *argv[]) {
	
	FILE *fp;
	char * line1 = NULL;
    size_t len1 = 0;
    char * line2 = NULL;
    size_t len2 = 0;
    ssize_t read;
    char *cmdargv1[1024];
    char *cmdargv2[1024];
    int n=0;
    int mypipe[2];
    
    pid_t pid;
	
	fp=fopen(argv[1], "r");
	if (fp == NULL)
        exit(EXIT_FAILURE);
        
    read = getline(&line1, &len1, fp);
    printf("%s", line1);
   while (cmdargv1[n]=strtok(line1," \n")) {
    		printf("%s",cmdargv1[n]);
    		line1=NULL;
    		n++;
    	}
    
    n=0;
    read = getline(&line2, &len2, fp);
    while (cmdargv2[n]=strtok(line2," \n")) {
    		line2=NULL;
    		n++;
    	}
    
    	
    	
    	if( pipe(mypipe) == -1){
          perror("Pipe failed");
          exit(1);
        }
    	
    	if( fork() == 0) {
    		close(STDOUT_FILENO);  //closing stdout
    		close(mypipe[0]);	   //closing pipe read
            dup(mypipe[1]);         //replace stdout
            //close(mypipe[1]);
            printf("%s",cmdargv1[1]);
    		execvp(cmdargv1[0],cmdargv1);
    		
    		perror("execvp of first failed");
            exit(1);
    	}
    	
    	if( fork() == 0) {
    		
    		close(STDIN_FILENO);   //closing stdin
            close(mypipe[1]);       //closing pipe write
            dup(mypipe[0]);         //replacing stdin with pipe read
            //close(mypipe[0]);
            execvp(cmdargv2[0],cmdargv2);
    		perror("execvp of second failed");
            exit(1);
        }

    	close(mypipe[0]);
        close(mypipe[1]);
        wait(0);
        wait(0);
        
        
    
	
		
	
	
	
	fclose(fp);
	

}
