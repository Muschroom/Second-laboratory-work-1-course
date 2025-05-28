#pragma once
#include <stdexcept>

template<typename T>
class Array {
public:
    Array();
    Array(const T* items, int count);
    Array(const Array<T>& array);
    Array(int size);

    int getSize() const;
    T& get(int index) const;

    Array& operator=(const Array<T>& array);
    T& operator[](int index) const;
    void set(int index, const T& value);
    void resize(int new_size);

    ~Array();

private:
    int size;
    T* data;
    
    void checkRange(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("index out of range"); 
        }
    }

    void checkArray() const {
        if (!data && size != 0) {
            throw std::logic_error("array data is corrupted");
        }
    }
};

template<typename T>
Array<T>::Array() : data(nullptr), size(0) {}  

template<typename T>
Array<T>::Array(const T* items, int count) : size(count) {
    if (count < 0) {
        throw std::invalid_argument("count cannot be negative");
    }
    data = new T[count];
    for (int i = 0; i < count; i++) {
        data[i] = items[i];
    }
}

template<typename T>
Array<T>::Array(const Array<T>& array) : size(array.size) {
    data = new T[array.size];
    for (int i = 0; i < size; ++i) {
        data[i] = array.data[i];
    }
}

template<typename T>
Array<T>::Array(int size) : size(size) {
    if (size < 0) {
        throw std::invalid_argument("size cannot be negative");
    }
    data = new T[size]();
}

template<typename T>
int Array<T>::getSize() const {
    checkArray();
    return size;
}

template<typename T>
T& Array<T>::get(int index) const {
    checkArray();
    checkRange(index);
    return data[index];
}

template<typename T>
Array<T>& Array<T>::operator=(const Array<T>& other) {
    if (this == &other) {
        return *this;
    }
    
    T* new_data = new T[other.size];
    for (int i = 0; i < other.size; i++) {
        new_data[i] = other.data[i];
    }
    
    delete[] data;
    data = new_data;
    size = other.size;
    
    return *this;
}

template<typename T>
T& Array<T>::operator[](int index) const {
    checkArray();
    checkRange(index);
    return data[index];
}

template<typename T>
void Array<T>::set(int index, const T& value) {
    checkArray();
    checkRange(index);
    data[index] = value;
}

template<typename T>
void Array<T>::resize(int new_size) {
    if (new_size < 0) {
        throw std::invalid_argument("new_size cannot be negative");
    }
    
    checkArray();
    
    if (new_size == size) {
        return;
    }
    
    T* new_data = new T[new_size]();
    
    if (data) {
        int elements_to_copy = new_size < size ? new_size : size;
        for (int i = 0; i < elements_to_copy; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
    }
    
    data = new_data;
    size = new_size;
}

template<typename T>
Array<T>::~Array() {
    delete[] data;
}