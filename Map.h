#ifndef DATA_STRUCTURES_MAP_H
#define DATA_STRUCTURES_MAP_H
#include <exception>
class Map_invalid_key_error : public std::exception {
public:
    [[nodiscard]] const char* what() const noexcept override{ return "Invalid Key"; }
};

template <typename K, typename V>
struct Map{
    virtual void put(K key, V value) = 0;
    virtual V &get(const K &key) = 0;
    virtual bool has_key(const K &key) const = 0;
    virtual bool remove(const K &key) = 0;
};
#endif //DATA_STRUCTURES_MAP_H
