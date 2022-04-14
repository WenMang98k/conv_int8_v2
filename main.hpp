#include <stdio.h>

#include "xparameters.h"
#include "xaxidma.h"
#include "xil_printf.h"
#include "xil_exception.h"
#include "xil_cache.h"
#include "xtime_l.h"
#include "xconvolution.h"
#include "xconvolution_hw.h"
#include "ff.h"
#include "network.hpp"
#include "netconfig.hpp"

typedef signed char dtype;
#define TM 32
#define TN 64
#define CONV_TOP_LITE_BASE 0xa0000000



#define CONV_0_ID XPAR_CONVOLUTION_0_DEVICE_ID
#define DMA_CH0_ID XPAR_AXI_DMA_0_DEVICE_ID
#define DMA_CH1_ID XPAR_AXI_DMA_1_DEVICE_ID
#define DMA_CH2_ID XPAR_AXI_DMA_2_DEVICE_ID
#define DMA_CH3_ID XPAR_AXI_DMA_3_DEVICE_ID

typedef struct
{
	XTime t_start;
	XTime t_end;
	int stamp;
}XTimeTypedef;

int CPU_load_file(void);
int CPU_load_picture(void);
int CPU_load_weights_file(void);
int load_layer_0_result(void);
int load_file(char *filename, int address);

void compare_test(dtype *a, dtype *b);
void layer_0_result_data_reorg(dtype *p_org, dtype *p_org_1, dtype *p_dst ,Layer_Type &layer_cur, Layer_Type &layer_next);
void layer_1_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst);
void layer_2_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst);
void layer_3_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst);
void layer_4_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst);
void layer_4_result_data_reorg_1(dtype *p_org_0, dtype *p_org_1, dtype *p_dst);
void layer_5_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst);
void layer_6_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst);
void layer_7_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst);
void layer_result_data_reorg_stride1(dtype *p_org_0, dtype *p_org_1, dtype *p_dst, Layer_Type &layer_cur,Layer_Type &layer_next);

int ifm_len(Layer_Type * layer);
int weights_len(Layer_Type * layer);
int ofm_len(Layer_Type  * layer);

void data_fill(dtype *pixcels, int lenth, dtype value);
void data_fill_weights(dtype *pixcels, int lenth, dtype value);
void data_print(dtype *pixcels, int offset, int length);
void data_check_ifm(dtype * p0, dtype * p1, Layer_Type &layer);
void data_check_bw(dtype * p0, dtype * p1, Layer_Type &layer);
void data_check_ofm(dtype * p0, dtype * p1, Layer_Type &layer);
void data_check(dtype * p0, dtype * p1,int length);
int FPGA_core_Init(void);
int DMA_CH_Init(void);
int DMA_Work(Layer_Type &layer);

void XFPGA_setLayerConfig(Layer_Type &layer);
void XFPGA_read_LayerConfig();
void XFPGA_setStartTime(void);
void XFPGA_setEndTime(void);
unsigned int XFPGA_getElapsedTime(void);

void orig_block_transfer_in(float *org_input, float *blk_input, layer_t layer);
void orig_block_transfer_w(float *org_weights, float *blk_weights, layer_t layer);
void block_orig_transfer_out(float *blk_output, float *org_output, layer_t layer);
