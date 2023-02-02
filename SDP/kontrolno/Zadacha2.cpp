#include <iostream>

template <typename T>
struct DLListElem {
    T data;
    DLListElem<T>* next;
    DLListElem<T>* prev;

    DLListElem(const T& el) {
        data = el;
    }
};

//O(N^2) runtime; O(1) additional memory
template <typename T>
void sortDescending(DLListElem<T>* start, DLListElem<T>* end) {
    DLListElem<T>* currEnd = end;

    bool changed = true;
    while (changed) {
        changed = false;

        auto curr = start;
        while (curr != currEnd) {
            if (curr->data < curr->next->data) {
                std::swap(curr->data, curr->next->data);
                changed = true;
            }
            curr = curr->next;
        }

        currEnd = currEnd->prev;
    }
}

int main() {
    /*
    //test
    //trqbva da se zachisti pametta

    auto a7 = new DLListElem<int>(8); 
    auto a6 = new DLListElem<int>(9); a6->next = a7; a7->prev = a6; 
    auto a5 = new DLListElem<int>(2); a5->next = a6; a6->prev = a5;
    auto a4 = new DLListElem<int>(8); a4->next = a5; a5->prev = a4;
    auto a3 = new DLListElem<int>(1); a3->next = a4; a4->prev = a3;
    auto a2 = new DLListElem<int>(3); a2->next = a3; a3->prev = a2;
    auto a1 = new DLListElem<int>(7); a1->next = a2; a2->prev = a1;

    sortDescending(a1, a7);

    std::cout<<a1->data<<' '<<a2->data<<' '<<a3->data<<' '<<a4->data<<' '<<a5->data<<' '<<a6->data<<' '<<a7->data<<std::endl;
    */
} 