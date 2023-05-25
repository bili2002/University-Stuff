#include <iostream>
#include <fstream>
#include <cstring>

template<typename T>
class Vector {
private:
    int capacity;
    int currSize;
    T* arr;

    void expand() {
        int newSize = capacity == 0 ? 1 : capacity*2;

        T* temp = new T[newSize];
        for (int i=0; i<currSize; i++) {
            temp[i] = std::move(arr[i]);
        }
        delete[] arr;
        arr = temp;
        capacity = newSize;
    }

public:
    Vector() {
        capacity = 0;
        currSize = 0;
        arr = nullptr;
    }

    ~Vector() {
        delete[] arr;
    }

    Vector(const Vector& oth) {
        capacity = oth.capacity;
        currSize = oth.currSize;
        arr = new T[currSize];

        for (int i=0; i<currSize; i++) {
            arr[i] = oth.arr[i];
        }
    }


    Vector(Vector&& oth) {
        capacity = oth.capacity;
        currSize = oth.currSize;
        arr = new T[currSize];

        for (int i=0; i<currSize; i++) {
            arr[i] = std::move(oth[i]);
        }
    }

    Vector& operator=(const Vector& oth) {
        if (this == &oth) {
            return *this;
        }

        capacity = oth.capacity;
        currSize = oth.currSize;
        if (this != nullptr) {
            arr = new T[currSize];
        }

        for (int i=0; i<currSize; i++) {
            arr[i] = oth.arr[i];
        }
    }

    bool operator==(const Vector& oth) const {
        if (currSize != oth.currSize) {
            return false;
        }

        for (int i=0; i<currSize; i++) {
            if (arr[i] != oth.arr[i]) {
                return false;
            }
        }
        return true;
    }
    
    void push_back(const T& el) {
        if (currSize == capacity) {
            expand();
        }

        arr[currSize++] = el;
    }

     void push_back(T&& el) {
        if (currSize == capacity) {
            expand();
        }

        arr[currSize++] = std::move(el);
    }

    T& operator[](size_t i) const {
        return arr[i];
    }

    bool empty() const {
        return currSize == 0;
    }

    size_t size() const {
        return currSize;
    }

    void clear() {
        delete[] arr;
    }
};

class String {
private:
    int capacity;
    int currSize;
    char* arr;

    void expand() {
        int newSize = capacity == 0 ? 1 : capacity*2;

        char* temp = new char[newSize];
        for (int i=0; i<currSize; i++) {
            temp[i] = std::move(arr[i]);
        }
        delete[] arr;
        arr = temp;
        capacity = newSize;
    }

public:
    String() {
        capacity = 1;
        currSize = 1;
        arr = new char[1];
        arr[0] = '\0';
    }

    ~String() {
        delete[] arr;
    }

    String(const String& oth) {
        capacity = oth.capacity;
        currSize = oth.currSize;
        arr = new char[currSize];

        for (int i=0; i<currSize; i++) {
            arr[i] = oth.arr[i];
        }
    }

    String& operator=(const String& oth) {
        if (this == &oth) {
            return *this;
        }

        capacity = oth.capacity;
        currSize = oth.currSize;
        if (this != nullptr) {
            arr = new char[currSize];
        }

        for (int i=0; i<currSize; i++) {
            arr[i] = oth.arr[i];
        }
    }

    void push_back(const char& el) {
        if (currSize == capacity) {
            expand();
        }

        arr[currSize-1] = el;
        arr[currSize++] = '\0';
    }

    friend std::ostream& operator<<(std::ostream& os, const String& str) {
        std::cout<<str.arr;
    }

    bool operator==(const String& oth) const {
        if (currSize != oth.currSize) {
            return false;
        }

        for (int i=0; i<currSize; i++) {
            if (arr[i] != oth.arr[i]) {
                return false;
            }
        }
        return true;
    }

    void operator+=(const char& el) {
        push_back(el);
    }
    
    void operator+=(const char* els) {
        for (size_t i=0; i<strlen(els); i++) {
            push_back(els[i]);
        }
    }

    void operator+=(const String& oth) {
        for (size_t i=0; i<oth.size(); i++) {
            push_back(oth.arr[i]);
        }
    }

    char* toChar() const {
        return arr;
    }

    char& operator[](size_t i) const {
        return arr[i];
    }

    bool empty() const {
        return currSize == 1;
    }

    size_t size() const {
        return currSize-1;
    }
    
    void clear() {
        delete[] arr;
    }

    static void getString(String& str, int maxSize) {
        str.clear();

        char temp;
        std::cin.get(temp);
        while (temp == ' ' && temp == '\n') {
            std::cin.get(temp);
        }

        int cnt = 0;
        while (temp != ' ' && temp != '\n') {
            if (cnt < 100) {
                cnt++;
                str += temp;
            }
            std::cin.get(temp);
        }
    }
};

class SimplePair {
protected:
    String key;
    String value;

    bool isLetter(char ch) {
        return ch > 'a' && ch < 'z';
    }

public:

    virtual ~SimplePair() {};
    virtual SimplePair* clone() const {
        SimplePair* temp;
        temp->key = key;
        temp->value = value;

        return temp;
    };

    virtual bool setKey(const String& newKey) {
        if (newKey.empty()) {
            std::cout<<"Error: The string is empty!\n";
            return false;
        } 

        if (!isLetter(newKey[0])) {
            std::cout<<"Error: Wrong name!\n";
            return false;
        }

        for (int i=0; i<newKey.size(); i++) {
            if (!isLetter(newKey[i]) || newKey[i] != '_') {
                std::cout<<"Error: Wrong name!\n";
                return false;
            }
        }

        key = newKey;
        return true;
    }

