#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "type.h"

#define SIZE 40

void addWord(Word * wd, Tree * wt);
void showWords(const Tree * wt);
void printItem(Word item);
void findWord(const Tree * wt);
void menu(void);
void uppercase(char * str);
char * s_gets(char * st, int n);

int main(void)
{
    FILE * fp;
    Word item;
    Tree words;
    int choice;

    InitializeTree(&words);

    if((fp = fopen("text.dat", "r")) == NULL)
    {
        puts("Не получилось открыть файл.");
        exit(EXIT_FAILURE);
    }

    while(fscanf(fp, "%s", item.metWord) > 0)
    {
        addWord(&item, &words);
    }
    menu();
    while(scanf("%d", &choice) > 0)
    {
        switch(choice)
        {
            case 1: showWords(&words);
              break;
            case 2: findWord(&words);
              break;
            case 3: DeleteAll(&words);
              break;
        }
        menu();
    }

    if(fclose(fp) != 0)
      fprintf(stderr, "Ошибка при закрытии файла.\n");

    return 0;
}
//Определения функций
void addWord(Word * wd, Tree * wt)
{
    uppercase(wd->metWord);

    AddItem(wd, wt);
}
void showWords(const Tree * wt)
{
    if(TreeIsEmpty(wt))
      puts("Записи отсутствуют!");
    else
      Traverse(wt, printItem);
}
void printItem(Word item)
{
    printf("Слово: %-19s встречалось: %d раз\n", item.metWord, item.count);
}
void findWord(const Tree * wt)
{
    Word temp;

    if(TreeIsEmpty(wt))
    {
        puts("Нет слов!");
        return;
    }
    puts("Введите слово, которое нужно найти:");
    while(getchar() != '\n')
      continue;
    s_gets(temp.metWord, SIZE);
    uppercase(temp.metWord);
    printf("Слово %s ", temp.metWord);

    if(InTree(&temp, wt))
      printf("встречается: %d раз\n", temp.count);
}
void menu(void)
{
    puts("Выберите, что делать:");
    puts("1 - показать все слова по порядку.");
    puts("2 - найти какое-то одно слово");
    puts("3 - удалить все.");
    puts("q - для выхода.");
}
void uppercase(char * str)
{
    while(*str)
    {
        *str = toupper(*str);
        str++;
    }
}
char * s_gets(char * st, int n)
{
    char * ret_val;
    char * find;

    ret_val = fgets(st, n, stdin);

    if(ret_val)
    {
        find = strchr(st, '\n');
        if(find)
          *find = '\0';
        else
          while(getchar() != '\n')
            continue;
    }

    return ret_val;
}