# PokerRaceEngine
双人明牌斗地主

仿照象棋类AI，利用[alpha-beta搜索](http://www.xqbase.com/computer/search_alphabeta.htm)，求解最优出牌策略。


## 引擎命令

|命令|说明|
|:--|:--|
|`r[un] mycard opponentcard [ last_pattern ]`|开始搜索 |
|`s[top]`|停止搜索 |
|`q[uit]`|退出 |

牌面用字符串表示，一张牌用一个字符表示，大小写敏感，对应关系如下：

|3|4|5|6|7|8|9|10|J|Q|K|A|2|joker|Joker|
|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|
|'3'|'4'|'5'|'6'|'7'|'8'|'9'|'0'|'J'|'Q'|'K'|‘A’|'2'|'c'|'C'|

牌面之间用空格隔开，支持任意排列顺序。

## 用例
启动引擎输入：

```bash
    run AAK003 AKQJJ00
```

引擎输出：
```bash
depth 5
score: 200, time: 0.00176931s
principal variation: 
3 A pass 00 pass 

depth 6
score: 40, time: 0.00656179s
principal variation: 
00 JJ AA pass 3 Q 

depth 7
score: 200, time: 0.0181402s
principal variation: 
3 Q A pass 00 JJ pass 

depth 8
score: 40, time: 0.044348s
principal variation: 
3 Q A pass A pass 00 JJ 

depth 9
score: 166, time: 0.0983591s
principal variation: 
3 K A pass 0 Q A pass 0 

depth 10
score: 100, time: 0.215005s
principal variation: 
3 K A pass 0 A pass 00 pass JJ 

depth 11
score: 100, time: 0.407475s
principal variation: 
00 JJ AA pass 3 K pass 00 pass A pass 

depth 12
score: -100, time: 0.797829s
principal variation: 
00 JJ pass 00 pass Q A pass A pass 3 K 

depth 13
score: 29987, time: 1.24342s
principal variation: 
K A pass 00 pass J A pass 00 pass 3 J A 

.
.
.

bestmove: K
```

对手变招，输入
```bash
    run AA003 KQ JJ
```

引擎输出：
```bash
depth 5
score: 29995, time: 4.6002e-05s
principal variation: 
AA pass 00 pass 3 

.
.
.

bestmove: AA
```

从而赢得游戏。

