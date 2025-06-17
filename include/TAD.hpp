#ifndef TAD_H
#define TAD_H

struct list {
    int id;
    list* next;
    list* prev;

    int len();

    list(int id);
    ~list();
};

struct package {
    int id;
    int post_time;
    int origin;
    int destination;
};

struct pile{
    pile* next;
    pile* top;
    package value;

    void push(package p);
    package pop();
    void clear();

    ~pile();
    pile();
    pile(pile* next, pile* top);
};

struct storage {
    int id;
    list adjacents_storages;
    pile storage_pile;

    void conect_storage(storage storage);
    void add_package(package p);
    package remove_package();
    int get_storage_pile_size();

    ~storage();
    storage();
    storage(int id);
    storage(int id, storage* adjacents_storages);
};

#endif