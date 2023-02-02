#include <iostream>

template <typename T>
struct Node {
    T value;
    Node<T>* next;
};

//for testing
template <typename T>
void print(Node<T>* list) {
    while (list) {
        std::cout<<list->value<<' ';
        list = list->next;
    }
    std::cout<<std::endl;
}

bool makeTotal(Node<int>* list) {
    Node<int>* prev = list;
    Node<int>* curr = list->next;

    if (!curr || !curr->next) { 
        return false;
    }

    bool isTotal = true;
    while (curr->next) { 
        if (prev->value + curr->next->value != curr->value) {
            isTotal = false;
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    if (isTotal) {
        return true;
    }

    prev = list;
    curr = list->next;
    while (curr->next) { 
        if (prev->value + curr->next->value == curr->value) {
            prev->next = curr->next;
            delete curr;
            curr = prev->next;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }

    return false;
}

int main() {
    /*
    Node<int>* node = new Node<int>({1, new Node<int>({-4, new Node<int>({-5, new Node<int>({-1, new Node<int>({4, nullptr})})})})});
    print(node);
    std::cout<<makeTotal(node)<<"   -   ";
    print(node);std::cout<<std::endl;


    node = new Node<int>({10, new Node<int>({5, new Node<int>({-5, new Node<int>({-15, new Node<int>({2, nullptr})})})})});
    print(node);
    std::cout<<makeTotal(node)<<"   -   ";
    print(node);std::cout<<std::endl;
    */
}