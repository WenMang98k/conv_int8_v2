#include "convolution.hpp"

// load ifm padding data from AXI4-Stream to BRAM
// 按照[row][col][channel_in] channel_in->col->row
void load_ifm(
		int RC_block_size,  // layer.RC_block_size
		int CHI_block_size,  // layer.CHI_block_size
		AXI_VAL_I *str_in_0,// 8bit * 16 =128
		dtype ifm[IFM_MAX + 2][IFM_MAX + 2][TN])
{
	load_ifm_label0:for(int i = 0; i < RC_block_size; i ++){
#pragma HLS LOOP_TRIPCOUNT min=8 max=66
		for(int j = 0; j < RC_block_size; j ++){
#pragma HLS LOOP_TRIPCOUNT min=8 max=66
#pragma HLS PIPELINE

			for(int k = 0; k < TN; k +=16){
//#pragma HLS UNROLL
				AXI_VAL_I input = *str_in_0;
				str_in_0 ++;
				ifm[i][j][k+0] = input.data[0];
				ifm[i][j][k+1] = input.data[1];
				ifm[i][j][k+2] = input.data[2];
				ifm[i][j][k+3] = input.data[3];
				ifm[i][j][k+4] = input.data[4];
				ifm[i][j][k+5] = input.data[5];
				ifm[i][j][k+6] = input.data[6];
				ifm[i][j][k+7] = input.data[7];
				ifm[i][j][k+8] = input.data[8];
				ifm[i][j][k+9] = input.data[9];
				ifm[i][j][k+10] = input.data[10];
				ifm[i][j][k+11] = input.data[11];
				ifm[i][j][k+12] = input.data[12];
				ifm[i][j][k+13] = input.data[13];
				ifm[i][j][k+14] = input.data[14];
				ifm[i][j][k+15] = input.data[15];
			}
		}
	}
}

// load bias and weights data from AXI-4_Stream to DRAM
void load_ibw(
		int kernel,  // layer.kernel
		int CHI_block_size,  // layer.CHI_block_size
		int CHO_block_size,  // layer.CHO_block_size
		AXI_VAL_I *str_in_1,
		int bias[TM],
		dtype weights[KERNEL][KERNEL][TN][TM])
{
	AXI_VAL_I input;

	load_bias:
	for(int i = 0; i < TM / 4; i ++){  // TM * 32 / 128
#pragma HLS LOOP_TRIPCOUNT min=64 max=64
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
#pragma HLS LOOP_TRIPCOUNT min=1 max=3
		for(int j = 0; j < kernel; j ++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=3
#pragma HLS PIPELINE
			for(int k = 0; k < TM; k ++){
#pragma HLS UNROLL
				for(int l = 0; l < TN; l +=16){
#pragma HLS UNROLL
					input = *str_in_1;
					str_in_1 ++;
					weights[i][j][l+0][k] = input.data[0];
					weights[i][j][l+1][k] = input.data[1];
					weights[i][j][l+2][k] = input.data[2];
					weights[i][j][l+3][k] = input.data[3];

					weights[i][j][l+4][k] = input.data[4];
					weights[i][j][l+5][k] = input.data[5];
					weights[i][j][l+6][k] = input.data[6];
					weights[i][j][l+7][k] = input.data[7];

					weights[i][j][l+8][k] = input.data[8];
					weights[i][j][l+9][k] = input.data[9];
					weights[i][j][l+10][k] = input.data[10];
					weights[i][j][l+11][k] = input.data[11];

					weights[i][j][l+12][k] = input.data[12];
					weights[i][j][l+13][k] = input.data[13];
					weights[i][j][l+14][k] = input.data[14];
					weights[i][j][l+15][k] = input.data[15];

				}
			}
		}
	}
}


//对于输入、输出通道改变的情况，应该建立乘累加缓冲树

