#pragma once

#include "datastructure.h"
#include "hashcache.hpp"

class BattleWithLandlord {
public:
    BattleWithLandlord(std::istream &is = std::cin, std::ostream &os = std::cout);

    BattleWithLandlord& operator=(const BattleWithLandlord &that);

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

    bool Str2CardStyle(const std::string &str, CardStyle &res);

    static bool GenCarry(const std::map<byte, int> &count,
                         unsigned len, int carry,
                         std::vector<std::multiset<byte> > &res);

    std::vector<CardStyle> GenStrategy();

    std::vector<CardStyle> GenStrategy(const std::multiset<byte> &style, const CardStyle &last) const;

    void Move(const std::multiset<byte> &pattern);

    void Unmove(const std::multiset<byte> &pattern);
    
    int Evaluate() const;

    int Eval_weight() const;

    int AlphaBeta(int depth, int alpha, int beta, std::list<CardStyle> &pv);
    void AlphaBetaDFS(BattleWithLandlord *bld, int &score, bool &res);
    
    int GetBestMove(int depth = 128);
};
