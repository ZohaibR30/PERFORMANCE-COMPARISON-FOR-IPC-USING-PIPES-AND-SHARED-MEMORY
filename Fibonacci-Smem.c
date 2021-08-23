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
    long fib_sequence[20];
    int sequence_size;
} shared_data;

int main()
{
    int fterm = 0, lterm = 1, size = 20, next, segment_id;    
    
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
    
    printf("\n\nTHIS IS THE PROBLEM REGARDING FIBONACCI SERIES.\nTHIS IS SOLVED BY SHARED MEMORY.\n");
    
    pid_t pid;
    pid = fork();

    if (pid == 0){
        clock_t ct = clock();
    
        printf("\nCHILD IS PRODUCUING THE FIBONACCI SERIES...\n");
    
        shared_memory -> fib_sequence[0] = fterm;
        shared_memory -> fib_sequence[1] = lterm;
        
            for (int i=2; i<shared_memory -> sequence_size; i++){
                next = fterm + lterm;
                shared_memory -> fib_sequence[i] = next;
                fterm = lterm;
                lterm = next;
            }
        
        ct = clock() - ct;
        double time_taken = (double)ct / CLOCKS_PER_SEC;
        
        printf("CHILD PROCESS TIME: %f\n", time_taken);
    }
    
    else{
        clock_t pt = clock();
    
        printf("\nPARENT IS WAITING FOR CHILD TO PRODUCE THE SERIES...\n");
        wait(NULL);
        printf("\nPARENT PRINTS THE SERIES CALCULATED BY THE CHILD\n\n");

            for(int i=0; i<shared_memory -> sequence_size; i++) 
                printf("TERM NUMBER %d OF FIBONACCI SERIES: %ld\n", i+1, shared_memory -> fib_sequence[i]);
        
        printf("\nPARENT ENDS\n");
        pt = clock() - pt;
        double time_taken = (double)pt / CLOCKS_PER_SEC;
        printf("PARENT PROCESS TIME: %f\n\n", time_taken);
    }
    
    destroy:
        shmctl(segment_id, IPC_RMID, NULL); 

    return 0;
}