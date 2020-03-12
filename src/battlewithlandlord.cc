#include "battlewithlandlord.h"

using std::cin;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;
using std::stringstream;
using std::string;
using std::vector;
using std::list;
using std::set;
using std::multiset;
using std::map;
using std::unordered_map;
using std::sort;
using std::thread;
using std::mutex;

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

void BattleWithLandlord::cui_exec() {
    string cmd;
    _running = true;
    while (_running) {
        getline(_is, cmd);
        stringstream ss;
        ss << cmd;
        cmd.clear();
        ss >> cmd;
        if (cmd == "q" || cmd == "quit") {
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
            {
                std::lock_guard<std::mutex> lck(_mtx2);
                Move(_pv.front().pattern);
            }
        } else if (cmd == "m" || cmd == "move") {
            string str;
            ss >> str;
            multiset<byte> pattern;
            if (str.size() && str != "pass") {
                for (auto x: str) {
                    pattern.insert(_name2id[x]);
                }
            }
            Move(pattern);
        } else {
            _os << "invalid command" << endl;
        }
    }
    
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
    bool valid;
    if (last != "" && last != "pass") {
        multiset<byte> ms;
        for (auto x: last) {
            ms.insert(_name2id[x]);
        }
        _last = CardStyle();
        auto res = GenStrategy(ms, _last);
        valid = false;
        for (auto x: res) {
            if (x.pattern.size() == last.size()) {
                _last = x;
                valid = true;
            }
        }
    } else {
        valid = true;
    }
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
        cout << "_cards are not loaded yet" << endl;
        return;
    }
    auto start = std::chrono::steady_clock::now();
    std::lock_guard<std::mutex> lck(_mtx2);
    CardStyle bestmove;
    for (int i = 5; !_stopsearch && i < 32; ++i) {
        cout << "depth " << i << endl;
        int val = GetBestMove(i);
        if (!_stopsearch) bestmove = _pv.front();
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapse = end-start;
        cout << "score: " << val << ", time: " << elapse.count() << "s" << endl;
        cout << "principal variation: " << endl;
        for (auto x: _pv) {
            cout << Translate(x) << " ";
        }
        cout << endl << endl;
    }
    cout << "bestmove: " << Translate(bestmove) << endl;
}
void BattleWithLandlord::PrintCards() {
    cout << "cards0: " << Translate(_cards[0]) << endl;
    cout << "cards1: " << Translate(_cards[1]) << endl;
    auto &&res1 = GenStrategy(_cards[0], _last);
    for (auto x: res1) {
        cout << Translate(x) << " ";
    }
    cout << endl;
    auto &&res2 = GenStrategy(_cards[1], _last);
    for (auto x: res2) {
        cout << Translate(x) << " ";
    }
    cout << endl;
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
                for (auto it = c; it != count.end();) {
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
                for (auto it = c; it != count.end();) {
                    if ((++it)->first != ++prev || it->second < k) break;
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
                auto count_ = count;
                for (auto it = c; it != count.end();) {
                    count_[it->first] -= k;
                    for (int carry: {1, 2}) {
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
                    }
                    if ((++it)->first != ++prev || it->second < k) break;
                    ms.insert(it->first);
                    ms.insert(it->first);
                    ms.insert(it->first);
                    ++len;
                    all.emplace_back(CardStyle::TRIPLE, c->first, ms, len);
                }
            } else {
                all.emplace_back(CardStyle::QUARD_BOMB, c->first, ms);
                auto count_ = count;
                count_[c->first] -= k;
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

    vector<CardStyle> strategies;
    for (auto x: all) {
        if (last < x) {
            strategies.push_back(x);
            sort(strategies.begin(), strategies.end(),
            [](const CardStyle &a, const CardStyle &b) {
                return a.pattern.size() > b.pattern.size();
            });
        }
    }
    if (last.type != CardStyle::PASS)
        strategies.emplace_back(); // PASS
    // sort(strategies.begin(), strategies.end(),
    // [](CardStyle a, CardStyle b) {return a.pattern.size() > b.pattern.size();});
    return strategies;
}

vector<CardStyle> BattleWithLandlord::GenStrategy_ex() {
    vector<CardStyle> res;
    auto origin = GenStrategy(_cards[_side], _last);
    set<vector<CardStyle> > previous;
    for (auto &x: origin) {
        auto response = GenStrategy(_cards[1-_side], x);
        if (previous.find(response) == previous.end()) {
            res.push_back(x);
            previous.insert(response);
        }
    }
    return res;
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
    int val = Eval_weight_ex();// + Eval_len_diff()*10;
    return val;
    // return Eval_len_diff()*10 + Eval_cmp();
    // return Eval_cmp();
    // return Eval_len_diff();
    // return Eval_respond() + Eval_len_diff()*2 + Eval_weight();
    return 0;
}

int BattleWithLandlord::Eval_weight_ex() const {
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
int BattleWithLandlord::Eval_weight() const {
    const int weight[] = {
        0, 0, 0,
        -13, -12, -11, -10, -9, -8,   // 3,4,5,6,7,8
        -7, -6, -5, -4, -3, -2, 0, -1,  // 9,10,J,Q,K,A,2
        0, 1                   // Jokers
    };

    int score1 = 0, score2 = 0;
    // for (auto x: _cards[0]) score1 += x;
    // for (auto x: _cards[1]) score2 += x;
    
    map<byte, int> count_0, count_1;
    for (auto c = _cards[0].begin(); c != _cards[0].end(); ++c) {
        if (count_0.find(*c) != count_0.end()) {
            ++count_0[*c];
        } else {
            count_0[*c] = 1;
        }
    }
    for (auto &x: count_0) {
        if (x.second == 1)
            score1 += weight[x.first];
    }
    for (auto c = _cards[1].begin(); c != _cards[1].end(); ++c) {
        if (count_1.find(*c) != count_1.end()) {
            ++count_1[*c];
        } else {
            count_1[*c] = 1;
        }
    }
    for (auto &x: count_1) {
        if (x.second == 1)
                score2 += weight[x.first];
    }
    return score1 - score2;
}


int BattleWithLandlord::Eval_cmp() const {
    auto it0 = _cards[0].begin(), it0_e = _cards[0].end();
    auto it1 = _cards[1].begin(), it1_e = _cards[1].end();
    int s = 0;
    while (it0 != it0_e && it1 != it1_e) {
        s += (*(it0++) > *(it1++));
    }
    while (it0++ != it0_e) --s;
    while (it1++ != it1_e) ++s;
    return s;
}

int BattleWithLandlord::Eval_respond() const {
    auto &&res = GenStrategy(_cards[_side], _last);
    if (res.size()) return _side ? -1 : 1;
    return 0;
}

// int BattleWithLandlord::Eval_Monte_Carlo() const {
//   // int total_trials = 1000, wins = 0;
//   // auto cards = _cards;
//   // for (cards0.size() && cards1.size()) {

//   // }
// }

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

    if (depth <= 0 || !_cards[_side].size() || !_cards[1-_side].size()) {
        auto v = Evaluate();
        int score = _side ? -v: v;
        return score;
    }

    int value;
    list<CardStyle> line;
    auto &&strategies = GenStrategy();

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
#if !USE_HASHMAP
        if (alpha > WIN_SCORE) break;
#endif
    }
#if USE_HASHMAP
    _hash.put(_cards[_side], _cards[1-_side], last, pv, depth, alpha);
#endif

    _last = last;
    return alpha; 
}

int BattleWithLandlord::GetBestMove(int depth) {
    if (depth <= 0) depth = 100;
    _max_depth = depth;
    return AlphaBeta(depth, -MAX_SCORE, MAX_SCORE, _pv);
}