// on chip macc
void macc(
		int kernel,  // layer.kernel
		int RC_out_size,  // layer.RC_real_size
		int stride,  // layer.max_pool_control
		int channel_in_iter,
		int CHI_block_size,  // layer.CHI_block_size
		int CHO_block_size,  // layer.CHO_block_size
		dtype ifm[IFM_MAX + 2][IFM_MAX + 2][TN],
		dtype weights[KERNEL][KERNEL][TN][TM],
		int bias[TM],
		int ofm[OFM_MAX][OFM_MAX][TM])
{
	for(int i = 0; i < kernel; i ++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=3
		for(int j = 0; j < kernel; j ++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=3
			for(int k = 0; k < RC_out_size; k ++){
#pragma HLS LOOP_TRIPCOUNT min=8 max=64
				for(int l = 0; l < RC_out_size; l ++){
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=8 max=64
					for(int m = 0; m < TM; m ++){
#pragma HLS UNROLL
						int bias_res;

						if((i == 0) && (j == 0) && (channel_in_iter == 0))
							bias_res = bias[m];
						else
							bias_res = ofm[k][l][m];

						// TN = 16, compute through each channel
						int x_offset = i + k * stride;
						int y_offset = j + l * stride;

						// multiply
						int mul_res_0  = ifm[x_offset][y_offset][0]  * weights[i][j][0][m];
						int mul_res_1  = ifm[x_offset][y_offset][1]  * weights[i][j][1][m];
						int mul_res_2  = ifm[x_offset][y_offset][2]  * weights[i][j][2][m];
						int mul_res_3  = ifm[x_offset][y_offset][3]  * weights[i][j][3][m];
						int mul_res_4  = ifm[x_offset][y_offset][4]  * weights[i][j][4][m];
						int mul_res_5  = ifm[x_offset][y_offset][5]  * weights[i][j][5][m];
						int mul_res_6  = ifm[x_offset][y_offset][6]  * weights[i][j][6][m];
						int mul_res_7  = ifm[x_offset][y_offset][7]  * weights[i][j][7][m];
						int mul_res_8  = ifm[x_offset][y_offset][8]  * weights[i][j][8][m];
						int mul_res_9  = ifm[x_offset][y_offset][9]  * weights[i][j][9][m];
						int mul_res_10 = ifm[x_offset][y_offset][10] * weights[i][j][10][m];
						int mul_res_11 = ifm[x_offset][y_offset][11] * weights[i][j][11][m];
						int mul_res_12 = ifm[x_offset][y_offset][12] * weights[i][j][12][m];
						int mul_res_13 = ifm[x_offset][y_offset][13] * weights[i][j][13][m];
						int mul_res_14 = ifm[x_offset][y_offset][14] * weights[i][j][14][m];
						int mul_res_15 = ifm[x_offset][y_offset][15] * weights[i][j][15][m];
						int mul_res_16 = ifm[x_offset][y_offset][16] * weights[i][j][16][m];
						int mul_res_17 = ifm[x_offset][y_offset][17] * weights[i][j][17][m];
						int mul_res_18 = ifm[x_offset][y_offset][18] * weights[i][j][18][m];
						int mul_res_19 = ifm[x_offset][y_offset][19] * weights[i][j][19][m];
						int mul_res_20 = ifm[x_offset][y_offset][20] * weights[i][j][20][m];
						int mul_res_21 = ifm[x_offset][y_offset][21] * weights[i][j][21][m];
						int mul_res_22 = ifm[x_offset][y_offset][22] * weights[i][j][22][m];
						int mul_res_23 = ifm[x_offset][y_offset][23] * weights[i][j][23][m];
						int mul_res_24 = ifm[x_offset][y_offset][24] * weights[i][j][24][m];
						int mul_res_25 = ifm[x_offset][y_offset][25] * weights[i][j][25][m];
						int mul_res_26 = ifm[x_offset][y_offset][26] * weights[i][j][26][m];
						int mul_res_27 = ifm[x_offset][y_offset][27] * weights[i][j][27][m];
						int mul_res_28 = ifm[x_offset][y_offset][28] * weights[i][j][28][m];
						int mul_res_29 = ifm[x_offset][y_offset][29] * weights[i][j][29][m];
						int mul_res_30 = ifm[x_offset][y_offset][30] * weights[i][j][30][m];
						int mul_res_31 = ifm[x_offset][y_offset][31] * weights[i][j][31][m];

						// accumulation
						int acc_res_0 = mul_res_0 + mul_res_1;
						int acc_res_1 = mul_res_2 + mul_res_3;
						int acc_res_2 = mul_res_4 + mul_res_5;
						int acc_res_3 = mul_res_6 + mul_res_7;
						int acc_res_4 = mul_res_8 + mul_res_9;
						int acc_res_5 = mul_res_10 + mul_res_11;
						int acc_res_6 = mul_res_12 + mul_res_13;
						int acc_res_7 = mul_res_14 + mul_res_15;
						int acc_res_8 = mul_res_16 + mul_res_17;
						int acc_res_9 = mul_res_18 + mul_res_19;
						int acc_res_10 = mul_res_20 + mul_res_21;
						int acc_res_11 = mul_res_22 + mul_res_23;
						int acc_res_12 = mul_res_24 + mul_res_25;
						int acc_res_13 = mul_res_26 + mul_res_27;
						int acc_res_14 = mul_res_28 + mul_res_29;
						int acc_res_15 = mul_res_30 + mul_res_31;

						int acc_res_16 = acc_res_0 + acc_res_1;
						int acc_res_17 = acc_res_2 + acc_res_3;
						int acc_res_18 = acc_res_4 + acc_res_5;
						int acc_res_19 = acc_res_6 + acc_res_7;
						int acc_res_20 = acc_res_8 + acc_res_9;
						int acc_res_21 = acc_res_10 + acc_res_11;
						int acc_res_22 = acc_res_12 + acc_res_13;
						int acc_res_23 = acc_res_14 + acc_res_15;

						int acc_res_24 = acc_res_16 + acc_res_17;
						int acc_res_25 = acc_res_18 + acc_res_19;
						int acc_res_26 = acc_res_20 + acc_res_21;
						int acc_res_27 = acc_res_22 + acc_res_23;

						int acc_res_28 = acc_res_24 + acc_res_25;
						int acc_res_29 = acc_res_26 + acc_res_27;

						int macc_res = acc_res_28 + acc_res_29;

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
		int CHO_block_size,
		int ofm[OFM_MAX][OFM_MAX][TM],
		dtype ofm_mpq[OFM_MAX][OFM_MAX][TM])
{
	for(int i = 0; i < RC_out_size; i ++){
#pragma HLS LOOP_TRIPCOUNT min=8 max=64
		for(int j = 0; j < RC_out_size; j ++){
#pragma HLS LOOP_TRIPCOUNT min=8 max=64
#pragma HLS PIPELINE
			for(int k = 0; k < TM; k ++){
#pragma HLS UNROLL
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
#pragma HLS LOOP_TRIPCOUNT min=32 max=64
		for(int j = 0; j < layer.RC_out_size; j ++){  // 16
#pragma HLS LOOP_TRIPCOUNT min=32 max=64
			AXI_VAL_O output_0;
			AXI_VAL_O output_1;
			for(int k = 0; k < TM / 16 / 2; k ++){  // 2
#pragma HLS PIPELINE
				output_0.data[0] = ofm_mpq[i][j][k * TN + 0];
				output_0.data[1] = ofm_mpq[i][j][k * TN + 1];
				output_0.data[2] = ofm_mpq[i][j][k * TN + 2];
				output_0.data[3] = ofm_mpq[i][j][k * TN + 3];
				output_0.data[4] = ofm_mpq[i][j][k * TN + 4];
				output_0.data[5] = ofm_mpq[i][j][k * TN + 5];
				output_0.data[6] = ofm_mpq[i][j][k * TN + 6];
				output_0.data[7] = ofm_mpq[i][j][k * TN + 7];
				output_0.data[8] = ofm_mpq[i][j][k * TN + 8];
				output_0.data[9] = ofm_mpq[i][j][k * TN + 9];
				output_0.data[10] = ofm_mpq[i][j][k * TN + 10];
				output_0.data[11] = ofm_mpq[i][j][k * TN + 11];
				output_0.data[12] = ofm_mpq[i][j][k * TN + 12];
				output_0.data[13] = ofm_mpq[i][j][k * TN + 13];
				output_0.data[14] = ofm_mpq[i][j][k * TN + 14];
				output_0.data[15] = ofm_mpq[i][j][k * TN + 15];

				output_1.data[0] = ofm_mpq[i][j][k * TN + 0 + layer.CHO_block_size / 2];
				output_1.data[1] = ofm_mpq[i][j][k * TN + 1 + layer.CHO_block_size / 2];
				output_1.data[2] = ofm_mpq[i][j][k * TN + 2 + layer.CHO_block_size / 2];
				output_1.data[3] = ofm_mpq[i][j][k * TN + 3 + layer.CHO_block_size / 2];
				output_1.data[4] = ofm_mpq[i][j][k * TN + 4 + layer.CHO_block_size / 2];
				output_1.data[5] = ofm_mpq[i][j][k * TN + 5 + layer.CHO_block_size / 2];
				output_1.data[6] = ofm_mpq[i][j][k * TN + 6 + layer.CHO_block_size / 2];
				output_1.data[7] = ofm_mpq[i][j][k * TN + 7 + layer.CHO_block_size / 2];
				output_1.data[8] = ofm_mpq[i][j][k * TN + 8 + layer.CHO_block_size / 2];
				output_1.data[9] = ofm_mpq[i][j][k * TN + 9 + layer.CHO_block_size / 2];
				output_1.data[10] = ofm_mpq[i][j][k * TN + 10 + layer.CHO_block_size / 2];
				output_1.data[11] = ofm_mpq[i][j][k * TN + 11 + layer.CHO_block_size / 2];
				output_1.data[12] = ofm_mpq[i][j][k * TN + 12 + layer.CHO_block_size / 2];
				output_1.data[13] = ofm_mpq[i][j][k * TN + 13 + layer.CHO_block_size / 2];
				output_1.data[14] = ofm_mpq[i][j][k * TN + 14 + layer.CHO_block_size / 2];
				output_1.data[15] = ofm_mpq[i][j][k * TN + 15 + layer.CHO_block_size / 2];

				if(
						(i == (layer.RC_real_size - 1))
						&& (j == (layer.RC_real_size - 1))
						&& (k == (layer.CHO_block_size / 16 / 2 - 1))
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

					load_ifm(layer.RC_block_size, layer.CHI_block_size,str_in_0, ifm);

					load_ibw(layer.kernel, layer.CHI_block_size, layer.CHO_block_size, str_in_1, bias, weights);

					macc(layer.kernel, layer.RC_out_size, layer.stride, CHI_iter, layer.CHI_block_size, layer.CHO_block_size,ifm, weights, bias, ofm);
				}
				cout<<"onchip_sclaer:"<<layer.scaler<<endl;
				last_proc(layer.is_yolo, layer.scaler, layer.RC_out_size, layer.CHO_block_size,  ofm, ofm_mpq);

				transfer_ofm(layer, R_iter, C_iter, CHO_iter, ofm_mpq, str_out_0, str_out_1);
			}
		}
	}
}
