#include "battlewithlandlord.h"
#include <chrono>
#include <iterator>

using namespace std;

BattleWithLandlord::BattleWithLandlord(istream &is, ostream &os):_is(is), _os(os) {
    for (char c = '3'; c <= '9'; ++c) {
        _name2id[c] = c - '0';
        _id2name[c - '0'] = c;
    }
    _name2id['0'] = 10; _id2name[10] = '0';
    _name2id['J'] = 11; _id2name[11] = 'J';
    _name2id['Q'] = 12; _id2name[12] = 'Q';
    _name2id['K'] = 13; _id2name[13] = 'K';
    _name2id['A'] = 14; _id2name[14] = 'A';
    _name2id['2'] = 16; _id2name[16] = '2';
    _name2id['c'] = 17; _id2name[17] = 'c';
    _name2id['C'] = 18; _id2name[18] = 'C'; //Clown
    _name2id['x'] = -9; _id2name[-9] = 'x';
    _name2id['y'] = -7; _id2name[-7] = 'y';
    _name2id['z'] = -5; _id2name[-5] = 'z';
    _ready = false;
}

BattleWithLandlord& BattleWithLandlord::operator=(const BattleWithLandlord &that) {
    if (this != &that) {
        _cards[0] = that._cards[0];
        _cards[1] = that._cards[1];
        _last = that._last;
        _round = that._round;
        _side = that._side;
        _ready = true;
        _stopsearch = false;
    }
    return *this;
}


