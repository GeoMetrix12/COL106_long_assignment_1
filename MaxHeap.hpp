#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP

#include <vector>
#include <stdexcept>
using namespace std;

template<typename T>
class MaxHeap{
    private:
        vector<T> heap;
        void heapify_up(int idx);
        void heapify_down(int idx);
    public:
        bool empty();
        void insert(const T& value);
        void delete_at(int idx);
        void delete_value(const T& value);
        T top() const;
        void extract_max();
};

#include "MaxHeap.tpp"

#endif