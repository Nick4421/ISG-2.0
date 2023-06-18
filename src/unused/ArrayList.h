#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <assert.h>
#include <iostream>

class ArrayList {
public:
    ArrayList();
    ~ArrayList();

    bool isEmpty();
    int size();
    char at(int index);

    void pushBack(char c);
    void editAt(int index, char newVal);

    void print(FILE *fp);

private:
    int capacity;
    int num_items;
    char *data;

    void expand();
};

#endif
