#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *wordle(char *file, unsigned int len)
{
    int date = 0;
    time_t time_raw_format;
    struct tm * ptr_time;

    time ( &time_raw_format );
    ptr_time = localtime ( &time_raw_format );

    date |= ptr_time->tm_mday;
    date |= (ptr_time->tm_mday & 0xff);
    date |= (ptr_time->tm_mon & 0xff) << 8;
    date |= (ptr_time->tm_year & 0xffff) << 16;

    srand(date);
    int num = (rand() % (len + 1));
    char str[6];
    FILE * pFile = fopen(file, "r");
    fseek(pFile, 7*num, SEEK_SET);
    fgets(str, 6, pFile);
    fclose(pFile);
    str[5] = '\0';

    return strdup(str);
}

int isWord(char *word, char *file, unsigned int len)
{
    char str[6];
    FILE * pFile = fopen(file, "r");
    fseek(pFile, 0, SEEK_SET);
    for(int i = 0; i < len; i++){
        fseek(pFile, 7*i, SEEK_SET);
        fgets(str, 6, pFile);
        str[5] = '\0';
        if(strcmp(str, word) == 0){
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    unsigned int len = 15918;
    char *file = "words.csv";
    char *str = wordle(file, len);
    if(str == NULL){
        fprintf(stderr, "Allocation failed\n");
        exit(1);
    }

    if(argc != 2){
        printf("Provide exactly two arguments, ./a.out [some word]\n");
        exit(1);
    }

    if(strlen(argv[1]) != 5){
        printf("Provided word is not 5 letters long\n");
        exit(1);
    }
    if(!isWord(argv[1], file, len)){
        printf("Provided input is not a word\n");
        exit(1);
    }
    int correct = 1;
    for(int i = 0; i < 5; i++){
        if(argv[1][i] == str[i]){
            printf("\033[0;32m");
            printf("%c", argv[1][i]);
            printf("\033[0m");
        }
        else if(strchr(str, argv[1][i]) != NULL){
            printf("\033[0;33m");
            printf("%c", argv[1][i]);
            printf("\033[0m");
            correct = 0;
        }
        else{
            printf("%c", argv[1][i]);
            correct = 0;
        }
    }
    printf("\n");
    if(correct){
        printf("Congrats, you got the word!\n");
    }
    free(str);
    return 0;
}
