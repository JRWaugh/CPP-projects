#include<stdio.h>
#include<stdlib.h>
void main(int argc,char *argv[]){

  FILE * f;
  char fname[50];
  FILE* outfile = fopen("my.trace", "w");

  long diff;
  unsigned long ea = 0;
  int i;

  if(argc > 1){
    /*If there is a command line argument, use it as the gzip
     file name*/
    sprintf(fname,"%s",argv[1]);
    printf("%s\n",fname);
  }
  else{
    /*If there are no command line arguments, ask for one and exit*/
    printf("Need to enter trace file name as parameter.\n");
    exit(-1);
  }

  f = fopen(fname,"r");
  if (!f) {
    /*If the given file does not exist, tell the user and exit*/
    printf("Need to create %s first.\n",argv[1]);
    exit(-1);
  }


  while(fscanf(f,"%ld",&diff) != EOF){
    ea = ea + diff;
    fprintf(outfile, "%ld \n",ea);
  }
}

