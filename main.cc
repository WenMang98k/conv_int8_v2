#include "main.hpp"
using namespace std;

XConvolution Conv;
XConvolution_Config *CONV_Ptr;
XAxiDma AxiDma_0, AxiDma_1, AxiDma_2, AxiDma_3;
XAxiDma_Config *CH0_CfgPtr, *CH1_CfgPtr, *CH2_CfgPtr, *CH3_CfgPtr;

int SIZE_PTXT	= 0x00100000;
int IMAGE_BASE	= 0x40000000;
int WEIGHT_BASE = 0x48000000;
int OUT0_BASE   = 0x50000000;
int OUT1_BASE   = 0x58000000;
int OUT2_BASE   = 0x60000000;
int OUT3_BASE   = 0x68000000;

int main()
{
	printf("DMA_CH_Init\n");
	DMA_CH_Init();
	FPGA_core_Init();

	//						id RC_bn RC_bs	RC_rs RC_os	CHI_bn CHI_bs CHI_rs CHO_bn CHO_bs CHO_rs kernel stride	psca		          is_bool nsca
	Layer_Type layer_0 = {	0, 4,    66,    64,   64,   1,     64,    64,    1,     32,    32,    3,     1,      0.0034051907714456320,0,	0.0034051907714456320};
	Layer_Type layer_1 = {	1, 4,    66,    64,   32,   1,     64,    64,    2,     32,    32,    3,     2,      0.011046293191612,		0,      0.011046293191612};
	Layer_Type layer_2 = {	2, 2,    64,    64,   64,   1,     64,    64,    1,     32,    32,    1,     1,      0.016516543924809,		0,      0.016516543924809};
	Layer_Type layer_3 = {	3, 2,    66,    64,   64,   1,     64,    64,    2,     32,    32,    3,     1,      0.003612776985392,	0,       0.003612776985392};
	Layer_Type layer_4 = {	4, 2,    66,    64,   32,   1,     64,    64,    4,     32,    32,    3,     2,      0.00377606973052,	0,      0.00377606973052};
	Layer_Type layer_5 = {	5, 1,    64,    64,   64,   2,     64,    64,    2,     32,    32,    1,     1,      0.005535673350096,	0,      0.005535673350096};
	Layer_Type layer_6 = {	6, 1,    66,    64,   64,   1,     64,    64,    4,     32,    32,    3,     1,      0.003420350141823,	0,      0.003420350141823};
	Layer_Type layer_7 = {	7, 1,    64,    64,   64,   2,     64,    64,    2,     32,    32,    1,     1,      0.007086453028023,	0,      0.007086453028023};
	Layer_Type layer_8 = {	8, 1,    66,    64,   64,   1,     64,    64,    4,     32,    32,    3,     1,      0.002834026934579,	0,      0.002834026934579};
	Layer_Type layer_9 = {	9, 1,    66,    64,   32,   2,     64,    64,    8,     32,    32,    3,     2,      0.004447642248124,	0,      0.004447642248124};
	Layer_Type layer_10 = {	10, 1,    32,    32,   32,   4,     64,    64,    2,     32,    32,    1,    1,      0.005550272762775,	0,      0.005550272762775};
	Layer_Type layer_11 = {	11, 1,    34,    32,   32,   1,     64,    64,    8,     32,    32,    3,    1,      0.003903054399416,	0,      0.003903054399416};
	Layer_Type layer_12 = {	12, 1,    32,    32,   32,   4,     64,    64,    2,     32,    32,    1,    1,      0.007421235553920,	0,      0.007421235553920};
	Layer_Type layer_13 = {	13, 1,    34,    32,   32,   1,     64,    64,    8,     32,    32,    3,    1,      0.003486304543912 ,0, 		0.003486304543912};
	Layer_Type layer_14 = {	14, 1,    32,    32,   32,   4,     64,    64,    2,     32,    32,    1,    1,      0.006390209775418,	0,      0.006390209775418};
	Layer_Type layer_15 = {	15, 1,    34,    32,   32,   1,     64,    64,    8,     32,    32,    3,    1,      0.003799889469519,	0,      0.003799889469519};
	Layer_Type layer_16 = {	16, 1,    32,    32,   32,   4,     64,    64,    2,     32,    32,    1,    1,      0.007047590799630,	0,      0.007047590799630};
	Layer_Type layer_17 = {	17, 1,    34,    32,   32,   1,     64,    64,    8,     32,    32,    3,    1,      0.004480268806219,	0,      0.004480268806219};
	Layer_Type layer_18 = {	18, 1,    32,    32,   32,   4,     64,    64,    2,     32,    32,    1,    1,      0.004294893238693,	0,      0.004294893238693};
	Layer_Type layer_19 = {	19, 1,    34,    32,   32,   1,     64,    64,    8,     32,    32,    3,    1,      0.004211354535073,	0,      0.004211354535073};
	Layer_Type layer_20 = {	20, 1,    34,    32,   16,   4,     64,    64,   16,     32,    32,    3,    2,      0.002542974194512,	0,      0.002542974194512};
	Layer_Type layer_21 = { 21, 1,    16,    16,   16,   8,     64,    64,    2,     32,    32,    1,    1,      0.004363661166281,	0,      0.004363661166281};
	Layer_Type layer_22 = {	22, 1,    18,    16,   16,   1,     64,    64,   16,     32,    32,    3,    1,      0.004775461740792,	0,      0.004775461740792};
	Layer_Type layer_23 = {	23, 1,    16,    16,   16,   8,     64,    64,    2,     32,    32,    1,    1,      0.006709467619658,	0,      0.006709467619658};
	Layer_Type layer_24 = {	24, 1,    18,    16,   16,   1,     64,    64,   16,     32,    32,    3,    1,      0.005527479574084,	0,      0.005527479574084};
	Layer_Type layer_25 = {	25, 1,    18,    16,    8,   8,     64,    64,   30,     32,    32,    3,    2,      0.002295827260241,	0,      0.002295827260241};
	Layer_Type layer_26 = {	26, 1,    8,      8,    8,   15,     64,   64,   4,     32,    32,     1,    1,      0.005028726533055,	0,      0.005028726533055};
	Layer_Type layer_27 = {	27, 1,    10,     8,    8,   2,     64,    64,   30,     32,    32,    3,    1,      0.011254572309554,	0,      0.011254572309554};
	Layer_Type layer_28 = {	28, 1,    8,      8,    8,   15,     64,   64,    12,     32,    32,   1,    1,      0.007299824152142,	0,      0.007299824152142};
	Layer_Type layer_29 = {	29, 1,    10,     8,    8,   6,     64,    64,    26,     32,    32,   3,    1,      0.003344013122842,	0,      0.003344013122842};
	Layer_Type layer_30 = {	30, 1,    8,      8,    8,   13,     64,   64,    12,     32,    32,   1,    1,      0.003649215679616,	0,      0.003649215679616};
	Layer_Type layer_31 = {	31, 1,    10,     8,    8,   6,     64,    64,    28,     32,    32,   3,    1,      0.002113708760589,	0,      0.002113708760589};
	Layer_Type layer_32 = {	32, 1,    8,      8,    8,   14,     64,   64,    14,     32,    32,   1,    1,      0.003481580177322,	0,      0.003481580177322};
	Layer_Type layer_33 = {	33, 1,    10,     8,    8,   7,     64,    64,    32,     32,    32,   3,    1,      0.001957067754120,	0,      0.001957067754120};
	Layer_Type layer_34 = {	34, 1,    8,      8,    8,   16,     64,   64,    3,     32,    32,    1,    1,      0.001736760372296,	1,      0.001736760372296};
	//yolo
	Layer_Type layer_35 = {	35, 1,    8,      8,    8,   7,     64,    64,    8,     32,    32,    1,    1,      0.002467300044373,	0,      0.002467300044373};
	Layer_Type layer_36 = {	36, 1,    16,    16,   16,   12,     64,   64,    6,     32,    32,    1,    1,      0.004166347905993,	0,      0.004166347905993};
	Layer_Type layer_37 = {	37, 1,    18,    16,   16,   3,     64,    64,    12,     32,    32,   3,    1,      0.001692668069154,	0,      0.001692668069154};
	Layer_Type layer_38 = {	38, 1,    16,    16,   16,   6,     64,    64,    6,     32,    32,    1,    1,      0.004968570079654,	0,      0.004968570079654};
	Layer_Type layer_39 = {	39, 1,    18,    16,   16,   3,     64,    64,    14,     32,    32,   3,    1,      0.003092883853242,	0,      0.003092883853242};
	Layer_Type layer_40 = {	40, 1,    16,    16,   16,   7,     64,    64,    6,     32,    32,    1,    1,      0.008578182198107,	0,      0.008578182198107};
	Layer_Type layer_41 = {	41, 1,    18,    16,   16,   3,     64,    64,   16,     32,    32,    3,    1,      0.002975841984153,	0,      0.002975841984153};
	Layer_Type layer_42 = {	42, 1,    16,    16,   16,   8,     64,    64,    3,     32,    32,    1,    1,      0.001412006327882,	1,      0.001412006327882};
	//yolo
	Layer_Type layer_43 = {	43, 1,    16,    16,   16,   3,     64,    64,    4,     32,    32,    1,    1,      0.004228822886944,	0,      0.004228822886944};
	Layer_Type layer_44 = {	44, 1,    32,    32,   32,   6,     64,    64,    4,     32,    32,    1,    1,      0.007910576649010,	0,      0.007910576649010};
	Layer_Type layer_45 = {	45, 1,    34,    32,   32,   2,     64,    64,    6,     32,    32,    3,    1,      0.001746389316395,	0,      0.001746389316395};
	Layer_Type layer_46 = {	46, 1,    32,    32,   32,   3,     64,    64,    4,     32,    32,    1,    1,      0.005618951283395,	0,      0.005618951283395};
	Layer_Type layer_47 = {	47, 1,    34,    32,   32,   2,     64,    64,    6,     32,    32,    3,    1,      0.001943383482285,	0,      0.001943383482285};
	Layer_Type layer_48 = {	48, 1,    32,    32,   32,   3,     64,    64,    4,     32,    32,    1,    1,      0.009292103350163,	0,      0.009292103350163};
	Layer_Type layer_49 = {	49, 1,    34,    32,   32,   2,     64,    64,    8,     32,    32,    3,    1,      0.001526988693513,	0,      0.001526988693513};
	Layer_Type layer_50 = {	50, 1,    32,    32,   32,   4,     64,    64,    3,     32,    32,    1,    1,      0.002612790092826,	1,      0.002612790092826};
	//yolo
	// load ifm
	load_file((char *)"i_0.bin", IMAGE_BASE);
	int len_ifm_byte = ifm_len(&layer_0);
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_start, t_end;
	XTime_GetTime(&t_start);
	XFPGA_setLayerConfig(layer_0);
////	XFPGA_read_LayerConfig();
	XTime t_w0_0, t_w0_1;
	XTime_GetTime(&t_w0_0);
	load_file((char *)"w_0.bin", WEIGHT_BASE);
	int len_w_byte = weights_len(&layer_0);
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w0_1);
	u32 tw0 = (t_w0_1 - t_w0_0);
	DMA_Work(layer_0);
