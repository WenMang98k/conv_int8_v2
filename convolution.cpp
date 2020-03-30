#include "convolution.hpp"


// load ifm padding data from AXI4-Stream to BRAM
// 按照[row][col][channel_in] channel_in->col->row
void load_ifm(
		int RC_block_size,  // layer.RC_block_size
		AXI_VAL_I *str_in_0,// 8bit * 16 =128
		dtype ifm[IFM_MAX][IFM_MAX][TN])
{
	load_ifm_label0:for(int i = 0; i < RC_block_size; i ++){
#pragma HLS LOOP_TRIPCOUNT min=8 max=66
		for(int j = 0; j < RC_block_size; j ++){
#pragma HLS LOOP_TRIPCOUNT min=8 max=66
#pragma HLS PIPELINE
			for(int k = 0; k < TN; k +=16){
#pragma HLS UNROLL
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
		AXI_VAL_I *str_in_1,
		int bias[TM],
		dtype weights[KERNEL][KERNEL][TN][TM])
{
	AXI_VAL_I input;
	union s8_s32 union_t;
	load_bias:
	for(int i = 0; i < 16; i ++){  // TM * 32 / 128
#pragma HLS LOOP_TRIPCOUNT min=16 max=16
#pragma HLS PIPELINE
		input = *str_in_1;
		str_in_1 ++;
		int index = i * 4;
		int temp_bias ;
		union_t.union_s8[0] = input.data[0];
		union_t.union_s8[1] = input.data[1];
		union_t.union_s8[2] = input.data[2];
		union_t.union_s8[3] = input.data[3];
		temp_bias= union_t.union_s32;
		bias[index] = temp_bias;

		union_t.union_s8[0] = input.data[4];
		union_t.union_s8[1] = input.data[5];
		union_t.union_s8[2] = input.data[6];
		union_t.union_s8[3] = input.data[7];
		temp_bias= union_t.union_s32;
		bias[index + 1] = temp_bias;
		union_t.union_s8[0] = input.data[8];
		union_t.union_s8[1] = input.data[9];
		union_t.union_s8[2] = input.data[10];
		union_t.union_s8[3] = input.data[11];
		temp_bias= union_t.union_s32;
		bias[index + 2] = temp_bias;

		union_t.union_s8[0] = input.data[12];
		union_t.union_s8[1] = input.data[13];
		union_t.union_s8[2] = input.data[14];
		union_t.union_s8[3] = input.data[15];
		temp_bias= union_t.union_s32;
		bias[index + 3] = temp_bias;
		/*
		for(int j = 0; j < 4; j ++){  // 一次传进来128bit，每32bit为一个bias，即这里传进来bias 分为4*8bit
#pragma HLS LOOP_TRIPCOUNT min=4 max=4
//#pragma HLS UNROLL
			union s8_s32 union_t;
			union_t.union_s8[0] = input.data[j * 4];
			union_t.union_s8[1] = input.data[j * 4 + 1];
			union_t.union_s8[2] = input.data[j * 4 + 2];
			union_t.union_s8[3] = input.data[j * 4 + 3];
			bias[i * 4 + j] = union_t.union_s32;
		}*/
	}
	//按照[kernal_x][kernal_y][channel_in][channel_out] channel_in->chaneel_out->kernal_y->kernal_x
	load_weights:
	for(int i = 0; i < kernel; i ++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=3
		for(int j = 0; j < kernel; j ++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=3
#pragma HLS PIPELINE
			for(int k = 0; k < TM; k ++){
#pragma HLS PIPELINE
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
		dtype ifm[IFM_MAX][IFM_MAX][TN],
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
				int x_offset = k * stride + i;
				for(int l = 0; l < RC_out_size; l ++){
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=8 max=64
					// TN = 16, compute through each channel
					int y_offset = l * stride + j;

					for(int m = 0; m < TM; m ++){
#pragma HLS UNROLL
						int bias_res;

						if((i == 0) && (j == 0) && (channel_in_iter == 0))
							bias_res = bias[m];
						else
							bias_res = ofm[k][l][m];//读后写ofm[k][l][m] = bias_res + macc_res;



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

						/*

						int mul_res_32 = ifm[x_offset][y_offset][32] * weights[i][j][32][m];
						int mul_res_33 = ifm[x_offset][y_offset][33] * weights[i][j][33][m];
						int mul_res_34 = ifm[x_offset][y_offset][34] * weights[i][j][34][m];
						int mul_res_35 = ifm[x_offset][y_offset][35] * weights[i][j][35][m];
						int mul_res_36 = ifm[x_offset][y_offset][36] * weights[i][j][36][m];
						int mul_res_37 = ifm[x_offset][y_offset][37] * weights[i][j][37][m];
						int mul_res_38 = ifm[x_offset][y_offset][38] * weights[i][j][38][m];
						int mul_res_39 = ifm[x_offset][y_offset][39] * weights[i][j][39][m];
						int mul_res_40 = ifm[x_offset][y_offset][40] * weights[i][j][40][m];
						int mul_res_41 = ifm[x_offset][y_offset][41] * weights[i][j][41][m];
						int mul_res_42 = ifm[x_offset][y_offset][42] * weights[i][j][42][m];
						int mul_res_43 = ifm[x_offset][y_offset][43] * weights[i][j][43][m];
						int mul_res_44 = ifm[x_offset][y_offset][44] * weights[i][j][44][m];
						int mul_res_45 = ifm[x_offset][y_offset][45] * weights[i][j][45][m];
						int mul_res_46 = ifm[x_offset][y_offset][46] * weights[i][j][46][m];
						int mul_res_47 = ifm[x_offset][y_offset][47] * weights[i][j][47][m];
						int mul_res_48 = ifm[x_offset][y_offset][48] * weights[i][j][48][m];
						int mul_res_49 = ifm[x_offset][y_offset][49] * weights[i][j][49][m];
						int mul_res_50 = ifm[x_offset][y_offset][50] * weights[i][j][50][m];
						int mul_res_51 = ifm[x_offset][y_offset][51] * weights[i][j][51][m];
						int mul_res_52 = ifm[x_offset][y_offset][52] * weights[i][j][52][m];
						int mul_res_53 = ifm[x_offset][y_offset][53] * weights[i][j][53][m];
						int mul_res_54 = ifm[x_offset][y_offset][54] * weights[i][j][54][m];
						int mul_res_55 = ifm[x_offset][y_offset][55] * weights[i][j][55][m];
						int mul_res_56 = ifm[x_offset][y_offset][56] * weights[i][j][56][m];
						int mul_res_57 = ifm[x_offset][y_offset][57] * weights[i][j][57][m];
						int mul_res_58 = ifm[x_offset][y_offset][58] * weights[i][j][58][m];
						int mul_res_59 = ifm[x_offset][y_offset][59] * weights[i][j][59][m];
						int mul_res_60 = ifm[x_offset][y_offset][60] * weights[i][j][60][m];
						int mul_res_61 = ifm[x_offset][y_offset][61] * weights[i][j][61][m];
						int mul_res_62 = ifm[x_offset][y_offset][62] * weights[i][j][62][m];
						int mul_res_63 = ifm[x_offset][y_offset][63] * weights[i][j][63][m];
						*/

						// accumulation_0
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

						/*
						int acc_res_16 = mul_res_32 + mul_res_33;
						int acc_res_17 = mul_res_34 + mul_res_35;
						int acc_res_18 = mul_res_36 + mul_res_37;
						int acc_res_19 = mul_res_38 + mul_res_39;
						int acc_res_20 = mul_res_40 + mul_res_41;
						int acc_res_21 = mul_res_42 + mul_res_43;
						int acc_res_22 = mul_res_44 + mul_res_45;
						int acc_res_23 = mul_res_46 + mul_res_47;
						int acc_res_24 = mul_res_48 + mul_res_49;
						int acc_res_25 = mul_res_50 + mul_res_51;
						int acc_res_26 = mul_res_52 + mul_res_53;
						int acc_res_27 = mul_res_54 + mul_res_55;
						int acc_res_28 = mul_res_56 + mul_res_57;
						int acc_res_29 = mul_res_58 + mul_res_59;
						int acc_res_30 = mul_res_60 + mul_res_61;
						int acc_res_31 = mul_res_62 + mul_res_63;
						*/

						//accumulation_1
						int acc_res1_0  = acc_res_0 + acc_res_1;
						int acc_res1_1  = acc_res_2 + acc_res_3;
						int acc_res1_2  = acc_res_4 + acc_res_5;
						int acc_res1_3  = acc_res_6 + acc_res_7;
						int acc_res1_4  = acc_res_8 + acc_res_9;
						int acc_res1_5  = acc_res_10 + acc_res_11;
						int acc_res1_6  = acc_res_12 + acc_res_13;
						int acc_res1_7  = acc_res_14 + acc_res_15;

						/*

						int acc_res1_8  = acc_res_16 + acc_res_17;
						int acc_res1_9  = acc_res_18 + acc_res_19;
						int acc_res1_10 = acc_res_20 + acc_res_21;
						int acc_res1_11 = acc_res_22 + acc_res_23;
						int acc_res1_12 = acc_res_24 + acc_res_25;
						int acc_res1_13 = acc_res_26 + acc_res_27;
						int acc_res1_14 = acc_res_28 + acc_res_29;
						int acc_res1_15 = acc_res_30 + acc_res_31;
						*/

						//accumulation_2
						int acc_res2_0  = acc_res1_0 + acc_res1_1;
						int acc_res2_1  = acc_res1_2 + acc_res1_3;
						int acc_res2_2  = acc_res1_4 + acc_res1_5;
						int acc_res2_3  = acc_res1_6 + acc_res1_7;

//						int acc_res2_4  = acc_res1_8 + acc_res1_9;
//						int acc_res2_5  = acc_res1_10 + acc_res1_11;
//						int acc_res2_6  = acc_res1_12 + acc_res1_13;
//						int acc_res2_7  = acc_res1_14 + acc_res1_15;

						//accumulation_3
						int acc_res3_0  = acc_res2_0 + acc_res2_1;
						int acc_res3_1  = acc_res2_2 + acc_res2_3;
//						int acc_res3_2  = acc_res2_4 + acc_res2_5;
//						int acc_res3_3  = acc_res2_6 + acc_res2_7;
						//accumulation_4
						int acc_res4_0  = acc_res3_0 + acc_res3_1;
//						int acc_res4_1  = acc_res3_2 + acc_res3_3;

						//accumulation_5
						int macc_res = acc_res4_0;

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
		float pscaler,
		float nscaler,
		int RC_out_size,  // layer.RC_output_size
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
					pixel_aq = float(g) * pscaler  ;
				else
					pixel_aq = float(g) * nscaler ;

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
			for(int k = 0; k < 32 ; k +=16){  // 2
#pragma HLS PIPELINE
				/*
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
				*/
				output_0.data[0] = ofm_mpq[i][j][k + 0];
				output_0.data[1] = ofm_mpq[i][j][k + 1];
				output_0.data[2] = ofm_mpq[i][j][k + 2];
				output_0.data[3] = ofm_mpq[i][j][k + 3];
				output_0.data[4] = ofm_mpq[i][j][k + 4];
				output_0.data[5] = ofm_mpq[i][j][k + 5];
				output_0.data[6] = ofm_mpq[i][j][k + 6];
				output_0.data[7] = ofm_mpq[i][j][k + 7];
				output_0.data[8] = ofm_mpq[i][j][k + 8];
				output_0.data[9] = ofm_mpq[i][j][k + 9];
				output_0.data[10] = ofm_mpq[i][j][k + 10];
				output_0.data[11] = ofm_mpq[i][j][k + 11];
				output_0.data[12] = ofm_mpq[i][j][k + 12];
				output_0.data[13] = ofm_mpq[i][j][k + 13];
				output_0.data[14] = ofm_mpq[i][j][k + 14];
				output_0.data[15] = ofm_mpq[i][j][k + 15];

				output_1.data[0] = ofm_mpq[i][j][k + 32];
				output_1.data[1] = ofm_mpq[i][j][k + 33];
				output_1.data[2] = ofm_mpq[i][j][k + 34];
				output_1.data[3] = ofm_mpq[i][j][k + 35];
				output_1.data[4] = ofm_mpq[i][j][k + 36];
				output_1.data[5] = ofm_mpq[i][j][k + 37];
				output_1.data[6] = ofm_mpq[i][j][k + 38];
				output_1.data[7] = ofm_mpq[i][j][k + 39];
				output_1.data[8] = ofm_mpq[i][j][k + 40];
				output_1.data[9] = ofm_mpq[i][j][k + 41];
				output_1.data[10] = ofm_mpq[i][j][k + 42];
				output_1.data[11] = ofm_mpq[i][j][k + 43];
				output_1.data[12] = ofm_mpq[i][j][k + 44];
				output_1.data[13] = ofm_mpq[i][j][k + 45];
				output_1.data[14] = ofm_mpq[i][j][k + 46];
				output_1.data[15] = ofm_mpq[i][j][k + 47];
				if(
						(i == (layer.RC_real_size - 1))
						&& (j == (layer.RC_real_size - 1))
						&& (k == (layer.CHO_block_size / 32 - 1))
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
void first_loop(
		Layer_Type layer,

		AXI_VAL_I *str_in_0,  // HP0
		AXI_VAL_I *str_in_1,  // HP1
		int ofm[OFM_MAX][OFM_MAX][TM]
){
convCHI_lebel:for(int CHI_iter = 0; CHI_iter < layer.CHI_block_num; CHI_iter ++){
//#pragma HLS DATAFLOW
#pragma HLS LOOP_TRIPCOUNT min=1 max=15
		dtype ifm[IFM_MAX][IFM_MAX][TN];
#pragma HLS ARRAY_PARTITION variable=ifm complete dim=3
		load_ifm(layer.RC_block_size, str_in_0,ifm);

		dtype weights[KERNEL][KERNEL][TN][TM];
#pragma HLS ARRAY_PARTITION variable=weights complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights complete dim=4
		int bias[TM];
#pragma HLS ARRAY_PARTITION variable=bias complete dim=1
		load_ibw(layer.kernel, str_in_1, bias, weights);
		//load_ibw(layer.kernel, str_in_1);

		macc(layer.kernel, layer.RC_out_size, layer.stride, CHI_iter, ifm, weights, bias, ofm);
		//macc(layer.kernel, layer.RC_out_size, layer.stride, CHI_iter);
	}
}

void last_loop(
		Layer_Type layer,
		int R_iter_temp,int C_iter_temp,int CHO_iter_temp,
		int ofm[OFM_MAX][OFM_MAX][TM],
		AXI_VAL_O *str_out_0,  // HP2
		AXI_VAL_O *str_out_1
){
	dtype ofm_mpq[OFM_MAX][OFM_MAX][TM];
#pragma HLS ARRAY_PARTITION variable=ofm_mpq complete dim=3
	last_proc(layer.is_yolo, layer.pscaler,layer.nscaler, layer.RC_out_size,  ofm, ofm_mpq);
	//last_proc(layer.is_yolo, layer.pscaler,layer.nscaler, layer.RC_out_size);
	transfer_ofm(layer, R_iter_temp, C_iter_temp, CHO_iter_temp, ofm_mpq, str_out_0, str_out_1);
	//transfer_ofm(layer, R_iter, C_iter, CHO_iter,  str_out_0, str_out_1);

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
#pragma HLS INTERFACE depth=17424 port=str_in_0

#pragma HLS INTERFACE axis register both port=str_in_1
#pragma HLS DATA_PACK variable=str_in_1
#pragma HLS INTERFACE depth=2320 port=str_in_0

#pragma HLS INTERFACE axis register both port=str_out_0
#pragma HLS DATA_PACK variable=str_out_0 struct_level
#pragma HLS INTERFACE depth=8192 port=str_in_0
#pragma HLS INTERFACE axis register both port=str_out_1
#pragma HLS DATA_PACK variable=str_out_1 struct_level
#pragma HLS INTERFACE depth=8192 port=str_in_0



	for(int R_iter = 0; R_iter < layer.RC_block_num; R_iter ++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=4
		for(int C_iter = 0; C_iter < layer.RC_block_num; C_iter ++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=4
			for(int CHO_iter = 0; CHO_iter < layer.CHO_block_num; CHO_iter ++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=15

				int R_iter_temp = R_iter;
				int C_iter_temp = C_iter;
				int CHO_iter_temp = CHO_iter;

				int ofm[OFM_MAX][OFM_MAX][TM];
#pragma HLS ARRAY_PARTITION variable=ofm complete dim=3
				first_loop(layer,str_in_0,str_in_1,ofm);
				//cout<<"onchip_sclaer:"<<layer.scaler<<endl;
				last_loop(layer,R_iter_temp,C_iter_temp,CHO_iter_temp,ofm,str_out_0,str_out_1);
			}
		}
	}
}
