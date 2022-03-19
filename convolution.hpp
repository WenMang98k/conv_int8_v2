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

#define IFM_MAX      64
#define OFM_MAX      64
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

	int RC_block_num;
	int RC_block_size;
	int RC_real_size;
	int RC_out_size;

	int CHI_block_num;
	int CHI_block_size;
	int CHI_real_size;

	int CHO_block_num;
	int CHO_block_size;
	int CHO_real_size;

	int kernel;
	int stride;
	float scaler;
	bool is_yolo;
};

void convolution(
		Layer_Type layer,

		AXI_VAL_I * str_in_0,  // HP0
		AXI_VAL_I * str_in_1,  // HP1

		AXI_VAL_O * str_out_0,  // HP2
		AXI_VAL_O * str_out_1);  // HP3
