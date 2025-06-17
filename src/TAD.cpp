#include <iostream>
#include <iomanip>
#include "../include/TAD.hpp"

//Funções para lista (list)

int list::len() {
    int count = 0;
    list* current = this;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}
list::list(int id) : id(id), next(nullptr), prev(nullptr) {
    // Constructor body can be empty as initialization is done in the member initializer list
} 
list::~list() {
    // Destructor body can be empty as we are not managing any dynamic memory in this class
}

//Funções para pilha (stack) de pacotes
void pile::push(package p) {
    pile* new_pile = new pile;
    new_pile->value = p;
    new_pile->next = top;
    top = new_pile;
}
package pile::pop() {
    if (top == nullptr) {
        throw std::runtime_error("Stack is empty");
    }
    package p = top->value;
    pile* temp = top;
    top = top->next;
    delete temp;
    return p;
}
void pile::clear() {
    while (top != nullptr) {
        pop();
    }
}
pile::~pile() {
    clear();
}
pile::pile() : next(nullptr), top(nullptr) {
    // Constructor body can be empty as initialization is done in the member initializer list
}
pile::pile(pile* next, pile* top) : next(next), top(top) {
    // Constructor body can be empty as initialization is done in the member initializer list
}

//Funções para armazenamentos (storage)
void storage::conect_storage(storage* storage) {
    if (storage == nullptr) return;
    adjacents_storages.next = new list(storage->id);
    adjacents_storages.next->prev = &adjacents_storages;
}