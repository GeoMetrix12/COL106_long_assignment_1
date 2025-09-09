#include "MaxHeap.hpp"
#include <stdexcept>
#include <string>
using namespace std;

template<typename T>
const string& get_key(const T& value){
    return value.file->filename;
}
template<typename T>
void MaxHeap<T>::swap_positions(int i, int j){
    swap(heap[i], heap[j]);
    index_map.insert(get_key(heap[i]), i);
    index_map.insert(get_key(heap[j]), j);
}
template<typename T>
void MaxHeap<T>::heapify_up(int idx){
    while(idx > 0){
        int par = (idx - 1)/2;
        if(heap[idx] > heap[par]){
            swap_positions(idx, par);
            idx = par;
        }
        else{
            break;
        }
    }
}
template<typename T>
void MaxHeap<T>::heapify_down(int idx){
    int n = (int)heap.size();
    while(true){
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int largest = idx;
        if(left < n && heap[left] > heap[largest]){
            largest = left;
        }
        if(right < n && heap[right] > heap[largest]){
            largest = right;
        }
        if(largest != idx){
            swap_positions(idx, largest);
            idx = largest;
        }
        else{
            break;
        }
    }
}
template<typename T>   
bool MaxHeap<T>::empty(){
    return heap.empty();
}
template<typename T>
void MaxHeap<T>::insert(const T& value){
    heap.push_back(value);
    index_map.insert(get_key(value), (int)heap.size() - 1);
    heapify_up((int)heap.size() - 1);
}
template<typename T>
void MaxHeap<T>::delete_at(int idx){
    int n = (int)heap.size();
    if(idx < 0 || idx >= n){
        throw out_of_range("Index out of range");
    }
    const string& key_to_remove = get_key(heap[idx]);
    index_map.remove(key_to_remove);
    if(idx == n - 1){
        heap.pop_back();
        return;
    }
    swap_positions(idx, n - 1);
    heap.pop_back();
    if(idx > 0 && heap[idx] > heap[(idx - 1)/2]){
        heapify_up(idx);
    }
    else{ 
        heapify_down(idx);
    }
}
template<typename T>
void MaxHeap<T>::delete_value(const string& filename){
    int idx = index_map.get(filename);
    if(idx >= 0 && idx < (int)heap.size() && get_key(heap[idx]) == filename){
        delete_at(idx);
    }
}
template<typename T>
T MaxHeap<T>::top() const{
    if(!heap.empty()){
        return heap[0];
    }
    throw runtime_error("Heap is empty");
}
template<typename T>
void MaxHeap<T>::extract_max(){
    if(heap.empty()){
        throw runtime_error("Heap is empty");
    }
    const string& max_key = get_key(heap[0]);
    index_map.remove(max_key);
    swap_positions(0, static_cast<int>(heap.size()) - 1);
    heap.pop_back();
    if(!heap.empty()){
        heapify_down(0);
    }
}