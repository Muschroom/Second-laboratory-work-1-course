#pragma once
#include <functional>
#include <stdexcept>
#include <iostream>
#include "LinkedList.h"
#include "Sequence.h"
using namespace std;

template <typename T>
class SegmentedList : public Sequence<T> {
private:
    int size = 0;
    int segmentSize = 0;

    struct Node {
        T value;
        Node* next = nullptr;
        Node* skip = nullptr;
    };

    Node* head = nullptr;
    Node* tail = nullptr;

    Node* getNode(int ind) const {
        checkIndex(ind);
        int now = 0;
        Node* now_node = tail;

        while (now_node && now + segmentSize <= ind) {
            now += segmentSize;
            now_node = now_node->skip;
        }
        
        while (now_node && now < ind) {
            now_node = now_node->next;
            now++;
        }

        return now_node;
    }

    void buildSkips() {
        if (size <= segmentSize) return;

        Node* now = tail;
        while (now) {
            Node* skip = now;
            for (int i = 0; i < segmentSize && skip; i++) {
                skip = skip->next;
            }
            now->skip = skip;
            now = (skip) ? skip->next : nullptr;
        }
    }

    int getSegmentSize() const { return segmentSize; }

    void checkIndex(int index) const {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range");
        }
    }

    void checkEmpty() const {
        if (size == 0) {
            throw logic_error("List is empty");
        }
    }

    void checkSubsequenceRange(int start, int end) const {
        if (start < 0 || end >= size || start > end) {
            throw out_of_range("Invalid subsequence range");
        }
    }

public:
    SegmentedList(int segmentSize) : segmentSize(segmentSize) {
        if (segmentSize <= 0) {
            throw invalid_argument("Segment size must be positive");
        }
    }

    SegmentedList(T* items, int count, int segmentSize) : segmentSize(segmentSize) {
        if (segmentSize <= 0) {
            throw invalid_argument("Segment size must be positive");
        }
        for (int i = 0; i < count; i++) {
            this->Append(items[i]);
        }
    }

    SegmentedList(const SegmentedList<T>& other) {
        this->segmentSize = other.getSegmentSize();
        for (int i = 0; i < other.size; i++) {
            this->Append(other.Get(i));
        }
    }


    const T& GetFirst() const {
        checkEmpty();
        return tail->value;
    }

    const T& GetLast() const {
        checkEmpty();
        return head->value;
    }

    const T& Get(int index) const {
        checkIndex(index);
        return getNode(index)->value;
    }

    T& Get(int index) override {
        checkIndex(index);
        return getNode(index)->value;
    }

    T& GetFirst() override {
        checkEmpty();
        return tail->value;
    }


    T& GetLast() override {
        checkEmpty();
        return head->value;
    }

    Sequence<T>* GetSubsequence(int start, int end) const override {
        checkSubsequenceRange(start, end);
        Sequence<T>* new_seq = new SegmentedList<T>(segmentSize);
        Node* now = getNode(start);
        for (int i = start; i <= end; i++) {
            new_seq->Append(now->value);
            now = now->next;
        }
        return new_seq;
    }

    int GetLength() const override { return size; }

    Sequence<T>* Append(const T& value) override {
        Node* new_node = new Node();
        new_node->value = value;
        
        if (!head) {
            head = new_node;
            tail = new_node;
        } else {
            head->next = new_node;
            head = new_node;
        }

        size++;
        buildSkips();
        return this;
    }

    Sequence<T>* Prepend(const T& value) override {
        Node* new_node = new Node();
        new_node->value = value;
        new_node->next = tail;
        
        if (!tail) {
            head = new_node;
            tail = new_node;
        } else {
            tail = new_node;
        }

        size++;
        buildSkips();
        return this;
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        checkIndex(index);

        if (index == 0) return Prepend(item);
        if (index == size) return Append(item);

        Node* new_node = new Node();
        new_node->value = item;
        Node* prev = getNode(index - 1);
        new_node->next = prev->next;
        prev->next = new_node;

        size++;
        buildSkips();
        return this;
    }

    Sequence<T>* Concat(const Sequence<T>* other) override {
        if (!other) {
            throw invalid_argument("Other sequence cannot be null");
        }
        
        SegmentedList<T>* new_seq = new SegmentedList<T>(this->segmentSize);
        for (int i = 0; i < this->GetLength(); i++) {
            new_seq->Append(this->Get(i));
        }
        for (int i = 0; i < other->GetLength(); i++) {
            new_seq->Append(other->Get(i));
        }
        return new_seq;
    }

    Sequence<T>* Map(const std::function<T(const T&)>& func) const override {
        SegmentedList<T>* new_list = new SegmentedList<T>(segmentSize);
        Node* current = tail;
        while (current) {
            new_list->Append(func(current->value));
            current = current->next;
        }
        return new_list;
    }

    T Reduce(const std::function<T(const T&, const T&)>& func, const T& value) const override {
        checkEmpty();
        T result = value;
        Node* current = tail;
        while (current) {
            result = func(result, current->value);
            current = current->next;
        }
        return result;
    }

    Sequence<T>* Where(const std::function<bool(const T&)>& func) const override {
        SegmentedList<T>* new_list = new SegmentedList<T>(segmentSize);
        Node* current = tail;
        while (current) {
            if (func(current->value)) {
                new_list->Append(current->value);
            }
            current = current->next;
        }
        return new_list;
    }

    T& operator[](int index) override {
        return this->Get(index);
    }

    ~SegmentedList() {
        Node* current = tail;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
};