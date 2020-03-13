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

## 用例1

![](https://github.com/SimonFang1/PokerRaceAI/blob/master/doc/1.png?raw=true)

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
![](https://github.com/SimonFang1/PokerRaceAI/blob/master/doc/2.png?raw=true)

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

## 用例2

![](https://github.com/SimonFang1/PokerRaceAI/blob/master/doc/3.png?raw=true)

启动引擎输入：

```bash
run J000075433 C22KQQJ965
```

引擎输出：

```bash
depth 5
score: -320, time: 0.0290819s
principal variation: 
450000 pass 33 QQ pass 

depth 6
score: -488, time: 0.0898888s
principal variation: 
450000 pass 33 QQ pass 5 

depth 7
score: -311, time: 0.443217s
principal variation: 
450000 pass 33 QQ pass J pass 

depth 8
score: -500, time: 1.06018s
principal variation: 
450000 pass 33 QQ pass J pass 5 

depth 9
score: -200, time: 4.531s
principal variation: 
450000 pass 33 QQ pass 9 J 2 pass 

depth 10
score: -400, time: 10.6756s
principal variation: 
450000 pass 33 QQ pass J pass 2 pass 5 

depth 11
score: -100, time: 41.0669s
principal variation: 
450000 pass 33 QQ pass 5 7 J pass 2 pass 

depth 12
score: -200, time: 94.9332s
principal variation: 
450000 pass 33 QQ pass 5 7 J pass 2 pass 6 

depth 13
score: 0, time: 325.606s
principal variation: 
450000 pass 33 QQ pass 5 7 J pass K pass 2 pass 

depth 14
score: -100, time: 732.227s
principal variation: 
450000 pass 33 QQ pass 5 7 J pass K pass 2 pass 6 

depth 15
score: 100, time: 2415.43s
principal variation: 
450000 pass 33 QQ pass 5 7 J pass K pass 2 pass 2 pass
```

输入 `s` 停止搜索， 输出 `bestmove： 450000`

输入:
```bash
run J733 C22KQQJ965
```

引擎输出：

```bash
depth 5
score: -311, time: 0.00109556s
principal variation: 
33 QQ pass J pass 

depth 6
score: -500, time: 0.00416641s
principal variation: 
33 QQ pass J pass 5 

depth 7
score: -200, time: 0.0138747s
principal variation: 
33 QQ pass 9 J 2 pass 

depth 8
score: -400, time: 0.0389265s
principal variation: 
33 QQ pass J pass 2 pass 5 

depth 9
score: -100, time: 0.105671s
principal variation: 
33 QQ pass 5 7 J pass 2 pass 

depth 10
score: -200, time: 0.259695s
principal variation: 
33 QQ pass 5 7 J pass 2 pass 6 

depth 11
score: 0, time: 0.65673s
principal variation: 
33 QQ pass 5 7 J pass K pass 2 pass 

depth 12
score: -100, time: 1.52733s
principal variation: 
33 QQ pass 5 7 J pass K pass 2 pass 6 

depth 13
score: 100, time: 3.54921s
principal variation: 
33 QQ pass 5 7 J pass K pass 2 pass 2 pass 

depth 14
score: 0, time: 7.40922s
principal variation: 
33 QQ pass 5 7 J pass K pass 2 pass 2 pass 6 

depth 15
score: 200, time: 15.9773s
principal variation: 
33 QQ pass 5 7 J pass K pass 2 pass 2 pass C pass 

depth 16
score: 100, time: 30.505s
principal variation: 
33 QQ pass 5 7 J pass K pass 2 pass 2 pass C pass 6

depth 17
score: 300, time: 60.1327s
principal variation: 
33 pass 7 J pass Q pass Q pass K pass 2 pass 2 pass C pass
```

输入 `s` 停止搜索， 输出 `bestmove： 33`

分析对手视角输入：

```bash
run C22KQQJ965 J7 33
```
引擎输出：

```bash
depth 5
score: 500, time: 0.00045479s
principal variation: 
QQ pass J pass 5 

depth 6
score: 200, time: 0.00115632s
principal variation: 
QQ pass 9 J 2 pass 

depth 7
score: 400, time: 0.00344838s
principal variation: 
QQ pass J pass 2 pass 5 

depth 8
score: 100, time: 0.00744271s
principal variation: 
QQ pass 5 7 J pass 2 pass 

depth 9
score: 200, time: 0.0196456s
principal variation: 
QQ pass 5 7 J pass 2 pass 6 

depth 10
score: 0, time: 0.0393747s
principal variation: 
QQ pass 5 7 J pass K pass 2 pass 

depth 11
score: 100, time: 0.0854902s
principal variation: 
QQ pass 5 7 J pass K pass 2 pass 6 

depth 12
score: -100, time: 0.153101s
principal variation: 
QQ pass 5 7 J pass K pass 2 pass 2 pass 

depth 13
score: 0, time: 0.274384s
principal variation: 
QQ pass 5 7 J pass K pass 2 pass 2 pass 6 

depth 14
score: -200, time: 0.479855s
principal variation: 
QQ pass 5 7 J pass K pass 2 pass 2 pass C pass 

depth 15
score: -100, time: 0.761846s
principal variation: 
QQ pass 5 7 J pass K pass 2 pass 2 pass C pass 6 

depth 16
score: -300, time: 0.857873s
principal variation: 
pass 7 J pass Q pass Q pass K pass 2 pass 2 pass C pass 

depth 17
score: -200, time: 0.973115s
principal variation: 
pass 7 J pass Q pass Q pass K pass 2 pass 2 pass C pass 5 

depth 18
score: -29982, time: 1.02085s
principal variation: 
pass 7 J pass Q pass Q pass K pass 2 pass 2 pass C pass 5 J 

```

发现对手已经必败。

## 用例3

稍复杂的局面

![](https://github.com/SimonFang1/PokerRaceAI/blob/master/doc/4.png?raw=true)


启动引擎输入：

```bash
run 2KK9988765444 AAKK000765543
```

引擎输出：

```bash
depth 5
score: 1458, time: 0.0643409s
principal variation: 
56789 pass 444 pass 8 

depth 6
score: 976, time: 0.295891s
principal variation: 
56789 pass 2 pass 444 pass 

depth 7
score: 1050, time: 1.39318s
principal variation: 
9 0 2 pass 45678 pass 44 

depth 8
score: 837, time: 5.33497s
principal variation: 
56789 pass 8 pass 2 pass 444 pass 

depth 9
score: 714, time: 22.3487s
principal variation: 
56789 pass 8 K 2 pass 444 pass 9 

depth 10
score: 163, time: 92.7462s
principal variation: 
8 K 2 pass 456789 pass 44 00 KK pass 

depth 11
score: 200, time: 329.773s
principal variation: 
8 K 2 pass 456789 pass 44 AA pass 34567 pass 

depth 12
score: 100, time: 1375.11s
principal variation: 
444KK 55000 pass K 2 pass 56789 pass 8 A pass 3 

depth 13
score: 466, time: 4980.95s
principal variation: 
444KK 55000 pass K pass A 2 pass 6 7 8 pass 5 
```

深度优先搜索`444KK`，分析发现对手只能出招`55000`，之后陷入必败局面。

## 用例4

复杂的局面

![](https://github.com/SimonFang1/PokerRaceAI/blob/master/doc/5.png?raw=true)

分析双方所需要的最小轮次：

|先手|单牌| 对子| 其他|
|:--|--:|:--|:--|
|**我方**|8   J A 2|33 QQ|5554|
|对手|4 6 7 0 J A 2|99 KK|Cc| 

注意到对手单牌较多，如果我方出了`5554`，对方选择出王炸的话，会导致轮次落后输掉局面。

|先手 |单牌| 对子|
|:--|--:|:--|
|我方|8   J A 2|33 QQ|
|**对手**|4 6 7 0 J A 2|99 KK|

对方继续出`99`，保持先手，但还是必败
|先手|单牌| 对子|
|:--|--:|:--|
|我方|8   J A 2|33|
|**对手**|4 6 7 0 J A 2|

可以用引擎验证：

```bash
run 2AJ0764 2AJ833

depth 12
score: -29988, time: 0.636052s
principal variation: 
4 8 0 J 2 pass 6 A pass 33 pass 2  
```

因此我方出了`5554`这种小牌后，对手选择不出。这是最顽强的抵抗，把大小王拆成单牌出，可以尽可能多的保持先手。

|先手|单牌| 对子|
|:--|--:|:--|
|**我方**|8   J A 2|33 QQ|
|对手|4 6 7 0 J A 2 c C|99 KK|

注意到我方最小的牌是`33`,大牌不占优，应利用最后一次先手机会出掉，如果对方选择出`99`的话，双方都能把对子出光。由于我方的单牌`8`压着对手的`4 6 7`，这一局面对方是必败的。所以对方选择先出`KK`。

|先手|单牌| 对子|
|:--|--:|:--|
|我方|8   J A 2|QQ|
|**对手**|4 6 7 0 J A 2 c C|99|

对手先出`4`，我方最后进入必胜局面


```bash
run 2AQQJ8 Cc2AJ09976 4

depth 17
score: 133, time: 5098.32s
principal variation: 
8 0 J A 2 c pass 6 A 2 pass 7 pass J Q C pass 

depth 18
score: -100, time: 10521.6s
principal variation: 
J A 2 c pass 6 Q 2 pass 99 pass 7 Q C pass 0 A pass 

depth 19
score: 29981, time: 10522.2s
principal variation: 
J A 2 c pass 6 Q 2 pass 99 pass 7 Q C pass 0 A pass 8 
```

