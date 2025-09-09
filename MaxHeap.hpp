#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP

#include <vector>
#include <stdexcept>
#include "HashMap_File.hpp"
using namespace std;

template<typename T>
class MaxHeap{
    private:
        vector<T> heap;
        HashMap_File<int> index_map;
        void heapify_up(int idx);
        void heapify_down(int idx);
        void swap_positions(int i, int j);
    public:
        MaxHeap() = default;
        bool empty();
        void insert(const T& value);
        void delete_at(int idx);
        void delete_value(const string& value);
        T top() const;
        void extract_max();
};

#include "MaxHeap.tpp"

#endif