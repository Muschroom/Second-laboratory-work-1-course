#pragma once
#include "Array.h"
#include "Sequence.h"

template<typename T>
class ArraySequence : public Sequence<T> {
private:
    Array<T> *array;
public:
    class Iterator {
    private:
        Array<T>* data;
        int position;
    public:
        Iterator(Array<T>* data, int size)
            : data(data)
            , position(size) {}

        T& operator*() const {
            return (*data)[position];
        }

        Iterator& operator++() {
            position++;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return position != other.position;
        }
    };

    Iterator begin() {
        return Iterator(array, 0);
    }

    Iterator end() {
        return Iterator(array, array->getSize());
    }

    ArraySequence();
    ArraySequence(const T* items, int count);
    ArraySequence(const ArraySequence<T>& other);
    
    const T& GetFirst() const override;
    const T& GetLast() const override;
    const T& Get(int index) const override;

    T& GetFirst() override;
    T& GetLast() override;
    T& Get(int index) override;
    ArraySequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
    int GetLength() const override;

    ArraySequence<T>* Append(const T& value) override;
    ArraySequence<T>* Prepend(const T& value) override;
    ArraySequence<T>* InsertAt(const T& item, int index) override;
    Sequence<T>* Concat(const Sequence<T>* other) override;

    ArraySequence& operator=(const ArraySequence<T>& other);
    T& operator[](int index) override;

    Sequence<T>* Map(const std::function<T(const T&)>& func) const override;
    T Reduce(const std::function<T(const T&, const T&)>& func, const T& value) const override;
    Sequence<T>* Where(const std::function<bool(const T&)>& func) const override;
    Sequence<Sequence<T>*>* Slice(const std::function<bool(const T&)>& func);

    virtual ArraySequence<T>* Instance() = 0;
    ~ArraySequence() override;

    ArraySequence(ArraySequence&& other) noexcept: array(other.array) {
        other.array = nullptr; 
    }

    ArraySequence& operator=(ArraySequence&& other) noexcept {
        if (this != &other) {
            delete array; 
            array = other.array;
            other.array = nullptr;
        }
        return *this;
    }

private:
    void checkRange(int index) const {
        if(index < 0 || index >= this->GetLength()){
            throw std::out_of_range("index out of range"); 
        }
    }

    ArraySequence<T>* AppendInterval(const T& value) {
        this->array->resize(array->getSize() + 1);
        this->array->set(array->getSize() - 1, value);
        return this;
    }

    ArraySequence<T>* PrependInterval(const T& value) {
        this->array->resize(this->array->getSize() + 1);
        for (int i = this->array->getSize() - 1; i > 0; --i) {
            this->array->set(i, this->array->get(i - 1));
        }
        this->array->set(0, value);
        return this;
    }

    ArraySequence<T>* InsertIntervalAt(const T& item, int index) {
        checkRange(index);
        this->array->resize(this->array->getSize() + 1);
        for(int i = this->array->getSize() - 1; i > index; i--){
            this->array->set(i, array->get(i-1));
        }
        this->array->set(index, item);
        return this;
    }
    
    Sequence<T>* ConcatInterval(const Sequence<T>* other) {
        for(int i = 0; i < other->GetLength(); i++){
            this->AppendInterval(other->Get(i));
        }
        return this;
    }
};

template <typename T>
class MutableArraySequence: public ArraySequence<T> {
public:
    MutableArraySequence(): ArraySequence<T>() {}
    MutableArraySequence(const T* items, int count): ArraySequence<T>(items, count) {}
    MutableArraySequence(const MutableArraySequence<T>& other): ArraySequence<T>(other) {}
    MutableArraySequence(MutableArraySequence<T>&& other) noexcept : ArraySequence<T>(std::move(other)) {}

    ArraySequence<T>* Instance() override {
        return this;
    }
};

template <typename T>
class ImmutableArraySequence: public ArraySequence<T> {
public:
    ImmutableArraySequence(): ArraySequence<T>() {}
    ImmutableArraySequence(const T* items, int count): ArraySequence<T>(items, count) {}
    ImmutableArraySequence(const ImmutableArraySequence<T>& other): ArraySequence<T>(other) {}
    ImmutableArraySequence(ImmutableArraySequence<T>&& other) noexcept : ArraySequence<T>(std::move(other)) {}

    ArraySequence<T>* Instance() override {
        return new ImmutableArraySequence<T>(*this);
    }
};

template<typename T>
ArraySequence<T>::ArraySequence(): array(new Array<T>()) {}

template<typename T>
ArraySequence<T>::ArraySequence(const T* items, int count): array(new Array<T>(items, count)) {}

template<typename T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& other): array(new Array<T>(*other.array)) {}

template<typename T>
const T& ArraySequence<T>::GetFirst() const {
    return this->array->get(0);
}

