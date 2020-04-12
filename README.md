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
score: 29985, time: 0.00353555s
principal variation: 
K A pass 00 pass Q A pass 3 pass 0 J A pass 0 
bestmove: K
```
![](https://github.com/SimonFang1/PokerRaceAI/blob/master/doc/2.png?raw=true)

对手变招，输入
```bash
run AA003 KQ JJ
```

引擎输出：
```bash
score: 29995, time: 6.8366e-05s
principal variation: 
AA pass 00 pass 3 
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
score: 29979, time: 0.0590824s
principal variation: 
450000 pass 33 pass 7 J pass Q pass Q pass K pass 2 pass 2 pass C pass 5 J 
bestmove: 450000
```

## 用例3

![](https://github.com/SimonFang1/PokerRaceAI/blob/master/doc/4.png?raw=true)


启动引擎输入：

```bash
run 2KK9988765444 AAKK000765543
```

引擎输出：

```bash
score: 29979, time: 0.302796s
principal variation: 
444KK 55000 pass KK pass AA pass 3 5 7 8 pass 99 pass 6 pass 7 pass 8 pass 2 
bestmove: 444KK
```


## 用例4

稍复杂的局面

![](https://github.com/SimonFang1/PokerRaceAI/blob/master/doc/5.png?raw=true)


```bash
run 2AQQJ8555433 Cc2AKKJ099764

depth 6
score: 171, time: 0.232574s
principal variation: 
33555 cC pass 99 QQ pass pass 

depth 7
score: 366, time: 1.67943s
principal variation: 
33555 cC pass 99 QQ KK pass 

depth 8
score: 18, time: 6.1941s
principal variation: 
33555 cC pass 99 QQ KK pass 4 

score: 29975, time: 6.19417s
principal variation: 
4555 pass 33 KK pass 4 J A 2 c pass 6 Q 2 pass 99 pass 7 Q C pass 0 A pass 8 
bestmove: 4555
```

## 其他复杂局面

```bash
run cKKQQJ0995533 2AAK008866443

depth 6
score: 533, time: 0.103042s
principal variation: 
3 6 9 K c pass 

depth 7
score: 1000, time: 0.652943s
principal variation: 
3 4 5 6 0 K pass 

depth 8
score: 466, time: 2.70239s
principal variation: 
3 4 5 0 K A pass 66 

depth 9
score: 905, time: 14.1309s
principal variation: 
3 6 9 K c pass 0 A pass 

depth 10
score: 444, time: 58.6331s
principal variation: 
3 4 5 6 0 K c pass 9 pass 

depth 11
score: 550, time: 110.881s
principal variation: 
33 44 QQ AA pass 66 99 00 KK pass 55 

score: 29965, time: 110.881s
principal variation: 
3 6 0 2 c pass 3 K pass 6 9 pass QQ pass 9 pass J A pass 3 pass 8 pass 8 pass 4 5 0 K A pass 4 K pass 5 
bestmove: 3
```
