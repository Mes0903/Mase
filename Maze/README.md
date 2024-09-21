# C++ 迷宮
會叫 Mase 的原因是因為我叫 Mes

# 工具

1. C++
2. Qt

我使用 MVC 架構來實作，將整個程式切成三個部分：迷宮(Maze)、顯示視窗(MainWindow)與控制器(Animator)。

影片[連結](https://www.youtube.com/watch?v=hkr8zXTF1jI&ab_channel=Mes)

# 生成迷宮的方法：

## 1. Random Prim

一開始先把地圖初始化成這樣

![](https://i.imgur.com/6fr5Lez.png)

0 代表牆壁，1 代表陸地，之後隨機找一點標記起來

![](https://i.imgur.com/NRhfyE5.png)

將周遭的牆加入判斷的依據，之後隨機選一個牆跳過去，像跳格子那樣。

![](https://i.imgur.com/1Io1X7M.png)

每次都隨機選一個點，隨機挑一個牆跳過去，如果跳過去發現是已經走過的路，那就把這個牆壁留著，如果是還沒走過的路，那就把這個牆壁打掉，持續到整個迷宮的格子都被走過。

我的作法是建兩個 deque，一個負責將地圖生成時產生的走過的格子恢復成陸地，一個是目前有的牆壁，每次隨機挑一個牆壁判斷，上下左右，有沒有可以跳過去ㄉ。

這種方法生成的迷宮會非常的曲折，但主路不會這麼長。

![](https://i.imgur.com/BRVUjC9.png)

另外還可以自己決定要不要隨機把牆打掉，以此來製作多條可以到終點的路。

![](https://i.imgur.com/Sdg0l2J.png)


## 2. Recursive Backtracker

一開始先把地圖初始化成這樣

![](https://i.imgur.com/6fr5Lez.png)

0 代表牆壁，1 代表陸地，之後隨機找一點標記起來

![](https://i.imgur.com/qQutbyk.png)

然後隨機開始走迷宮，把牆壁打通

![](https://i.imgur.com/5GfOAJk.png)

當走到不行再走的時候就往回

![](https://i.imgur.com/6bTiPdz.png)
圖片[連結](https://zhuanlan.zhihu.com/p/27381213)

如此一來就可以建出一個迷宮了。

我的作法是建一個 deque 來存一開始隨機走的路徑，走的路徑設成已走過的路徑(紅色)，走完整個迷宮後再將這裡面的 Node 用回成陸地(黃色)。

迷宮中的每個 Node 我使用一個 Struct 來存取，每個 Node 裡面的資訊有他的 x, y 的座標，還有一個 vector 存要走的方向(上下左右)，每次建 Node 的時候會打亂這個 vector，這樣他就會亂走ㄌ，之後還有一個 index，存現在走到哪個方向ㄌ。之後就利用 stack 和 while 迴圈來生成迷宮。

這種方法生成出來的迷宮會有一條明顯的主路。

![](https://i.imgur.com/cdGvPE1.png)

## 3. Recursive Division

一開始先把地圖初始化成這樣

![](https://i.imgur.com/6cnV6Le.png)

之後判斷目前的 x、y 哪邊比較長，如果 x 比較長就切直的，像這樣:

![](https://i.imgur.com/y7AjfJG.png)

如果 y 比較長就切橫的。之後以剛剛分割出來的左右或上下的區域，進入遞迴不停判斷，直到不能再切的時後返回，像這樣：

![](https://i.imgur.com/PTGw2r7.png)

當同一個方向都切完的時候開始打牆，可以自己決定要打幾面牆，我是只打一面牆，最後就會長得像這樣：

![](https://i.imgur.com/TPNFA1S.png)

這種方法生成出來的迷宮會有很多明顯的區塊，複雜度要看妳打的牆數來決定。

# 解迷宮的方法

## 1. DFS

從起點開始走，走到不符合條件的 Node 時返回上一層，簡單來說就是走到不能在走時就往回：

![](https://i.imgur.com/aRBvdUa.jpg)

## 2. BFS

從起點開始走，上下左右都先走一遍，簡單來說就是讓目前每個節點都走一步，如此循環：

![](https://i.imgur.com/UMy7qP7.jpg)

BFS 可以找到最佳解

## 3. UCS

一開始會先建一個 Priority Queue，這種 Queue 會將妳丟入的資料排序，要由小到大還是由大到小看妳怎麼設計。

我用 struct 來實作每一個 Node，每個 Node 裡面有他的 x,y 座標，還有一個 weight 的數值，用來供 Priority Queue 排序，由於是 UCS，因此 weight 的計算只有一個 Past function，Past function 等於每次走訪時都會將此次的 Cost 加上過去累計的 Cost，而 Cost 可以自己設，不同的 Cost 會有不同的效果。

![](https://i.imgur.com/zJ8sjf8.jpg)

此次我做了三種 Cost，分別是「到終點的曼哈頓距離」、「到終點的 Tow Norm 平方」、「區間」，可以到影片那邊看以空白地圖來跑的效果。

區間我是將長、寬切10等分，因此就會有十層階層區域，越外層的 Cost 越高。

## 4. GREEDY

與 UCS 很像，一開始一樣會先建一個 Priority Queue，並一樣以 struct 來實作每一個 Node，每個 Node 裡面有他的 x,y 座標，還有一個 weight 的數值，用來供 Priority Queue 排序。

GREEDY 的 weight 的計算方法與 UCS 不同，它的權重是不累加的，也就是說節點的權重與過去無關，因此較可能會有一條路走下去的情況(也有可能不會啦)。 這種權重的計算方式我們叫他 Heuristic function。

![](https://i.imgur.com/nYLnGFI.jpg)

這次我的 Heuristic function 設的是到終點的 Two Norm 平方。

## 5. A\*

和前面兩個一樣，一開始會先建一個 Priority Queue，並一樣以 struct 來實作每一個 Node，每個 Node 裡面有他的 x,y 座標，還有一個 weight 的數值，用來供 Priority Queue 排序。

A\* 的 weight 的計算方法是 Past Function + Heuristic Function。

![](https://i.imgur.com/ge4denq.jpg)

這次我做了兩種組合

第一種組合：
Past Function ： Cost = 50
Heuristic Function ： 到終點的曼哈頓距離

第二種組合：
Past Function ： Cost 以區間計算
Heuristic Function ： 到終點的 Two Norm 平方
