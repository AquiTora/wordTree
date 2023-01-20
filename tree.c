#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "type.h"

//Локальный тип данных
typedef struct pair {
    Trnode * parent;
    Trnode * child;
} Pair;

//Прототипы локальных функций
static Trnode * MakeNode(const Word * wi);
static Pair SeekItem(Word * wi, const Tree * wtree);
static void AddNode(Trnode * new_node, Trnode * root);
static bool ToLeft(const Word * w1, const Word * w2);
static bool ToRight(const Word * w1, const Word * w2);
static void InOrder(const Trnode * root, void (* wfun)(Word word));
static Pair upCount(Word * wi, const Tree * wtree);
static void DeleteAllNodes(Trnode * root);

void InitializeTree(Tree * wtree)
{
    wtree->root = NULL;
    wtree->size = 0;
}
bool TreeIsEmpty(const Tree * wtree)
{
    if(wtree->root == NULL)
      return true;
    else
      return false;
}
int TreeItemCount(const Tree * ptree)
{
    return ptree->size;
}
bool AddItem(Word * wi, Tree * wtree)
{
    Trnode * new_node;

    if(upCount(wi, wtree).child != NULL)
    {
        wi->count++;     //Ведет подсчет повторяющихся слов
        return false;    //преждевременный возврат
    }
    else
      wi->count = 1;

    new_node = MakeNode(wi);    //указывает на новый узел
    if(new_node == NULL)
    {
        fprintf(stderr, "Не удалось создать узел\n");
        return false;
    }
    //Успешное создание нового узла
    wtree->size++;

    if(wtree->root == NULL)             //Случай 1: дерево пустое
      wtree->root = new_node;           //Новый узел - корень дерева
    else                                //Случай 2: дерево не пустое
      AddNode(new_node, wtree->root);   //Добавление узла к дереву

    return true;                        //Возврат в случае успеха
}
bool InTree(Word * wi, const Tree * wtree)
{
    return (SeekItem(wi, wtree).child == NULL) ? false : true;
}
void Traverse(const Tree * wtree, void (* wfun)(Word word))
{
    if(wtree != NULL)
      InOrder(wtree->root, wfun);
}
void DeleteAll(Tree * wtree)
{
    if(wtree != NULL)
      DeleteAllNodes(wtree->root);
    wtree->root = NULL;
    wtree->size = 0;
}

//Локальные функции
static Trnode * MakeNode(const Word * wi)
{
    Trnode * new_node;

    new_node = (Trnode *) malloc(sizeof(Trnode));
    if(new_node != NULL)
    {
        new_node->word = *wi;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    return new_node;
}
static Pair SeekItem(Word * wi, const Tree * wtree)
{
    Pair look;

    look.parent = NULL;
    look.child = wtree->root;

    if(look.child == NULL)
      return look;

    while(look.child != NULL)
    {
        if(ToLeft(wi, &(look.child->word)))
        {
            look.parent = look.child;
            look.child = look.child->left;
        }
        else if(ToRight(wi, &(look.child->word)))
        {
            look.parent = look.child;
            look.child = look.child->right;
        }
        else
          break;
    }
    if(look.child != NULL)
      wi->count = look.child->word.count;
    return look;
}
static void AddNode(Trnode * new_node, Trnode * root)
{
    if(ToLeft(&new_node->word, &root->word))
    {
        if(root->left == NULL)              //Пустое поддерево
          root->left = new_node;            //Поэтому добавить сюда узел
        else
          AddNode(new_node, root->left);    //Иначе обработать поддерево
    }
    else if(ToRight(&new_node->word, &root->word))
    {
        if(root->right == NULL)
          root->right = new_node;
        else
          AddNode(new_node, root->right);
    }
    else
    {
        fprintf(stderr, "Ошибка местоположения в AddNode()\n");
        exit(1);
    }
}
static bool ToLeft(const Word * w1, const Word * w2)
{
    int comp1;

    if((comp1 = strcmp(w1->metWord, w2->metWord)) < 0)
      return true;
    else
      return false;
}
static bool ToRight(const Word * w1, const Word * w2)
{
    int comp1;

    if((comp1 = strcmp(w1->metWord, w2->metWord)) > 0)
      return true;
    else
      return false;
}
static void InOrder(const Trnode * root, void (* wfun)(Word word))
{
    if(root != NULL)
    {
        InOrder(root->left, wfun);
        (*wfun)(root->word);
        InOrder(root->right, wfun);
    }
}
static Pair upCount(Word * wi, const Tree * wtree)
{
    Pair look;

    look.parent = NULL;
    look.child = wtree->root;

    if(look.child == NULL)
      return look;

    while(look.child != NULL)
    {
        if(ToLeft(wi, &(look.child->word)))
        {
            look.parent = look.child;
            look.child = look.child->left;
        }
        else if(ToRight(wi, &(look.child->word)))
        {
            look.parent = look.child;
            look.child = look.child->right;
        }
        else
          break;
    }
    if(look.child != NULL)
      look.child->word.count++;

    return look;
}
static void DeleteAllNodes(Trnode * root)
{
    Trnode * pright;

    if(root != NULL)
    {
        pright = root->right;
        DeleteAllNodes(root->left);
        free(root);
        DeleteAllNodes(pright);
    }
}