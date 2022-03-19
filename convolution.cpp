#include "convolution.hpp"

// load ifm padding data from AXI4-Stream to BRAM
// 按照[row][col][channel_in] channel_in->col->row
void load_ifm(
		int RC_real_size,  // layer.RC_real_size
		AXI_VAL_I *str_in_0,// 8bit * 16 =128
		dtype ifm[IFM_MAX + 2][IFM_MAX + 2][TN])
{
	load_ifm_label0:for(int i = 0; i < RC_real_size + 2; i ++){
#pragma HLS LOOP_TRIPCOUNT min=66 max=66
		for(int j = 0; j < RC_real_size + 2; j ++){
#pragma HLS LOOP_TRIPCOUNT min=66 max=66
#pragma HLS PIPELINE

			AXI_VAL_I input = *str_in_0;
			str_in_0 ++;

			for(int k = 0; k < TN; k ++){
#pragma HLS LOOP_TRIPCOUNT min=16 max=16
//#pragma HLS UNROLL
				ifm[i][j][k] = input.data[k];
			}
		}
	}
}

// load bias and weights data from AXI-4_Stream to DRAM
void load_ibw(
		int kernel,  // layer.kernel
		int CHO_real_size,  // layer.CHO_real_size
		AXI_VAL_I *str_in_1,
		int bias[TM],
		dtype weights[KERNEL][KERNEL][TN][TM])
{
	AXI_VAL_I input;

	load_bias:
	for(int i = 0; i < CHO_real_size / 4; i ++){  // TM * 32 / 128
#pragma HLS LOOP_TRIPCOUNT min=16 max=16
#pragma HLS PIPELINE
		input = *str_in_1;
		str_in_1 ++;

		for(int j = 0; j < 4; j ++){  // 一次传进来128bit，每32bit为一个bias，即这里传进来bias 分为4*8bit
#pragma HLS LOOP_TRIPCOUNT min=4 max=4
//#pragma HLS UNROLL
			union s8_s32 union_t;
			union_t.union_s8[0] = input.data[j * 4];
			union_t.union_s8[1] = input.data[j * 4 + 1];
			union_t.union_s8[2] = input.data[j * 4 + 2];
			union_t.union_s8[3] = input.data[j * 4 + 3];
			bias[i * 4 + j] = union_t.union_s32;
		}
	}
	//按照[kernal_x][kernal_y][channel_in][channel_out] channel_in->chaneel_out->kernal_y->kernal_x
	load_weights:
	for(int i = 0; i < kernel; i ++){
#pragma HLS LOOP_TRIPCOUNT min=3 max=3
		for(int j = 0; j < kernel; j ++){
#pragma HLS LOOP_TRIPCOUNT min=3 max=3
#pragma HLS PIPELINE
			for(int k = 0; k < CHO_real_size; k ++){
#pragma HLS LOOP_TRIPCOUNT min=64 max=64
				input = *str_in_1;
				str_in_1 ++;

				for(int l = 0; l < TN; l ++){
#pragma HLS LOOP_TRIPCOUNT min=16 max=16
#pragma HLS UNROLL
					weights[i][j][l][k] = input.data[l];
				}
			}
		}
	}
}