void BattleWithLandlord::cui_exec() {
    string cmd;
    _running = true;
    while (_running) {
        getline(_is, cmd);
        _os << endl;
        stringstream ss;
        ss << cmd;
        cmd.clear();
        ss >> cmd;
        if (cmd == "q" || cmd == "quit") {
            {
                std::lock_guard<std::mutex> lck(_mtx);
                _stopsearch = true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            _running = false;
        } else if (cmd == "r" || cmd == "run") {
            string c1, c2, last;
            ss >> c1 >> c2 >> last;
            _stopsearch = false;
            LoadCards(c1, c2, last);
            _thread = thread(std::mem_fun(&BattleWithLandlord::Run), this);
            _thread.detach();
        } else if (cmd == "s" || cmd == "stop") {
            {
                std::lock_guard<std::mutex> lck(_mtx);
                _stopsearch = true;
            }
        } else if (cmd == "m" || cmd == "move") {
            string str;
            ss >> str;

            CardStyle style;

            bool valid = Str2CardStyle(str, style);
            if (!valid ||
                style.type != CardStyle::PASS && !(_last < style) ||
                style.type == CardStyle::PASS && _last.type == CardStyle::PASS) {
                _os << "illegal move" << endl;
                continue;
            }
            {
                std::lock_guard<std::mutex> lck(_mtx2);
                Move(style.pattern);
                _last = style;

                _round = 0;
                _stopsearch = false;
            }
            _thread = thread(std::mem_fun(&BattleWithLandlord::Run), this);
            _thread.detach();
        } else {
            _os << "invalid command" << endl;
        }
    }
    
}

bool BattleWithLandlord::Str2CardStyle(const string &str, CardStyle &style) {
    if (str.empty() || str == "" || str == "pass") {
        style = CardStyle();
        return true;
    }

    multiset<byte> ms;
    for (auto x: str) {
        ms.insert(_name2id[x]);
    }
    auto res = GenStrategy(ms, _last);
    for (auto x: res) {
        if (x.pattern.size() == str.size()) {
            style = x;
            return true;
        }
    }
    return false;
}

bool BattleWithLandlord::LoadCards(const string &str1, const string &str2, const string &last) {
    _cards[0].clear();
    _cards[1].clear();
    for (auto x: str1) {
        _cards[0].insert(_name2id[x]);
    }
    for (auto x: str2) {
        _cards[1].insert(_name2id[x]);
    }
    
    bool valid = Str2CardStyle(last, _last);

    _round = 0;
    _side = 0;
    _ready = true;
    // PrintCards();
    return valid;
}

string BattleWithLandlord::Translate(const CardStyle &style) {
    string str;
    if (style.type == CardStyle::PASS) {
        return string("pass");
    }
    for (auto x: style.pattern) {
        str += _id2name[x];
    }
    return str;
}

string BattleWithLandlord::Translate(const multiset<byte> &pattern) {
    string str;
    if (!pattern.size()) return "pass";
    for (auto x: pattern) {
        str += _id2name[x];
    }
    return str;
}

void BattleWithLandlord::Run() {
    if (!_ready) {
        _os << "_cards are not loaded yet" << endl;
        return;
    }
    std::lock_guard<std::mutex> lck(_mtx2);
    CardStyle bestmove;
    // PrintPartition();

    BattleWithLandlord bld(cin, cout);
    bld = *this;

    int score, score_dfs, score_ab;
    bool res_dfs;
    list<CardStyle> pv;

    auto start = std::chrono::steady_clock::now();
    auto th = thread(&BattleWithLandlord::AlphaBetaDFS,
                     this, &bld, std::ref(score_dfs), std::ref(res_dfs));

    for (int i = 6; i < 128; ++i) {
        _os << "depth " << i << endl;
        score_ab = GetBestMove(i);
        if (!_stopsearch) bestmove = _pv.front();
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapse = end-start;
        _os << "score: " << score_ab << ", time: " << elapse.count() << "s" << endl;
        _os << "principal variation: " << endl;
        for (auto x: _pv) {
            _os << Translate(x) << " ";
        }
        _os << endl << endl;
        if (_stopsearch || score_ab > WIN_SCORE || score_ab < -WIN_SCORE) {
            {
                std::lock_guard<std::mutex> lck(bld._mtx);
                bld._stopsearch = true;
            }
            break;
        }
    }

    th.join();

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapse = end-start;

    if (res_dfs) {
        score = score_dfs;
        pv = bld._pv;
    } else {
        score = score_ab;
        pv = _pv;
    }
    bestmove = pv.front();

    _os << "score: " << score << ", time: " << elapse.count() << "s" << endl;
    _os << "principal variation: " << endl;
    for (auto x: pv) {
        _os << Translate(x) << " ";
    }
    _os << endl;
    _os << "bestmove: " << Translate(bestmove) << endl << endl;
}

void BattleWithLandlord::AlphaBetaDFS(BattleWithLandlord *bld, int &score, bool &res) {

    score = bld->GetBestMove(128);
    if (score > WIN_SCORE || score < -WIN_SCORE) {
        {
            std::lock_guard<std::mutex> lck(_mtx);
            _stopsearch = true;
        }
        res = true;
    } else {
        res = false;
    }
}

void BattleWithLandlord::PrintCards() {
    cout << "cards0: " << Translate(_cards[0]) << endl;
    cout << "cards1: " << Translate(_cards[1]) << endl;
}

bool BattleWithLandlord::GenCarry(const map<byte, int> &count, unsigned len, int carry, vector<multiset<byte> > &res) {
    res.clear();
    if (carry == 2) {
        vector<byte> v;
        for (auto &x: count) {
            if (x.second > 1) {
                v.push_back(x.first);
            }
        }
        if (v.size() < len) return false;
        vector<unsigned> vi;
        for (unsigned i = 0; i < len; ++i) {
            vi.push_back(i);
        }
        do {
            multiset<byte> ms;
            for (auto x: vi) {
                ms.insert(v[x]);
                ms.insert(v[x]);
            }
            res.push_back(ms);
        } while (next_combination(vi.begin(),vi.size(),v.size()));
    } else if (carry == 1) {
        vector<byte> v;
        for (auto &x: count) {
#ifdef DIFF_CARRY
            if (x.second)
                v.push_back(x.first);
#else
            for (int i = 0; i < x.second; ++i) {
                v.push_back(x.first);
            }
#endif
        }
        if (v.size() < len) return false;
        vector<unsigned> vi;
        for (unsigned i = 0; i < len; ++i) {
            vi.push_back(i);
        }
        do {
            multiset<byte> ms;
            for (auto x: vi) {
                ms.insert(v[x]);
            }
            res.push_back(ms);
        } while(next_combination(vi.begin(),vi.size(),v.size()));
        // bug remain, need to drop repeated items
        // fix it later
        // this is the reason why stl do not contain next_combination
    }
    return true;
}

vector<CardStyle> BattleWithLandlord::GenStrategy() {
    return GenStrategy(_cards[_side], _last);
}

vector<CardStyle> BattleWithLandlord::GenStrategy(const multiset<byte> &style, const CardStyle &last) const {
    vector <CardStyle> all;
    map<byte, int> count;
    for (auto c = style.begin(); c != style.end(); ++c) {
        if (count.find(*c) != count.end()) {
            ++count[*c];
        } else {
            count[*c] = 1;
        }
    }
    auto vice_joker = count.find(17);
    auto joker = count.find(18);
    if (vice_joker != count.end() && joker != count.end()){
        multiset<byte> ms;
        ms.insert(17);
        ms.insert(18);
        all.emplace_back(CardStyle::JOKER_BOMB, 19, ms);
    }
    if (last.type == CardStyle::PASS) {
        for (auto c = count.begin(); c != count.end(); ++c) {
            for (int k = 1; k <= c->second && k <= 4; ++k) {
                multiset<byte> ms;
                for (int i = 0; i < k; ++i) {
                    ms.insert(c->first);
                }
                if (k == 1) {
                    if (c->first > 0)
                        all.emplace_back(CardStyle::MONO, c->first, ms);
                    else
                        all.emplace_back(CardStyle::EXT, c->first, ms);
                    int len = 1;
                    int prev = c->first;
                    for (auto it = c; next(it) != count.end();) {
                        if ((++it)->first != ++prev) break;
                        ms.insert(it->first);
                        if (++len > 4) {
                            all.emplace_back(CardStyle::MONO, c->first, ms, len);
                        }
                    }
                } else if (k == 2) {
                    all.emplace_back(CardStyle::PAIR, c->first, ms);
                    int len = 1;
                    int prev = c->first;
                    for (auto it = c; next(it) != count.end();) {
                        if ((++it)->first != ++prev || it->second < 2) break;
                        ms.insert(it->first);
                        ms.insert(it->first);
                        if (++len > 2) {
                            all.emplace_back(CardStyle::PAIR, c->first, ms, len);
                        }
                    }
                } else if (k == 3) {
                    all.emplace_back(CardStyle::TRIPLE, c->first, ms);
                    int len = 1;
                    int prev = c->first;

                    for (auto it = c; next(it) != count.end();) {
                        if ((++it)->first != ++prev || it->second < 3) break;
                        ms.insert(it->first);
                        ms.insert(it->first);
                        ms.insert(it->first);
                        ++len;
                        all.emplace_back(CardStyle::TRIPLE, c->first, ms, len);
                    }
                    
                    for (int carry : {1, 2}) {
                        ms.clear();
                        ms.insert(c->first);
                        ms.insert(c->first);
                        ms.insert(c->first);
                        len = 1;
                        prev = c->first;
                        auto count_ = count;
                        for (auto it = c; it != count.end();) {
                            count_[it->first] -= 3;

                            vector<multiset<byte> > comb_res;
                            if (GenCarry(count_, len, carry, comb_res)) {
                                for (auto &cs: comb_res) {
                                    multiset<byte> _union = ms;
                                    for (auto x: cs) {
                                        _union.insert(x);
                                    }
                                    all.emplace_back(CardStyle::TRIPLE, c->first, _union, len, carry);
                                }
                            }
                            if (++it == count.end() || it->first != ++prev || it->second < 3) break;
                            ms.insert(it->first);
                            ms.insert(it->first);
                            ms.insert(it->first);
                            ++len;
                        }
                    }
                } else {
                    all.emplace_back(CardStyle::QUARD_BOMB, c->first, ms);
                    auto count_ = count;
                    count_[c->first] -= 4;
                    for (auto carry: {1, 2}) {
                        vector<multiset<byte> > comb_res;
                        if (GenCarry(count_, 2, carry, comb_res)) {
                            for (auto &cs: comb_res) {
                                multiset<byte> _union = ms;
                                for (auto x: cs) {
                                    _union.insert(x);
                                }
                                all.emplace_back(CardStyle::QUARD, c->first, _union, 2, carry);
                            }
                        }
                    }
                }
            }
        }
        sort(all.begin(), all.end(),
             [](const CardStyle &a, const CardStyle &b) {
             return a.pattern.size() > b.pattern.size();
        });
    } else {
        for (auto c = count.begin(); c != count.end(); ++c) {
            multiset<byte> ms;
            if (c->second == 4) {
                ms.insert(c->first);
                ms.insert(c->first);
                ms.insert(c->first);
                ms.insert(c->first);
                auto style = CardStyle(CardStyle::QUARD_BOMB, c->first, ms);
                if (last < style) {
                    all.emplace_back(style);
                }
                if (last.type == CardStyle::QUARD) {
                    if (c->first <= last.id) continue;
                    auto count_ = count;
                    count_[c->first] -= 4;
                    vector<multiset<byte> > comb_res;
                    if (GenCarry(count_, 2, last.carry, comb_res)) {
                        for (auto &cs: comb_res) {
                            multiset<byte> _union = ms;
                            for (auto x: cs) {
                                _union.insert(x);
                            }
                            all.emplace_back(CardStyle::QUARD, c->first, _union, 2, last.carry);
                        }
                    }
                }
                continue;
            }
            if (last.type == CardStyle::MONO) {
                if (c->first <= last.id) continue;
                ms.insert(c->first);
                if (last.len == 1) {
                    all.emplace_back(CardStyle::MONO, c->first, ms);
                } else {
                    int len = 1;
                    int prev = c->first;
                    for (auto it = c; next(it) != count.end();) {
                        if ((++it)->first != ++prev) break;
                        ms.insert(it->first);
                        if (++len > 4) {
                            if (len == last.len) {
                                all.emplace_back(CardStyle::MONO, c->first, ms, len);
                                break;
                            }
                        }
                    }
                }
            } else if (last.type == CardStyle::PAIR) {
                if (c->first <= last.id || c->second < 2) continue;
                ms.insert(c->first);
                ms.insert(c->first);
                if (last.len == 1) {
                    all.emplace_back(CardStyle::PAIR, c->first, ms);
                } else {
                    int len = 1;
                    int prev = c->first;
                    for (auto it = c; next(it) != count.end();) {
                        if ((++it)->first != ++prev || it->second < 2) break;
                        ms.insert(it->first);
                        ms.insert(it->first);
                        if (++len > 2) {
                            if (len == last.len) {
                                all.emplace_back(CardStyle::PAIR, c->first, ms, len);
                                break;
                            }
                        }
                    }
                }
            } else if (last.type == CardStyle::TRIPLE) {
                if (c->first <= last.id || c->second < 3) continue;
                ms.insert(c->first);
                ms.insert(c->first);
                ms.insert(c->first);
                if (last.carry == 0) {
                    if (last.len == 1) {
                        all.emplace_back(CardStyle::TRIPLE, c->first, ms);
                    } else {
                        int len = 1;
                        int prev = c->first;
                        for (auto it = c; next(it) != count.end();) {
                            if ((++it)->first != ++prev || it->second < 3) break;
                            ms.insert(it->first);
                            ms.insert(it->first);
                            ms.insert(it->first);
                            ++len;
                            if (len == last.len) {
                                all.emplace_back(CardStyle::TRIPLE, c->first, ms, len);
                                break;
                            }
                        }
                    }
                } else {
                    ms.clear();
                    ms.insert(c->first);
                    ms.insert(c->first);
                    ms.insert(c->first);
                    int len = 1;
                    int prev = c->first;
                    auto count_ = count;
                    for (auto it = c; it != count.end();) {
                        count_[it->first] -= 3;

                        vector<multiset<byte> > comb_res;
                        if (len == last.len && GenCarry(count_, len, last.carry, comb_res)) {
                            for (auto &cs: comb_res) {
                                multiset<byte> _union = ms;
                                for (auto x: cs) {
                                    _union.insert(x);
                                }
                                all.emplace_back(CardStyle::TRIPLE, c->first, _union, len, last.carry);
                            }
                            break;
                        }

                        if (++it == count.end() || it->first != ++prev || it->second < 3) break;
                        ms.insert(it->first);
                        ms.insert(it->first);
                        ms.insert(it->first);
                        ++len;
                    }
                }
            }
        }
        all.emplace_back(); // PASS
    }
    return all;
}

void BattleWithLandlord::Move(const multiset<byte> &pattern) {
    if (pattern.size()) {
        for (auto x: pattern) {
            _cards[_side].erase(_cards[_side].find(x));
        }
    }
    _side = 1 - _side;
    ++_round;
}

void BattleWithLandlord::Unmove(const multiset<byte> &pattern) {
    _side = 1 - _side;
    --_round;
    if (!pattern.size()) return;
    for (auto x: pattern) {
        _cards[_side].insert(x);
    }
}

int BattleWithLandlord::Evaluate() const {
    if (!_cards[0].size()) return MAX_SCORE - _round;
    if (!_cards[1].size()) return -MAX_SCORE + _round;
    int val = Eval_weight();
    return val;
    return 0;
}

int BattleWithLandlord::Eval_weight() const {
    map<byte, int> count;
    for (int i: {0,1}) {
        for (auto x: _cards[i]) {
            if (count.find(x) != count.end()) {
                ++count[x];
            } else {
                count[x] = 1;
            }
        }
    }
    int sum = 0, index = 0;
    for (auto x: count) {
        sum += index * x.second;
        count[x.first] = index++;
    }
    
    double score1 = 0, score2 = 0, aver = ((double) sum) / (_cards[0].size()+_cards[1].size());
    for (auto x: _cards[0]) score1 += count[x] - aver;
    for (auto x: _cards[1]) score2 += count[x] - aver;
    return (score1 - score2) * 100;

}

int BattleWithLandlord::AlphaBeta(int depth, int alpha, int beta, list<CardStyle> &pv) {
    {
        // std::lock_guard<std::mutex> lck(_mtx);
        if (_stopsearch) return 0;
    }

#if USE_HASHMAP
    if (_hash.find(_cards[_side], _cards[1-_side], _last, depth)) {
        auto val = _hash.get();
        pv = val.path;
        return val.score;
        // else continue alpha-beta search
    }
#endif

    auto strategies = GenStrategy();

    if (!_cards[_side].size() || !_cards[1-_side].size() || 
        depth <= 0 && strategies.size() > 1) {
        
        auto v = Evaluate();
        int score = _side ? -v: v;
        return score;
    }

    int value;
    list<CardStyle> line;


    CardStyle last = _last;
    for (auto &s: strategies) {
        Move(s.pattern);
        _last = s;
        value = -AlphaBeta(depth-1, -beta, -alpha, line);
        Unmove(s.pattern);

        if (value > beta) {
            return beta;
        }
        if (value > alpha) {
            alpha = value;
            pv = line;
            pv.push_front(s);
        }
// #if !USE_HASHMAP
        if (alpha > WIN_SCORE) break;
// #endif
    }
#if USE_HASHMAP
    if (depth == _max_depth && (alpha > WIN_SCORE || alpha < -WIN_SCORE )) {
        _hash.put(_cards[_side], _cards[1-_side], last, pv, depth, alpha);
    } 
#endif

    _last = last;
    return alpha; 
}

int BattleWithLandlord::GetBestMove(int depth) {
    if (depth <= 0) depth = 128;
    _max_depth = depth;
    return AlphaBeta(depth, -MAX_SCORE, MAX_SCORE, _pv);
}
