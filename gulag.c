#include <stdio.h>

int main() {
    FILE *fp;
    fp = fopen("input", "r+"); //opens input file
    
    //char contents[10];
    //int n = 0;
    int task;
    int key;
    char hash;
    
    fscanf(fp, "%d %c %d", &task, &hash, &key); //debug code: reading instructions
    printf("%d %c %d\n", task, hash, key);      //debug code: printing instructions
    
    while (!feof(fp)) { //debug code: printing text block
        char tmp[0];
        fscanf(fp, "%c", &tmp[0]);
        printf("%d\n", tmp[0]);
    } 

        
          
        
    fclose(fp);
return 0; }