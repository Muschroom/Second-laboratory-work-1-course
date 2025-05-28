#pragma once
#include <stdexcept>

template<typename T>
class LinkedList {
public:
    LinkedList() = default;
    LinkedList(const T* items, int count);
    LinkedList(const LinkedList& other);

    T& getFirst() const;
    T& getLast() const;
    T& get(int index) const;
    int getSize() const;
    LinkedList<T>* getSub(int start, int end);

    LinkedList& operator=(const LinkedList<T>& array);
    T operator[](int index);
    void prepend(const T& value);
    void append(const T& value);
    LinkedList<T>* concat(const LinkedList<int>* list);
    void insertAt(const T& value, int index);

    ~LinkedList();

    
private:
    struct Node {
        Node* next;
        T value;

        Node(const T& value) : value(value), next(nullptr) {}
    };

    Node* head = nullptr;
    int size = 0;
    
    void checkRange(int index) const{
        if(index < 0 || index >= size){
            throw std::out_of_range("index out of range"); 
        }
    }

    void checkList(const LinkedList<T>* other) const{
        if(!other){
            throw std::logic_error("list does not exist");
        }
    }
};


template<typename T>
LinkedList<T>::LinkedList(const T* items, int count) {
    this->size = count;
    for (int i = 0; i < count; i++) {
        this->append(items[i]);
    }
}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList& other) {
    this->size = other.size;
    for (int i = 0; i < other.size; i++) {
        this->append(other.get(i));
    }
}

template<typename T>
T& LinkedList<T>::getFirst() const {
    checkList(this);
    if (!head) {
        throw std::out_of_range("list is empty");
    }
    return head->value;
}

template<typename T>
T& LinkedList<T>::getLast() const {
    checkList(this);
    if (!head) {
        throw std::out_of_range("list is empty");
    }
    Node* next = head;
    while (next->next != nullptr) {
        next = next->next;
    }
    return next->value;
}

template<typename T>
T& LinkedList<T>::get(int index) const {
    checkList(this);
    checkRange(index);

    Node* node = head;
    for (int i = 0; i < index; i++) {
        if (!node) {
            throw std::out_of_range("list is empty");
        }
        node = node->next;
    }
    return node->value;
}

template<typename T>
int LinkedList<T>::getSize() const {
    checkList(this);
    return size;
}

template<typename T>
LinkedList<T>* LinkedList<T>::getSub(int start, int end) {
    checkList(this);
    checkRange(start);
    checkRange(end);

    LinkedList<T>* newList = new LinkedList<T>();
    Node* now = head;
    for (int i = 0; i < size; i++) {
        if (i >= start && i <= end) {
            newList->append(now->value);
        }
        now = now->next;
    }
    return newList;
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other){
    checkList(this);
    if (this == &other) {
        return *this;
    }

    Node* next = nullptr;
    Node* now = this->head;

    while (now != nullptr) {
        next = now->next;
        delete now;
        now = next;
    }

    this->size = other.size;
    for (int i = 0; i < other.size; i++) {
        this->append(other.get(i));
    }
    return *this;
}

template<typename T>
T LinkedList<T>::operator[](int index){
    checkList(this);
    checkRange(index);
    if(index == 0){
        return getFirst();
    }
    if(index == size - 1){
        return getLast();
    }
    return get(index);
}

template<typename T>
void LinkedList<T>::prepend(const T& value) {
    checkList(this);
    if (head == nullptr) {
        head = new Node(value);
        head->next = nullptr;
        size = 1;
        return;
    }

    Node* node = new Node(value);
    node->next = head;
    head = node;
    size++;
}

template<typename T>
void LinkedList<T>::append(const T& value) {
    checkList(this);
    if (head == nullptr) {
        head = new Node(value);
        head->next = nullptr;
        size = 1;
        return;
    }

    Node* node = head;
    for (int i = 1; i < size; i++) {
        node = node->next;
    }
    node->next = new Node(value);
    node->next->next = nullptr;
    size++;
}


template<typename T>
LinkedList<T>* LinkedList<T>::concat(const LinkedList<int>* list) {
    checkList(this);
    LinkedList<T>* newList = new LinkedList<T>();

    Node* now = head;
    while (now != nullptr) {
        newList->append(now->value);
        now = now->next;
    }

    now = list->head;
    while (now != nullptr) {
        newList->append(now->value);
        now = now->next;
    }
    return newList;
}

template<typename T>
void LinkedList<T>::insertAt(const T& value, int index) {
    checkList(this);
    checkRange(index);
    
    Node* now = head;
    for (int i = 0; i < size; i++) {
        if (i == index) {
            Node* node = new Node(value);
            node->next = now->next;
            now->next = node;
        }
    }
    size++;
}

template<typename T>
LinkedList<T>::~LinkedList() {
    
    Node* next = nullptr;
    Node* now = this->head;

    while (now != nullptr) {
        next = now->next;
        delete now;
        now = next;
    }
}



