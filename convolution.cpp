#include "convolution.hpp"

ap_int<32> MUL_MAC(ap_int<8> A, ap_int<8> W0, ap_int<8> W1)
{
    ap_int<24> W;
    W = (W0, ap_uint<16>(0)) + ap_int<24>(W1);

    ap_int<16> r0;
    ap_int<16> r1;

    (r0, r1) = A*W;

    r0 = r0+r1[15];

    return (r0,r1);
}
float MUL_FP(float A, float W)
{
    float res;
    res = A*W;
    return res;
}
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
	for(int i = 0; i < 	TM; i += 4 ){  // TM * 32 / 128
#pragma HLS LOOP_TRIPCOUNT min=16 max=16
#pragma HLS UNROLL
#pragma HLS PIPELINE II=1
		input = *str_in_1;
		str_in_1 ++;
		int index = i;
		int temp_bias0;
		int temp_bias1;
		int temp_bias2;
		int temp_bias3;

		union_t.union_s8[0] = input.data[0];
		union_t.union_s8[1] = input.data[1];
		union_t.union_s8[2] = input.data[2];
		union_t.union_s8[3] = input.data[3];
		temp_bias0 = union_t.union_s32;
		bias[index] = temp_bias0;

		union_t.union_s8[0] = input.data[4];
		union_t.union_s8[1] = input.data[5];
		union_t.union_s8[2] = input.data[6];
		union_t.union_s8[3] = input.data[7];
		temp_bias1  = union_t.union_s32;
		bias[index + 1] = temp_bias1;

		union_t.union_s8[0] = input.data[8];
		union_t.union_s8[1] = input.data[9];
		union_t.union_s8[2] = input.data[10];
		union_t.union_s8[3] = input.data[11];
		temp_bias2   = union_t.union_s32;
		bias[index + 2] = temp_bias2;

		union_t.union_s8[0] = input.data[12];
		union_t.union_s8[1] = input.data[13];
		union_t.union_s8[2] = input.data[14];
		union_t.union_s8[3] = input.data[15];
		temp_bias3 = union_t.union_s32;
		bias[index + 3] = temp_bias3;
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
		ap_uint<2> stride,  // layer.max_pool_control
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
#pragma HLS LOOP_TRIPCOUNT min=8 max=64
#pragma HLS PIPELINE II=1
					// TN = 16, compute through each channel
					int y_offset = l * stride + j;
					for(int m = 0; m < TM; m +=2){
#pragma HLS UNROLL
						int bias_res_0,bias_res_1;

						if((i == 0) && (j == 0) && (channel_in_iter == 0))
							{
								bias_res_0 = bias[m];
								bias_res_1 = bias[m+1];
							}
						else
							{
								bias_res_0 = ofm[k][l][m];
								bias_res_1 = ofm[k][l][m+1];
							}

						// multiply
						mul_type mul_res_0_0, mul_res_1_0; (mul_res_0_0, mul_res_1_0)  = MUL_MAC( ifm[x_offset][y_offset][0], weights[i][j][0][m], weights[i][j][0][m+1]);
						mul_type mul_res_0_1, mul_res_1_1; (mul_res_0_1, mul_res_1_1)  = MUL_MAC( ifm[x_offset][y_offset][1], weights[i][j][1][m], weights[i][j][1][m+1]);
						mul_type mul_res_0_2, mul_res_1_2; (mul_res_0_2, mul_res_1_2)  = MUL_MAC( ifm[x_offset][y_offset][2], weights[i][j][2][m], weights[i][j][2][m+1]);
						mul_type mul_res_0_3, mul_res_1_3; (mul_res_0_3, mul_res_1_3)  = MUL_MAC( ifm[x_offset][y_offset][3], weights[i][j][3][m], weights[i][j][3][m+1]);
						mul_type mul_res_0_4, mul_res_1_4; (mul_res_0_4, mul_res_1_4)  = MUL_MAC( ifm[x_offset][y_offset][4], weights[i][j][4][m], weights[i][j][4][m+1]);
						mul_type mul_res_0_5, mul_res_1_5; (mul_res_0_5, mul_res_1_5)  = MUL_MAC( ifm[x_offset][y_offset][5], weights[i][j][5][m], weights[i][j][5][m+1]);
						mul_type mul_res_0_6, mul_res_1_6; (mul_res_0_6, mul_res_1_6)  = MUL_MAC( ifm[x_offset][y_offset][6], weights[i][j][6][m], weights[i][j][6][m+1]);
						mul_type mul_res_0_7, mul_res_1_7; (mul_res_0_7, mul_res_1_7)  = MUL_MAC( ifm[x_offset][y_offset][7], weights[i][j][7][m], weights[i][j][7][m+1]);
						mul_type mul_res_0_8, mul_res_1_8; (mul_res_0_8, mul_res_1_8)  = MUL_MAC( ifm[x_offset][y_offset][8], weights[i][j][8][m], weights[i][j][8][m+1]);
						mul_type mul_res_0_9, mul_res_1_9; (mul_res_0_9, mul_res_1_9)  = MUL_MAC( ifm[x_offset][y_offset][9], weights[i][j][9][m], weights[i][j][9][m+1]);
						mul_type mul_res_0_10,mul_res_1_10;(mul_res_0_10,mul_res_1_10) = MUL_MAC(ifm[x_offset][y_offset][10],weights[i][j][10][m],weights[i][j][10][m+1]);
						mul_type mul_res_0_11,mul_res_1_11;(mul_res_0_11,mul_res_1_11) = MUL_MAC(ifm[x_offset][y_offset][11],weights[i][j][11][m],weights[i][j][11][m+1]);
						mul_type mul_res_0_12,mul_res_1_12;(mul_res_0_12,mul_res_1_12) = MUL_MAC(ifm[x_offset][y_offset][12],weights[i][j][12][m],weights[i][j][12][m+1]);
						mul_type mul_res_0_13,mul_res_1_13;(mul_res_0_13,mul_res_1_13) = MUL_MAC(ifm[x_offset][y_offset][13],weights[i][j][13][m],weights[i][j][13][m+1]);
						mul_type mul_res_0_14,mul_res_1_14;(mul_res_0_14,mul_res_1_14) = MUL_MAC(ifm[x_offset][y_offset][14],weights[i][j][14][m],weights[i][j][14][m+1]);
						mul_type mul_res_0_15,mul_res_1_15;(mul_res_0_15,mul_res_1_15) = MUL_MAC(ifm[x_offset][y_offset][15],weights[i][j][15][m],weights[i][j][15][m+1]);
						mul_type mul_res_0_16,mul_res_1_16;(mul_res_0_16,mul_res_1_16) = MUL_MAC(ifm[x_offset][y_offset][16],weights[i][j][16][m],weights[i][j][16][m+1]);
						mul_type mul_res_0_17,mul_res_1_17;(mul_res_0_17,mul_res_1_17) = MUL_MAC(ifm[x_offset][y_offset][17],weights[i][j][17][m],weights[i][j][17][m+1]);
						mul_type mul_res_0_18,mul_res_1_18;(mul_res_0_18,mul_res_1_18) = MUL_MAC(ifm[x_offset][y_offset][18],weights[i][j][18][m],weights[i][j][18][m+1]);
						mul_type mul_res_0_19,mul_res_1_19;(mul_res_0_19,mul_res_1_19) = MUL_MAC(ifm[x_offset][y_offset][19],weights[i][j][19][m],weights[i][j][19][m+1]);
						mul_type mul_res_0_20,mul_res_1_20;(mul_res_0_20,mul_res_1_20) = MUL_MAC(ifm[x_offset][y_offset][20],weights[i][j][20][m],weights[i][j][20][m+1]);
						mul_type mul_res_0_21,mul_res_1_21;(mul_res_0_21,mul_res_1_21) = MUL_MAC(ifm[x_offset][y_offset][21],weights[i][j][21][m],weights[i][j][21][m+1]);
						mul_type mul_res_0_22,mul_res_1_22;(mul_res_0_22,mul_res_1_22) = MUL_MAC(ifm[x_offset][y_offset][22],weights[i][j][22][m],weights[i][j][22][m+1]);
						mul_type mul_res_0_23,mul_res_1_23;(mul_res_0_23,mul_res_1_23) = MUL_MAC(ifm[x_offset][y_offset][23],weights[i][j][23][m],weights[i][j][23][m+1]);
						mul_type mul_res_0_24,mul_res_1_24;(mul_res_0_24,mul_res_1_24) = MUL_MAC(ifm[x_offset][y_offset][24],weights[i][j][24][m],weights[i][j][24][m+1]);
						mul_type mul_res_0_25,mul_res_1_25;(mul_res_0_25,mul_res_1_25) = MUL_MAC(ifm[x_offset][y_offset][25],weights[i][j][25][m],weights[i][j][25][m+1]);
						mul_type mul_res_0_26,mul_res_1_26;(mul_res_0_26,mul_res_1_26) = MUL_MAC(ifm[x_offset][y_offset][26],weights[i][j][26][m],weights[i][j][26][m+1]);
						mul_type mul_res_0_27,mul_res_1_27;(mul_res_0_27,mul_res_1_27) = MUL_MAC(ifm[x_offset][y_offset][27],weights[i][j][27][m],weights[i][j][27][m+1]);
						mul_type mul_res_0_28,mul_res_1_28;(mul_res_0_28,mul_res_1_28) = MUL_MAC(ifm[x_offset][y_offset][28],weights[i][j][28][m],weights[i][j][28][m+1]);
						mul_type mul_res_0_29,mul_res_1_29;(mul_res_0_29,mul_res_1_29) = MUL_MAC(ifm[x_offset][y_offset][29],weights[i][j][29][m],weights[i][j][29][m+1]);
						mul_type mul_res_0_30,mul_res_1_30;(mul_res_0_30,mul_res_1_30) = MUL_MAC(ifm[x_offset][y_offset][30],weights[i][j][30][m],weights[i][j][30][m+1]);
						mul_type mul_res_0_31,mul_res_1_31;(mul_res_0_31,mul_res_1_31) = MUL_MAC(ifm[x_offset][y_offset][31],weights[i][j][31][m],weights[i][j][31][m+1]);
						mul_type mul_res_0_32,mul_res_1_32;(mul_res_0_32,mul_res_1_32) = MUL_MAC(ifm[x_offset][y_offset][32],weights[i][j][32][m],weights[i][j][32][m+1]);
						mul_type mul_res_0_33,mul_res_1_33;(mul_res_0_33,mul_res_1_33) = MUL_MAC(ifm[x_offset][y_offset][33],weights[i][j][33][m],weights[i][j][33][m+1]);
						mul_type mul_res_0_34,mul_res_1_34;(mul_res_0_34,mul_res_1_34) = MUL_MAC(ifm[x_offset][y_offset][34],weights[i][j][34][m],weights[i][j][34][m+1]);
						mul_type mul_res_0_35,mul_res_1_35;(mul_res_0_35,mul_res_1_35) = MUL_MAC(ifm[x_offset][y_offset][35],weights[i][j][35][m],weights[i][j][35][m+1]);
						mul_type mul_res_0_36,mul_res_1_36;(mul_res_0_36,mul_res_1_36) = MUL_MAC(ifm[x_offset][y_offset][36],weights[i][j][36][m],weights[i][j][36][m+1]);
						mul_type mul_res_0_37,mul_res_1_37;(mul_res_0_37,mul_res_1_37) = MUL_MAC(ifm[x_offset][y_offset][37],weights[i][j][37][m],weights[i][j][37][m+1]);
						mul_type mul_res_0_38,mul_res_1_38;(mul_res_0_38,mul_res_1_38) = MUL_MAC(ifm[x_offset][y_offset][38],weights[i][j][38][m],weights[i][j][38][m+1]);
						mul_type mul_res_0_39,mul_res_1_39;(mul_res_0_39,mul_res_1_39) = MUL_MAC(ifm[x_offset][y_offset][39],weights[i][j][39][m],weights[i][j][39][m+1]);
						mul_type mul_res_0_40,mul_res_1_40;(mul_res_0_40,mul_res_1_40) = MUL_MAC(ifm[x_offset][y_offset][40],weights[i][j][40][m],weights[i][j][40][m+1]);
						mul_type mul_res_0_41,mul_res_1_41;(mul_res_0_41,mul_res_1_41) = MUL_MAC(ifm[x_offset][y_offset][41],weights[i][j][41][m],weights[i][j][41][m+1]);
						mul_type mul_res_0_42,mul_res_1_42;(mul_res_0_42,mul_res_1_42) = MUL_MAC(ifm[x_offset][y_offset][42],weights[i][j][42][m],weights[i][j][42][m+1]);
						mul_type mul_res_0_43,mul_res_1_43;(mul_res_0_43,mul_res_1_43) = MUL_MAC(ifm[x_offset][y_offset][43],weights[i][j][43][m],weights[i][j][43][m+1]);
						mul_type mul_res_0_44,mul_res_1_44;(mul_res_0_44,mul_res_1_44) = MUL_MAC(ifm[x_offset][y_offset][44],weights[i][j][44][m],weights[i][j][44][m+1]);
						mul_type mul_res_0_45,mul_res_1_45;(mul_res_0_45,mul_res_1_45) = MUL_MAC(ifm[x_offset][y_offset][45],weights[i][j][45][m],weights[i][j][45][m+1]);
						mul_type mul_res_0_46,mul_res_1_46;(mul_res_0_46,mul_res_1_46) = MUL_MAC(ifm[x_offset][y_offset][46],weights[i][j][46][m],weights[i][j][46][m+1]);
						mul_type mul_res_0_47,mul_res_1_47;(mul_res_0_47,mul_res_1_47) = MUL_MAC(ifm[x_offset][y_offset][47],weights[i][j][47][m],weights[i][j][47][m+1]);
						mul_type mul_res_0_48,mul_res_1_48;(mul_res_0_48,mul_res_1_48) = MUL_MAC(ifm[x_offset][y_offset][48],weights[i][j][48][m],weights[i][j][48][m+1]);
						mul_type mul_res_0_49,mul_res_1_49;(mul_res_0_49,mul_res_1_49) = MUL_MAC(ifm[x_offset][y_offset][49],weights[i][j][49][m],weights[i][j][49][m+1]);
						mul_type mul_res_0_50,mul_res_1_50;(mul_res_0_50,mul_res_1_50) = MUL_MAC(ifm[x_offset][y_offset][50],weights[i][j][50][m],weights[i][j][50][m+1]);
						mul_type mul_res_0_51,mul_res_1_51;(mul_res_0_51,mul_res_1_51) = MUL_MAC(ifm[x_offset][y_offset][51],weights[i][j][51][m],weights[i][j][51][m+1]);
						mul_type mul_res_0_52,mul_res_1_52;(mul_res_0_52,mul_res_1_52) = MUL_MAC(ifm[x_offset][y_offset][52],weights[i][j][52][m],weights[i][j][52][m+1]);
						mul_type mul_res_0_53,mul_res_1_53;(mul_res_0_53,mul_res_1_53) = MUL_MAC(ifm[x_offset][y_offset][53],weights[i][j][53][m],weights[i][j][53][m+1]);
						mul_type mul_res_0_54,mul_res_1_54;(mul_res_0_54,mul_res_1_54) = MUL_MAC(ifm[x_offset][y_offset][54],weights[i][j][54][m],weights[i][j][54][m+1]);
						mul_type mul_res_0_55,mul_res_1_55;(mul_res_0_55,mul_res_1_55) = MUL_MAC(ifm[x_offset][y_offset][55],weights[i][j][55][m],weights[i][j][55][m+1]);
						mul_type mul_res_0_56,mul_res_1_56;(mul_res_0_56,mul_res_1_56) = MUL_MAC(ifm[x_offset][y_offset][56],weights[i][j][56][m],weights[i][j][56][m+1]);
						mul_type mul_res_0_57,mul_res_1_57;(mul_res_0_57,mul_res_1_57) = MUL_MAC(ifm[x_offset][y_offset][57],weights[i][j][57][m],weights[i][j][57][m+1]);
						mul_type mul_res_0_58,mul_res_1_58;(mul_res_0_58,mul_res_1_58) = MUL_MAC(ifm[x_offset][y_offset][58],weights[i][j][58][m],weights[i][j][58][m+1]);
						mul_type mul_res_0_59,mul_res_1_59;(mul_res_0_59,mul_res_1_59) = MUL_MAC(ifm[x_offset][y_offset][59],weights[i][j][59][m],weights[i][j][59][m+1]);
						mul_type mul_res_0_60,mul_res_1_60;(mul_res_0_60,mul_res_1_60) = MUL_MAC(ifm[x_offset][y_offset][60],weights[i][j][60][m],weights[i][j][60][m+1]);
						mul_type mul_res_0_61,mul_res_1_61;(mul_res_0_61,mul_res_1_61) = MUL_MAC(ifm[x_offset][y_offset][61],weights[i][j][61][m],weights[i][j][61][m+1]);
						mul_type mul_res_0_62,mul_res_1_62;(mul_res_0_62,mul_res_1_62) = MUL_MAC(ifm[x_offset][y_offset][62],weights[i][j][62][m],weights[i][j][62][m+1]);
						mul_type mul_res_0_63,mul_res_1_63;(mul_res_0_63,mul_res_1_63) = MUL_MAC(ifm[x_offset][y_offset][63],weights[i][j][63][m],weights[i][j][63][m+1]);

						// accumulation_0
						acc_type0 acc_res_0_0  = mul_res_0_0  + mul_res_0_1;
						acc_type0 acc_res_0_1  = mul_res_0_2  + mul_res_0_3;
						acc_type0 acc_res_0_2  = mul_res_0_4  + mul_res_0_5;
						acc_type0 acc_res_0_3  = mul_res_0_6  + mul_res_0_7;
						acc_type0 acc_res_0_4  = mul_res_0_8  + mul_res_0_9;
						acc_type0 acc_res_0_5  = mul_res_0_10 + mul_res_0_11;
						acc_type0 acc_res_0_6  = mul_res_0_12 + mul_res_0_13;
						acc_type0 acc_res_0_7  = mul_res_0_14 + mul_res_0_15;
						acc_type0 acc_res_0_8  = mul_res_0_16 + mul_res_0_17;
						acc_type0 acc_res_0_9  = mul_res_0_18 + mul_res_0_19;
						acc_type0 acc_res_0_10 = mul_res_0_20 + mul_res_0_21;
						acc_type0 acc_res_0_11 = mul_res_0_22 + mul_res_0_23;
						acc_type0 acc_res_0_12 = mul_res_0_24 + mul_res_0_25;
						acc_type0 acc_res_0_13 = mul_res_0_26 + mul_res_0_27;
						acc_type0 acc_res_0_14 = mul_res_0_28 + mul_res_0_29;
						acc_type0 acc_res_0_15 = mul_res_0_30 + mul_res_0_31;
						acc_type0 acc_res_0_16 = mul_res_0_32 + mul_res_0_33;
						acc_type0 acc_res_0_17 = mul_res_0_34 + mul_res_0_35;
						acc_type0 acc_res_0_18 = mul_res_0_36 + mul_res_0_37;
						acc_type0 acc_res_0_19 = mul_res_0_38 + mul_res_0_39;
						acc_type0 acc_res_0_20 = mul_res_0_40 + mul_res_0_41;
						acc_type0 acc_res_0_21 = mul_res_0_42 + mul_res_0_43;
						acc_type0 acc_res_0_22 = mul_res_0_44 + mul_res_0_45;
						acc_type0 acc_res_0_23 = mul_res_0_46 + mul_res_0_47;
						acc_type0 acc_res_0_24 = mul_res_0_48 + mul_res_0_49;
						acc_type0 acc_res_0_25 = mul_res_0_50 + mul_res_0_51;
						acc_type0 acc_res_0_26 = mul_res_0_52 + mul_res_0_53;
						acc_type0 acc_res_0_27 = mul_res_0_54 + mul_res_0_55;
						acc_type0 acc_res_0_28 = mul_res_0_56 + mul_res_0_57;
						acc_type0 acc_res_0_29 = mul_res_0_58 + mul_res_0_59;
						acc_type0 acc_res_0_30 = mul_res_0_60 + mul_res_0_61;
						acc_type0 acc_res_0_31 = mul_res_0_62 + mul_res_0_63;

						//accumulation_1
						acc_type1  acc_res_0_1_0  = acc_res_0_0 + acc_res_0_1;
						acc_type1  acc_res_0_1_1  = acc_res_0_2 + acc_res_0_3;
						acc_type1  acc_res_0_1_2  = acc_res_0_4 + acc_res_0_5;
						acc_type1  acc_res_0_1_3  = acc_res_0_6 + acc_res_0_7;
						acc_type1  acc_res_0_1_4  = acc_res_0_8 + acc_res_0_9;
						acc_type1  acc_res_0_1_5  = acc_res_0_10 + acc_res_0_11;
						acc_type1  acc_res_0_1_6  = acc_res_0_12 + acc_res_0_13;
						acc_type1  acc_res_0_1_7  = acc_res_0_14 + acc_res_0_15;
						acc_type1  acc_res_0_1_8  = acc_res_0_16 + acc_res_0_17;
						acc_type1  acc_res_0_1_9  = acc_res_0_18 + acc_res_0_19;
						acc_type1  acc_res_0_1_10 = acc_res_0_20 + acc_res_0_21;
						acc_type1  acc_res_0_1_11 = acc_res_0_22 + acc_res_0_23;
						acc_type1  acc_res_0_1_12 = acc_res_0_24 + acc_res_0_25;
						acc_type1  acc_res_0_1_13 = acc_res_0_26 + acc_res_0_27;
						acc_type1  acc_res_0_1_14 = acc_res_0_28 + acc_res_0_29;
						acc_type1  acc_res_0_1_15 = acc_res_0_30 + acc_res_0_31;

						//accumulation_2
						acc_type2 acc_res_0_2_0  = acc_res_0_1_0 + acc_res_0_1_1;
						acc_type2 acc_res_0_2_1  = acc_res_0_1_2 + acc_res_0_1_3;
						acc_type2 acc_res_0_2_2  = acc_res_0_1_4 + acc_res_0_1_5;
						acc_type2 acc_res_0_2_3  = acc_res_0_1_6 + acc_res_0_1_7;
						acc_type2 acc_res_0_2_4  = acc_res_0_1_8 + acc_res_0_1_9;
						acc_type2 acc_res_0_2_5  = acc_res_0_1_10 + acc_res_0_1_11;
						acc_type2 acc_res_0_2_6  = acc_res_0_1_12 + acc_res_0_1_13;
						acc_type2 acc_res_0_2_7  = acc_res_0_1_14 + acc_res_0_1_15;

						//accumulation_3
						acc_type3 acc_res_0_3_0  = acc_res_0_2_0 + acc_res_0_2_1;
						acc_type3 acc_res_0_3_1  = acc_res_0_2_2 + acc_res_0_2_3;
						acc_type3 acc_res_0_3_2  = acc_res_0_2_4 + acc_res_0_2_5;
						acc_type3 acc_res_0_3_3  = acc_res_0_2_6 + acc_res_0_2_7;
						//accumulation_4
						acc_type4 acc_res_0_4_0  = acc_res_0_3_0 + acc_res_0_3_1;
						acc_type4 acc_res_0_4_1  = acc_res_0_3_2 + acc_res_0_3_3;

						//accumulation_5
						acc_type5 macc_res_0 = acc_res_0_4_0 + acc_res_0_4_1;
						ofm[k][l][m] = bias_res_0 + macc_res_0;

						// accumulation_0
						acc_type0 acc_res_1_0  = mul_res_1_0  + mul_res_1_1;
						acc_type0 acc_res_1_1  = mul_res_1_2  + mul_res_1_3;
						acc_type0 acc_res_1_2  = mul_res_1_4  + mul_res_1_5;
						acc_type0 acc_res_1_3  = mul_res_1_6  + mul_res_1_7;
						acc_type0 acc_res_1_4  = mul_res_1_8  + mul_res_1_9;
						acc_type0 acc_res_1_5  = mul_res_1_10 + mul_res_1_11;
						acc_type0 acc_res_1_6  = mul_res_1_12 + mul_res_1_13;
						acc_type0 acc_res_1_7  = mul_res_1_14 + mul_res_1_15;
						acc_type0 acc_res_1_8  = mul_res_1_16 + mul_res_1_17;
						acc_type0 acc_res_1_9  = mul_res_1_18 + mul_res_1_19;
						acc_type0 acc_res_1_10 = mul_res_1_20 + mul_res_1_21;
						acc_type0 acc_res_1_11 = mul_res_1_22 + mul_res_1_23;
						acc_type0 acc_res_1_12 = mul_res_1_24 + mul_res_1_25;
						acc_type0 acc_res_1_13 = mul_res_1_26 + mul_res_1_27;
						acc_type0 acc_res_1_14 = mul_res_1_28 + mul_res_1_29;
						acc_type0 acc_res_1_15 = mul_res_1_30 + mul_res_1_31;
						acc_type0 acc_res_1_16 = mul_res_1_32 + mul_res_1_33;
						acc_type0 acc_res_1_17 = mul_res_1_34 + mul_res_1_35;
						acc_type0 acc_res_1_18 = mul_res_1_36 + mul_res_1_37;
						acc_type0 acc_res_1_19 = mul_res_1_38 + mul_res_1_39;
						acc_type0 acc_res_1_20 = mul_res_1_40 + mul_res_1_41;
						acc_type0 acc_res_1_21 = mul_res_1_42 + mul_res_1_43;
						acc_type0 acc_res_1_22 = mul_res_1_44 + mul_res_1_45;
						acc_type0 acc_res_1_23 = mul_res_1_46 + mul_res_1_47;
						acc_type0 acc_res_1_24 = mul_res_1_48 + mul_res_1_49;
						acc_type0 acc_res_1_25 = mul_res_1_50 + mul_res_1_51;
						acc_type0 acc_res_1_26 = mul_res_1_52 + mul_res_1_53;
						acc_type0 acc_res_1_27 = mul_res_1_54 + mul_res_1_55;
						acc_type0 acc_res_1_28 = mul_res_1_56 + mul_res_1_57;
						acc_type0 acc_res_1_29 = mul_res_1_58 + mul_res_1_59;
						acc_type0 acc_res_1_30 = mul_res_1_60 + mul_res_1_61;
						acc_type0 acc_res_1_31 = mul_res_1_62 + mul_res_1_63;

						//accumulation_1
						acc_type1  acc_res_1_1_0  = acc_res_1_0 + acc_res_1_1;
						acc_type1  acc_res_1_1_1  = acc_res_1_2 + acc_res_1_3;
						acc_type1  acc_res_1_1_2  = acc_res_1_4 + acc_res_1_5;
						acc_type1  acc_res_1_1_3  = acc_res_1_6 + acc_res_1_7;
						acc_type1  acc_res_1_1_4  = acc_res_1_8 + acc_res_1_9;
						acc_type1  acc_res_1_1_5  = acc_res_1_10 + acc_res_1_11;
						acc_type1  acc_res_1_1_6  = acc_res_1_12 + acc_res_1_13;
						acc_type1  acc_res_1_1_7  = acc_res_1_14 + acc_res_1_15;
						acc_type1  acc_res_1_1_8  = acc_res_1_16 + acc_res_1_17;
						acc_type1  acc_res_1_1_9  = acc_res_1_18 + acc_res_1_19;
						acc_type1  acc_res_1_1_10 = acc_res_1_20 + acc_res_1_21;
						acc_type1  acc_res_1_1_11 = acc_res_1_22 + acc_res_1_23;
						acc_type1  acc_res_1_1_12 = acc_res_1_24 + acc_res_1_25;
						acc_type1  acc_res_1_1_13 = acc_res_1_26 + acc_res_1_27;
						acc_type1  acc_res_1_1_14 = acc_res_1_28 + acc_res_1_29;
						acc_type1  acc_res_1_1_15 = acc_res_1_30 + acc_res_1_31;

						//accumulation_2
						acc_type2 acc_res_1_2_0  = acc_res_1_1_0 + acc_res_1_1_1;
						acc_type2 acc_res_1_2_1  = acc_res_1_1_2 + acc_res_1_1_3;
						acc_type2 acc_res_1_2_2  = acc_res_1_1_4 + acc_res_1_1_5;
						acc_type2 acc_res_1_2_3  = acc_res_1_1_6 + acc_res_1_1_7;
						acc_type2 acc_res_1_2_4  = acc_res_1_1_8 + acc_res_1_1_9;
						acc_type2 acc_res_1_2_5  = acc_res_1_1_10 + acc_res_1_1_11;
						acc_type2 acc_res_1_2_6  = acc_res_1_1_12 + acc_res_1_1_13;
						acc_type2 acc_res_1_2_7  = acc_res_1_1_14 + acc_res_1_1_15;

						//accumulation_3
						acc_type3 acc_res_1_3_0  = acc_res_1_2_0 + acc_res_1_2_1;
						acc_type3 acc_res_1_3_1  = acc_res_1_2_2 + acc_res_1_2_3;
						acc_type3 acc_res_1_3_2  = acc_res_1_2_4 + acc_res_1_2_5;
						acc_type3 acc_res_1_3_3  = acc_res_1_2_6 + acc_res_1_2_7;
						//accumulation_4
						acc_type4 acc_res_1_4_0  = acc_res_1_3_0 + acc_res_1_3_1;
						acc_type4 acc_res_1_4_1  = acc_res_1_3_2 + acc_res_1_3_3;

						//accumulation_5
						acc_type5 macc_res_1 = acc_res_1_4_0 + acc_res_1_4_1;
						ofm[k][l][m+1] = bias_res_1 + macc_res_1;
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
			for(int k = 0; k < TM; k ++){
#pragma HLS PIPELINE
				int g = ofm[i][j][k];
				float gg = float(g);
				// anti-quantization, leaky-ReLu, saturation, quantization
				float pixel_aq;
				if((g > 0) || is_yolo)
					pixel_aq = MUL_FP(gg,pscaler);
				else
					pixel_aq = MUL_FP(gg,nscaler);

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
#pragma HLS LOOP_TRIPCOUNT min=8 max=64
		for(int j = 0; j < layer.RC_out_size; j ++){  // 16
#pragma HLS LOOP_TRIPCOUNT min=8 max=64
			AXI_VAL_O output_0;
			AXI_VAL_O output_1;
			int temp = 16;//TM/2
			for(int k = 0; k < temp; k +=16){  // 2
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

				output_1.data[0] = ofm_mpq[i][j][k + temp];
				output_1.data[1] = ofm_mpq[i][j][k + temp + 1];
				output_1.data[2] = ofm_mpq[i][j][k + temp + 2];
				output_1.data[3] = ofm_mpq[i][j][k + temp + 3];
				output_1.data[4] = ofm_mpq[i][j][k + temp + 4];
				output_1.data[5] = ofm_mpq[i][j][k + temp + 5];
				output_1.data[6] = ofm_mpq[i][j][k + temp + 6];
				output_1.data[7] = ofm_mpq[i][j][k + temp + 7];
				output_1.data[8] = ofm_mpq[i][j][k + temp + 8];
				output_1.data[9] = ofm_mpq[i][j][k + temp + 9];
				output_1.data[10] = ofm_mpq[i][j][k + temp + 10];
				output_1.data[11] = ofm_mpq[i][j][k + temp + 11];
				output_1.data[12] = ofm_mpq[i][j][k + temp + 12];
				output_1.data[13] = ofm_mpq[i][j][k + temp + 13];
				output_1.data[14] = ofm_mpq[i][j][k + temp + 14];
				output_1.data[15] = ofm_mpq[i][j][k + temp + 15];
				if(
						(i == (layer.RC_out_size - 1))
						&& (j == (layer.RC_out_size - 1))
						&& (k == (temp - 16))
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
#pragma HLS LOOP_TRIPCOUNT min=1 max=1
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

//	#pragma HLS DATAFLOW
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
//#pragma HLS INTERFACE depth=17424 port=str_in_0

#pragma HLS INTERFACE axis register both port=str_in_1
#pragma HLS DATA_PACK variable=str_in_1
//#pragma HLS INTERFACE depth=2320 port=str_in_1

#pragma HLS INTERFACE axis register both port=str_out_0
#pragma HLS DATA_PACK variable=str_out_0 struct_level
//#pragma HLS INTERFACE depth=8192 port=str_out_0

#pragma HLS INTERFACE axis register both port=str_out_1
#pragma HLS DATA_PACK variable=str_out_1 struct_level
//#pragma HLS INTERFACE depth=8192 port=str_out_1
	dtype ifm[IFM_MAX][IFM_MAX][TN];
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
#pragma HLS LOOP_TRIPCOUNT min=1 max=4
		for(int C_iter = 0; C_iter < layer.RC_block_num; C_iter ++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=4
			convolution_label0:for(int CHO_iter = 0; CHO_iter < layer.CHO_block_num; CHO_iter ++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=32

				int R_iter_temp = R_iter;
				int C_iter_temp = C_iter;
				int CHO_iter_temp = CHO_iter;


				convCHI_lebel:for(int CHI_iter = 0; CHI_iter < layer.CHI_block_num; CHI_iter ++){
				#pragma HLS DATAFLOW
	#pragma HLS LOOP_TRIPCOUNT min=1 max=16

						load_ifm(layer.RC_block_size, str_in_0,ifm);

						load_ibw(layer.kernel, str_in_1, bias, weights);
						//load_ibw(layer.kernel, str_in_1);

						macc(layer.kernel, layer.RC_out_size, layer.stride, CHI_iter, ifm, weights, bias, ofm);
						//macc(layer.kernel, layer.RC_out_size, layer.stride, CHI_iter);
					}
				#pragma HLS DATAFLOW
				last_proc(layer.is_yolo, layer.pscaler,layer.nscaler, layer.RC_out_size,  ofm, ofm_mpq);
				//last_proc(layer.is_yolo, layer.pscaler,layer.nscaler, layer.RC_out_size);
				transfer_ofm(layer, R_iter_temp, C_iter_temp, CHO_iter_temp, ofm_mpq, str_out_0, str_out_1);
			}
		}
	}
}
