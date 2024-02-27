#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char policy[4];
char fileName[20];
int timeSlice;

struct job
{
    int id;
    int length;
    struct job *next;
    int responseTime;
    int turnaroundTime;
    int waitTime;
};

void analyze(struct job *head)
{
    struct job *current = head;
    int systemTime = 0;
    while (current != NULL)
    {
        current->responseTime = systemTime;
        current->turnaroundTime = systemTime + current->length;
        current->waitTime = systemTime;
        printf("Job %d -- Response Time: %d Turnaround: %d Wait: %d\n", current->id, current->responseTime, current->turnaroundTime, current->waitTime);
        systemTime += current->length;
        current = current->next;
    }

    float totalResponseTime = 0.0;
    float totalTurnaroundTime = 0.0;
    float totalWaitTime = 0.0;
    float totalJobs = 0.0;
    current = head;

    while (current != NULL)
    {
        totalResponseTime += current->responseTime;
        totalTurnaroundTime += current->turnaroundTime;
        totalWaitTime += current->waitTime;
        totalJobs++;
        current = current->next;
    }

    printf("Average -- Response Time: %0.2f Turnaround: %0.2f Wait: %0.2f\n", totalResponseTime / totalJobs, totalTurnaroundTime / totalJobs, totalWaitTime / totalJobs);
}

void FIFO(struct job *head)
{
    printf("Execution trace with FIFO:\n");
    struct job *current = head;

    // prints all jobs in order
    while (current != NULL)
    {
        printf("Job %d ran for: %d\n", current->id, current->length);
        current = current->next;
    }

    printf("End of execution with FIFO.\n");

    printf("Begin analyzing FIFO: \n");

    analyze(head);
    
    printf("End analyzing FIFO.\n");
}

void SJF(struct job *head)
{
    printf("Execution trace with SJF:\n");

    struct job *current, *temp;
    int tempvar;

    current = head;
    // sorts jobs in order of increasing length
    while (current != NULL)
    {
        temp = current->next;

        while (temp != NULL)
        {
            if (current->length > temp->length)
            {
                tempvar = current->length;
                current->length = temp->length;
                temp->length = tempvar;
                tempvar = current->id;
                current->id = temp->id;
                temp->id = tempvar;
            }

            temp = temp->next;
        }

        current = current->next;
    }

    current = head;

    // print all jobs
    while (current != NULL)
    {
        printf("Job %d ran for: %d\n", current->id, current->length);
        current = current->next;
    }

    printf("End of execution with SJF.\n");

    printf("Begin analyzing SJF: \n");

    analyze(head);

    printf("End analyzing SJF.\n");
}

void RR(struct job *head)
{
    printf("Execution trace with RR:\n");
    struct job *current = head;
    int remainingJob = 1;

    while (remainingJob)
    {
        remainingJob = 0;

        while (current != NULL)
        {
            if (current->length > timeSlice)
            {
                printf("Job %d ran for: %d\n", current->id, timeSlice);
                current->length -= timeSlice;
                remainingJob = 1;
            }
            else if (current->length > 0)
            {
                printf("Job %d ran for: %d\n", current->id, current->length);
                current->length = 0;
                remainingJob = 1;
            }

            current = current->next;
        }
        current = head;
    }

    printf("End of execution with RR.\n");

    printf("Begin analyzing RR: \n");

    analyze(head);

    printf("End analyzing RR.\n");
}

int main(int argc, char *argv[])
{
    // user input
    // printf("Enter a scheduling policy, workload file, and timeslice duration: ");
    // scanf("%s %s %d", policy, fileName, &timeSlice);

    strcpy(policy, argv[1]);
    strcpy(fileName, argv[2]);
    timeSlice = atoi(argv[3]);
    
    FILE *file;
    file = fopen(fileName, "r");
    if (file == NULL) // checking if the file exists
        printf("Error opening file %s\n", fileName);

    // job creation
    struct job *head = NULL;
    struct job *current = NULL;

    char line[50];
    int id = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        struct job *newJob = (struct job *)malloc(sizeof(struct job));
        sscanf(line, "%d", &newJob->length);
        newJob->id = id++;
        newJob->responseTime = 0;
        newJob->turnaroundTime = 0;
        newJob->waitTime = 0;
        newJob->next = NULL;

        if (head == NULL)
        {
            head = newJob;
            current = newJob;
        }
        else
        {
            current->next = newJob;
            current = newJob;
        }
    }
    fclose(file);

    // policy checking
    if (strcmp(policy, "FIFO") == 0)
    {
        FIFO(head);
    }
    else if (strcmp(policy, "SJF") == 0)
    {
        SJF(head);
    }
    else if (strcmp(policy, "RR") == 0)
    {
        RR(head);
    }
    else
        printf("Invalid policy");
}