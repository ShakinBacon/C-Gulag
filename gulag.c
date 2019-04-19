#include <stdio.h>
#include <stdlib.h>



int main() {
    FILE *fp;
    FILE *fo;
    fp = fopen("input", "r");   //opens input file
    fo = fopen("output", "w");  //opens output file
    int task;
    int key = 1;
    char hash = 35;
    char subKey[25] = {0};
    char tmp1[1000] = {0};
    char realAlpha[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    
    fscanf(fp, "%d", &task);
    if (task == 1 || task == 3) {               //This "if" statement group is used to scan the file header for its instructions, and scan them into appropriate variables
        fscanf(fp, "%c %d", &hash, &key); 
    } else if (task == 2 || task == 4) {
        int count = 0;
        while (count < 28) {
            fscanf(fp, "%c", &subKey[count]);
            count++;
        }
    } else {
        int count = 0;
        while (!feof(fp)){
            fscanf(fp, "%c", &tmp1[count]);
            ++count;
        }
    }

    int count;
    
    printf("%d %c%d\n", task, hash, key);         //debug code; printing instructions
    switch (task) {                                    //detects and prints the selected task and key for user clarity, and directs the code to the correct encryption/decryption operation
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
            while(!feof(fp)) {                         //fully functioning substitution cipher (Alphabet key must be CAPS for assessment reasons the code works fine with lower case though)
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
            while (!feof(fp)) {                        //fully functioning rotation cipher text
                char tmp[1] = {0};
                fscanf(fp, "%c", &tmp[0]);
                int count = 0;
                while (count < 26) {                   //checks the current encrypted character for its possition (0-25) in the given alphabet key
                    if (tmp[0] == subKey[count+2]) {   //when the code finds a match it picks out which letter is in the same position on a real alphabet (realAlpha), count+2 is there because it saved the subKey weirdly
                        tmp[0] = realAlpha[count];
                        break;
                    }
                    ++count;
                }
                fprintf(fo, "%c", tmp[0]);
                printf("%c", tmp[0]);
            }
            break;
        case 5: printf("Brute forcing rotation cipher text:\n");
            count = 0;
            CYCLE: while (count < 1000) {                  //counts up to the possible limit of letters in tmp1, basically this makes sure every letter of cipher text is read, also the CYCLE label is here for a later goto function to use
                count = 0;
                while (tmp1[count] != 0) {                 //copy pasted code from case 1 reused to rotate unknown text
                    char tmp[1] = {0};
                    tmp[0] = tmp1[count];
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
                    ++count;
                }
                ++key;
                
                if (key < 27) {                            //loops if necessary to rotate the text
                    printf("\n\n");
                    fprintf(fo, "\n");
                    goto CYCLE;                            //goto function sends the code back to the top of the while loop in case 5
                } else {                                   //ends the cycle so as to not rotate the text too far
                    break;
                }
            }
            break;
        case 6: printf("Brute forcing substitution cipher text, this may not be fully accurate:\n");
            count = 0;
            int numOfLetters[26] = {0};                    //array to keep track of number of letters in laphabetical order
            while (tmp1[count] != 0) {                     //counts the numbers of letters in the cipher text and assigns them to appropriate positions in the above array
                int holdThis = tmp1[count];
                if (holdThis > 64 && holdThis < 91) {    
                    holdThis = holdThis - 64;
                    ++numOfLetters[holdThis];
                }
                ++count;
            }
            int largest;
            for (count = 0; count < 25; ++count){          //finds the highest number in the above array
                if (numOfLetters[count] > numOfLetters[count+1]) {
                    largest = numOfLetters[count];
                } else if (numOfLetters[count+1] > numOfLetters[count]) {
                    largest = numOfLetters[count+1];
                }
            }
            printf("%d\n", largest); //debug highest number
            for (count = 0; count < 25; ++count){          //deduces which letter there is the most of
                if (numOfLetters[count] == largest) {
                    break;
                }
            }
            int letter = count+64;
            printf("%c\n", letter); //debug what letter
            count = 0;
            while (tmp1[count] != 0) {                     //assigns the most common letter the value of 69, ASCII for 'E', assuming this from its commonality in the english language
                if (tmp1[count] == letter){
                    tmp1[count] = 69;
                }
                ++count;
            }
            
            
            
            
            
            
            
            
            
            printf("%s", tmp1); //debug full text
            break;
        default: printf("Unkown command %d", task);
    }
    
    
    
    fclose(fp);                                          //close input file
    fclose(fo);                                          //close output file
printf("\n\ndone");         //debug end flag
return 0; 
}

