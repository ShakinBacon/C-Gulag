#include <stdio.h>


int main() {
    FILE *fp;
    FILE *fo;
    fp = fopen("input", "r"); //opens input file
    fo = fopen("output", "w"); // opens output file
    //char contents[10];
    //int n = 0;
    int task;
    int key;
    char hash;
    
    fscanf(fp, "%d %c %d", &task, &hash, &key); //reading instructions/key
    printf("%d %c %d\n", task, hash, key);      //debug code; printing instructions/key
    switch (task) { //prints the performed task and key for user clarity
        case 1: printf("Encrypting with rotation cipher using key: %d\n", key); 
            break;
        case 2: printf("Encrypting with substitution cipher using key: %d\n", key); 
            break;
        case 3: printf("Decrypting rotation cipher text using key: %d\n", key); 
            break;
        case 4: printf("Decrypting substitution cipher text using key: %d\n", key); 
            break;
        default: printf("Unkown command %d\n", task);
    }
    
    while (!feof(fp)) { //fully functioning rotation cipher prototype (key 0 >>> 26)
        char tmp[0];
        fscanf(fp, "%c", &tmp[0]);
        int n = tmp[0];
        if (n > 64 && n < 91) {                //checks for capital letters
            if (key + n > 90) {                //if looping occurs
                tmp[0] = tmp[0] - (26-key);
                fprintf(fo, "%c", tmp[0]);
                
            } else {                           // if no looping occurs
                tmp[0] = tmp[0] + key;
                fprintf(fo, "%c", tmp[0]);
                
            }

        } else if (n > 96 && n < 123){         //checks for lower case letters
            if (n + key > 122) {               // if looping occurs
                tmp[0] = tmp[0] - (26-key);
                fprintf(fo, "%c", tmp[0]);
                
            } else {                           // if no looping occurs
                tmp[0] = tmp[0] + key;
                fprintf(fo, "%c", tmp[0]);
                
            }
        } else if (feof(fp)){                  //stops code at last file item
            tmp[0] = 0;
            fprintf(fo, "%c", tmp[0]);
        } else {                               //accounts for non-letter characters
            fprintf(fo, "%c", tmp[0]);
            
        }    
        printf("%c", tmp[0]);
    } 
    fclose(fp);
return 0; 
}