//	load_file((char *)"o_0.bin", OUT2_BASE);
//	data_check_ofm((dtype *)OUT2_BASE, (dtype *)OUT0_BASE,layer_0);
//	load_file((char *)"o1_0.bin", OUT3_BASE);
//	data_check_ofm((dtype *)OUT3_BASE, (dtype *)OUT1_BASE,layer_0);
//	layer_0_result_data_reorg((dtype*)OUT0_BASE, (dtype*)OUT1_BASE, (dtype*)IMAGE_BASE ,layer_0, layer_1);
//	load_file((char *)"i_1.bin", OUT2_BASE);
//	int len = layer_1.RC_block_num*layer_1.RC_block_num*layer_1.CHI_block_num*layer_1.CHO_block_num*layer_1.RC_block_size*layer_1.RC_block_size*layer_1.CHI_block_size;
//	data_check((dtype *)OUT2_BASE, (dtype *)IMAGE_BASE,len);

	XFPGA_setLayerConfig(layer_1);   																	//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_1.bin", IMAGE_BASE);
	len_ifm_byte = ifm_len(&layer_1);  																	//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w1_0, t_w1_1;  																			// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w1_0);																			//t_wid_0,
	load_file((char *)"w_1.bin", WEIGHT_BASE);														//bw_id.bin
	len_w_byte = weights_len(&layer_1);  															 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w1_1);																			//t_wid_1,
	u32 tw1 = (t_w1_1 - t_w1_0);																	// tw_id,  t_wid_1 -t_wid_,
	int len_ofm_byte = ofm_len(&layer_1);
	DMA_Work(layer_1);																				 //layer_id
