#include "Node.h"
#include <iostream>

class EmptyStackException: public std::exception{
public:
    const char * what() noexcept{
        return "Attempt to Pop/Peek an Empty Stack";
    }
};

/*
 * NOTE: The dynamic memory allocation of this class are not completely implemented since it is mostly the same
 * with the Linked_list class in linkedlist.cpp
 */
template<typename T>
class Stack{
public:
    Stack() {
        head = nullptr;
        size = 0;
    }
    void push(T data){
        head = new Node<T>{std::move(data), head};
        size += 1;
    }
    T pop(){
        if (size == 0){
            throw EmptyStackException();
        }
        T data = head->data;
        auto new_head = head->next;
        delete head;
        head = new_head;
        size -= 1;
        return std::move(data);
    }
    T &peek() const{
        if (size == 0){
            throw EmptyStackException();
        }
        return head->data;
    }
    bool empty() const{ return size == 0; }
    int get_size() const {return size; }
private:
    Node<T> *head;
    int size;
};

//int main(){
//    Stack<int> stack{};
//    for (int i = 0; i < 10; i++){
//        stack.push(i);
//    }
//    for (int i = 0; i < 10; i++){
//        std::cout << stack.pop() << std::endl;
//    }
//    stack.pop();
//}