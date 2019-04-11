#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    FILE *fo;
    fp = fopen("input", "r");   //opens input file
    fo = fopen("output", "w");  //opens output file
    int task;
    int key = 0;
    char hash = 35;
    char subKey[25] = {0};
    //fscanf(fp, "%d %c %d", &task, &hash, &key);        //reading instructions/key
    
    fscanf(fp, "%d", &task);
    if (task == 1 || task == 3) {
        fscanf(fp, "%c %d", &hash, &key); 
    } else if (task == 2 || task == 4) {
        int count = 0;
        while (count < 28) {
            fscanf(fp, "%c", &subKey[count]);
            count++;
        }        
    } else {
        printf ("why are you like this?");
    }

    
    
    
    
    
    printf("%d %c%d\n", task, hash, key);             //debug code; printing instructions
    switch (task) {                                    //detects and prints the selected task and key for user clarity
        case 1: printf("Encrypting with rotation cipher using key: %c%d\n", hash, key); 
            while (!feof(fp)) {                        //fully functioning rotation cipher (key 0 >>> 25)
                char tmp[1] = {0};
                fscanf(fp, "%c", &tmp[0]);
                int n = tmp[0];
                if (n > 64 && n < 91) {                //checks for capital letters
                    if (key + n > 90) {                //if looping occurs
                        tmp[0] = tmp[0] - (26-key);
                    } else {                           // if no looping occurs
                        tmp[0] = tmp[0] + key;
                    }
                    fprintf(fo, "%c", tmp[0]); 
                } else if (n > 96 && n < 123){         //checks for lower case letters
                    if (n + key > 122) {               // if looping occurs
                        tmp[0] = (tmp[0]-32) - (26-key);
                    } else {                           // if no looping occurs
                        tmp[0] = (tmp[0]-32) + key;
                    }
                    fprintf(fo, "%c", tmp[0]);
                } else {                               //accounts for non-letter characters
                    fprintf(fo, "%c", tmp[0]);
                }    
                printf("%c", tmp[0]);
            } 
            break;
        case 2: printf("Encrypting with substitution cipher using alphabet key:\n%s\n", subKey);
            while(!feof(fp)) {                         //fully functioning substitution cipher (Alphabet key must be CAPS)
                char tmp[1] = {0};
                fscanf(fp, "%c", & tmp[0]);
                int n = tmp[0];
                if (n > 64 && n < 91) {                //checks for capital letters
                    n = n - 63;
                    tmp[0] = subKey[n];
                } else if (n > 96 && n < 123) {        //checks for lower case letters
                    n = n - 95;
                    tmp[0] = subKey[n];
                }
                fprintf(fo, "%c", tmp[0]);
                printf("%c", tmp[0]);
            }      
            break;
        case 3: printf("Decrypting rotation cipher text using key: %d\n", key); 
            while (!feof(fp)) {                        //fully functioning rotation cipher decryptor
                char tmp[1] = {0};
                fscanf(fp, "%c", &tmp[0]);
                int n = tmp[0];
                if (n > 64 && n < 91) {                //checks for capital letters
                    if (n - key < 65) {                //if looping occurs
                        tmp[0] = tmp[0] + (26-key);
                    } else {                           // if no looping occurs
                        tmp[0] = tmp[0] - key;
                    }
                    fprintf(fo, "%c", tmp[0]);
                } else if (n > 96 && n < 123){         //checks for lower case letters
                    if (n - key < 97) {                // if looping occurs
                        tmp[0] = (tmp[0]-32) + (26-key);
                    } else {                           // if no looping occurs
                        tmp[0] = (tmp[0]-32) - key;
                    }
                    fprintf(fo, "%c", tmp[0]);
                } else {                               //accounts for non-letter characters
                    fprintf(fo, "%c", tmp[0]);
                }    
                printf("%c", tmp[0]);
            }
            break; 
        case 4: printf("Decrypting substitution cipher text using alphabet key:\n%s\n", subKey); 
            
            break;
        default: printf("Unkown command %d", task);
    }
    
    
    fclose(fp);
    fclose(fo);
return 0; 
}

