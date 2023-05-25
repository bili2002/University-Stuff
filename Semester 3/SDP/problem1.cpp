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


struct myPair {
    int left, right;
};

template <typename T>
void reverseSubList(Node<T>*& list, myPair& interval, size_t n) {
    if (interval.left > interval.right || interval.left < 0 || interval.right >= n) {
        throw ;
    } 

    if (interval.left == interval.right) {
        return;
    }

    Node<T>* subListHead;
    int i = 0;

    if (interval.left == 0) {
        subListHead = new Node<T>({T(), list});
    } else {
        subListHead = list;
        while (i+1 < interval.left) {
            subListHead = subListHead->next;
            i++;
        }
    }

    Node<T>* currNode = subListHead;
    currNode = currNode->next;
    i++;
    while (i < interval.right) {
        Node<T>* temp = currNode->next;
        currNode->next = currNode->next->next;

        temp->next = subListHead->next;
        subListHead->next = temp;
        i++;
    }

    if (interval.left == 0) {
        list = subListHead->next;
        delete subListHead;
    }
}

template <typename T>
void reverseSubLists(Node<T>*& list, Node<myPair>* intervals) {
    size_t n = 0;
    Node<T>* temp = list;
    while (temp) { 
        n++;
        temp = temp->next;
    }

    while (intervals != nullptr) {
        reverseSubList(list, intervals->value, n);
        intervals = intervals->next;
    }
}

int main() {
    /*
    Node<int>* node = new Node<int>({5, new Node<int>({10, new Node<int>({12, new Node<int>({1, new Node<int>({15, new Node<int>({17, new Node<int>({13, nullptr})})})})})})});
    print(node);
    Node<myPair>* intervals = new Node<myPair>({{0, 3}, new Node<myPair>({{2, 5}, nullptr})});
    reverseSubLists(node, intervals);
    print(node);


    node = new Node<int>({11, new Node<int>({4, new Node<int>({3, new Node<int>({7, new Node<int>({13, new Node<int>({4, new Node<int>({5, nullptr})})})})})})});
    print(node);
    Node<myPair>* intervals = new Node<myPair>({{1, 3}, new Node<myPair>({{2, 5}, new Node<myPair>({{5, 6}, nullptr})})});
    reverseSubLists(node, intervals);
    print(node);
    */
}