
  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void quicksort(int sjfq[], char* pq[], int min, int max);

int main()
{



int *queue = malloc (1000*sizeof(int));
int *sjfqueue = malloc (1000*sizeof(int));
char **pqueue = malloc (1000*sizeof(char*));
char **wqueue = malloc (1000*sizeof(char*));
int  *turnqueue = malloc (1000*sizeof(int));
char **turnpqueue = malloc (1000*sizeof(char*));
int *aqueue = malloc (1000*sizeof(int));


int numq=0;
int nump=0;
float count=0;
int tq;
//int ptime;
int sum;
float avg;

char* line = NULL;
size_t lbsize=0;
char *token;


FILE *fp, *fo;


fp = fopen("cpu_scheduling_input_file.txt", "r");
if(fp==NULL)
{
    printf("File does not exist");
    return 0;
}


// open file to write to
fo = fopen( "cpu_scheduling_output_file.txt", "w+" );

if(fo==NULL)
{
    printf("Error when opening file");
    return 0;
}


// until the end of the file is reached
while (!feof(fp))
{
    // get the line for the file
    getline(&line, &lbsize, fp);
    
    token = strtok(line," "); // q
    numq++;// increment the queue number
    
    
    token = strtok(NULL," "); // skip the q number
    token = strtok(NULL," ");// skip the tq
    token = strtok(NULL," "); // get the tq value
    tq = atoi(token); // convert the token to an int val

     
     token = strtok(NULL, " "); // read the first process
     
    while (token!=NULL)
    {
    
    pqueue[nump]= token; //  store it in the process queue
    wqueue[nump]= token;
    turnpqueue[nump] = token;
        
    token = strtok(NULL, " "); // read the first process burst time
    queue[nump] = atoi(token);//store the burst time
    sjfqueue[nump]= atoi(token);
    turnqueue[nump] = atoi(token);
    nump++; // increment the number of processes
    count++;
    token = strtok(NULL, " "); // read the next process
    }
    

        
        fprintf(fo,"Ready Queue %d Applying FCFS Scheduling: \n",numq);
        printf("Ready Queue %d Applying FCFS Scheduling: \n",numq);
        fprintf(fo,"Order of selection by CPU: \n");
        printf("Order of selection by CPU: \n");
        
        // FCFS Algorithm
        
        for(int i=0; i<nump; i++)
        {
            fprintf(fo,"%s ",pqueue[i]);
            printf("%s ",pqueue[i]);
            
        }
        
        
        fprintf(fo,"\n\nIndividual waiting time for each process \n");
        printf("\n\nIndividual waiting time for each process \n");
        
        int avgsum=0;
        sum=0;
        for(int i=0; i < nump; i++)
        {
            fprintf(fo,"%s = %d\n",pqueue[i],sum);
            printf("%s = %d\n",pqueue[i],sum);
            avgsum+=sum;
            sum+=queue[i];
            
            
        }
        
        avg = avgsum/((float)nump);
        fprintf(fo,"\n\nAverage waiting time for each process: %.1f\n",avg);
        printf("\n\nAverage waiting time for each process: %.1f\n",avg);
        
        
        //SJF Alogrithm
        
         
        // do a quick sort and change the values of the processes as well
        
        quicksort (sjfqueue, wqueue, 0,nump-1);
        
        fprintf(fo,"\nReady Queue %d Applying SJF Scheduling: \n",numq);
        printf("\nReady Queue %d Applying SJF Scheduling: \n",numq);
        
        fprintf(fo,"\nOrder of Selection: \n");
        printf("\nOrder of Selection: \n");
        
        for(int i=0; i<nump; i++)
        {
            fprintf(fo,"%s ",wqueue[i]);
            printf("%s ",wqueue[i]);
        }
        fprintf(fo,"\n\nIndividual waiting time for each process \n");
        printf("\n\nIndividual waiting time for each process \n");
        
        
        sum=0;
        avgsum=0;
        for(int i=0; i<nump; i++)
        {
            
            fprintf(fo,"%s = %d\n",wqueue[i],sum);
            printf("%s = %d\n",wqueue[i], sum);
            avgsum+=sum;
            sum+=sjfqueue[i];
        }
        
        avg = avgsum/((float)nump);
        fprintf(fo,"\n\nAverage waiting time for each process: %.1f\n",avg);
        printf("\n\nAverage waiting time for each process: %.1f\n",avg);
        
        fprintf(fo,"\nReady Queue %d Applying RR Scheduling: \n",numq);
        printf("\nReady Queue %d Applying RR Scheduling: \n",numq);
        
        
        int unfinished = nump;
        int more = nump;
        int tqtotal=0,index=0;
        
         
        int atime=0;
        
        
        for (int i=0; i<nump; i++)
        {
            aqueue[i]= atime;
            // add the arrival time for each process
            if(queue[i]<tq)
            {
                atime+=queue[i]; //arrival time is less than 4 so add that val
            }
            else
            {
                atime += tq;
            }
        }
            
        
        int bool;
        
        while (unfinished!=0)
        {
            if(queue[index]<= tq && queue[index]>0)
            {
                tqtotal = tqtotal + queue[index];// get the total turnaround time
                queue[index] = 0;
                //printf("%s's time is now %d\n",pqueue[index],queue[index]);
                bool=1;
            }
            else if (queue[index] > 0)
            {
                queue[index]=queue[index]-tq;
                pqueue[more] = pqueue[index];
                more++;
                tqtotal = tqtotal+tq;
                }
            
            
            if(queue[index] == 0 && bool ==1)
            {
                unfinished --;
                turnqueue[index] = tqtotal-aqueue[index]; // store the total turnaround tijme for that process
                turnpqueue [index]= pqueue[index];// store the process turnaround time
                bool=0;
            }
            if(index == nump-1)
            {
                index =0;
            }
            else if( aqueue[index+1]<=tqtotal)
            {
                index++;
            }
            else
            {
                index = 0;
            }
            
            //printf("\nthe number of unfinished processed is %d\n",unfinished);
        }
        
        fprintf(fo,"\nOrder of Selection: \n");
        printf("\nOrder of Selection: \n");
        
        for (int i=0; i<more; i++)
        {
            fprintf(fo,"%s ",pqueue[i]);
            printf("%s ",pqueue[i]);
        }
        
        fprintf(fo,"\n\nIndividual turnaround time for each process \n");
        printf("\n\nIndividual turnaround time for each process \n");
        
        sum=0;
        for(int i=0; i<nump; i++)
        {
            
            fprintf(fo,"%s = %d\n",turnpqueue[i],turnqueue[i]);
            printf("%s = %d\n",turnpqueue[i], turnqueue[i]);
            sum+=turnqueue[i];
        }
        
        nump=0;
        printf("\n\n");
        fprintf(fo,"\n\n");
        
        
    
}
free(queue);
free(pqueue);
free(sjfqueue);
free(wqueue);
free(turnqueue);
free(turnpqueue);
free(aqueue);
fclose(fp);
fclose(fo);
            
    return 0;

        
        
            
    }
    
void swap (int* aa, int* ab, char** ba, char** bb)
{
    int tempa= *aa;
            *aa=*ab;
            *ab = tempa;
    char* tempb =*ba;
            *ba=*bb;
            *bb = tempb;
}
int splitter(int sjfq[], char* pq[], int min, int max)
{
    int piv = sjfq[max];
    int i = (min-1);
    
    for (int j = min; j<=max-1;j++)
    {
        if(sjfq[j]<piv)
        {
            i++;
            swap(&sjfq[i],&sjfq[j],&pq[i],&pq[j]);
        }
        
    }
    swap(&sjfq[i+1],&sjfq[max],&pq[i+1],&pq[max]);
    return (i+1);
}
    
            
    
void quicksort(int sjfq[], char* pq[], int min, int max)
    {
        int split;

        
        if (min<max)
        {
            
            split = splitter(sjfq,pq,min,max);
            
            quicksort(sjfq, pq, min, split-1);
            quicksort(sjfq, pq, split+1, max);
        }
    }
        
    
        
        
    
        
        
        
        
        
        
    
        
           
       
       
      
© 2019 GitHub, Inc.
Terms
Privacy
Security
Status
Help
Contact GitHub
Pricing
API
Training
Blog
About
