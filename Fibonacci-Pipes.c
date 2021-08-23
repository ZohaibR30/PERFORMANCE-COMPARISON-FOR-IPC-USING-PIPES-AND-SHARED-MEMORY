# include <stdlib.h>
# include <stdio.h>
# include <sys/shm.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>
# include <time.h>

int main(){
    int fd1[2], fd2[2];
        pipe(fd1);
        pipe(fd2);
    int size = 20, limit, arr[20], buffer[20], fterm = 0, lterm = 1, next;
    
    printf("\n\nTHIS IS THE PROBLEM REGARDING FIBONACCI SERIES.\nTHIS IS SOLVED BY PIPES\n");

    int pid = fork();

    if(!pid){
        clock_t ct = clock();
        
        close(fd1[0]);
        write(fd1[1], &size, 4);
        close(fd2[1]);
        read(fd2[0], buffer, size * sizeof(int));
        
        printf("NOW CHILD WILL PRINT THE RESULT CALCULATED BY PARENT\n\n");
            for(int i = 0; i<size; i++)
                printf("TERM NUMBER %d OF FIBONACCI SERIES: %d\n", i+1, buffer[i]);
        
        ct = clock() - ct;
        double time_taken = (double)ct / CLOCKS_PER_SEC;
        printf("\nCHILD PROCESS TIME: %f\n", time_taken);
    }
    
    else{
        clock_t pt = clock();
        close(fd1[1]);
        read(fd1[0], &limit, 4);
        
        arr[0]=0;
        arr[1]=1;

        printf("\nPARENT CALCULATING FIBONACCI SERIES OF FIRST %d FIGURES...\n", size);

            for(int i=2; i<limit; i++){
                next = lterm + fterm;
                fterm = lterm;
                lterm = next;
                arr[i] = next;
            }
            
        close(fd2[0]);
        write(fd2[1], arr, size * sizeof(int));
        
        pt = clock() - pt;
        double time_taken = (double)pt / CLOCKS_PER_SEC;
        
        printf("PARENT PROCESS TIME: %f\n", time_taken);
    }

    printf("\n");
}