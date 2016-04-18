/*Il secondo esercizio estende il primo. Il file passato come parametro in questo caso ha un numero arbitario di righe
e non solo due come nell'esercizio1. Ogni riga contiene un comando con I rispettivi parametri. Il programma deve
attivare tutti I comandi del file in modo concorrente, e terminare quando tutti finiscono.
L'output di tutti I comandi, tranne l'ultimo, deve essere fornito in input all'ultimo comando (quello nell'ultima riga
significativa del file).
Es: se il file contenesse:
ls -l
ls -l
ls -l
cat
l'output dovrebbe essere la lista dei file della corrente directory 3 volte (nell'output puo' capitare che righe di
comandi diversi vengano frapposte, l'accesso alla pipe e' conteso).
*/


#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

#define MAXBUF 1024 
#define SHELL "/bin/sh"



int main(int argc, char *argv[]){
	
	FILE *fp;
	char buf[MAXBUF];
	int i=0;
	int j=0;
	int last;
	char *lines[MAXBUF];
	int mypipe[2];
	pid_t pid;
	int status;
	int p;
	int n;
	
	fp=fopen(argv[1], "r");
	if (fp == NULL)
        exit(EXIT_FAILURE);
        
	
	
	while(fgets(buf, MAXBUF, fp)){
		lines[i]=strdup(buf);
		i++;
		
	}
	last=i-1;
	n=last;

	
	
	printf("%d\n", i);
	
	
	
	if( pipe(mypipe) == -1){
          perror("Pipe failed");
          exit(1);
        }
        
     while(j<last){
		pid=fork();
		if (pid == -1) {
			printf( "fork error\n" );
			exit(-1);
		} else if( pid==0) {
    		close(STDOUT_FILENO);  //closing stdout
    		close(mypipe[0]);	   //closing pipe read
            dup2(mypipe[1], 1);         //replace stdout
            //close(mypipe[1]);
            //printf("%s",cmdargv1[1]);
    		execl(SHELL, SHELL, "-c", lines[j], NULL);
    		
    		_exit(EXIT_FAILURE);
    	} 
		j++;
	}
	
	
	pid=fork();
		if (pid == -1) {
			printf( "fork error\n" );
			exit(-1);
		} else if( pid==0) {
    		close(STDIN_FILENO);  //closing stdout
    		close(mypipe[1]);	   //closing pipe read
            dup2(mypipe[0], 0);         //replace stdout
            
            //close(mypipe[1]);
            //printf("%s",cmdargv1[1]);
    		execl(SHELL, SHELL, "-c", lines[last], NULL);
    		
    		_exit(EXIT_FAILURE);
    	} 
    	
    	
    	while (n >0) {
 		 p = wait(&status);
  		printf("Child with PID %ld exited with status 0x%x.\n", (long)p, status);
  		--n;  // TODO(pts): Remove pid from the pids array.
		}
	
	
fclose(fp);

return 0;

}
