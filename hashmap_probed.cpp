#include "Map.h"
#include <utility>
#include <vector>
#include <iostream>
#include <functional>

using std::vector;
using std::function;
using std::cout;

template<typename K, typename V>
class Hashmap_probed : public Map<K, V> {
private:
    enum Status {
        vacant, occupied, deleted
    };

    struct Entry {
        Status status;
        K key;
        V value;

        Entry() : status(vacant) {}

        Entry(K key, V value) : key(key), value(value), status(occupied) {}
    };

public:
    using size_type = typename vector<Entry>::size_type;
    using Hash_func = function<V(size_type)>;
    using Probe_func = function<size_type(size_type)>;
    static inline Probe_func linear_probing(int c1) {
        return [=](size_type step) { return c1 * step; };
    }

    static inline Probe_func quadratic_probing(int c1, int c2) {
        return [=](size_type step) { return c1 * step + c2 * step * step; };
    }

private:
    const static size_type DEFAULT_CAPACITY = 10;
    constexpr static double DEFAULT_LAMBDA = 0.5f;
    size_type used;
    size_type capacity;
    const double lambda;
    const Hash_func hash;
    const Probe_func probe;
    vector<Entry> map;

    [[nodiscard]] inline double load_factor() const { return static_cast<double>(used) / capacity; }

    [[nodiscard]] inline size_type index(const K &key) const { return hash(key) % capacity; }

    void put(vector<Entry> &new_map, K key, V value){
        size_type pos, step;
        for (pos = index(key), step = 0;
             new_map[pos].status == occupied;
             pos = probe(step) % capacity, step++);
       new_map[pos] = Entry(std::move(key), std::move(value));
    }

    void rehash() {
        auto new_map = vector<Entry>(capacity * 2);
        capacity = capacity * 2;
        for (auto &entry : map){
            if (entry.status == occupied){
                put(new_map, entry.key, entry.value);
            }
        }
        map = std::move(new_map);
    }

    [[nodiscard]] size_type find(const K &key) const{
        size_type pos, step;
        for (pos = index(key), step = 0;
             map[pos].status != vacant && map[pos].key != key;
             pos = probe(step) % capacity, step++);
        return pos;
    }

public:
    explicit Hashmap_probed(Hash_func hash, Probe_func probe = linear_probing(1), double lambda = DEFAULT_LAMBDA) :
            capacity(DEFAULT_CAPACITY), used(0), lambda(lambda), hash(std::move(hash)),
            probe(std::move(probe)), map(DEFAULT_CAPACITY) {}

    void put(K key, V value) override {
        if (load_factor() > lambda){
            rehash();
        }
        put(this->map, std::move(key), std::move(value));
        used++;
    };

    V &get(const K &key) override {
        if (not has_key(key)){
            throw Map_invalid_key_error();
        }
        return map[find(key)].value;
    };

    [[nodiscard]] bool has_key(const K &key) const override {
        return map[find(key)].key == key;
    };

    bool remove(const K &key) override {
        auto pos = find(key);
        if (map[pos].key == key){
            map[pos].status = deleted;
            return true;
        }
        return false;
    };

    V &operator[] (const K &key){
        return get(key);
    }
};

//int main() {
//    Hashmap_probed<int, int> map{[](int x){ return x; }};
//    map.put(0, 0);
//    map.put(10, 1);
//    map.put(20, 2);
//    map.put(1, 3);
//    map.put(2, 4);
//    map.put(8, 8);
//    cout << map[0] << std::endl;
//    cout << map[10] << std::endl;
//    cout << map[20] << std::endl;
//    cout << map[1]<< std::endl;
//    cout << map[2] << std::endl;
//    cout << map[8] << std::endl;
//    map.put(7, 7);
//    map.put(6, 6);
//    map.put(5, 5);
//    map.remove(20);
//    map.remove(2);
//    cout << map.get(0) << std::endl;
//    cout << map.get(7) << std::endl;
//    cout << map.get(10) << std::endl;
//    cout << map.get(1) << std::endl;
//    cout << map.get(6) << std::endl;
//    cout << map.get(8) << std::endl;
//    cout << map.get(5) << std::endl;
//}