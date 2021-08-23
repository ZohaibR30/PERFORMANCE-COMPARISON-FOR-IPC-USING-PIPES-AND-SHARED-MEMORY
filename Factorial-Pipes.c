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
    int limit, fd1[2], fd2[2], size = 20;
        pipe(fd1);
        pipe(fd2);
    long int fact = 1, arr[20], buffer[20] = {0};
    
    printf("\n\nTHIS IS THE PROBLEM REGARDING FACTORIAL CALCULATION.\nTHIS IS SOLVED BY PIPES\n");

    int pid = fork();   
    
    if(!pid){
        clock_t ct = clock();
        
        close(fd1[0]);
        write(fd1[1], &size, 4);
        close(fd2[1]);
        read(fd2[0], &buffer, size * sizeof(long int));
        
        printf("\nFACTORIAL OF FIRST 20 NUMBERS PRINTED BY CHILD\n\n");

        for(int i = 0; i < size; i++)
            printf("FACTORAIL OF %d: %ld\n", i+1, buffer[i]);

        ct = clock() - ct;
        double time_taken = (double)ct / CLOCKS_PER_SEC;        
        printf("\n\nCHILD PROCESS TIME: %f\n", time_taken);
    }

    else{
        clock_t pt = clock();
        close(fd1[1]);
        read(fd1[0], &limit, 4);

        printf("\nPARENT CALCULATING FACTORIAL OF 20 NUMBERS....");
        
            for (int i=1; i <= limit; i++){
                fact = fact * i;
                arr[i-1] = fact;
            }

        close(fd2[0]);
        write(fd2[1], arr, limit * sizeof(long int));
        
        pt = clock() - pt;
        double time_taken = (double)pt / CLOCKS_PER_SEC;
        
        printf("\nPARENT PROCESS TIME: %f", time_taken);
    }
 
    printf("\n");
}