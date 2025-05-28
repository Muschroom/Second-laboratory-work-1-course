#pragma once
#include "LinkedList.h"
#include "Sequence.h"

template <typename T> class MutableListSequence;
template <typename T> class ImmutableListSequence;

template<typename T>
class ListSequence: public Sequence<T>{
public:
    template<typename... Ts>
    static ListSequence<std::tuple<Ts...>>* ZipN(Sequence<Ts>*... sequences) {
        auto* result = new MutableListSequence<std::tuple<Ts...>>();
        
        std::initializer_list<int> lengths = {sequences->GetLength()...};
        int Length = 1e9;
        for(auto iter = lengths.begin(); iter != lengths.end(); iter++){
            if(Length > *iter){
                Length = *iter;
            }
        } 
        for (int i = 0; i < Length; i++) {
            result->Append(std::make_tuple(sequences->Get(i)...));
        }
        
        return result;
    }

    class Iterator {
    private:
        LinkedList<T>* data;
        int position;
    public:
        Iterator(LinkedList<T>* data, int size)
            : data(data)
            , position(size) {}

        T operator*() const {
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
        return Iterator(list, 0);
    }

    Iterator end() {
        return Iterator(list, list->getSize());
    }

    ListSequence();
    ListSequence(T* items, int count);
    ListSequence(const ListSequence<T>& other);
    
    const T& GetFirst() const override;
    const T& GetLast() const override;
    const T& Get(int index) const override;

    T& GetFirst()  override;
    T& GetLast()  override;
    T& Get(int index)  override;
    ListSequence<T>* GetSubsequence(int start, int end) const override;
    int GetLength() const override;

    ListSequence<T>* Append(const T& item) override;
    ListSequence<T>* Prepend(const T& item) override;
    ListSequence<T>* InsertAt(const T& item, int index) override;
    Sequence<T>* Concat(const Sequence<T>* list) override;
    ListSequence& operator=(const ListSequence<T>& other);

    T& operator[](int index) override;

    Sequence<T>* Map(const std::function<T(const T&)>& func) const override;
    T Reduce(const std::function<T(const T&, const T&)>& func, const T& value) const override;
    Sequence<T>* Where(const std::function<bool(const T&)>& func) const override;

    virtual ListSequence<T>* Instance() = 0;
    ~ListSequence() override;

    ListSequence(ListSequence&& other) noexcept : list(other.list) {
        other.list = nullptr; 
    }

    ListSequence& operator=(ListSequence&& other) noexcept {
        if (this != &other) {
            delete list; 
            list = other.list;
            other.list = nullptr;
        }
        return *this;
    }

private:
    LinkedList<T> *list;

    ListSequence<T>* AppendInterval(const T& item) {
        list->append(item);
        return this;
    }

    ListSequence<T>* PrependInterval(const T& item) {
        list->prepend(item);
        return this;
    }

    ListSequence<T>* InsertIntervalAt(const T& item, int index) {
        list->insertAt(item, index);
        return this;
    }

    Sequence<T>* ConcatInterval(const Sequence<T>* other) {
        for(int i = 0; i < other->GetLength(); i++) {
            this->AppendInterval(other->Get(i));
        }
        return this;
    }
};

template<typename T>
class MutableListSequence: public ListSequence<T> {
public:
    MutableListSequence(): ListSequence<T>() {}
    MutableListSequence(T* items, int count): ListSequence<T>(items, count) {}
    MutableListSequence(const MutableListSequence<T>& other): ListSequence<T>(other) {}
    MutableListSequence(MutableListSequence<T>&& other) noexcept : ListSequence<T>(std::move(other)) {}

    ListSequence<T>* Instance() override {
        return this;
    }
};

template<typename T>
class ImmutableListSequence: public ListSequence<T> {
public:
    ImmutableListSequence(): ListSequence<T>() {}
    ImmutableListSequence(T* items, int count): ListSequence<T>(items, count) {}
    ImmutableListSequence(const ImmutableListSequence<T>& other): ListSequence<T>(other) {}
    ImmutableListSequence(ImmutableListSequence<T>&& other) noexcept : ListSequence<T>(std::move(other)) {}

    ListSequence<T>* Instance() override {
        return new ImmutableListSequence<T>(*this);
    }
};

template<typename T>
ListSequence<T>::ListSequence(): list(new LinkedList<T>()) {}

template<typename T>
ListSequence<T>::ListSequence(T* items, int count): list(new LinkedList<T>(items, count)) {}

template<typename T>
ListSequence<T>::ListSequence(const ListSequence<T>& other): list(new LinkedList<T>(*other.list)) {}

template<typename T>
const T& ListSequence<T>::GetFirst() const {
    return this->list->getFirst();
}

template<typename T>
const T& ListSequence<T>::GetLast() const {
    return this->list->getLast();
}

template<typename T>
const T& ListSequence<T>::Get(int index) const {
    return this->list->get(index);
}

template<typename T>
T& ListSequence<T>::GetFirst() {
    return this->list->getFirst();
}

template<typename T>
T& ListSequence<T>::GetLast() {
    return this->list->getLast();
}

template<typename T>
T& ListSequence<T>::Get(int index) {
    return this->list->get(index);
}

template<typename T>
ListSequence<T>* ListSequence<T>::GetSubsequence(int start, int end) const {
    ListSequence<T>* new_list = new MutableListSequence<T>();
    for(int i = start; i < end + 1; i++) {
        new_list->Append(list->get(i));
    }
    return new_list;
}

template<typename T>
int ListSequence<T>::GetLength() const {
    return this->list->getSize();
}

template<typename T>
ListSequence<T>* ListSequence<T>::Append(const T& item) {
    return Instance()->AppendInterval(item);
}

template<typename T>
ListSequence<T>* ListSequence<T>::Prepend(const T& item) {
    return Instance()->PrependInterval(item);
}

template<typename T>
ListSequence<T>* ListSequence<T>::InsertAt(const T& item, int index) {
    return Instance()->InsertIntervalAt(item, index);
}

template<typename T>
ListSequence<T>& ListSequence<T>::operator=(const ListSequence<T>& other) {
    if (this != &other) {
        delete list;
        list = new LinkedList<T>(*other.list);
    }
    return *this;
}

template<typename T>
Sequence<T>* ListSequence<T>::Concat(const Sequence<T>* other) {
    return Instance()->ConcatInterval(other);
}

template<typename T>
T& ListSequence<T>::operator[](int index) {
    return (*list)[index];
}

template<typename T>
Sequence<T>* ListSequence<T>::Map(const std::function<T(const T&)>& func) const {
    Sequence<T>* new_array = new MutableListSequence<T>();
    for(int i = 0; i < this->GetLength(); i++) {
        new_array->Append(func(this->Get(i)));
    }
    return new_array;  
}

template<typename T>
T ListSequence<T>::Reduce(const std::function<T(const T&, const T&)>& func, const T& value) const {
    T result = func(this->GetFirst(), value);
    for(int i = 1; i < this->GetLength(); i++) {
        result = func(this->Get(i), result);
    }
    return result;
}

template<typename T>
Sequence<T>* ListSequence<T>::Where(const std::function<bool(const T&)>& func) const {
    Sequence<T>* new_array = new MutableListSequence<T>();
    for(int i = 0; i < this->GetLength(); i++) {
        if (func(this->Get(i))) {
            new_array->Append(this->Get(i));
        }
    }
    return new_array;  
}

template<typename T>
ListSequence<T>::~ListSequence() {
    delete list;
}