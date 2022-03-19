#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <cmath>
#include <ap_int.h>
#include <ap_axi_sdata.h>
#include <hls_stream.h>

using namespace std;
using namespace hls;

#define IFM_MAX      66
#define OFM_MAX      66
#define KERNEL    3
#define TN  32
#define TM 64

typedef signed char dtype;

union s8_s32
{
	signed char union_s8[4];
	signed int union_s32;
};

struct AXI_VAL_I{
	dtype data[16];
};

struct AXI_VAL_O{
	dtype data[16];
	bool last;
};

struct BindingType{
	dtype data[16];
};

struct Layer_Type{
	int layer_id;

	int RC_block_num; // 指宽高 分块系数
	int RC_block_size; // 指子模型的输入宽高
	int RC_real_size; // 指子模型在实际模型中对应的宽高
	int RC_out_size; // 指子模型的输出宽高，主要受卷积步长的影响

	int CHI_block_num;   // 实际模型对应子模型输入通道的分块系数
	int CHI_block_size; // 子模型在实际模型中的对应的输入通道数,需要是16的倍数
	int CHI_real_size;  // 一般情况下等于CHI_block_size

	int CHO_block_num;  // 实际模型对应子模型输出通道的分块系数
	int CHO_block_size;  //  子模型在实际模型中的对应的输出通道数
	int CHO_real_size;  //   一般情况下等于CHO_block_size

	int kernel;  // 卷积核的尺寸，1或者3
	int stride;  // 卷积核的步长，1或者2
	float scaler;  // 反量化因子
	bool is_yolo;  // 是否是输出层，输出层的话，不做relu
};

void convolution(
		Layer_Type layer,

		AXI_VAL_I * str_in_0,  // HP0
		AXI_VAL_I * str_in_1,  // HP1

		AXI_VAL_O * str_out_0,  // HP2
		AXI_VAL_O * str_out_1);  // HP3
