#pragma once
#include <string>
#include <vector>
#include <iostream>

template<typename T>
struct SkipListNode {
    T val;
    SkipListNode<T>* prev = nullptr;
    SkipListNode<T>* next = nullptr;

    SkipListNode<T>* skip = nullptr;

    SkipListNode(const T& el) {
        val = el;
    }
};

template<typename T>
class SkipListIterator {
    SkipListNode<T>* curr;

public:
    SkipListIterator() {
        curr = nullptr;
    }

    SkipListIterator(SkipListNode<T>* el) {
        curr = el;
    }

    SkipListNode<T>& operator=(const SkipListNode<T>& oth) {
        if (this == &oth) {
            return this;
        }

        curr = oth.curr;
        return this;
    }
    
    bool operator==(const SkipListIterator<T>& oth) const {
        return oth.curr == curr;
    }

    bool operator!=(const SkipListIterator<T>& oth) const {
        return oth.curr != curr;
    }

    bool isValid() {
        return curr != nullptr;
    }

    SkipListIterator<T>& operator++() {
        curr = curr->next;
        return *this;
    }

    SkipListIterator<T> operator++(int) {
        auto temp = *this;
        curr = curr->next;
        return temp;
    }

    SkipListIterator<T>& operator--() {
        curr = curr->prev;
        return *this;
    }

    SkipListIterator<T> operator--(int) {
        auto temp = *this;
        curr = curr->prev;
        return temp;
    }

    T& operator*() {
        return curr->val;
    }

    T* operator->() {
        return curr;
    }

    SkipListIterator<T>& skip() {
        //proverka dali ima skip??
        curr = curr->skip;
        return curr; 
    }

    SkipListIterator<T> getSkip() {
        auto temp = SkipListIterator<T>(curr->skip);
        return temp; 
    }

    SkipListIterator<T> getNext() {
        auto temp = SkipListIterator<T>(curr->next);
        return temp; 
    }

    void setSkip(const SkipListIterator<T>& oth) {
        curr->skip = oth.curr;
    }
};

template<typename T>
class SkipList {
    SkipListNode<T>* front;
    SkipListNode<T>* back;
    int sz;

public:
    SkipList() {
        sz = 0;
        front = back = nullptr;
    }

    SkipList(const SkipList<T>& oth) {
        sz = oth.sz;
        for (auto curr : oth) {
            push_back(new SkipListNode<T>(curr->val));
        }
    }

    SkipList<T>& operator=(const SkipList<T>& oth) {
        if (&oth == this) {
            return this;
        }

        ~SkipList();
        
        for (auto curr : oth) {
            push_back(new SkipListNode<T>(curr->val));
        }

        return this;
    }

    ~SkipList() {
        while (sz) {
            pop_back();
        }
    }

    void pop_back() {
        if (back == nullptr) {
            throw std::runtime_error("No elements in list.");
        }

        auto temp = back;
        back = back->prev;
        delete temp;

        sz--;
    }

    void pop_front() {
        if (front == nullptr) {
            throw std::runtime_error("No elements in list.");
        }

        auto temp = front;
        front = front->next;
        delete temp;

        sz--;
    }

    void push_back(const T& el) {
        sz++;

        if (front == nullptr) {
            front = back = new SkipListNode<T>(el);
            return;
        }

        auto temp = back;
        back = new SkipListNode<T>(el);
        temp->next = back;
        back->prev = temp;
    } 

    void push_front(const T& el) {
        sz++;

        if (front == nullptr) {
            front = back = new SkipListNode<T>(el);
            return;
        }

        auto temp = front;
        front = new SkipListNode<T>(el);
        temp->prev = front;
        front->next = temp;
    }

    SkipListIterator<T> begin() {
        return SkipListIterator<T>(front);
    }
    
    SkipListIterator<T> end() {
        return SkipListIterator<T>(back->next);
    }

    void print() {
        std::cout<<sz<<std::endl;
        for (auto curr : *this) {
            std::cout<<curr<<' ';
        }
        std::cout<<std::endl;
    }
};
