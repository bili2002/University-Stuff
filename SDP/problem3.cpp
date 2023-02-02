#include <stack>
#include <string>
#include <iostream>

bool readAndDelete(std::stack<char>& st, const std::string& word) {
    std::stack<char> savedSymb;

    size_t i = 0;
    while (!st.empty() && i < word.size()) {
        if (word[i] == st.top()) {
            st.pop();
            i++;
        } else {
            savedSymb.push(st.top());
            st.pop();
        }
    }

    while (!savedSymb.empty()) {
        st.push(savedSymb.top());
        savedSymb.pop();
    }

    return i == word.size();
}

int main() {
    /*
    std::stack<char> st;
    st.push('m');st.push('o');st.push('o');st.push('f');st.push('m');
    std::cout<<readAndDelete(st, "mom")<<" - ";
    while (!st.empty()) {
        std::cout<<st.top()<<' ';
        st.pop();
    }std::cout<<std::endl;


    st.push('y');st.push('a');st.push('d');st.push('c');st.push('c');
    std::cout<<readAndDelete(st, "car")<<" - ";
    while (!st.empty()) {
        std::cout<<st.top()<<' ';
        st.pop();
    }std::cout<<std::endl;
    */
}