// on chip macc
void macc(
		int kernel,  // layer.kernel
		int RC_out_size,  // layer.RC_real_size
		int stride,  // layer.max_pool_control
		int channel_in_iter,
		dtype ifm[IFM_MAX + 2][IFM_MAX + 2][TN],
		dtype weights[KERNEL][KERNEL][TN][TM],
		int bias[TM],
		int ofm[OFM_MAX][OFM_MAX][TM])
{
	for(int i = 0; i < kernel; i ++){
#pragma HLS LOOP_TRIPCOUNT min=3 max=3
		for(int j = 0; j < kernel; j ++){
#pragma HLS LOOP_TRIPCOUNT min=3 max=3
			for(int k = 0; k < RC_out_size; k ++){
#pragma HLS LOOP_TRIPCOUNT min=32 max=64
				for(int l = 0; l < RC_out_size; l ++){
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=32 max=64
					for(int m = 0; m < TM; m ++){
#pragma HLS LOOP_TRIPCOUNT min=64 max=64
						int bias_res;

						if((i == 0) && (j == 0) && (channel_in_iter == 0))
							bias_res = bias[m];
						else
							bias_res = ofm[k][l][m];

						// TN = 16, compute through each channel
						int x_offset = i + k * stride;
						int y_offset = j + l * stride;

						// multiply
						int mul_res_0 = ifm[x_offset][y_offset][0] * weights[i][j][0][m];
						int mul_res_1 = ifm[x_offset][y_offset][1] * weights[i][j][1][m];
						int mul_res_2 = ifm[x_offset][y_offset][2] * weights[i][j][2][m];
						int mul_res_3 = ifm[x_offset][y_offset][3] * weights[i][j][3][m];
						int mul_res_4 = ifm[x_offset][y_offset][4] * weights[i][j][4][m];
						int mul_res_5 = ifm[x_offset][y_offset][5] * weights[i][j][5][m];
						int mul_res_6 = ifm[x_offset][y_offset][6] * weights[i][j][6][m];
						int mul_res_7 = ifm[x_offset][y_offset][7] * weights[i][j][7][m];
						int mul_res_8 = ifm[x_offset][y_offset][8] * weights[i][j][8][m];
						int mul_res_9 = ifm[x_offset][y_offset][9] * weights[i][j][9][m];
						int mul_res_10 = ifm[x_offset][y_offset][10] * weights[i][j][10][m];
						int mul_res_11 = ifm[x_offset][y_offset][11] * weights[i][j][11][m];
						int mul_res_12 = ifm[x_offset][y_offset][12] * weights[i][j][12][m];
						int mul_res_13 = ifm[x_offset][y_offset][13] * weights[i][j][13][m];
						int mul_res_14 = ifm[x_offset][y_offset][14] * weights[i][j][14][m];
						int mul_res_15 = ifm[x_offset][y_offset][15] * weights[i][j][15][m];

						// accumulation
						int acc_res_0 = mul_res_0 + mul_res_1;
						int acc_res_1 = mul_res_2 + mul_res_3;
						int acc_res_2 = mul_res_4 + mul_res_5;
						int acc_res_3 = mul_res_6 + mul_res_7;
						int acc_res_4 = mul_res_8 + mul_res_9;
						int acc_res_5 = mul_res_10 + mul_res_11;
						int acc_res_6 = mul_res_12 + mul_res_13;
						int acc_res_7 = mul_res_14 + mul_res_15;

						int acc_res_8 = acc_res_0 + acc_res_1;
						int acc_res_9 = acc_res_2 + acc_res_3;
						int acc_res_10 = acc_res_4 + acc_res_5;
						int acc_res_11 = acc_res_6 + acc_res_7;

						int acc_res_12 = acc_res_8 + acc_res_9;
						int acc_res_13 = acc_res_10 + acc_res_11;

						int macc_res = acc_res_12 + acc_res_13;

						ofm[k][l][m] = bias_res + macc_res;
					}
				}
			}
		}
	}
}

// max pool, anti-quantization, leaky-ReLu, saturation, quantization
void last_proc(
		bool is_yolo,
		float scaler,
		int RC_out_size,  // layer.RC_output_size
		int CHO_real_size,  // layer.CHO_real_size
		//int max_pool,
		//int max_pool_stride,
		int ofm[OFM_MAX][OFM_MAX][IFM_MAX],
		dtype ofm_mpq[OFM_MAX][OFM_MAX][TM])
{
	for(int i = 0; i < RC_out_size; i ++){
#pragma HLS LOOP_TRIPCOUNT min=32 max=64
		for(int j = 0; j < RC_out_size; j ++){
#pragma HLS LOOP_TRIPCOUNT min=32 max=64
			for(int k = 0; k < CHO_real_size; k ++){
#pragma HLS LOOP_TRIPCOUNT min=64 max=64
#pragma HLS PIPELINE
				int g = ofm[i][j][k];
				// anti-quantization, leaky-ReLu, saturation, quantization
				float pixel_aq;
				if((g > 0) || is_yolo)
					pixel_aq = float(g) * scaler;
				else
					pixel_aq = float(g) * scaler * 0.1;

				ofm_mpq[i][j][k] = dtype(pixel_aq);
			}
		}
	}
}