//	load_file((char *)"o_1.bin", OUT2_BASE); 														 //ofm_id.bin
//	data_check_ofm((dtype *)OUT2_BASE, (dtype *)OUT0_BASE,layer_1);										//layer_id
//	load_file((char *)"o1_1.bin", OUT3_BASE);																//ofm1_id.bin
//	data_check_ofm((dtype *)OUT3_BASE, (dtype *)OUT1_BASE,layer_1);										//layer_id

	XFPGA_setLayerConfig(layer_2);   																	//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_2.bin", IMAGE_BASE);                                                           //i_id.bin
//	data_check_ifm((dtype *)IMAGE_BASE, (dtype *)IMAGE_BASE,layer_2);										//layer_id
	len_ifm_byte = ifm_len(&layer_2);  																	//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w2_0, t_w2_1;  													// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w2_0);																			//t_wid_0,
	load_file((char *)"w_2.bin", WEIGHT_BASE);														//bw_id.bin
	len_w_byte = weights_len(&layer_2);  									 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
//	data_check_bw((dtype *)WEIGHT_BASE, (dtype *)WEIGHT_BASE,layer_2);										//layer_id
	XTime_GetTime(&t_w2_1);																			//t_wid_1,
	u32 tw2 = (t_w2_1 - t_w2_0);																	// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_2);															//layer_id
	DMA_Work(layer_2);																				 //layer_id
