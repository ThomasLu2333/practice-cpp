#include <utility>
#include <vector>
#include <iostream>
#include<cstdlib>

using std::vector;
using std::endl;
using std::cout;
using std::string;
using std::ostream;
using std::rand;

template<typename T>
using comparator = int (*)(const T &a, const T &b);

template<typename T>
using viter = typename vector<T>::iterator;

template<typename T>
void swap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}

template<typename T>
void bubble_sort(vector<T> &lst) {
    bool flag;
    do {
        flag = false;
        for (int i = 0; i < lst.size() - 1; i++) {
            if (lst[i + 1] < lst[i]) {
                flag = true;
                swap<T>(lst[i], lst[i + 1]);
            }
        }
    } while (flag);
}

template<typename T>
void insertion_sort(vector<T> &lst, comparator<T> compare) {
    for (int i = 0; i < lst.size(); i++) {
        for (int j = i; j > 0; j--) {
            if (compare(lst[j - 1], lst[j]) > 0) {
                swap(lst[j - 1], lst[j]);
            }
        }
    }
}

template<typename T>
void merge(viter<T> start, viter<T> mid, viter<T> end, comparator<T> compare) {
    /**
     * Merges lst[start...end)
     * Requires lst[start...mid) and lst[mid...end) is sorted
     */
    vector<T> temp{};
    auto start1 = start;
    auto start2 = mid;
    while (start1 < mid || start2 < end) {
        if (start1 == mid || (start2 != end && compare(*start1, *start2) >= 0)) {
            temp.push_back(*start2);
            start2++;
        } else {
            temp.push_back(*start1);
            start1++;
        }
    }
    for (auto it1 = start, it2 = temp.begin(); it1 < end; it1++, it2++) {
        *it1 = *it2;
    }
}

template<typename T>
void merge_sort_aux(viter<T> start, viter<T> end, comparator<T> compare) {
    /**
     * Merge sorts lst[start...end)
     */
    if (start == end || start == end - 1) {
        return;
    }
    auto mid = start + (end - start) / 2;
    merge_sort_aux(start, mid, compare);
    merge_sort_aux(mid, end, compare);
    merge(start, mid, end, compare);
}

template<typename T>
void merge_sort(vector<T> &lst, comparator<T> compare) {
    merge_sort_aux(lst.begin(), lst.end(), compare);
}

template<typename T>
viter<T> partition(viter<T> begin, viter<T> end, comparator<T> compare) {
    /**
     * partitions lst[begin...end).
     * requires: pivot placed at begin.
     */
    auto i = begin;
    auto j = end - 1;
    while (i < j) {
        if (compare(*j, *i) > 0) {
            j--;
        } else {
            swap(*i, *j);
            swap(*j, *(i + 1));
            i++;
        }
    }
    return i;
}

template<typename T>
void quick_sort_aux(viter<T> begin, viter<T> end, comparator<T> compare) {
    /**
     * quick sorts lst[begin...end)
     */
    if (begin == end || begin == end - 1) {
        return;
    }
    viter<T> pivot_pos = begin + (rand() % (end - begin));
    swap(*pivot_pos, *begin);
    pivot_pos = partition(begin, end, compare);
    quick_sort_aux(begin, pivot_pos, compare);
    quick_sort_aux(pivot_pos, end, compare);
}

template<typename T>
void quick_sort(vector<T> &lst, comparator<T> compare) {
    quick_sort_aux(lst.begin(), lst.end(), compare);
}

template<typename T>
void print_vec(vector<T> vec) {
    for (auto &val: vec) {
        cout << val << " ";
    }
    cout << endl;
}

struct Student{
    double gpa;
    string name;

    Student(double gpa, string name) : gpa(gpa), name(std::move(name)) {}

    bool operator<(const Student &other) const {
        return gpa < other.gpa || (gpa == other.gpa && name < other.name);
    }
};

ostream &operator<<(ostream &os, const Student &student) {
    os << student.name << ":" << student.gpa;
    return os;
}

comparator<int> rev_comp_int = [](const int &x, const int &y) { return y - x; };

comparator<int> comp_int = [](const int &x, const int &y) { return x - y; };

//int main() {
//    vector<int> l1{1, 4, 2, 5, 3};
//    bubble_sort<int>(l1);
//    print_vec(l1);
//    vector<int> l2{1, 4, 2, 5, 3};
//    insertion_sort<int>(l2,rev_comp_int);
//    print_vec(l2);
//    vector<Student> l3{Student(4, "Thomas"),
//                       Student(3, "Henry"),
//                       Student(3.5, "George"),
//                       Student(3.5, "Georgina"),
//                       Student(4, "Tommy")};
//    bubble_sort<Student>(l3);
//    print_vec(l3);
//    vector<int> l4{1, 4, 3, 5, 2, 10, 11, 9, 8};
//    merge_sort<int>(l4,comp_int);
//    print_vec(l4);
//    vector<int> l5{5, 4, 3, 1, 2, 10, 11, 9, 8};
//    partition(l5.begin(), l5.end(), comp_int);
//    print_vec(l5);
//}
