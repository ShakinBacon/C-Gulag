#include <stdio.h>

int main() {
    FILE *fp;
    fp = fopen("input", "r");
    
    //char contents[100], contents2[10];
    
    int task;
    int key;
    char hash;
    
    fscanf(fp, "%d %c %d", &task, &hash, &key);
    printf("%d %c%d\n", task, hash, key);
    
    
    fclose(fp);
return 0; }