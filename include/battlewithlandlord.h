#pragma once

#include "datastructure.h"
#include "hashcache.hpp"

class BattleWithLandlord {
public:
    BattleWithLandlord(std::istream &is, std::ostream &os);

    void cui_exec();

    bool LoadCards(const std::string &str1, const std::string &str2, const std::string &last);

    std::string Translate(const CardStyle &style);

    std::string Translate(const std::multiset<byte> &pattern);

    void Run();

    void PrintCards();


 private:
    std::unordered_map<char, byte> _name2id;
    std::unordered_map<byte, char> _id2name;
    std::multiset<byte> _cards[2];
    std::list<CardStyle> _pv; //主要变例
    CardStyle _last;
    int _side;
    int _round, _max_depth;
    bool _ready;
    HashCache _hash;
    std::istream &_is;
    std::ostream &_os;
    bool _running;
    bool _stopsearch;
    std::thread _thread;
    std::mutex _mtx, _mtx2;
    // std::condition_variable _cv;

    static bool GenCarry(const std::map<byte, int> &count,
                         unsigned len, int carry,
                         std::vector<std::multiset<byte> > &res);

    std::vector<CardStyle> GenStrategy();

    std::vector<CardStyle> GenStrategy(const std::multiset<byte> &style, const CardStyle &last) const;

    std::vector<CardStyle> GenStrategy_ex();

    CardStyle GenStrategy_greedy(const std::multiset<byte> &style, const CardStyle &last) const;

    std::pair<std::vector<std::vector<CardStyle> >, std::vector<std::vector<CardStyle> > >
    Partition(std::multiset<byte> a, std::multiset<byte> b) const;

    std::vector<std::vector<CardStyle> >
    Partition(std::multiset<byte> a) const;

    CardStyle BetterThan(const std::multiset<byte> &cards, const CardStyle &a, const CardStyle &b) const;

    void Move(const std::multiset<byte> &pattern);

    void Unmove(const std::multiset<byte> &pattern);
    
    int Evaluate() const;

    int Eval_weight_ex() const;
    int Eval_weight() const;

    inline int Eval_len_diff() const {
        return  _cards[1].size() - _cards[0].size();
    }

    int Eval_cmp() const;

    int Eval_respond() const;

    int Eval_turns() const;

    int AlphaBeta(int depth, int alpha, int beta, std::list<CardStyle> &pv);
    
    int GetBestMove(int depth);

    void PrintPartition();
};
