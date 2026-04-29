#pragma once
#include <vector>


template<typename T, size_t N>
struct NaryHeap {
    std::vector<T> data;
    
    NaryHeap(size_t reserve_size = 0) {
        data.reserve(reserve_size);
    }

    void push(T val) {
        data.push_back(val);
        sift_up(data.size() - 1);
    }
    
    T pop() {
        T top = data[0];
        data[0] = data.back();
        data.pop_back();
        sift_down(0);
        return top;
    }
    
    bool empty() const { return data.empty(); }

    private:
    void sift_up(size_t i) {
        while(i > 0) {
            size_t parent = (i - 1) / N;
            if(data[i] < data[parent]) {
                std::swap(data[i], data[parent]);
                i = parent;
            } else break;
        }
    }
    
    void sift_down(size_t i) {
        while(true) {
            size_t best = i;
            for(size_t k = 1; k <= N; k++) {
                size_t child = N * i + k;
                if(child < data.size() && data[child] < data[best])
                    best = child;
            }
            if(best == i) break;
            std::swap(data[i], data[best]);
            i = best;
        }
    }
};