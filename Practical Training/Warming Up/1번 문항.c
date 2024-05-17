#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define Max 60

int main() {
    char sentence[Max + 1], command, save[Max+1];
    int i = 0,j=0;
    printf("Input sentence: ");
    fgets(sentence, sizeof(sentence), stdin);
    sentence[strcspn(sentence, "\n")] = '\0';
    for (int i = 0; i < Max; i++)
    {
        save[i] = sentence[i];
    }
    char last, first;
    
    do {
        printf("¸í·É¾î: ");
        scanf(" %c", &command);

        if (command >= 'a' && command <= 'z')
        {
            for (int i = 0; i < strlen(sentence); i++)
            {
                if (save[i] == command && sentence[i] != '*') {
                    sentence[i] = '*';
                }
                else if (save[i] == command && sentence[i] == '*')
                {
                    sentence[i] = save[i];
                }
            }
            command = toupper(command);
            for (j = 0; j < strlen(sentence); j++)
            {
                if (save[j] == command && sentence[j] != '*') {
                    sentence[j] = '*';
                }
                else if (save[j] == command && sentence[j] == '*')
                {
                    sentence[j] = save[j];
                }
            }
            command = tolower(command);
            printf("%s", sentence);
        }
        else
        {


            switch (command) {
            case '1':
                for (int i = 0; sentence[i] != '\0'; i++) {
                    if (sentence[i] == ' ') {
                        sentence[i] = '@';
                    }
                    else if (sentence[i] == '@') {
                        sentence[i] = ' ';
                    }
                }
                printf("%s\n", &sentence);
                break;
            case '2':
            {
                int count = 1;
                for (int i = 0; sentence[i] != '\0'; i++) {
                    if (sentence[i] == ' ' && sentence[i + 1] != ' ') {
                        count++;
                    }
                }
                for (int i = 0; sentence[i] != '\0'; i++) {
                    if (sentence[i] == '@' && sentence[i + 1] != '@') {
                        count++;
                    }
                }
                printf("Result: %d words\n", count);
            }
            break;
            case '3':
                for (int i = 0; i < strlen(sentence); i++) {
                    if (isupper(sentence[i])) {
                        sentence[i] = tolower(sentence[i]);
                    }
                    else if (islower(sentence[i])) {
                        sentence[i] = toupper(sentence[i]);
                    }
                    else {

                    }
                }
                printf("%s\n", sentence);
                break;
            case '4':
                first = sentence[0];
                for (i = 1; i < strlen(sentence); i++)
                {
                    sentence[i - 1] = sentence[i];
                }
                sentence[strlen(sentence) - 2] = first;
                printf("%s\n", &sentence);
                break;
            case '5':
                last = sentence[strlen(sentence) - 2];
                for (i = strlen(sentence) - 2; i > 0; i--)
                {

                    sentence[i] = sentence[i - 1];

                }
                sentence[0] = last;
                printf("%s\n", &sentence);
                break;
            case '0':
                break;
            default:
                break;
            }

        }
    } while (command != '0');
       return 0;
}