    virtual bool setValue(const String& newValue) {
        value = newValue;
    }

    const String& getKey() const {
        return key;
    }

    virtual void printValue() {
        std::cout<<value<<std::endl;
    }
};

class InterpolationPair : protected virtual SimplePair {
protected:
    static const int MaxParameterSize = 100;
public:
    bool setValue(const String& newValue) override {
        value.clear();
        Vector<String> parameters;

        for (int i=0; i<newValue.size(); i++) {
            if (newValue[i] == '%') {
                String currParameter;
                if (i+1 < newValue.size() && newValue[i+1] == '{') {
                    i += 2;
                    while (i < newValue.size() && newValue[i] != '}') {
                        currParameter += newValue[i];
                        if (isLetter(newValue[i])) {
                            std::cout<<"Error: Wrong parameter!\n";
                            return false;
                        }
                        i++;
                    }

                    for (int i=0; i<parameters.size(); i++) {
                        if (parameters[i] == currParameter) {
                            std::cout<<"Error: Parameters with dublicate names!\n";
                            return false;
                        }
                    }
                    parameters.push_back(currParameter);
                }
            }
        }

        value = newValue;
        return true;
    }
    
    String interpoliate() { 
        Vector<String> parameters;
        for (int i=0; i<value.size(); i++) {
            if (value[i] == '%') {
                String currParameter;
                i += 2;
                while (i < value.size() && value[i] != '}') {
                    currParameter += value[i];
                    i++;
                }
                parameters.push_back(currParameter);
            }
        }

        String interValue;

        Vector<String> parameterValues;
        for (int i=0; i<parameters.size(); i++) {
            std::cout<<"Enter value for "<<parameters[i]<<": ";

            String currValue;
            String::getString(currValue, MaxParameterSize);
            parameterValues.push_back(currValue);
        }

        int currPar = 0;

        int currParamater = 0;
        for (int i=0; i<value.size(); i++) {
            interValue += value[i];
            if (value[i] == '%') {
                interValue += parameterValues[currParamater++];
                while (value[i++] != '}');
            }
        }
    }

    void printValue() override {
        std::cout<<interpoliate()<<std::endl;
    }
};

class TagPair : protected virtual SimplePair {
public:
    bool setKey(const String& newKey) override {
        if (!SimplePair::setKey(newKey)) {
            return false;
        }

        key.clear();

        if (newKey.size() < 5) { //_html
            std::cout<<"Error: Wrong Key!\n";
            return false;
        }

        int sz = newKey.size();
        String temp;
        temp += "_html";

        for (int i=0; i<5; i++) {
            if (newKey[sz + i - 6] != temp[i]) {
                std::cout<<"Error: Wrong Key!\n";
                return false;
            }
        }

        key = newKey;
        return true;
    }

    bool setValue(const String& newValue) override {
        value.clear();

        if (newValue.size() < 2) { //""
            std::cout<<"Error: Wrong value!\n";
            return false;
        }

        if (newValue[0] != '\"' && newValue[newValue.size()-1] != '\"') {
            std::cout<<"Error: Wrong value!\n";
            return false;
        }

        value = newValue;
        return true;
    }

    static String changeTags(const String& value) {
        String newValue;

        int currCaps = 0;
        for (int i=0; i<value.size(); i++) {
            if (value[i] == '<') {
                i++;
                if (i<value.size() && value[i] == 'b') {
                    i++;
                    if (i<value.size() && value[i] == 'r') { //<br/>
                        newValue += '\n';
                        i += 2;
                    } else { //<b>
                        currCaps++;
                    }
                } else if (i<value.size() && value[i] == 'h'){ //<hr/>
                    newValue += "\n--------------------\n";
                    i += 3;
                } else if (i<value.size() && value[i] == '/') { //<b/>
                    currCaps--;
                    i += 2;
                }
                continue;
            } 

            if (value[i] >= 'a' || value[i] <= 'z') {
                if (currCaps > 0) { 
                    newValue += value[i] - 'a' + 'A';
                } else {
                    newValue += value[i];
                }
            } else {
                newValue +=value[i];
            }
        }
    }

    void printValue() override {
        std::cout<<changeTags(value)<<std::endl;
    }
};

class ComplexPair : protected InterpolationPair, protected TagPair {
    virtual bool setKey(const String& newKey) override {
        key = newKey;
    }

    bool setValue(const String& newValue) override {
        value = newValue;
    }

    void printValue() override {
        std::cout<<changeTags(interpoliate())<<std::endl;
    }
};

class KNYAML {
    Vector<SimplePair*> collection;

    ~KNYAML() {
        for (int i=0; i<collection.size(); i++) {
            if (collection[i] != nullptr) {
                delete collection[i];
            }
        }
    }

    KNYAML(const KNYAML& oth) {
        for (int i=0; i<oth.collection.size(); i++) {
            collection.push_back(oth.collection[i]->clone());
        }
    }

    KNYAML& operator=(const KNYAML& oth) {
        this->~KNYAML();

        for (int i=0; i<oth.collection.size(); i++) {
            collection.push_back(oth.collection[i]->clone());
        }
    }

    void addElement(const SimplePair* el) {
        for (int i=0; i<collection.size(); i++) {
            if (collection[i]->getKey() == el->getKey()) {
                std::cout<<"ERROR: can't add!"<<std::endl;
                return;
            }
        }
        collection.push_back(el->clone());
    }

    void operator[](String key) {
        for (int i=0; i<collection.size(); i++) {
            if (key == collection[i]->getKey()) {
                collection[i]->printValue(); 
            }
        }
    }
};

int main() {

    return 0;
}