#include <iostream>
#include <vector>

template <typename T>
struct Node {
    T data;
    Node<T>* left = nullptr;
    Node<T>* right = nullptr;

    Node(const T& el) {
        data = el;
    }

    Node(const T& el, Node<T>* l, Node<T>* r) {
        data = el;
        left = l;
        right = r;
    }
};

size_t mystrlen(const char* word) {
    int n = 0;
    while (word[n] != '\0') {
        n++;
    }

    return n;
}

//A)
bool checkIfInside(const Node<char>* tree, const char* word, int n) {
    if (tree == nullptr || tree->data != word[0]) {
        return false;
    }

    if (n == 1) {
        return true;
    } 

    bool found = false;
    if (tree->left != nullptr) {
        found |= checkIfInside(tree->left, word + 1, n - 1);
    }
    if (tree->right != nullptr) {
        found |= checkIfInside(tree->right, word + 1, n - 1);
    }
    return found;
}

const int minSz = 2;
bool readWord(const Node<char>* tree, const char* word) {
    int n = mystrlen(word); //ne pomnq sintaksisa na strlen

    if (n < 2*minSz) {
        throw std::runtime_error("Word is too small.");
    }

    if (tree->left == nullptr || tree->right == nullptr) {
        return false;
    }

    bool found = false;       
    for (int i=minSz; i<=n-minSz; i++) {
        found |= checkIfInside(tree->right, word, i) & checkIfInside(tree->left, word + i, n - i);
    }

    return found;
}

//B)
bool checkIfDescending(const std::vector<const Node<char>*>& currNodes) {
    for (int i=0; i<(int)currNodes.size()-1; i++) {
        if (currNodes[i]->data <= currNodes[i+1]->data) {
            return false;
        }
    }
    return true;
}

void goDeeper(std::vector<const Node<char>*>& currNodes) {
    std::vector<const Node<char>*> newNodes;

    for (auto& node : currNodes) {
        if (node->left != nullptr) {
            newNodes.push_back(node->left);
        }
        if (node->right != nullptr) {
            newNodes.push_back(node->right);
        }
    }

    std::swap(currNodes, newNodes);
}

size_t minLeven(const Node<char>* tree) {
    std::vector<const Node<char>*> currNodes;
    currNodes.push_back(tree);

    int cnt = 1;
    do {
        goDeeper(currNodes);
        cnt++;
    }
    while (!checkIfDescending(currNodes));

    if (currNodes.empty()) {
        //Mowe i da se throw-va error, no ako dadeno nivo e prazno - otgovarq na uslovieto
    }

    return cnt;
}

int main() {
    /*
    //test A)
    //trqbva da se zachisti pametta

    auto C = new Node<char>('C');
    auto E = new Node<char>('E');
    auto D = new Node<char>('D', C, E);
    
    auto A = new Node<char>('A');
    auto B = new Node<char>('B', A, D);

    auto H = new Node<char>('H');
    auto I = new Node<char>('I'); I->left = H;
    auto G = new Node<char>('G'); G->right = I;

    auto F = new Node<char>('F', B, G);

    std::cout<<readWord(F, "GIBDC")<<std::endl;
    std::cout<<readWord(F, "GIBD")<<std::endl;
    std::cout<<readWord(F, "GIBCD")<<std::endl;
    std::cout<<readWord(F, "GHBD")<<std::endl;
    */

    /*
    //test B)
    //trqbva da se zachisti pametta

    auto C = new Node<char>('C');
    auto E = new Node<char>('E');
    auto D = new Node<char>('D', C, E);
    
    auto A = new Node<char>('A');
    auto B = new Node<char>('B', A, D);

    auto H = new Node<char>('H');
    auto I = new Node<char>('I'); I->left = H;
    auto G = new Node<char>('G'); G->right = I;

    auto F = new Node<char>('F', B, G);

    std::cout<<minLeven(F)<<std::endl; //5

    E->data = 'M'; C->data = 'Q';
    std::cout<<minLeven(F)<<std::endl; // 4

    D->data = 'O'; A->data = 'N';
    std::cout<<minLeven(F)<<std::endl; //4

    A->data = 'X';
    std::cout<<minLeven(F)<<std::endl; //3

    B->data = 'Y';
    std::cout<<minLeven(F)<<std::endl; //2
    */
}