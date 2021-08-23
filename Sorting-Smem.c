# include <stdlib.h>
# include <stdio.h>
# include <sys/shm.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>
# include <time.h>

typedef struct{
    int ar[10];
    int sequence_size;
} shared_data;

int main()
{
    int arr[10] = {120, 90, 20, 80, 30, 70, 40, 60, 100, 50}, size = 10, temp, segment_id;     
    
    size_t segment_size = sizeof(shared_data);
    segment_id = shmget(IPC_PRIVATE, segment_size, S_IRUSR | S_IWUSR);
    
    if (segment_id == -1) {
        perror("shmget FAILED");
        return EXIT_FAILURE;
    }

    shared_data *shared_memory = shmat(segment_id, NULL, 0);

    if ((void*)shared_memory == (void*)-1) {
        perror("shmat FAILED");
        goto destroy; // clean up
    }
    printf("\nSHARED MEMORY SEGMENT %d ATTACHED AT ADDRESS %p\n", segment_id, (void*)shared_memory);

    shared_memory -> sequence_size = size;
    
    printf("\n\nTHIS IS THE PROBLEM REGARDING SORTING.\nTHIS IS SOLVED BY SHARED MEMORY.\n");
    
    pid_t pid;
    pid = fork();
    
    if (pid == 0){
        clock_t ct = clock();
        
        printf("CHILD IS SORTING THE ARRAY...");
        
            for (int i=0; i<size-1; i++){     
                for (int j=0; j<size-i-1; j++){
                    if (arr[j] > arr[j+1]){
                        temp = arr[j];
                        arr[j] = arr[j+1];
                        arr[j+1] = temp;          
                    }
                }
            }

            for (int i=0; i<size; i++)
                shared_memory -> ar[i] = arr[i];
        
        printf("\nCHILD ENDS\n");
        
        ct = clock() - ct;
        double time_taken = (double)ct / CLOCKS_PER_SEC;
        
        printf("CHILD PROCESS TIME: %f", time_taken);
    }

    else{
        clock_t pt = clock();
        
        printf("\nPARENT IS WAITING FOR CHILD TO COMPLETE...\n\n");
        wait(NULL);        
        
        printf("\n\nPARENT PRINTS THE SORTED ARRAY BY CHILD\n");
        for(int i=0; i < shared_memory -> sequence_size; i++) {
            printf("%d ", shared_memory -> ar[i]);
        }

        printf("\n\nPARENT ENDS");
        pt = clock() - pt;
        double time_taken = (double)pt / CLOCKS_PER_SEC;
        printf("\nPARENT PROCESS TIME: %f\n\n", time_taken);
    }
    
    destroy:
        shmctl(segment_id, IPC_RMID, NULL); 

    return 0;
}