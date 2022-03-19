分支：dev

# 8bits定点卷积

## 设计思路
整个加速器的设计思路可分为五步，即为加载输入特征图、加载偏置和权重、进行macc密集计算、进行激活反量化操作、传出输出特征图。这五步分别对应五个函数load_ifm|load_ibw|macc|last_proc|transfer_ofm。<br>

### 1.load_ifm
- 函数原型void load_ifm(int RC_real_size,  AXI_VAL_I *str_in_0, dtype ifm[IFM_MAX + 2][IFM_MAX + 2][TN])；<br>
- 输入参数<br>
> - int RC_real_size：输入特征图的宽高，这里***要求特征图为正方形***<br>
> - AXI_VAL_I *str_in_0：输入的AXI_stream，这里采用指针的形式<br>
> - dtype ifm[IFM_MAX + 2][IFM_MAX + 2][TN]：输入特征图的缓存池<br>



*** 

##  工作日志
***2021***.**08**.__26__ _haha_ // ~~write~~ in [how use markdown](https://www.youtube.com/watch?v=EigxHkpqJdA "需要用到翻墙软件") 
## 2022.03.18  第二次综合
```c++
#define IFM_MAX      64
#define OFM_MAX      64
#define KERNEL    3
#define TN  16
#define TM 64
```
主要改变了一些循环的tripout和pipeline。综合结果见图1<br>
![img02](https://github.com/BintaoWang/conv_int8_v2/blob/master/result/64-64-16-64sulotion2.jpg "第二次综合结果")

 |     |    bram  |      dsp    |
 | :----------   |  :---------:  | ---------------: |
 |   total  |  706       |  653  |
 |available|1824|2520|
 |utilization   |  38      |25       | 

## 2022.03.19  第三次综合
出于第二次 综合结果对fpga资源的应用不够充分，因此基于第二次结果对加速器进行优化，以望提高资源利用效率 。







*** 



# markdown 

## 测试引用

>澳门首家线上赌场、美女荷官在线发牌

### 热爱生命

 > 我不去想， 
 > 
 > 是否能够成功 ，


 






[img02]:https://cdn-media-1.freecodecamp.org/images/1*D3L--z7Mx3-LqL9o6sbUgQ.png "测试文章内引用"

