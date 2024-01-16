#include "Node.h"
#include<iostream>
class EmptyQueueException: public std::exception{
public:
    const char * what() noexcept{
        return "Attempt to Dequeue/Peek an Empty Queue";
    }
};

/*
 * NOTE: The dynamic memory allocation of this class are not completely implemented since it is mostly the same
 * with the Linked_list class in linkedlist.cpp
 */
template <typename T>
class Queue{
public:
    Queue(){
        head = tail = nullptr;
        size = 0;
    }
    void enqueue(T data){
        auto ptr = new Node<T>{std::move(data), tail};
        if (size == 0){
            head = tail = ptr;
        }
        else{
            tail->next = ptr;
            tail = ptr;
        }
        size += 1;
    }
    T dequeue(){
        T data;
        if (size == 0){
            throw EmptyQueueException();
        }
        else{
            data = head->data;
            auto new_head = head->next;
            delete head;
            head = new_head;
            if (size == 1){
                head = tail = nullptr;
            }
        }
        size -= 1;
        return data;
    }
    T& peek() const{
        if (size == 0){
            return EmptyQueueException();
        }
        return tail->data;
    }
    bool empty() const { return size == 0; }
    int get_size() const { return size; }
private:
    Node<T> *head;
    Node<T> *tail;
    int size;
};

//int main(){
//    Queue<int> queue;
//    for (int i = 0; i < 10; i++){
//        queue.enqueue(i);
//    }
//    for (int i = 0; i < 10; i++){
//        std::cout << queue.dequeue() << std::endl;
//    }
//    queue.dequeue();
//}