#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Process: Each process has a name and has an execution time.
 *          We keep track of how long the process still needs
 *          to run on the processor. "remaining_time" is the
 *          remaining execution time of the process.
 */
typedef struct
{
    char *name;
    int starting_time;
    int remaining_time;
} Process;
/*
 *
 */
typedef struct Node
{
    Process data;
    struct Node* next;
} Node;
/*
 *
 */
typedef struct
{
    int count;
    Node* front, *end;
} Queue;
/*
 *
 */
Queue* init()
{
    Queue* temp = (Queue*)malloc(sizeof(Queue));
    temp->count=0;
    temp->front=(Node*)malloc(sizeof(Node));
    temp->end=(Node*)malloc(sizeof(Node));
    return temp;
}
/*
 *
 */
int isEmpty(Queue *q)
{
    if(q->count){
        return 0;
    }
    return 1;
}
/*
 *
 */
Process dequeue(Queue *q)
{
    Node* temp=malloc(sizeof(Node));
    temp = q->front;
    Process x = temp->data;
    q->front=temp->next;
    q->count--;
    free(temp);
    return x;
}
/*
 *
 */
void enqueue(Queue *q, Process x)
{
    Node* temp = malloc(sizeof(Node));
    temp->data = x;
    temp->next = NULL;
    if(isEmpty(q)){
        q->front=temp;
        q->end=temp;
    }
    else{
        q->end->next=temp;
        q->end=temp;
    }
    q->count++;
}
/*
 *
 */
void destroy(Queue *q)
{
    while(!isEmpty(q)){
        dequeue(q);
    }
    free(q);
}
int toDigit(char *str){
    char *p = str;
    int output=0;
    while (*p) {
        if ( isdigit(*p) ) {
            int val = strtol(p, &p, 10);
            output=output*10+val;
        } else {
            p++;
        }
    }
    return output;
}
/*
 * RoundRobin Scheduling
 */
void RoundRobin(char* filename)
{
    Queue *q=init();
    int slots,currentSlot=0,line;
    char buffer[100];
    FILE* f = fopen(filename,"r");
    if(fgets(buffer,sizeof(buffer),f)){
        slots=toDigit(buffer);
        line=ftell(f);
    }
    while(currentSlot<slots){
        fseek(f,line,SEEK_SET);
        while(fgets(buffer,sizeof(buffer),f)){
            Process temp;
            int k=1;
            char * token = strtok(buffer," ");
            while(token){
                switch(k){
                    case 1:
                        temp.name=malloc(sizeof(token));
                        strcpy(temp.name,token);
                        break;
                    case 2:
                        temp.starting_time=atoi(token);
                        break;
                    case 3:
                        temp.remaining_time=atoi(token);
                        break;
                }
                k++;
                token = strtok(NULL," ");
            }
            if(temp.starting_time==currentSlot){
                enqueue(q,temp);
            }
        }
        if(isEmpty(q)){
            printf("idle (%d-->%d)\n",currentSlot,currentSlot+1);
        }
        else{
            Process x=dequeue(q);
            x.remaining_time--;
            printf("%s   (%d-->%d)",x.name,currentSlot,currentSlot+1);
            if(!x.remaining_time){
                printf("%s aborts",x.name);
            }
            else{
                enqueue(q,x);
            }
            printf("\n");
        }
        currentSlot++;
    }
    destroy(q);
    printf("stop\n");
    fclose(f);
}
/*
 *
 */
int main()
{
    char filename[261];
    puts("Enter file name or Ctrl+Z to exit:");
    puts("----------------------------------");
    while(fgets(filename, 260, stdin) != NULL)
    {
        filename[strlen(filename)-1]='\0';
        if(fopen(filename,"r"))
            RoundRobin(filename);
        else
        {
            puts("File Not Found!");
            puts("----------------------------------");
        }
        puts("Enter file name or Ctrl+Z to exit:");
        puts("----------------------------------");
    }
    return 0;
}
