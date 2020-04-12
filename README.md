# PokerRaceEngine
双人明牌斗地主

仿照象棋类AI，利用[alpha-beta搜索](http://www.xqbase.com/computer/search_alphabeta.htm)，求解最优出牌策略。

## 安装

cmake 编译运行

```bash
mkdir build
cd build
cmake ..
make -j2
./battlewithlandlord
```

## 引擎命令

|命令|说明|
|:--|:--|
|`r[un] mycard opponentcard [ last_pattern ]`|开始搜索 |
|`s[top]`|停止搜索 |
|`m[ove] [pattern]`|出牌 |
|`q[uit]`|退出 |

牌面用字符串表示，一张牌用一个字符表示，大小写敏感，对应关系如下：

|3|4|5|6|7|8|9|10|J|Q|K|A|2|joker|Joker|
|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|
|'3'|'4'|'5'|'6'|'7'|'8'|'9'|'0'|'J'|'Q'|'K'|‘A’|'2'|'c'|'C'|

牌面之间用空格隔开，支持任意排列顺序。

空字符串或`"pass"` 表示不出。

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

最后获得胜利。


也可以通过输入 move 出牌，引擎会自动切换视角开始搜索。

```bash
run AAK003 AKQJJ00

depth 6
score: 40, time: 0.00270049s
principal variation: 
00 JJ AA pass 3 Q 

score: 29985, time: 0.00280196s
principal variation: 
K A pass 00 pass Q A pass 3 pass 0 J A pass 0 
bestmove: K

move K

depth 6
score: 225, time: 0.00113703s
principal variation: 
A pass 00 pass J pass 

score: -29986, time: 0.00121695s
principal variation: 
A pass 00 pass Q A pass 3 pass 0 J A pass 0 
bestmove: A

m A

depth 6
score: -142, time: 0.000682424s
principal variation: 
pass 00 pass J pass J 

score: 29987, time: 0.000729094s
principal variation: 
pass 00 pass Q A pass 3 pass 0 J A pass 0 
bestmove: pass

m pass

depth 6
score: 0, time: 0.000618376s
principal variation: 
00 pass J A pass 00 pass 

score: -29988, time: 0.000686211s
principal variation: 
00 pass Q A pass 3 pass 0 J A pass 0 
bestmove: 00

m JJ

depth 6
score: 29995, time: 0.00013174s
principal variation: 
AA pass 00 pass 3 

score: 29995, time: 0.000589385s
principal variation: 
AA pass 00 pass 3 
bestmove: AA
```

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


```bash
run 22J99876654 c2AQ00766543

depth 6
score: 320, time: 0.0716138s
principal variation: 
456789 pass 6 2 pass 34567 pass 

depth 7
score: 320, time: 0.500095s
principal variation: 
456789 pass 6 2 pass 34567 pass 

depth 8
score: 20, time: 2.10745s
principal variation: 
45678 pass 99 00 22 pass 6 7 

depth 9
score: 220, time: 9.33624s
principal variation: 
45678 pass 99 00 22 pass 6 Q pass 

depth 10
score: 0, time: 30.2344s
principal variation: 
7 0 2 Q 

score: 29969, time: 30.2345s
principal variation: 
6 7 9 0 J 2 pass c pass 4 5 pass 6 pass 4 pass 7 pass 22 pass 8 0 pass 66 pass Q pass A pass 3 9 
bestmove: 6
```


```bash

run AAKKQQ0008633 CcAK997655444

depth 6
score: 400, time: 0.574094s
principal variation: 
33 cC pass 44455 000QQ pass 

depth 7
score: 640, time: 2.04485s
principal variation: 
KK pass 33 cC pass 44455 pass 

depth 8
score: 233, time: 13.0908s
principal variation: 
QQ pass 33 55 00 cC pass 4446 pass 

depth 9
score: 400, time: 43.1871s
principal variation: 
33 55 QQ pass 6000 cC pass 4446 pass 

depth 10
score: 0, time: 51.3442s
principal variation: 
33000 pass AA cC 

score: 29969, time: 51.3442s
principal variation: 
A c pass 9 Q K A C pass 9 0 pass 8 A pass 4446 pass 7 0 pass KK pass 3 5 6 pass 0 pass Q pass 3 
bestmove: A
```

```bash
run C208776554443 cAKQJJ009863

depth 6
score: -519, time: 0.240666s
principal variation: 
44477 pass 55 00 pass 3 pass 

depth 7
score: -120, time: 2.53895s
principal variation: 
44477 pass 55 00 pass J pass 

depth 8
score: -514, time: 7.47996s
principal variation: 
44477 pass 55 00 pass J pass 3 

depth 9
score: -107, time: 55.3702s
principal variation: 
44477 pass 55 00 pass 8 0 J pass 

depth 10
score: -342, time: 164.755s
principal variation: 
345678 890JQK pass 3 5 6 7 0 pass J 

depth 11
score: 0, time: 433.055s
principal variation: 
345678 890JQK pass 3 7 0 C pass 44 pass 5 

score: 29963, time: 433.056s
principal variation: 
3 6 8 9 0 J pass 8 pass J pass Q pass K pass A pass 3 4 c C pass 6 pass 4 pass 4 pass 5 pass 5 pass 7 0 2 pass 7 
bestmove: 3
```