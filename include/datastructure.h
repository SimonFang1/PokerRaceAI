#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>

typedef char byte;

#define DIFF_CARRY 1
#define USE_HASHMAP 0
#define MAX_SCORE 30000
#define WIN_SCORE 29000

struct CardStyle {
    enum Type {
        PASS,
        MONO,
        PAIR,
        TRIPLE,
        QUARD,
        QUARD_BOMB,
        JOKER_BOMB,
        EXT
    } type;
    int id;
    int len;
    int carry;
    std::multiset<byte> pattern;
    CardStyle(Type t = PASS, int d = 0, std::multiset<byte> p=std::multiset<byte>(), int l = 1, int c = 0) {
        type = t;
        id = d;
        pattern = p;
        len = l;
        carry = c;
    }
};

inline bool operator<(const CardStyle &a, const CardStyle &b) {
    if (a.type == CardStyle::PASS && b.type != CardStyle::PASS)  return true;
    if (a.type == b.type && a.len == b.len && a.carry == b.carry) {
        return a.id < b.id;
    }
    if (b.type == CardStyle::JOKER_BOMB) {
        return true;
    }
    if (a.type != CardStyle::JOKER_BOMB &&
            b.type == CardStyle::QUARD_BOMB) {
        return true;
    }
    return false;
}

inline bool operator==(const CardStyle &a, const CardStyle &b) {
    return a.type == b.type && a.id == b.id && a.len == b.len && a.carry == b.carry;
}

template<class RandomAccessIterator, typename uint=unsigned>
bool next_combination(RandomAccessIterator index, uint k, uint n) {
    auto &last = index[k-1];
    if (last >= n) return false;
    if (last < n - 1) {
        ++last;
        return true;
    }
    if (k == 1) return false;
    uint i = k - 2;
    while (i > 0 && index[i] == index[i+1] - 1) --i;
    if (i == 0 && index[i] == index[i+1] - 1) return false;
    ++index[i];
    while (++i < k) {
        index[i] = index[i-1] + 1;
    }
    return true;
}

