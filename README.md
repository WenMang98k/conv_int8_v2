分支：dev

# 8bits定点卷积
```c++
typedef signed char dtype;
```
## 设计思路
加速器采用卷积层分块计算，子模型的最大输入为<br>
- IFM_MAX： 输入特征图的最大维度<br>
- KERNEL：卷积核的最大维度<br>
- TN：子模型的最大输入通道<br>
- TM：子模型的最大输出通道<br>
- OFM_MAX： 输出特征图的最大维度<br>
> ifm[IFM_MAX + 2][IFM_MAX + 2][TN]<br>
> weights[KERNEL][KERNEL][TN][TM]<br>
> ofm_mpq[OFM_MAX][OFM_MAX][TM]    bias[TM] <br>
- 卷积层结构体的定义<br>
```c++
struct Layer_Type{
	int layer_id;  //层id
	int RC_block_num; // 指宽高分块系数
	int RC_block_size; // 指子模型的输入宽高，最大为IFM_MAX: 
	int RC_real_size; // 指子模型在实际模型中对应的宽高 （这个参数是否需要）
	int RC_out_size; // 指子模型的输出宽高，主要受卷积步长的影响，最大为OFM_MAX： [计算方式参考](https://blog.csdn.net/FiveStarsGeneral/article/details/103306573)
	int CHI_block_num;   // 实际模型对应子模型输入通道的分块系数
	int CHI_block_size; // 子模型在实际模型中的对应的输入通道数,需要是32的倍数，因为在macc里面将输入通道按照32进行展开计算
	int CHI_real_size;  // 一般情况下等于CHI_block_size
	int CHO_block_num;  // 实际模型对应子模型输出通道的分块系数
	int CHO_block_size;  //  子模型在实际模型中的对应的输出通道数
	int CHO_real_size;  //   一般情况下等于CHO_block_size
	int kernel;  // 卷积核的尺寸，1或者3
	int stride;  // 卷积核的步长，1或者2
	float scaler;  // 反量化因子
	bool is_yolo;  // 是否是输出层，输出层的话，不做relu
};
```
整个加速器的设计思路可分为五步，即为加载输入特征图、加载偏置和权重、进行macc密集计算、进行激活反量化操作、传出输出特征图。这五步分别对应五个函数load_ifm|load_ibw|macc|last_proc|transfer_ofm。<br>


### 1.load_ifm
- 函数原型void load_ifm(int RC_block_size, int CHI_block_size, AXI_VAL_I *str_in_0, dtype ifm[IFM_MAX + 2][IFM_MAX + 2][TN])；<br>
- 输入参数<br>
> - int RC_block_size：块输入特征图的宽高，这里***要求特征图为正方形***<br>
> - int CHI_block_size: 子模型的输入通道<br>
> - AXI_VAL_I *str_in_0：输入的AXI_stream，这里采用指针的形式<br>
> - dtype ifm[IFM_MAX + 2][IFM_MAX + 2][TN]：输入特征图的缓存池<br>

### 2.load_ibw
- 函数原型void load_ibw(int kernel, int CHI_block_size, int CHO_real_size, AXI_VAL_I *str_in_1, int bias[TM], dtype weights[KERNEL][KERNEL][TN][TM])<br>
- 输入参数<br>
> - int kernel：卷积核的维度，***这里一般取3***<br>
> - int CHI_block_size: 子模型的输入通道<br>
> - int CHO_real_size：子模型的输出通道的<br>
> - AXI_VAL_I *str_in_1：偏置和权重的AXI_stream，这里采用指针的形式<br>
> - int bias[TM]：偏置的缓存池<br>
> - dtype weights[KERNEL][KERNEL][TN][TM]：权重的缓存池<br>

### 3.macc
- 函数原型 void macc(int kernel, int RC_out_size,  int stride,  int channel_in_iter, int CHI_block_size,  int CHO_block_size,  dtype ifm[IFM_MAX + 2][IFM_MAX + 2][TN], dtype weights[KERNEL][KERNEL][TN][TM], int bias[TM], int ofm[OFM_MAX][OFM_MAX][TM]);<br>
- 输入参数<br>
> - int kernel：卷积核的维度，***这里一般取3***<br>
> - int RC_out_size：块输出特征图的宽高<br>
> - int channel_in_iter: 输入通道上的分块索引<br>
> - int CHI_block_size: 子模型的输入通道<br>
> - int CHO_real_size：子模型的输出通道的<br>
> - AXI_VAL_I *str_in_1：偏置和权重的AXI_stream，这里采用指针的形式<br>
> - dtype weights[KERNEL][KERNEL][TN][TM]：权重的缓存池<br>
> - int bias[TM]：偏置的缓存池<br>
> - int ofm[OFM_MAX][OFM_MAX][TM]：输出特征图缓存池，此时的数据格式为int<br>

### 4.last_prc
- 函数原型void last_proc(bool is_yolo,float scaler,int RC_out_size,  int CHO_block_size,int ofm[OFM_MAX][OFM_MAX][TM],dtype ofm_mpq[OFM_MAX][OFM_MAX][TM])；<br>
- 输入参数<br>
> - bool is_yolo：是否是输出层，输出层的话，不做relu<br>
> - float scaler;  // 反量化因子<br>
> - int RC_out_size：块输出特征图的宽高<br>
> - int CHO_real_size：子模型的输出通道的<br>
> - int ofm[OFM_MAX][OFM_MAX][TM]：输出特征图缓存池，此时的数据格式为int<br>
> - dtype ofm_mpq[OFM_MAX][OFM_MAX][TM]：输出特征图缓存池，此时的数据格式为dtype<br>

### 5.transfer_ofm
- 函数原型void transfer_ofm(Layer_Type layer,int row_iter, int col_iter, int channel_out_iter,dtype ofm_mpq[OFM_MAX][OFM_MAX][TM],AXI_VAL_O * str_out_0, AXI_VAL_O * str_out_1)；<br>
- 输入参数<br>
> - Layer_Type layer：当前层信息<br>
> - int row_iter：行分块索引<br>
> - int col_iter：列分块索引<br>
> - int channel_out_iter：输出通道分块索引<br>
> - dtype ofm_mpq[OFM_MAX][OFM_MAX][TM]：输出特征图缓存池，此时的数据格式为dtype<br>
> - AXI_VAL_I *str_out_0：输出特征图的AXI_stream，这里采用指针的形式，将前一半输出通道的数据传出<br>
> - AXI_VAL_I *str_out_1：输出特征图的AXI_stream，这里采用指针的形式，将后一半输出通道的数据传出<br>

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

## 2022.03.19-20 第三次综合
出于第二次 综合结果对fpga资源的应用不够充分，因此基于第二次结果对加速器进行优化，以望提高资源利用效率 。
```c++
#define IFM_MAX      66
#define OFM_MAX      66
#define KERNEL    3
#define TN  32
#define TM 64
```

# bug汇总
- 对于HLS调试器堆栈溢出问题的解决办法。在linker flags处手动输入
-Wl,--stack,10485760 ![ug902资料](https://github.com/BintaoWang/conv_int8_v2/blob/dev/result/hl%E5%A0%86%E6%A0%88%E6%BA%A2%E5%87%BA%E8%A7%A3%E5%86%B3%E5%8A%9E%E6%B3%95.png)


*** 
