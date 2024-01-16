#include "Node.h"
#include <iostream>

class List_invalid_index_error : public std::runtime_error {
public:
    explicit List_invalid_index_error(int pos) :
            runtime_error("Invalid Index--" + std::to_string(pos)) {};
};

template<typename T>
class Linked_list {
private:
    Node<T> *head;
    Node<T> *tail;
    int size;

public:
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = int;
        using value_type = T;
        using pointer = T *;
        using reference = T &;
    private:
        Node<T> *now;
    public:
        explicit Iterator(Node<T> *now) : now(now) {}

        reference operator*() const { return now->data; }

        pointer operator->() { return &(now->data); }

        Iterator &operator++() {
            now = now->next;
            return *this;
        }

        Iterator operator++(int) {
            const Iterator temp = *this;
            ++(*this);
            return temp;
        }

        friend bool operator==(const Iterator &a, const Iterator &b) {
            return a.now == b.now;
        }

        friend bool operator!=(const Iterator &a, const Iterator &b) {
            return a.now != b.now;
        }
    };

    Iterator begin() { return Iterator(head); }

    Iterator end() { return Iterator(tail->next); }

    Linked_list() : head(nullptr), tail(nullptr), size(0) {}

    Linked_list(const Linked_list<T> &other) : Linked_list() {
        for (auto item: other) push_back(item);
    }

    ~Linked_list() {
        auto now = head;
        while (now) {
            auto next = now->next;
            delete now;
            now = next;
        }
    }

    friend bool operator==(const Linked_list<T> &a, const Linked_list<T> &b) {
        auto now_a = a.head;
        auto now_b = b.head;
        while (now_a->next && now_b->next) {
            if (now_a->data != now_b->data) {
                return false;
            }
        }
        return now_a->next == nullptr && now_b->next == nullptr;
    }

    Linked_list<T> &operator=(const Linked_list<T> &other) {
        if (this == other) {
            return *this;
        }
        while (size != 0) remove(0);
        for (auto item: other) push_back(item);
        return *this;
    }

    T &get(int pos) const {
        if (pos >= size || pos < 0) {
            throw List_invalid_index_error(pos);
        }
        auto now = head;
        while (pos != 0) {
            now = now->next;
            pos--;
        }
        return now->data;
    }

    void push_back(T data) {
        insert(std::move(data), size);
    }

    void insert(T data, int pos) {
        auto node = new Node<T>{data};
        if (pos > size || pos < 0) {
            throw List_invalid_index_error(pos);
        } else if (size == 0) {
            head = tail = node;
        } else if (pos == 0) {
            node->next = head;
            head = node;
        } else if (pos == size) {
            tail->next = node;
            tail = node;
        } else {
            auto now = head;
            while (pos != 1) {
                pos--;
                now = head->next;
            }
            auto old_next = now->next;
            now->next = node;
            node->next = old_next;
        }
        size++;
    }

    T remove(int pos) {
        T data;
        if (pos >= size || pos < 0) {
            throw List_invalid_index_error(pos);
        } else if (pos == 0) {
            data = head->data;
            auto head_next = head->next;
            delete head;
            head = head_next;
            if (size == 1) {
                tail = nullptr;
            }
        } else {
            auto now = head;
            while (pos != 1) {
                pos--;
                now = now->next;
            }
            if (now->next == tail) {
                data = tail->data;
                delete tail;
                tail = now;
                tail->next = nullptr;
            } else {
                data = now->next->data;
                auto new_next = now->next->next;
                delete now->next;
                now->next = new_next;
            }
        }
        size--;
        return std::move(data);
    }

    int get_size() const { return size; }
};

template<typename T>
void print_list(Linked_list<T> &lst) {
    for (auto item: lst) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

//int main() {
//    Linked_list<int> lst{};
//    for (int i = 0; i < 10; i++) {
//        lst.push_back(i);
//    }
//    print_list(lst);
//    lst.insert(100, 2);
//    lst.insert(101, 0);
//    print_list(lst);
//    lst.remove(11);
//    lst.remove(0);
//    lst.remove(1);
//    print_list(lst);
//}