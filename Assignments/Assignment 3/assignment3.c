#include <stdio.h>
#include <stdlib.h>       //include libraries
#include <string.h>
#include <semaphore.h>

struct bankAccount{        //define a struct for a bank account
  int number;
  int type;               //0 business, 1 personal
  int balance;
  int depositfee;
  int withdrawfee;
  int transferfee;
  int transactionfee;
  int transactions;
  int overdraft;
  int overdraftfee;
  int overcounter;
};

struct orderedPair{
  struct bankAccount x;
  struct bankAccount y;
};

sem_t mutex;
struct bankAccount accounts[50];


struct bankAccount deposit (struct bankAccount baccount, int amount){
    printf("a%d\n",baccount.number);
    printf("before%d\n",baccount.balance);


    sem_wait(&mutex);

    if (baccount.transactions > 0){
      if (((baccount.balance+amount-baccount.depositfee)<0)&&baccount.overdraft==0){
        sem_post(&mutex);
        return baccount;
      }
    }else{
      if (((baccount.balance+amount-baccount.depositfee-baccount.transactionfee)<0)&&baccount.overdraft==0){
        sem_post(&mutex);
        return baccount;
      }
    }
    baccount.balance += amount;
    baccount.balance -= baccount.depositfee;
    if (baccount.transactions < 1){
      baccount.balance -= baccount.transactionfee;
    }
    baccount.transactions -=1;
    if (baccount.balance < (baccount.overcounter*(-500))){
      baccount.balance -= baccount.overdraftfee;
      baccount.overcounter+=1;
    }

    sem_post(&mutex);
    printf("a%d\n",baccount.number);
    printf("after%d\n",baccount.balance);

    return baccount;
}

struct bankAccount withdraw (struct bankAccount baccount, int amount){
    printf("a%d\n",baccount.number);
    printf("before%d\n",baccount.balance);

    sem_wait(&mutex);
    if (baccount.transactions > 0){
      if (((baccount.balance-amount-baccount.withdrawfee)<0)&&baccount.overdraft==0){
        sem_post(&mutex);
        return baccount;
      }
    }else{
      if (((baccount.balance-amount-baccount.withdrawfee-baccount.transferfee)<0)&&baccount.overdraft==0){
        sem_post(&mutex);
        return baccount;
      }
    }
    baccount.balance -= amount;
    baccount.balance -= baccount.withdrawfee;
    if (baccount.transactions < 1){
      baccount.balance -= baccount.transactionfee;
    }
    baccount.transactions -=1;
    if (baccount.balance < (baccount.overcounter*(-500))){
      baccount.balance -= baccount.overdraftfee;
      baccount.overcounter+=1;
    }
    sem_post(&mutex);
    printf("a%d\n",baccount.number);
    printf("after%d\n",baccount.balance);
    return baccount;
}
struct orderedPair transfer (struct bankAccount baccount1, struct bankAccount baccount2, int amount){
    sem_wait(&mutex);
    printf("a%d\n",baccount1.number);
    printf("before%d\n",baccount1.balance);
    printf("a%d\n",baccount2.number);
    printf("before%d\n",baccount2.balance);

    if (baccount1.transactions > 0){
      if (((baccount1.balance-amount-baccount1.transferfee)<0)&&baccount1.overdraft==0){
        sem_post(&mutex);
        struct orderedPair new;
        new.x = baccount1;
        new.y = baccount2;
        return new;
      }
    }else{
      if (((baccount1.balance-amount-baccount1.transferfee-baccount1.transactionfee)<0)&&baccount1.overdraft==0){
        sem_post(&mutex);
        struct orderedPair new;
        new.x = baccount1;
        new.y = baccount2;
        return new;
      }
    }
    if (baccount2.transactions > 0){
      if (((baccount2.balance-amount-baccount2.transferfee)<0)&&baccount2.overdraft==0){
        sem_post(&mutex);
        struct orderedPair new;
        new.x = baccount1;
        new.y = baccount2;
        return new;
      }
    }else{
      if (((baccount2.balance-amount-baccount2.transferfee-baccount2.transactionfee)<0)&&baccount2.overdraft==0){
        sem_post(&mutex);
        struct orderedPair new;
        new.x = baccount1;
        new.y = baccount2;
        return new;
      }
    }
    baccount1.balance -= amount;
    baccount1.balance -= baccount1.transferfee;
    baccount2.balance += amount;
    baccount2.balance -= baccount2.transferfee;
    if (baccount1.transactions < 1){
      baccount1.balance -= baccount1.transactionfee;
    }
    if (baccount2.transactions < 1){
      baccount2.balance -= baccount2.transactionfee;
    }
    baccount1.transactions -=1;
    baccount2.transactions -=1;
    if (baccount1.balance < (baccount1.overcounter*(-500))){
      baccount1.balance -= baccount1.overdraftfee;
      baccount1.overcounter+=1;
    }
    if (baccount2.balance < (baccount2.overcounter*(-500))){
      baccount2.balance -= baccount2.overdraftfee;
      baccount2.overcounter+=1;
    }
    printf("a%d\n",baccount1.number);
    printf("after%d\n",baccount1.balance);
    printf("a%d\n",baccount2.number);
    printf("after%d\n",baccount2.balance);
    sem_post(&mutex);
    struct orderedPair new;
    new.x = baccount1;
    new.y = baccount2;
    return new;
}

void print_output(struct bankAccount account){
  FILE *fptr;
  fptr = fopen("assignment_3_output_file.txt", "a");
  if (account.type == 0){
    fprintf(fptr,"a%d type personal %d \n", account.number, account.balance);
  }else{
    fprintf(fptr,"a%d type business %d \n", account.number, account.balance);

  }
}


