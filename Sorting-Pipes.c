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
    int arr[10] = {120, 90, 20, 80, 30, 70, 40, 60, 100, 50};
    int fd1[2], fd2[2], size = 10, temp;
        pipe(fd1);
        pipe(fd2);
    int temparr[10], buffer[10] = {0}, limit;

    printf("\n\nTHIS IS THE PROBLEM REGARDING SORTING.\nTHIS IS SOLVED BY PIPES\n\n");    
    
    int pid = fork();   
    
    if(!pid){
        clock_t ct = clock();
        
        close(fd1[0]);
        write(fd1[1], &size, 4);
        close(fd2[1]);
        read(fd2[0], &buffer, size * sizeof(int));
        
        printf("CHILD PRINTING ARRAY NUMBERS SORTED\n");

        for(int i = 0; i < size; i++)
            printf("%d ", buffer[i]);

        ct = clock() - ct;
        double time_taken = (double)ct / CLOCKS_PER_SEC;
        
        printf("\n\nCHILD PROCESS TIME: %f\n", time_taken);
    }

    else{    	
    	printf("ELEMENTS OF ARRAY BEFORE SORTING: ");
        clock_t pt = clock();        
        
        close(fd1[1]);
        read(fd1[0], &limit, 4);

	    for (int i=0; i<limit; i++){
	    	printf("%d ", arr[i]);
	    }
	            
        printf("\n\nPARENT SORTING ARRAY NUMBERS....");
        
            for (int i = 0; i < limit-1; i++){     
                for (int j = 0; j < limit-i-1; j++){
                    if (arr[j] > arr[j+1]){
                        temp = arr[j];
                        arr[j] = arr[j+1];
                        arr[j+1] = temp;          
                    }
                }
           }

           for (int i=0; i<limit; i++)
                temparr[i] = arr[i]; 
    
        close(fd2[0]);
        write(fd2[1], temparr, limit * sizeof(int));
        
        pt = clock() - pt;
        double time_taken = (double)pt / CLOCKS_PER_SEC;
        
        printf("\nPARENT PROCESS TIME: %f\n", time_taken);
    }
 
    printf("\n");
}