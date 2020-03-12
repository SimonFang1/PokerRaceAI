#pragma once
#include "datastructure.h"

class HashCache {
public:
    enum HashType {
        EXACT, ALPHA, BETA
    };
protected:
    struct Key {
        std::multiset<byte> card1, card2;
        CardStyle style;
        Key(const std::multiset<byte> &c1, const std::multiset<byte> &c2, const CardStyle &s = CardStyle()) {
            card1 = c1;
            card2 = c2;
            style = s;
        }
        bool operator==(const Key &k) const {
            return card1 == k.card1 && card2 == k.card2 &&
                   (style < k.style || style == k.style);
        }
    };
    struct hashcode {
        unsigned operator() (const Key &k) const {
            unsigned res = 0;
            for (auto x: k.card1) {
                res ^= x;
            }
            for (auto x: k.card2) {
                res ^= x;
            }
            return res;
        }
    };
    // struct hashequal {
    //   bool operator () (const Key & a,const Key & b) const {
    //     return a == b;
    //   }
    // };
    struct Value {
        std::list<CardStyle> path;
        int score;
        int depth;
        HashType hashtype;
        Value() {}
        Value(const std::list<CardStyle> &p, int s = 0, int d = 0, HashType ht = EXACT) {
            path = p;
            score = s;
            depth = d;
            hashtype = ht;
        }
    };
    // typedef unordered_map<Key, Value, hashcode, hashequal> _Hashmap;
    typedef std::unordered_map<Key, Value, hashcode> _Hashmap;
    _Hashmap _table;
    _Hashmap::iterator _it;
public:
    bool find(const std::multiset<byte> &c1, const std::multiset<byte> &c2,
              const CardStyle &last, int depth) {
        _it = _table.find(Key(c1, c2, last));
        if (_it == _table.end()) return false;

        // {{
        //   auto & val = _it->second;
        //   cout << "_depth: " << val.depth << ", depth: " << depth << endl;
        //   cout << "id" << val.style.id << ", _style: ";
        //   for (auto x: val.style.pattern) {
        //     cout << (int) x << " ";
        //   }
        //   cout << ", style: ";
        //   for (auto x: last.pattern) {
        //     cout << (int) x << " ";
        //   }
        //   cout << endl;
        // }}
        if (_it->second.depth >= depth) return true;
        return false;
    }
    Value& get() {
        return _it->second;
    }
    void put(std::multiset<byte> c1, std::multiset<byte> c2,
             CardStyle style, std::list<CardStyle> path, int depth, int score, HashType ht) {
        if (!find(c1, c2, style, depth)) {
            _table[Key(c1, c2, style)] = Value(path, score, depth, ht);
        } else {
            if (depth >= _it->second.depth) {
                _it->second = Value(path, score, depth, ht);
            }
        }
    }
};
