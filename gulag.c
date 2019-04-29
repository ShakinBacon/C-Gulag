#include <stdio.h>
#include <stdlib.h>

/*
This code is designed to scan commands from the file header, which is to be structured as follows:
line 1: number 1-6 indicating desired operation
    1 - Encrypt with Rotarion
    2 - Encrypt with Substitution
    3 - Decrypt Rotation text (known key)
    4 - Decrypt Substitution text (known key)
    5 - Brute force Rotation text
    6 - Brute force Substitution text

line 2: The cipher key if required in the following format
    Rotation cipher key is to be written:   #*number 1-25*
    Substitution alphabet key is to be a set of 26 letters written in full caps i.e:  QWERTYUIOPASDFGHJKLZXCVBNM
    If no key is required (operations 5 & 6) put line 3 contents on line 2 instead
    
line 3: The block of text which is to be encrypted or decrypted
    The text must be written/pasted on a single line
    If the text is encrypted letters meant for decryption, it must be fully capitalised
    Otherwise there is no restriction on the contents of the text
*/

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
    if (task == 1 || task == 3) {               //This "if" statement is used to scan the file header for its instructions, and scan them into appropriate variables
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
    
    //printf("%d %c%d\n", task, hash, key);         //debug code; printing instructions
    switch (task) {                                    //detects and prints the selected task and key for user clarity, and directs the code to the correct encryption/decryption operation
        case 1: printf("Encrypting with rotation cipher using key: %c%d\n", hash, key); 
            while (!feof(fp)) {                        //loop cycles through the input file until it detects no content, i.e. the final letter (key 0 >>> 25)
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
            while(!feof(fp)) {                         //loop cycles through the input file until it detects no content, i.e. the final letter (Alphabet key must be CAPS for assessment reasons)
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
            while (!feof(fp)) {                        //loop cycles through the input file until it detects no content, i.e. the final letter
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
            while (!feof(fp)) {                        //loop cycles through the input file until it detects no content, i.e. the final letter
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
            char tmp2[1000] = {0};                         //array to save the cipher text to be reused
            CYCLE: while (count < 1000) {                  //counts up to the possible limit of letters in tmp1, basically this makes sure every letter of cipher text is read, also the CYCLE label is here for a later goto function to use
                count = 0;
                while (tmp1[count] != 0) {                 //copy pasted code from case 1 restructured to rotate unknown text multile times
                    char tmp[1] = {0};
                    tmp[0] = tmp1[count];
                    int n = tmp[0];
                    if (n > 64 && n < 91) {                //checks for letters using ascii values
                        if (key + n > 90) {                //if looping occurs
                            tmp[0] = tmp[0] - (26-key);
                        } else {                           //if no looping occurs
                            tmp[0] = tmp[0] + key;
                        }
                        tmp2[count] = tmp[0];
                    } else {                               //accounts for non-letter characters
                        tmp2[count] = tmp[0];
                    }    
                    //printf("%c", tmp[0]);
                    ++count;
                }
                ++key;
                
                /*bellow is a set of 3 filters searching the tmp2 array for the words: ' the ' ' and ' that ', 
                effectivley these are acting as a way to find the correct deciphered phrase rahter than 
                printing all 25 possible rotations of the unknown text*/
                int the = 0;                            
                for (count = 0; tmp2[count] != 0; ++count){
                    if (tmp2[count] == 32){
                        if (tmp2[count+1] == 84){
                            if (tmp2[count+2] == 72){
                                if (tmp2[count+3] == 69){
                                    if (tmp2[count+4] == 32){
                                        the = 1;
                                    }
                                }
                            }
                        }
                    }
                }
                
                int and = 0;
                for (count = 0; tmp2[count] != 0; ++count){
                    if (tmp2[count] == 32){
                        if (tmp2[count+1] == 65){
                            if (tmp2[count+2] == 78){
                                if (tmp2[count+3] == 68){
                                    if (tmp2[count+4] == 32){
                                        and = 1;
                                    }
                                }
                            }
                        }
                    }
                }

                
                int that = 0;
                for (count = 0; tmp2[count] != 0; ++count){
                    if (tmp2[count] == 32){
                        if (tmp2[count+1] == 84){
                            if (tmp2[count+2] == 72){
                                if (tmp2[count+3] == 65){
                                    if (tmp2[count+4] == 84){
                                        if (tmp2[count+5] == 32){
                                            that = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if (the == 0 && and == 0 && that == 0) {                                //if the text is still encrypted the rotation continues
                    if (key < 27) {                            //loops if necessary to rotate the text
                        goto CYCLE;                            //goto function sends the code back to the top of the while loop in case 5
                    } else {                                   //ends the cycle so as to not rotate the text too far
                        break;
                    }
                } else if (the == 1 || and == 1 || that == 1){                          //if the text is correct it is printed
                    printf("\n%s", tmp2);
                    fprintf(fo, "%s", tmp2);
                }

            }
            break;
        case 6: printf("Brute forcing substitution cipher text, this will not be fully accurate:\n");
            count = 0;
            int numOfLetters[26] = {0};                    //array to keep track of number of letters in laphabetical order
            while (tmp1[count] != 0) {                     //counts the numbers of letters in the cipher text and assigns them to appropriate positions in the above array
                int holdThis = tmp1[count];
                if (holdThis > 64 && holdThis < 91) {    
                    holdThis = holdThis - 64;
                    ++numOfLetters[holdThis-1];
                }
                ++count;
            }
            
            
            int largest = numOfLetters[0];
            for(count = 0; count < 26; count++){            //'for' loop used to find the largest number in the alphabetical numOfLetters array
                if (numOfLetters[count] > largest){
                    largest = numOfLetters[count];
                    
                }
            }
            //printf("\n%d\n", largest); //debug how many
            
            
            for (count = 0; count < 25; ++count){          //deduces which letter there is the most of
                if (numOfLetters[count] == largest) {
                    numOfLetters[count] = 0;               //deletes the value of the largest letter, to allow the following code to locate the seccond highest letter
                    ++count;
                    break;
                }
            }
            int letter = count+64;
            //printf("\n%c\n", letter); //debug what letter
            
            
            //Duplicated above code to find the seccond highest letter
            
            largest = numOfLetters[0];
            for(count = 0; count < 26; count++){            //'for' loop used to find the largest number in the alphabetical numOfLetters array
                if (numOfLetters[count] > largest){
                    largest = numOfLetters[count];
                    
                }
            }
            //printf("\n%d\n", largest); //debug how many
            
            
            for (count = 0; count < 25; ++count){          //deduces which letter there is the most of
                if (numOfLetters[count] == largest) {
                    numOfLetters[count] = 0;               //deletes value of seccond highest letter, to allow following code to find third highest letter
                    ++count;
                    break;
                }
            }
            int letter2 = count+64;
            //printf("\n%c\n", letter2); //debug what letter
            numOfLetters[count] = 0;
            
            
            
            /* This is a somewhat complicated to understnad analytical process, it simply places the symbolds '{' and '}'
            in place of the most common 2 letters E and T respectivley, knowing that the most common letters in english 
            are E & T in respectivley these symbols will be later replaced, but are now placeholders so as to not
            confuse the code between the deciphered E T's and the encoded E T's representing other letters, if this is
            not done the following filters will scramble the text beyond recognition
            */
            count = 0;
            while (tmp1[count] != 0) {
                if (tmp1[count] == letter){
                    tmp1[count] = 123;
                } else if (tmp1[count] == letter2){
                    tmp1[count] = 125;
                }
                ++count;
            }
            
            
            
            /*The following groups of nested if's are used to find a pattern within the cipher text by detecting them as ascii values, 
            when a pattern is successfully located using the 'filters' an unknown letter within is assumed based on the context of 
            the surrounding letters/symbols*/
            int letterH;
            for(count = 0; tmp1[count] !=0; ++count){          //a sort of filter to find the pattern ' t*e ' and assume * is 'h' and that the word is 'the'
                if (tmp1[count] == 32){
                    if (tmp1[count+1] == 125){
                        if (tmp1[count+3] == 123){
                            if (tmp1[count+4] == 32){
                                letterH = tmp1[count+2];
                                break;
                            }
                        }
                    }
                }
            }
            
            //for same reasons as for E & T, the decrpted letter is given an arbitrary symbol, in this case '|' for H
            for(count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == letterH){
                    tmp1[count] = 124;
                }
            }
            int letterA;
            for (count = 0; tmp1[count] !=0; ++count){      //a filter to find the pattern ' th*t ' assume that * is 'a' and that the word is 'that'
                if (tmp1[count] == 32){
                    if (tmp1[count+1] == 125){
                        if (tmp1[count+2] == 124){
                            if (tmp1[count+4] == 125){
                                if (tmp1[count+5] == 32){
                                    letterA = tmp1[count+3];
                                    break;
                                }
                            } 
                        }
                    }
                }
            }
            
            // substituting a newly decoded letter with an arbitrary symbol, in this case '~' for A
            for(count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == letterA){
                    tmp1[count] = 126;
                }
            }
            
            
            int letterR;
            for (count = 0; tmp1[count] !=0; ++count){           //a filter to find the pattern ' the*e ' assume that * is 'r' and that the word is 'there'
                if (tmp1[count] == 32){
                    if (tmp1[count+1] == 125){
                        if (tmp1[count+2] == 124){
                            if (tmp1[count+3] == 123){
                                if (tmp1[count+5] == 123){
                                    if (tmp1[count+6] == 32){
                                        letterR = tmp1[count+4];
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // substituting a newly decoded letter with an arbitrary symbol, in this case '!' for R
            for(count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == letterR){
                    tmp1[count] = 33;
                }
            }
            
            
            int letterV;                                    //filter looking for ' ha*e ' assuming * is v and the word is have
            for (count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == 32){
                    if (tmp1[count+1] == 124){
                        if (tmp1[count+2] == 126){
                            if (tmp1[count+4] == 123){
                                if (tmp1[count+5] == 32){
                                    letterV = tmp1[count+3];
                                }
                            }
                        }
                    }
                }
            }
            
            // substituting a newly decoded letter with an arbitrary symbol, in this case '@' for V
            for(count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == letterV){
                    tmp1[count] = 64;
                }
            }
            
            
            int letterI;                                //filter looking for ' *t ' assuming * is i and the word is it
            for (count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == 32){
                    if (tmp1[count+2] == 125){
                        if (tmp1[count+3] == 32){
                            letterI = tmp1[count+1];
                        }
                    }
                }
            }

            // substituting a newly decoded letter with an arbitrary symbol, in this case '#' for I
            for(count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == letterI){
                    tmp1[count] = 35;
                }
            }
            
            
            int letterO;                            //ilter looking for ' t* ' assuming * is o and the word is to
            for (count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == 32){
                    if (tmp1[count+1] == 125){
                        if (tmp1[count+3] == 32)
                        letterO = tmp1[count+2];
                    }
                }
            }
            
            // substituting a newly decoded letter with an arbitrary symbol, in this case '$' for O
            for(count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == letterO){
                    tmp1[count] = 36;
                }
            }
                
            int letterN;                                //filter looking for ' *ot ' assuming * is n and the word is not
            for (count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == 32){
                    if (tmp1[count+2] == 36){
                        if (tmp1[count+3] == 125){
                            if (tmp1[count+4] == 32){
                                letterN = tmp1[count+1];
                            }
                        }
                    }
                }
            }
            
            // substituting a newly decoded letter with an arbitrary symbol, in this case '%' for N
            for(count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == letterN){
                    tmp1[count] = 37;
                }
            }
            
            int letterD;                                //filter looking for ' an* ' assuming * is d and the word is and
            for (count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == 32){
                    if (tmp1[count+1] == 126){
                        if (tmp1[count+2] == 37){
                            if (tmp1[count+4] == 32){
                                letterD = tmp1[count+3];
                            }
                        }
                    }
                }
            }
            
            // substituting a newly decoded letter with an arbitrary symbol, in this case '^' for D
            for(count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == letterD){
                    tmp1[count] = 94;
                }
            }
            
            int letterS;                                //filter looking for ' *t'* ' assuming * is i and s and the word it's and
            for (count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == 32){
                    if (tmp1[count+2] == 125){
                        if (tmp1[count+3] == 39){
                            if (tmp1[count+5] == 32){
                                letterI = tmp1[count+1];
                                letterS = tmp1[count+4];
                            }
                            
                        }
                    }
                }
            }
            
            // substituting a newly decoded letter with an arbitrary symbol, in this case '#' for I
            // this letter might not have been deciphered using the previous I filter code, if it is this will have no effect
            for(count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == letterI){
                    tmp1[count] = 35;
                }
            }
            
            // substituting a newly decoded letter with an arbitrary symbol, in this case '&' for S
            for(count = 0; tmp1[count] !=0; ++count){
                if (tmp1[count] == letterS){
                    tmp1[count] = 38;
                }
            }
            
            
            //printf("\n%s\n", tmp1); //debug scrambled text
            
            for(count = 0; tmp1[count] != 0; ++count){      //switch;case function in a loop to decipher the symbols implemented by the filter functions into their appropriate letters
                int yeet = tmp1[count];                     //tmp1[count]'s ascii value is stored in an integer variable to be used in the switch;case function
                switch(yeet){
                    case 123:
                        tmp1[count] = 69;
                        break;
                    case 125:
                        tmp1[count] = 84;
                        break;
                    case 124:
                        tmp1[count] = 72;
                        break;
                    case 126:
                        tmp1[count] = 65;
                        break;
                    case 33:
                        tmp1[count] = 82;
                        break;
                    case 64:
                        tmp1[count] = 86;
                        break;
                    case 35:
                        tmp1[count] = 73;
                        break;
                    case 36:
                        tmp1[count] = 79;
                        break;
                    case 37:
                        tmp1[count] = 78;
                        break;
                    case 94:
                        tmp1[count] = 68;
                        break;
                    case 38:
                        tmp1[count] = 83;
                    default:
                        tmp1[count] = tmp1[count]; //ignores real special characters in the text, spaces and undeciphered characters
                        break;
                }
            }
            printf("\n%s", tmp1);
            fprintf(fo, "%s", tmp1);
            break;
        default: printf("Unkown operation: %d ", task);
    }

    
    fclose(fp);                                          //close input file
    fclose(fo);                                          //close output file
//printf("\n\ndone");         //debug end flag
return 0; 
}