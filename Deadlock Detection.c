#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
 
sem_t R1, R2; 
 
int graph[4][4] = {
    {0,1,0,0},  // P1 -> R2
    {0,0,1,0},  // R2 -> P2
    {0,0,0,1},  // P2 -> R1
    {1,0,0,0}   // R1 -> P1
};
 
int visited[4]; 
int stack[4]; 
 
void* P1(void* arg) 
{ 
    sem_wait(&R1); 
    printf("P1 locked R1\n"); 
 
    sleep(1); 
 
    printf("P1 waiting for R2\n"); 
    sem_wait(&R2); 
 
    printf("P1 locked R2\n"); 
 
    return NULL; 
} 
 
void* P2(void* arg) 
{ 
    sem_wait(&R2); 
    printf("P2 locked R2\n"); 
 
    sleep(1); 
 
    printf("P2 waiting for R1\n"); 
    sem_wait(&R1); 
 
    printf("P2 locked R1\n"); 
 

  return NULL; 
} 
 
int dfs(int v) 
{ 
    visited[v] = 1; 
    stack[v] = 1; 
 
    for(int i=0;i<4;i++) 
    { 
        if(graph[v][i]) 
        { 
            if(!visited[i] && dfs(i)) 
                return 1; 
            else if(stack[i]) 
                return 1; 
        } 
    } 
 
    stack[v] = 0; 
    return 0; 
} 
 
void showRAG() 
{ 
    printf("\nResource Allocation Graph:\n"); 
    printf("P1 -> R2\n"); 
    printf("R2 -> P2\n"); 
    printf("P2 -> R1\n"); 
    printf("R1 -> P1\n"); 
} 
 
void detectDeadlock() 
{ 
    for(int i=0;i<4;i++) 
    { 
        visited[i] = 0; 
        stack[i] = 0; 
    } 
 
    if(dfs(0)) 
    { 
        printf("\nCycle Found:\n"); 
        printf("P1 -> R2 -> P2 -> R1 -> P1\n"); 
        printf("\nDeadlock Detected\n"); 
 
   } 
    else 
    { 
        printf("\nNo Deadlock Detected\n"); 
    } 
} 
 
int main() 
{ 
    pthread_t t1, t2; 
 
    sem_init(&R1,0,1); 
    sem_init(&R2,0,1); 
 
    pthread_create(&t1,NULL,P1,NULL); 
    pthread_create(&t2,NULL,P2,NULL); 
 
    sleep(2); 
 
    showRAG(); 
    detectDeadlock(); 
 
    return 0; 
}