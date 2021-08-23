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
    long int fact[20];
    int sequence_size;
} shared_data;

int main()
{
    long int fact = 1;
    int  size = 20, next, segment_id;    
    
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
    printf("\n\nTHIS IS THE PROBLEM REGARDING FACTORIAL CALCULATION.\nTHIS IS SOLVED BY SHARED MEMORY.\n\n");
    
    pid_t pid;
    pid = fork();

    if (pid == 0){
        clock_t ct = clock();
        
        printf("CHILD IS CALCULATING FACTORIAL OF 20 NUMBERS...");
        
        shared_memory -> fact[0] = fact;
        
        for (int i=1; i<=shared_memory -> sequence_size; i++){
            fact = fact * i;
            shared_memory -> fact[i-1] = fact;
        }
        
        printf("\nCHILD ENDS\n");
        
        ct = clock() - ct;
        double time_taken = (double)ct / CLOCKS_PER_SEC;
        
        printf("CHILD PROCESS TIME: %f", time_taken);
    }
    
    else{
        clock_t pt = clock();
        
        printf("PARENT IS WAITING FOR CHILD TO COMPLETE...\n\n");
        wait(NULL);        
        
        printf("\n\nPARENT PRINTS THE FACTORIAL CALCULATED BY CHILD\n");
        for(int i=0; i<shared_memory -> sequence_size; i++) {
            printf("\nFACTORAIL OF %d: %ld", i+1, shared_memory -> fact[i]);
        }

        printf("\n\nPARENT ENDS"); 
        pt = clock() - pt;
        double time_taken = (double)pt / CLOCKS_PER_SEC;
        printf("\nPARENT PROCESS TIME: %f\n\n", time_taken);
    }

        if (shmdt(shared_memory) == -1){
            fprintf(stderr, "UNABLE TO DETACH\n");
        }
    
    destroy:
        shmctl(segment_id, IPC_RMID, NULL); 

    return 0;
}