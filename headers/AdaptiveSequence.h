#pragma once
#include "Array.h"
#include "Sequence.h"
using namespace std;


template <typename T>
class AdaptiveSequence : public Sequence<T> {
private:
    Array<T> *data;
    int start;  
    int end;   
    int size; 

    void allocate(){
        int capacity = (data->getSize() == 0 ? 1 : data->getSize()) * 2;
        int newStart = (capacity - size) / 2;
        Array<T>* newdata = new Array<T>(capacity);
        for(int i = 0; i < size; i++){
            newdata->set(newStart + i, this->Get(i));
        }
        delete data;
        data = newdata;
        start = newStart - 1;
        end = newStart + size;
    }
    
    void checkEnd() const{
        if(end >= data->getSize()){
            allocate();
        }
    }

    void checkStart() const{
        if(start <= 0){
            allocate();
        }
    }
public:
    AdaptiveSequence() : data(new Array<T>(4)), start(1), end(2), size(0) {}

    AdaptiveSequence(const T* items, int count) : data(new Array<T>(items,count)), start(-1), end(count), size(count) {}

    AdaptiveSequence(const AdaptiveSequence<T>& other) 
        : data(other.data), start(other.start), end(other.end), size(other.size) {}

    T& Get(int index) const override {
        return data->get(start + index + 1);
    }

    T& GetFirst() const override {
        return data->get(start + 1);
    }

    T& GetLast() const override {
        return data->get(end - 1);
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        AdaptiveSequence<T>* subseq = new AdaptiveSequence<T>();
        for (int i = startIndex; i < endIndex; i++) {
            subseq->Append(this->Get(i));
        }
        return subseq;
    }

    int GetLength() const override {
        return size;
    }

    Sequence<T>* Append( const T& value) override {
       checkEnd();
       data->set(end,value);
       end++;
       size++;
       return this;
    }

    Sequence<T>* Prepend(const T& value) override {
       checkStart();
       data->set(start,value);
       start--;
       size++;
       return this;
    }

    Sequence<T>* InsertAt( const T& item, int index) override {
        checkEnd();
        for (int i = end; i > start + 1 + index; i--) {
            data->set(i, data->get(i - 1));
        }
        data->set(start + 1 + index, item);
        end++;
        size++;
        return this;
    }
    
    Sequence<T>* Concat(const Sequence<T>* other) override {
        for (int i = 0; i < other->GetLength(); i++) {
            this->Append(other->Get(i));
        }
        return this;
    }

    Sequence<T>* Map(const std::function<T(const T&)>& func) const override {
        AdaptiveSequence<T>* result = new AdaptiveSequence<T>();
        for (int i = 0; i < size; i++) {
            result->Append(func(this->Get(i)));
        }
        return result;
    }

    T Reduce(const std::function<T(const T&, const T&)>& func, const T& value) const override {
        T result = value;
        for (int i = 0; i < size; i++) {
            result = func(result, this->Get(i));
        }
        return result;
    }

    Sequence<T>* Where(const std::function<bool(const T&)>& func) const override {
        AdaptiveSequence<T>* result = new AdaptiveSequence<T>();
        for (int i = 0; i < size; i++) {
            T item = this->Get(i);
            if (func(item)) {
                result->Append(item);
            }
        }
        return result;
    }

    T& operator[](int index) const override {
        return Get(index);
    }

    ~AdaptiveSequence() override{
        delete data;
    }
};