//	load_file((char *)"o_2.bin", OUT2_BASE); 														 //ofm_id.bin
//	data_check_ofm((dtype *)OUT2_BASE, (dtype *)OUT0_BASE,layer_2);										//layer_id
//	load_file((char *)"o1_2.bin", OUT3_BASE);																//ofm1_id.bin
//	data_check_ofm((dtype *)OUT3_BASE, (dtype *)OUT1_BASE,layer_2);										//layer_id

	XFPGA_setLayerConfig(layer_3);   																	//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_3.bin", IMAGE_BASE);                                                          //i_id.bin
	len_ifm_byte = ifm_len(&layer_3);  																	//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w3_0, t_w3_1;  																			// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w3_0);																			//t_wid_0,
	load_file((char *)"w_3.bin", WEIGHT_BASE);														//bw_id.bin
	len_w_byte = weights_len(&layer_3);  															 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w3_1);																			//t_wid_1,
	u32 tw3 = (t_w3_1 - t_w3_0);																	// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_3);   //layer_id
	DMA_Work(layer_3);	  // layer_id

	XFPGA_setLayerConfig(layer_4);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_4.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_4);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w4_0, t_w4_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w4_0);								//t_wid_0,
	load_file((char *)"w_4.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_4);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w4_1);								//t_wid_1,
	u32 tw4 = (t_w4_1 - t_w4_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_4);   			//layer_id
	DMA_Work(layer_4);	  								// layer_id

	XFPGA_setLayerConfig(layer_5);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_5.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_5);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w5_0, t_w5_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w5_0);								//t_wid_0,
	load_file((char *)"w_5.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_5);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w5_1);								//t_wid_1,
	u32 tw5 = (t_w5_1 - t_w5_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_5);   			//layer_id
	DMA_Work(layer_5);	  								// layer_id

	XFPGA_setLayerConfig(layer_6);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_6.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_6);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w6_0, t_w6_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w6_0);								//t_wid_0,
	load_file((char *)"w_6.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_6);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w6_1);								//t_wid_1,
	u32 tw6 = (t_w6_1 - t_w6_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_6);   			//layer_id
	DMA_Work(layer_6);	  								// layer_id

	XFPGA_setLayerConfig(layer_7);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_7.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_7);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w7_0, t_w7_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w7_0);								//t_wid_0,
	load_file((char *)"w_7.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_7);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w7_1);								//t_wid_1,
	u32 tw7 = (t_w7_1 - t_w7_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_7);   			//layer_id
	DMA_Work(layer_7);	  								// layer_id

	XFPGA_setLayerConfig(layer_8);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_8.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_8);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w8_0, t_w8_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w8_0);								//t_wid_0,
	load_file((char *)"w_8.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_8);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w8_1);								//t_wid_1,
	u32 tw8 = (t_w8_1 - t_w8_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_8);   			//layer_id
	DMA_Work(layer_8);	  								// layer_id

	XFPGA_setLayerConfig(layer_9);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_9.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_9);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w9_0, t_w9_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w9_0);								//t_wid_0,
	load_file((char *)"w_9.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_9);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w9_1);								//t_wid_1,
	u32 tw9 = (t_w9_1 - t_w9_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_9);   			//layer_id
	DMA_Work(layer_9);	  								// layer_id

	XFPGA_setLayerConfig(layer_10);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_10.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_10);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w10_0, t_w10_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w10_0);								//t_wid_0,
	load_file((char *)"w_10.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_10);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w10_1);								//t_wid_1,
	u32 tw10 = (t_w10_1 - t_w10_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_10);   			//layer_id
	DMA_Work(layer_10);	  								// layer_id

	XFPGA_setLayerConfig(layer_11);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_11.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_11);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w11_0, t_w11_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w11_0);								//t_wid_0,
	load_file((char *)"w_11.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_11);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w11_1);								//t_wid_1,
	u32 tw11 = (t_w11_1 - t_w11_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_11);   			//layer_id
	DMA_Work(layer_11);	  								// layer_id

	XFPGA_setLayerConfig(layer_12);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_12.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_12);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w12_0, t_w12_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w12_0);								//t_wid_0,
	load_file((char *)"w_12.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_12);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w12_1);								//t_wid_1,
	u32 tw12 = (t_w12_1 - t_w12_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_12);   			//layer_id
	DMA_Work(layer_12);	  								// layer_id

	XFPGA_setLayerConfig(layer_13);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_13.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_13);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w13_0, t_w13_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w13_0);								//t_wid_0,
	load_file((char *)"w_13.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_13);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w13_1);								//t_wid_1,
	u32 tw13 = (t_w13_1 - t_w13_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_13);   			//layer_id
	DMA_Work(layer_13);	  								// layer_id

	XFPGA_setLayerConfig(layer_14);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_14.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_14);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w14_0, t_w14_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w14_0);								//t_wid_0,
	load_file((char *)"w_14.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_14);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w14_1);								//t_wid_1,
	u32 tw14 = (t_w14_1 - t_w14_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_14);   			//layer_id
	DMA_Work(layer_14);	  								// layer_id

	XFPGA_setLayerConfig(layer_15);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_15.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_15);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w15_0, t_w15_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w15_0);								//t_wid_0,
	load_file((char *)"w_15.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_15);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w15_1);								//t_wid_1,
	u32 tw15 = (t_w15_1 - t_w15_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_15);   			//layer_id
	DMA_Work(layer_15);	  								// layer_id

	XFPGA_setLayerConfig(layer_16);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_16.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_16);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w16_0, t_w16_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w16_0);								//t_wid_0,
	load_file((char *)"w_16.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_16);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w16_1);								//t_wid_1,
	u32 tw16 = (t_w16_1 - t_w16_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_16);   			//layer_id
	DMA_Work(layer_16);	  								// layer_id

	XFPGA_setLayerConfig(layer_17);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_17.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_17);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w17_0, t_w17_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w17_0);								//t_wid_0,
	load_file((char *)"w_17.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_17);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w17_1);								//t_wid_1,
	u32 tw17 = (t_w17_1 - t_w17_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_17);   			//layer_id
	DMA_Work(layer_17);	  								// layer_id

	XFPGA_setLayerConfig(layer_18);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_18.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_18);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w18_0, t_w18_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w18_0);								//t_wid_0,
	load_file((char *)"w_18.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_18);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w18_1);								//t_wid_1,
	u32 tw18 = (t_w18_1 - t_w18_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_18);   			//layer_id
	DMA_Work(layer_18);	  								// layer_id

	XFPGA_setLayerConfig(layer_19);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_19.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_19);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w19_0, t_w19_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w19_0);								//t_wid_0,
	load_file((char *)"w_19.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_19);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w19_1);								//t_wid_1,
	u32 tw19 = (t_w19_1 - t_w19_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_19);   			//layer_id
	DMA_Work(layer_19);	  								// layer_id

	XFPGA_setLayerConfig(layer_20);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_20.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_20);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w20_0, t_w20_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w20_0);								//t_wid_0,
	load_file((char *)"w_20.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_20);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w20_1);								//t_wid_1,
	u32 tw20 = (t_w20_1 - t_w20_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_20);   			//layer_id
	DMA_Work(layer_20);	  								// layer_id

	XFPGA_setLayerConfig(layer_21);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_21.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_21);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w21_0, t_w21_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w21_0);								//t_wid_0,
	load_file((char *)"w_21.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_21);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w21_1);								//t_wid_1,
	u32 tw21 = (t_w21_1 - t_w21_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_21);   			//layer_id
	DMA_Work(layer_21);	  								// layer_id

	XFPGA_setLayerConfig(layer_22);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_22.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_22);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w22_0, t_w22_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w22_0);								//t_wid_0,
	load_file((char *)"w_22.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_22);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w22_1);								//t_wid_1,
	u32 tw22 = (t_w22_1 - t_w22_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_22);   			//layer_id
	DMA_Work(layer_22);	  								// layer_id

	XFPGA_setLayerConfig(layer_23);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_23.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_23);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w23_0, t_w23_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w23_0);								//t_wid_0,
	load_file((char *)"w_23.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_23);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w23_1);								//t_wid_1,
	u32 tw23 = (t_w23_1 - t_w23_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_23);   			//layer_id
	DMA_Work(layer_23);	  								// layer_id

	XFPGA_setLayerConfig(layer_24);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_24.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_24);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w24_0, t_w24_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w24_0);								//t_wid_0,
	load_file((char *)"w_24.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_24);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w24_1);								//t_wid_1,
	u32 tw24 = (t_w24_1 - t_w24_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_24);   			//layer_id
	DMA_Work(layer_24);	  								// layer_id

	XFPGA_setLayerConfig(layer_25);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_25.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_25);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w25_0, t_w25_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w25_0);								//t_wid_0,
	load_file((char *)"w_25.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_25);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w25_1);								//t_wid_1,
	u32 tw25 = (t_w25_1 - t_w25_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_25);   			//layer_id
	DMA_Work(layer_25);	  								// layer_id

	XFPGA_setLayerConfig(layer_26);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_26.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_26);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w26_0, t_w26_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w26_0);								//t_wid_0,
	load_file((char *)"w_26.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_26);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w26_1);								//t_wid_1,
	u32 tw26 = (t_w26_1 - t_w26_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_26);   			//layer_id
	DMA_Work(layer_26);	  								// layer_id

	XFPGA_setLayerConfig(layer_27);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_27.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_27);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w27_0, t_w27_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w27_0);								//t_wid_0,
	load_file((char *)"w_27.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_27);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w27_1);								//t_wid_1,
	u32 tw27 = (t_w27_1 - t_w27_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_27);   			//layer_id
	DMA_Work(layer_27);	  								// layer_id

	XFPGA_setLayerConfig(layer_28);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_28.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_28);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w28_0, t_w28_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w28_0);								//t_wid_0,
	load_file((char *)"w_28.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_28);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w28_1);								//t_wid_1,
	u32 tw28 = (t_w28_1 - t_w28_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_28);   			//layer_id
	DMA_Work(layer_28);	  								// layer_id

	XFPGA_setLayerConfig(layer_29);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_29.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_29);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w29_0, t_w29_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w29_0);								//t_wid_0,
	load_file((char *)"w_29.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_29);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w29_1);								//t_wid_1,
	u32 tw29 = (t_w29_1 - t_w29_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_29);   			//layer_id
	DMA_Work(layer_29);	  								// layer_id

	XFPGA_setLayerConfig(layer_30);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_30.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_30);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w30_0, t_w30_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w30_0);								//t_wid_0,
	load_file((char *)"w_30.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_30);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w30_1);								//t_wid_1,
	u32 tw30 = (t_w30_1 - t_w30_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_30);   			//layer_id
	DMA_Work(layer_30);	  								// layer_id

	XFPGA_setLayerConfig(layer_31);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_31.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_31);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w31_0, t_w31_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w31_0);								//t_wid_0,
	load_file((char *)"w_31.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_31);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w31_1);								//t_wid_1,
	u32 tw31 = (t_w31_1 - t_w31_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_31);   			//layer_id
	DMA_Work(layer_31);	  								// layer_id

	XFPGA_setLayerConfig(layer_32);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_32.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_32);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w32_0, t_w32_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w32_0);								//t_wid_0,
	load_file((char *)"w_32.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_32);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w32_1);								//t_wid_1,
	u32 tw32 = (t_w32_1 - t_w32_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_32);   			//layer_id
	DMA_Work(layer_32);	  								// layer_id

	XFPGA_setLayerConfig(layer_33);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_33.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_33);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w33_0, t_w33_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w33_0);								//t_wid_0,
	load_file((char *)"w_33.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_33);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w33_1);								//t_wid_1,
	u32 tw33 = (t_w33_1 - t_w33_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_33);   			//layer_id
	DMA_Work(layer_33);	  								// layer_id

	XFPGA_setLayerConfig(layer_34);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_34.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_34);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w34_0, t_w34_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w34_0);								//t_wid_0,
	load_file((char *)"w_34.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_34);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w34_1);								//t_wid_1,
	u32 tw34 = (t_w34_1 - t_w34_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_34);   			//layer_id
	DMA_Work(layer_34);	  								// layer_id

	XFPGA_setLayerConfig(layer_35);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_35.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_35);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w35_0, t_w35_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w35_0);								//t_wid_0,
	load_file((char *)"w_35.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_35);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w35_1);								//t_wid_1,
	u32 tw35 = (t_w35_1 - t_w35_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_35);   			//layer_id
	DMA_Work(layer_35);	  								// layer_id

	XFPGA_setLayerConfig(layer_36);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_36.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_36);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w36_0, t_w36_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w36_0);								//t_wid_0,
	load_file((char *)"w_36.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_36);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w36_1);								//t_wid_1,
	u32 tw36 = (t_w36_1 - t_w36_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_36);   			//layer_id
	DMA_Work(layer_36);	  								// layer_id

	XFPGA_setLayerConfig(layer_37);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_37.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_37);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w37_0, t_w37_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w37_0);								//t_wid_0,
	load_file((char *)"w_37.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_37);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w37_1);								//t_wid_1,
	u32 tw37 = (t_w37_1 - t_w37_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_37);   			//layer_id
	DMA_Work(layer_37);	  								// layer_id

	XFPGA_setLayerConfig(layer_38);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_38.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_38);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w38_0, t_w38_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w38_0);								//t_wid_0,
	load_file((char *)"w_38.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_38);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w38_1);								//t_wid_1,
	u32 tw38 = (t_w38_1 - t_w38_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_38);   			//layer_id
	DMA_Work(layer_38);	  								// layer_id

	XFPGA_setLayerConfig(layer_39);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_39.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_39);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w39_0, t_w39_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w39_0);								//t_wid_0,
	load_file((char *)"w_39.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_39);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w39_1);								//t_wid_1,
	u32 tw39 = (t_w39_1 - t_w39_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_39);   			//layer_id
	DMA_Work(layer_39);	  								// layer_id

	XFPGA_setLayerConfig(layer_40);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_40.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_40);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w40_0, t_w40_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w40_0);								//t_wid_0,
	load_file((char *)"w_40.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_40);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w40_1);								//t_wid_1,
	u32 tw40 = (t_w40_1 - t_w40_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_40);   			//layer_id
	DMA_Work(layer_40);	  								// layer_id

	XFPGA_setLayerConfig(layer_41);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_41.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_41);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w41_0, t_w41_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w41_0);								//t_wid_0,
	load_file((char *)"w_41.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_41);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w41_1);								//t_wid_1,
	u32 tw41 = (t_w41_1 - t_w41_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_41);   			//layer_id
	DMA_Work(layer_41);	  								// layer_id

	XFPGA_setLayerConfig(layer_42);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_42.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_42);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w42_0, t_w42_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w42_0);								//t_wid_0,
	load_file((char *)"w_42.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_42);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w42_1);								//t_wid_1,
	u32 tw42 = (t_w42_1 - t_w42_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_42);   			//layer_id
	DMA_Work(layer_42);	  								// layer_id

	XFPGA_setLayerConfig(layer_43);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_43.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_43);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w43_0, t_w43_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w43_0);								//t_wid_0,
	load_file((char *)"w_43.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_43);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w43_1);								//t_wid_1,
	u32 tw43 = (t_w43_1 - t_w43_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_43);   			//layer_id
	DMA_Work(layer_43);	  								// layer_id

	XFPGA_setLayerConfig(layer_44);   						//layer_id
////	XFPGA_read_LayerConfig();
	load_file((char *)"i_44.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_44);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w44_0, t_w44_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w44_0);								//t_wid_0,
	load_file((char *)"w_44.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_44);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w44_1);								//t_wid_1,
	u32 tw44 = (t_w44_1 - t_w44_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_44);   			//layer_id
	DMA_Work(layer_44);	  								// layer_id

	XFPGA_setLayerConfig(layer_45);   						//layer_id
//	XFPGA_read_LayerConfig();
	load_file((char *)"i_45.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_45);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w45_0, t_w45_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w45_0);								//t_wid_0,
	load_file((char *)"w_45.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_45);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w45_1);								//t_wid_1,
	u32 tw45 = (t_w45_1 - t_w45_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_45);   			//layer_id
	DMA_Work(layer_45);	  								// layer_id

	XFPGA_setLayerConfig(layer_46);   						//layer_id
//	XFPGA_read_LayerConfig();
	load_file((char *)"i_46.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_46);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w46_0, t_w46_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w46_0);								//t_wid_0,
	load_file((char *)"w_46.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_46);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w46_1);								//t_wid_1,
	u32 tw46 = (t_w46_1 - t_w46_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_46);   			//layer_id
	DMA_Work(layer_46);	  								// layer_id

	XFPGA_setLayerConfig(layer_47);   						//layer_id
//	XFPGA_read_LayerConfig();
	load_file((char *)"i_47.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_47);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w47_0, t_w47_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w47_0);								//t_wid_0,
	load_file((char *)"w_47.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_47);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w47_1);								//t_wid_1,
	u32 tw47 = (t_w47_1 - t_w47_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_47);   			//layer_id
	DMA_Work(layer_47);	  								// layer_id

	XFPGA_setLayerConfig(layer_48);   						//layer_id
//	XFPGA_read_LayerConfig();
	load_file((char *)"i_48.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_48);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w48_0, t_w48_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w48_0);								//t_wid_0,
	load_file((char *)"w_48.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_48);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w48_1);								//t_wid_1,
	u32 tw48 = (t_w48_1 - t_w48_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_48);   			//layer_id
	DMA_Work(layer_48);	  								// layer_id

	XFPGA_setLayerConfig(layer_49);   						//layer_id
//	XFPGA_read_LayerConfig();
	load_file((char *)"i_49.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_49);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w49_0, t_w49_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w49_0);								//t_wid_0,
	load_file((char *)"w_49.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_49);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w49_1);								//t_wid_1,
	u32 tw49 = (t_w49_1 - t_w49_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_49);   			//layer_id
	DMA_Work(layer_49);	  								// layer_id

	XFPGA_setLayerConfig(layer_50);   						//layer_id
//	XFPGA_read_LayerConfig();
	load_file((char *)"i_50.bin", IMAGE_BASE);               //i_id.bin
	len_ifm_byte = ifm_len(&layer_50);  					//layer_id
	Xil_DCacheFlushRange(IMAGE_BASE, len_ifm_byte);
	XTime t_w50_0, t_w50_1;  								// t_wid_0,t_wid_1;
	XTime_GetTime(&t_w50_0);								//t_wid_0,
	load_file((char *)"w_50.bin", WEIGHT_BASE);				//bw_id.bin
	len_w_byte = weights_len(&layer_50);  				 //layer_id
	Xil_DCacheFlushRange(WEIGHT_BASE, len_w_byte);
	XTime_GetTime(&t_w50_1);								//t_wid_1,
	u32 tw50 = (t_w50_1 - t_w50_0);						// tw_id,  t_wid_1 -t_wid_,
	len_ofm_byte = ofm_len(&layer_50);   			//layer_id
	DMA_Work(layer_50);	  								// layer_id


	XTime_GetTime(&t_end);
	u32 t_all = (t_end - t_start ) * 1000000 / COUNTS_PER_SECOND;

	printf("t_start-t_end:%ld ms.\n", (t_end-t_start)* 1000000 / COUNTS_PER_SECOND);
}



int load_file(char *filename, int address)
{
	static FATFS fatfs;
	static FIL fil;

	FRESULT Res;
	UINT NumBytesRead;
	TCHAR *Path = (char *)"0:/";

	Res = f_mount(&fatfs, Path, 0);
	if(Res != FR_OK)
		return XST_FAILURE;

	u8 Ts = 0;
	u8 Go_on_R = 1;

	Res = f_open(&fil, filename, FA_READ);
	if(Res)
		return XST_FAILURE;

	while(Go_on_R){
		Res = f_lseek(&fil, SIZE_PTXT * Ts );
		if(Res)
			return XST_FAILURE;

		Res = f_read(&fil, (void*)(long)(address + SIZE_PTXT * Ts), SIZE_PTXT, &NumBytesRead);
		if(Res)
			return XST_FAILURE;

		if((int)NumBytesRead < SIZE_PTXT){
			Xil_DCacheFlushRange((address + SIZE_PTXT * Ts), NumBytesRead);
			NumBytesRead = 0;
			Ts = 0;
			Go_on_R = 0;
		}

		else{
			Xil_DCacheFlushRange((address + SIZE_PTXT * Ts), SIZE_PTXT);
			Ts++;
		}
	}

	Go_on_R = 1;

	Res = f_close(&fil);
	if(Res)
		return XST_FAILURE;
	else
		return XST_SUCCESS;
}

/*******************************************[DMA_CH_Init]************************************************************/
int DMA_CH_Init(void)
{
	int Status;

	CH0_CfgPtr = XAxiDma_LookupConfig(DMA_CH0_ID);
	Status = XAxiDma_CfgInitialize(&AxiDma_0, CH0_CfgPtr);
	XAxiDma_IntrDisable(&AxiDma_0, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AxiDma_0, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	if(!CH0_CfgPtr || Status != XST_SUCCESS)
	{
		xil_printf("No config found for %d\r\n", DMA_CH0_ID);
		xil_printf("Initialization failed %d\r\n", Status);
		return XST_FAILURE;
	}

	CH1_CfgPtr = XAxiDma_LookupConfig(DMA_CH1_ID);
	Status = XAxiDma_CfgInitialize(&AxiDma_1, CH1_CfgPtr);
	XAxiDma_IntrDisable(&AxiDma_1, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AxiDma_1, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	if(!CH1_CfgPtr || Status != XST_SUCCESS)
	{
		xil_printf("No config found for %d\r\n", DMA_CH1_ID);
		xil_printf("Initialization failed %d\r\n", Status);
		return XST_FAILURE;
	}

	CH2_CfgPtr = XAxiDma_LookupConfig(DMA_CH2_ID);
	Status = XAxiDma_CfgInitialize(&AxiDma_2, CH2_CfgPtr);
	XAxiDma_IntrDisable(&AxiDma_2, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AxiDma_2, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	if(!CH2_CfgPtr || Status != XST_SUCCESS)
	{
		xil_printf("No config found for %d\r\n", DMA_CH2_ID);
		xil_printf("Initialization failed %d\r\n", Status);
		return XST_FAILURE;
	}

	CH3_CfgPtr = XAxiDma_LookupConfig(DMA_CH3_ID);
	Status = XAxiDma_CfgInitialize(&AxiDma_3, CH3_CfgPtr);
	XAxiDma_IntrDisable(&AxiDma_3, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AxiDma_3, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	if(!CH3_CfgPtr || Status != XST_SUCCESS)
	{
		xil_printf("No config found for %d\r\n", DMA_CH3_ID);
		xil_printf("Initialization failed %d\r\n", Status);
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*******************************************[FPGA_core_Init]**********************************************************/
int FPGA_core_Init(void)
{
	int status;

	CONV_Ptr = XConvolution_LookupConfig(CONV_0_ID);
	if(!CONV_Ptr){
		print("ERROR: Lookup of Accelerator Configuration failed.\n\r");
		return XST_FAILURE;
	}

	status = XConvolution_CfgInitialize(&Conv, CONV_Ptr);
	if(status != XST_SUCCESS){
		print("ERROR: Could not initialize Accelerator.\n\r");
		exit(-1);
	}

	return XST_SUCCESS;

}

/*********************************************[DMA_Work]**************************************************************/
int DMA_Work(Layer_Type &layer)
{
	int Status;

	// send ifm
	Status = XAxiDma_SimpleTransfer(
			&AxiDma_0,
			(UINTPTR)((dtype*)(IMAGE_BASE)),
			ifm_len(&layer),
			XAXIDMA_DMA_TO_DEVICE);
	if(Status != XST_SUCCESS)
	{
		// printf("send input failure \r\n");
		return XST_FAILURE;
	}

	// send weights
	Status = XAxiDma_SimpleTransfer(
			&AxiDma_1,
			(UINTPTR)((dtype*)(WEIGHT_BASE)),
			weights_len(&layer),
			XAXIDMA_DMA_TO_DEVICE);
	if(Status != XST_SUCCESS)
	{
		// printf("send weights failure \r\n");
		return XST_FAILURE;
	}

	// receive ofm
	Status = XAxiDma_SimpleTransfer(
			&AxiDma_2,
			(UINTPTR)((dtype*)(OUT0_BASE)),
			ofm_len(&layer),
			XAXIDMA_DEVICE_TO_DMA);
	if(Status != XST_SUCCESS)
	{
		// printf("receive output failure \r\n");
		return XST_FAILURE;
	}
	Xil_DCacheInvalidateRange(OUT0_BASE, ofm_len(&layer));

	// receive ofm
	Status = XAxiDma_SimpleTransfer(
			&AxiDma_3,
			(UINTPTR)((dtype*)(OUT1_BASE)),
			ofm_len(&layer),
			XAXIDMA_DEVICE_TO_DMA);
	if(Status != XST_SUCCESS)
	{
		// printf("receive output failure \r\n");
		return XST_FAILURE;
	}
	Xil_DCacheInvalidateRange(OUT1_BASE, ofm_len(&layer));

	u32 dma_busy[4]={0};

	XTime t_DMA_work_start, t_DMA_work_end;
	XTime_GetTime(&t_DMA_work_start);

	XConvolution_Start(&Conv);

	while(1)
	{
		dma_busy[0]=XAxiDma_Busy(&AxiDma_0, XAXIDMA_DMA_TO_DEVICE);
		dma_busy[1]=XAxiDma_Busy(&AxiDma_1, XAXIDMA_DMA_TO_DEVICE);

		if(!(dma_busy[0]||dma_busy[1]))
			break;
	}

	XTime_GetTime(&t_DMA_work_end);
	u32 tSum = (t_DMA_work_end - t_DMA_work_start) * 1000000 / COUNTS_PER_SECOND;
	// printf("PL work done, time elapsed is %d us\n",tSum);

	while(1)
	{

		dma_busy[2]=XAxiDma_Busy(&AxiDma_2, XAXIDMA_DEVICE_TO_DMA);
		dma_busy[3]=XAxiDma_Busy(&AxiDma_3, XAXIDMA_DEVICE_TO_DMA);

		if(!(dma_busy[2]||dma_busy[3]))
			break;
	}



	// printf("COMPUTE DONE, Receive data -> %d\n", ofm_len(layer));
	int layer_op = layer.kernel * layer.kernel * (layer.RC_out_size) * (layer.RC_out_size) * layer.RC_block_num * layer.RC_block_num * TN * TM * layer.CHO_block_num * layer.CHI_block_num;
	//printf("layer_%d operations is %d\n", layer.layer_id,layer_op);
	// printf("tp = %f\n", layer_op * 2.0 / (tSum * 1000));
	return XST_SUCCESS;
}

