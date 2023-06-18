#include "ArrayList.h"

ArrayList::ArrayList() {
    capacity = 10000;   // 10,000
    num_items = 0;
    data = new char[capacity];
}

ArrayList::~ArrayList() {
    delete [] data;
}

bool ArrayList::isEmpty() {
    return (num_items == 0);
}

int ArrayList::size() {
    return num_items;
}

char ArrayList::at(int index) {
    assert((index >= 0) and (index < num_items));
    return data[index];
}

void ArrayList::pushBack(char c) {
    if (num_items == capacity) {
        expand();
    }
    data[num_items] = c;
    num_items++;
}

void ArrayList::editAt(int index, char newVal) {
    assert((index >= 0) and (index < num_items));
    data[index] = newVal;
}

void ArrayList::print(FILE *fp) {
    int size = this->size();
    for (int i = 0; i < size; i++) {
        fprintf(fp, "%c", this->at(i));
    }
}

void ArrayList::expand() {
    capacity = 2 * capacity + 1;
    char *arr = new char [capacity];

    for (int i = 0; i < num_items; i++)
    {
        arr[i] = data[i];
    }

    delete [] data;
    data = arr;
}