// transfer ofm data out
void transfer_ofm(
		Layer_Type layer,
		int row_iter, int col_iter, int channel_out_iter,
		dtype ofm_mpq[OFM_MAX][OFM_MAX][TM],
		AXI_VAL_O * str_out_0, AXI_VAL_O * str_out_1)
{
	for(int i = 0; i < layer.RC_out_size; i ++){  // 16
		for(int j = 0; j < layer.RC_out_size; j ++){  // 16
#pragma HLS LOOP_TRIPCOUNT min=32 max=64
			AXI_VAL_O output_0;
			AXI_VAL_O output_1;

			for(int k = 0; k < layer.CHO_real_size / TN / 2; k ++){  // 2
#pragma HLS LOOP_TRIPCOUNT min=2 max=2
				for(int l = 0; l < TN; l ++){  // 16
#pragma HLS LOOP_TRIPCOUNT min=16 max=16
#pragma HLS UNROLL
					output_0.data[l] = ofm_mpq[i][j][k * TN + l];
					output_1.data[l] = ofm_mpq[i][j][k * TN + l + layer.CHO_real_size / 2];
				}

				if(
						(i == (layer.RC_real_size - 1))
						&& (j == (layer.RC_real_size - 1))
						&& (k == (layer.CHO_real_size / TN / 2 - 1))
						&& (row_iter == (layer.RC_block_num - 1))
						&& (col_iter == (layer.RC_block_num - 1))
						&& (channel_out_iter == (layer.CHO_block_num - 1))){
					output_0.last = 1;
					output_1.last = 1;
				}

				else{
					output_0.last = 0;
					output_1.last = 0;
				}

				*str_out_0 = output_0;
				*str_out_1 = output_1;

				str_out_0 ++;
				str_out_1 ++;
			}
		}
	}
}

void convolution(
		Layer_Type layer,

		AXI_VAL_I *str_in_0,  // HP0
		AXI_VAL_I *str_in_1,  // HP1

		AXI_VAL_O *str_out_0,  // HP2
		AXI_VAL_O *str_out_1)  // HP3
{
#pragma HLS INTERFACE s_axilite register port=layer bundle=axilite
#pragma HLS INTERFACE s_axilite register port=return bundle=axilite

#pragma HLS INTERFACE axis register both port=str_in_0
#pragma HLS DATA_PACK variable=str_in_0
#pragma HLS INTERFACE axis register both port=str_in_1
#pragma HLS DATA_PACK variable=str_in_1

#pragma HLS INTERFACE axis register both port=str_out_0
#pragma HLS DATA_PACK variable=str_out_0 struct_level
#pragma HLS INTERFACE axis register both port=str_out_1
#pragma HLS DATA_PACK variable=str_out_1 struct_level

	dtype ifm[IFM_MAX + 2][IFM_MAX + 2][TN];
#pragma HLS ARRAY_PARTITION variable=ifm complete dim=3

	dtype weights[KERNEL][KERNEL][TN][TM];
#pragma HLS ARRAY_PARTITION variable=weights complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights complete dim=4

	int bias[TM];
#pragma HLS ARRAY_PARTITION variable=bias complete dim=1

	int ofm[OFM_MAX][OFM_MAX][TM];
#pragma HLS ARRAY_PARTITION variable=ofm complete dim=3

	dtype ofm_mpq[OFM_MAX][OFM_MAX][TM];

#pragma HLS ARRAY_PARTITION variable=ofm_mpq complete dim=3

	for(int R_iter = 0; R_iter < layer.RC_block_num; R_iter ++){

		for(int C_iter = 0; C_iter < layer.RC_block_num; C_iter ++){

			for(int CHO_iter = 0; CHO_iter < layer.CHO_block_num; CHO_iter ++){

				convCHI_lebel:for(int CHI_iter = 0; CHI_iter < layer.CHI_block_num; CHI_iter ++){
#pragma HLS DATAFLOW

					load_ifm(layer.RC_real_size, str_in_0, ifm);

					load_ibw(layer.kernel, layer.CHO_real_size, str_in_1, bias, weights);

					macc(layer.kernel, layer.RC_out_size, layer.stride, CHI_iter, ifm, weights, bias, ofm);
				}

				last_proc(layer.is_yolo, layer.scaler, layer.RC_out_size, layer.CHO_real_size,  ofm, ofm_mpq);

				transfer_ofm(layer, R_iter, C_iter, CHO_iter, ofm_mpq, str_out_0, str_out_1);
			}
		}
	}
}
