#pragma once
#include <functional>
#include <iostream>
#include <tuple>

template <typename T>
class Sequence {
public:

    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast() const = 0;
    virtual const T& Get(int index) const = 0;

    virtual T& Get(int index) = 0;
    virtual T& GetFirst() = 0;
    virtual T& GetLast() = 0;
    virtual Sequence<T>* GetSubsequence(int start, int end) const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T>* Append(const T& value) = 0;
    virtual Sequence<T>* Prepend(const T& value) = 0;
    virtual Sequence<T>* InsertAt(const T& item, int index) = 0;
    virtual Sequence<T>* Concat(const Sequence<T>* other) = 0;

    virtual Sequence<T>* Map(const std::function<T(const T&)>& func) const = 0;
    virtual T Reduce(const std::function<T(const T&, const T&)>& func, const T& value) const = 0;
    virtual Sequence<T>* Where(const std::function<bool(const T&)>& func) const = 0;

    virtual T& operator[](int idnex)  = 0;
    
    virtual ~Sequence() {};
    class Iterator {
        private:
            Sequence<T>* data;
            int position;
        public:
            Iterator(Sequence<T>* seq, int ind) : data(seq), position(ind) {}
    
            T operator*() const {
                return data->Get(position);
            }
    
            Iterator& operator++() {
                ++position;
                return *this;
            }
    
            bool operator!=(const Iterator& other) const {
                return position != other.position;
            }
        };
    
        Iterator begin() {
            return Iterator(this, 0);
        }
    
        Iterator end() {
            return Iterator(this, this->GetLength());
        }
};