int main(){                           //main function
  sem_init(&mutex, 1, 1);
  remove("assignment_3_output_file.txt");
  int size = 0;
  FILE* fptr;
  char delim[] = " ";                  //declares vars, ptr, and delim, char
  fptr = fopen("assignment_3_input_file.txt", "r"); //opens input file
  char line[256];
  while (fgets(line, sizeof(line), fptr) && (line[0] != '\n')){
    accounts[size].number = size +1;
    accounts[size].balance = 0;
    accounts[size].overcounter = 0;
    char *ptr = strtok(line, delim);
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    if (strcmp(ptr,"personal")==0){
      accounts[size].type = 0;
    }else{
      accounts[size].type = 1;
    }
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    accounts[size].depositfee = atoi(ptr);      //converts to int from string
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    accounts[size].withdrawfee = atoi(ptr);
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    accounts[size].transferfee = atoi(ptr);
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    accounts[size].transactions = atoi(ptr);
    ptr = strtok(NULL, delim);
    accounts[size].transactionfee = atoi(ptr);
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    if (strcmp(ptr,"Y")==0){
      accounts[size].overdraft = 1;
      ptr = strtok(NULL, delim);
      accounts[size].overdraftfee = atoi(ptr);
    }else{
      accounts[size].overdraft = 0;
      accounts[size].overdraftfee = 0;
    }
    /*printf("%d\n",accounts[1].number);
    printf("%d\n",accounts[1].type);
    printf("%d\n",accounts[1].balance);
    printf("%d\n",accounts[1].depositfee);
    printf("%d\n",accounts[1].withdrawfee);
    printf("%d\n",accounts[1].transferfee);
    printf("%d\n",accounts[1].transactionfee);
    printf("%d\n",accounts[1].transactions);
    printf("%d\n",accounts[1].overdraft);
    printf("%d\n",accounts[1].overdraftfee);
    printf("%d\n",accounts[1].overcounter);
    */
    size++;
  }
  while (fgets(line, sizeof(line), fptr) && (line[0] != '\n')){
    printf("%s",line);
    char *ptr = strtok(line, delim);
    int act, amt;
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    while (ptr != NULL){
      if (strlen(ptr)==2){
        char num[1] = "";
        num[0] = ptr[1];
        act = atoi(num);
      }else if (strlen(ptr)==3){
        char num[2] = "";
        num[0] = ptr[1];
        num[1] = ptr[2];
        act = atoi(num);
      }
      ptr = strtok(NULL, delim);
      amt = atoi(ptr);
      struct bankAccount newval;
      newval = deposit(accounts[act-1],amt);
      accounts[act-1] = newval;
      if (ptr != NULL){
        ptr = strtok(NULL, delim);
      }
      if (ptr != NULL){
        ptr = strtok(NULL, delim);
      }
    }
  }

  while (fgets(line, sizeof(line), fptr) && (line[0] != '\n')){
    printf("%s",line);
    char *ptr = strtok(line, delim);
    int act1,act2, amt;
    while (ptr != NULL){
      ptr = strtok(NULL, delim);
      if (ptr == NULL){
        break;
      }
      if (strcmp(ptr,"d")==0){
        ptr = strtok(NULL, delim);
        if (strlen(ptr)==2){
          char num[1] = "";
          num[0] = ptr[1];
          act1 = atoi(num);
        }else if (strlen(ptr)==3){
          char num[2] = "";
          num[0] = ptr[1];
          num[1] = ptr[2];
          act1 = atoi(num);
        }
        ptr = strtok(NULL, delim);
        amt = atoi(ptr);
        struct bankAccount newval;
        newval = deposit(accounts[act1-1],amt);
        accounts[act1-1] = newval;
      }else if (strcmp(ptr, "w")==0){
        ptr = strtok(NULL, delim);
        if (strlen(ptr)==2){
          char num[1] = "";
          num[0] = ptr[1];
          act1 = atoi(num);
        }else if (strlen(ptr)==3){
          char num[2] = "";
          num[0] = ptr[1];
          num[1] = ptr[2];
          act1 = atoi(num);
        }
        ptr = strtok(NULL, delim);
        amt = atoi(ptr);
        struct bankAccount newval;
        newval = withdraw(accounts[act1-1],amt);
        accounts[act1-1] = newval;
      }else if (strcmp(ptr, "t")==0){
        ptr = strtok(NULL, delim);
        if (strlen(ptr)==2){
          char num[1] = "";
          num[0] = ptr[1];
          act1 = atoi(num);
        }else if (strlen(ptr)==3){
          char num[2] = "";
          num[0] = ptr[1];
          num[1] = ptr[2];
          act1 = atoi(num);
        }
        ptr = strtok(NULL, delim);
        if (strlen(ptr)==2){
          char num[1] = "";
          num[0] = ptr[1];
          act2 = atoi(num);
        }else if (strlen(ptr)==3){
          char num[2] = "";
          num[0] = ptr[1];
          num[1] = ptr[2];
          act2 = atoi(num);
        }
        ptr = strtok(NULL, delim);
        amt = atoi(ptr);
        struct orderedPair newv;
        newv =transfer(accounts[act1-1],accounts[act2-1],amt);
        accounts[act1-1] = newv.x;
        accounts[act2-1] = newv.y;
      }
    }
  }
  sem_destroy(&mutex);
  printf("%d%d",accounts[0].number, accounts[0].balance);
  printf("%d%d",accounts[1].number, accounts[1].balance);

  for (int i =size-1; i>=0; i--){
    print_output(accounts[i]);
  }
  return 0;
}