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
};

int main(int argc, char *argv[])
{
    // user input
    printf("Enter a scheduling policy, workload file, and timeslice duration: ");
    scanf("%s %s %d", &policy, &fileName, &timeSlice);
    FILE *file;
    file = fopen(fileName, "r");
    if (file == NULL) // checking if the file exists
        printf("Error opening file %s\n", fileName);

    // job creation 
    struct job *head = NULL;
    struct job *current = NULL;

    char line[50];
    int id = 1;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        struct job *newJob = (struct job *)malloc(sizeof(struct job));
        sscanf(line, "%d", &newJob->length);
        newJob->id = id++;
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


    //prints jobs

    current = head;
    while (current != NULL){
        printf("Job %d has a length of %d\n", current->id, current->length);
        current = current->next;
    }

    // policy checking
    if (strcmp(policy, "FIFO") == 0)
    {
    }
    else if (strcmp(policy, "SJF") == 0)
    {
    }
    else if (strcmp(policy, "RR") == 0)
    {
    }
    else
        printf("Invalid policy");
}