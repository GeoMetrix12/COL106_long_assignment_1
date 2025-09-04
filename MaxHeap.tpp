#include "MaxHeap.hpp"

template<typename T>
void MaxHeap<T>::heapify_up(int idx){
    while(idx > 0){
        int par = (idx - 1)/2;
        if(heap[idx] > heap[par]){
            swap(heap[idx], heap[par]);
            idx = par;
        }
        else{
            break;
        }
    }
}
template<typename T>
void MaxHeap<T>::heapify_down(int idx){
    int n = heap.size();
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
            swap(heap[idx], heap[largest]);
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
    heapify_up((int)heap.size() - 1);
}
template<typename T>
void MaxHeap<T>::delete_at(int idx){
    int n = heap.size();
    if(idx < 0 || idx >= n){
        throw out_of_range("Index out of range");
    }
    if(idx == n - 1){
        heap.pop_back();
        return;
    }
    swap(heap[idx], heap.back());
    heap.pop_back();
    if(idx > 0 && heap[idx] > heap[(idx - 1)/2]){
        heapify_up(idx);
    }
    else{ 
        heapify_down(idx);
    }
}
template<typename T>
void MaxHeap<T>::delete_value(const T& value){
    int n = heap.size();
    for(int i = 0; i < n; i++){
        if(heap[i] == value){
            delete_at(i);
            return;
        }
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
    swap(heap[0], heap.back());
    heap.pop_back();
    if(!heap.empty()){
        heapify_down(0);
    }
}