#include<vector>
#include<utility>
#include<iostream>
#include "Map.h"
using std::vector;
using std::pair;
using std::cout;
using std::endl;

template<typename K, typename V>
class Hashmap_chained : public Map<K, V>{
public:

    explicit Hashmap_chained(int (*hash) (const K& obj), double lambda = DEFAULT_LAMBDA) :
    capacity(DEFAULT_CAPACITY), used(0), map(DEFAULT_CAPACITY), hash(hash), lambda(lambda){ }

    void put(K key, V value) override{
        if (load_factor() > lambda){
            rehash();
        }
        int hash_value = hash(key) % capacity;
        for (auto &item : map[hash_value]) {
            if (item.first == key) {
                item.second = std::move(value);
                return;
            }
        }
        map[hash_value].push_back(pair<K, V>(std::move(key), std::move(value)));
        used++;
    }

    bool remove(const K &key) override{
        int hash_value = hash(key) % capacity;
        for (auto it = map[hash_value].begin(); it != map[hash_value].end(); it++){
            if (it->first == key){
                map[hash_value].erase(it);
                used--;
                return true;
            }
        }
        return false;
    }

    bool has_key(const K &key) const override{
        int hash_value = hash(key) % capacity;
        for (auto &item : map[hash_value]){
            if (item.first == key){
                return true;
            }
        }
        return false;
    }

    V& get(const K &key) override{
        int hash_value = hash(key) % capacity;
        for (auto &item : map[hash_value]){
            if (item.first == key){
                return item.second;
            }
        }
        throw Map_invalid_key_error();
    }

private:
    const static int DEFAULT_CAPACITY = 5;
    constexpr static double DEFAULT_LAMBDA = 0.75f;
    int (*hash) (const K& obj);
    vector<vector<pair<K, V>>> map;
    int capacity;
    int used;
    double lambda;

    inline double load_factor(){ return static_cast<double>(used) / capacity;}

    void rehash(){
        capacity = capacity * 2;
        auto new_map = vector<vector<pair<K, V>>>(capacity);
        for (auto &lst : map){
            for (auto &item : lst){
                new_map[hash(item.first) % capacity].push_back(item);
            }
        }
        map = new_map;
    }
};

int main(){
    Hashmap_chained<int, int> map{[](const int &a) { return a; }};
    for (int i = 0; i < 300; i++){
        map.put(i, i);
    }
    cout << map.get(1) << endl;
    cout << map.get(7) << endl;
    cout << map.get(13) << endl;
    map.put(14, 0);
    cout << map.get(14) << endl;
    cout << map.has_key(2) << endl;
    cout << map.has_key(20) << endl;
    map.remove(1);
    cout << map.has_key(1) << endl;
    const int& a = map.get(299);
    cout << a << endl;
}