template<typename T>
const T& ArraySequence<T>::GetLast() const {
    return this->array->get(this->array->getSize() - 1);
}

template<typename T>
const T& ArraySequence<T>::Get(int index) const {
    return this->array->get(index);
}

template<typename T>
T& ArraySequence<T>::GetFirst() {
    return this->array->get(0);
}

template<typename T>
T& ArraySequence<T>::GetLast() {
    return this->array->get(this->array->getSize() - 1);
}

template<typename T>
T& ArraySequence<T>::Get(int index) {
    return this->array->get(index);
}

template<typename T>
ArraySequence<T>* ArraySequence<T>::GetSubsequence(int start, int end) const {
    checkRange(start);
    checkRange(end);
    ArraySequence<T>* new_array = new MutableArraySequence<T>();
    new_array->array->resize(end - start + 1);
    for(int i = start; i < end + 1; i++) {
        new_array->array->set(i - start, array->get(i));
    }
    return new_array;  
}

template<typename T>
int ArraySequence<T>::GetLength() const {
    return this->array->getSize();
}

template<typename T>
ArraySequence<T>* ArraySequence<T>::Append(const T& value) {
    return Instance()->AppendInterval(value);
}

template<typename T>
ArraySequence<T>* ArraySequence<T>::Prepend(const T& value) {
    return Instance()->PrependInterval(value);
}

template<typename T>
ArraySequence<T>* ArraySequence<T>::InsertAt(const T& item, int index) {
    return Instance()->InsertIntervalAt(item, index);
}

template<typename T>
Sequence<T>* ArraySequence<T>::Concat(const Sequence<T>* other) {
    return Instance()->ConcatInterval(other);
}

template<typename T>
ArraySequence<T>& ArraySequence<T>::operator=(const ArraySequence<T>& other) {
    if (this != &other) {
        delete array;
        array = new Array<T>(*other.array);
    }
    return *this;
}

template<typename T>
T& ArraySequence<T>::operator[](int index) {
    return (*array)[index];
}

template<typename T>
Sequence<T>* ArraySequence<T>::Map(const std::function<T(const T&)>& func) const {
    Sequence<T>* new_array = new MutableArraySequence<T>();
    for(int i = 0; i < this->GetLength(); i++) {
        new_array->Append(func(this->Get(i)));
    }
    return new_array;  
}

template<typename T>
T ArraySequence<T>::Reduce(const std::function<T(const T&, const T&)>& func, const T& value) const {
    T result = func(this->GetFirst(), value);
    for(int i = 1; i < this->GetLength(); i++) {
        result = func(this->Get(i), result);
    }
    return result;
}

template<typename T>
Sequence<T>* ArraySequence<T>::Where(const std::function<bool(const T&)>& func) const {
    Sequence<T>* new_array = new MutableArraySequence<T>();
    for(int i = 0; i < this->GetLength(); i++) {
        if (func(this->Get(i))) {
            new_array->Append(this->Get(i));
        }
    }
    return new_array;  
}

template<typename T>
Sequence<Sequence<T>*>* ArraySequence<T>::Slice(const std::function<bool(const T&)>& func) {
    Sequence<Sequence<T>*>* new_seq = new MutableArraySequence<Sequence<T>*>();
    Sequence<T>* buff = new MutableArraySequence<T>();
    for (int index = 0; index < this->GetLength(); index++) {
        if(func(this->Get(index))) {
            new_seq->Append(buff);
            buff = new MutableArraySequence<T>();
        }
        else {
            buff->Append(this->Get(index));
        }
    }
    
    if(buff->GetLength() > 0) {
        new_seq->Append(buff);
    }
    else {
        delete buff;
    }

    return new_seq;
} 

template<typename T>
ArraySequence<T>::~ArraySequence() {
    delete array;
}

template <typename T1, typename T2>
Sequence<std::pair<T1, T2>>* Zip(const Sequence<T1>* left, const Sequence<T2>* right) {
    int length = std::min(left->GetLength(), right->GetLength());
    auto* new_array = new MutableArraySequence<std::pair<T1, T2>>();

    for (int i = 0; i < length; i++) {
        new_array->Append(std::make_pair(left->Get(i), right->Get(i)));
    }
    return new_array;
}

template <typename T1, typename T2>
std::pair<Sequence<T1>*, Sequence<T2>*> Unzip(const Sequence<std::pair<T1, T2>>* zipped) {
    auto* left = new MutableArraySequence<T1>();
    auto* right = new MutableArraySequence<T2>();

    for (int i = 0; i < zipped->GetLength(); i++) {
        auto pair = zipped->Get(i);
        left->Append(pair.first);
        right->Append(pair.second);
    }
    return std::make_pair(left, right);
}