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
#define OFM_MAX      64
#define KERNEL    3
#define TN  64
#define TM 32

#define BIT_WIDH  7
#define MUL_BIT_WIDTH 16

typedef signed char dtype;
typedef ap_int<MUL_BIT_WIDTH>  mul_type;
typedef ap_int<MUL_BIT_WIDTH+1>  acc_type0;
typedef ap_int<MUL_BIT_WIDTH+2>  acc_type1;
typedef ap_int<MUL_BIT_WIDTH+3>  acc_type2;
typedef ap_int<MUL_BIT_WIDTH+4>  acc_type3;
typedef ap_int<MUL_BIT_WIDTH+5>  acc_type4;
typedef ap_int<MUL_BIT_WIDTH+6>  acc_type5;

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
	int RC_block_size; // 指子模型的输入宽高,填充后的维度
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
	float pscaler;  // 反量化因子
	bool is_yolo;  // 是否是输出层，输出层的话，不做relu

	float nscaler;  // 反量化因子
	Layer_Type(int id,
			int rbn,int rbs,int rrs,int ros,
			int ibn,int ibs,int irs,
			int obn,int obs,int ors,
			int k,int s,float sc,bool i):
		 layer_id(id),

		 RC_block_num(rbn),
		 RC_block_size(rbs),
		 RC_real_size(rrs),
		 RC_out_size(ros),

		 CHI_block_num(ibn),
		 CHI_block_size(ibs),
		 CHI_real_size(irs),

		 CHO_block_num(obn),
		 CHO_block_size(obs),
		 CHO_real_size(ors),

		 kernel(k),
		 stride(s),
		 pscaler(sc),
		 is_yolo(i)
	{
		 nscaler=sc*0.1;
	};

};

void convolution(
		Layer_Type layer,

		AXI_VAL_I * str_in_0,  // HP0
		AXI_VAL_I * str_in_1,  // HP1

		AXI_VAL_O * str_out_0,  // HP2
		AXI_VAL_O * str_out_1);  // HP3
