# 8b定点卷积


***2021***.**08**.__26__ _haha_ // ~~write~~ in [how use markdown](https://www.youtube.com/watch?v=EigxHkpqJdA "需要用到翻墙软件") 


## 指针数据传输

- 用了四个DMA，两个输入，两个输出

### 为什么用指针

- 之前的streamyoubug不通过
- 利用师兄的开发经验

### 用指针需要做哪些

1. 子模块的建立
2. 数据复用与重排

*** 

## 2022.03.28  第二次综合

主要改变了一些循环的tripout和pipeline。综合结果见图1<br>
![img02](https://github.com/BintaoWang/conv_int8_v2/blob/master/result/64-64-16-64sulotion2.jpg "第二次综合结果")

 |     |    bram  |      dsp    |
 | :----------   |  :---------:  | ---------------: |
 |   total  |  706       |  653  |
 |available|1824|2520|
 |utilization   |  38      |25       | 

`hello world`

```c++
#include<iostream>
using namespace std;
int main(){
        cout<<"hello world"<<endl;
}
```





# markdown 

## 测试引用

>澳门首家线上赌场、美女荷官在线发牌

### 热爱生命

 > 我不去想， 
 > 
 > 是否能够成功 ，


 






[img02]:https://cdn-media-1.freecodecamp.org/images/1*D3L--z7Mx3-LqL9o6sbUgQ.png "测试文章内引用"

