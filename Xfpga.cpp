#include "main.hpp"
#include "xconvolution_hw.h"

XTimeTypedef XTimeStruct = {0, 0, 500000};


void XFPGA_setLayerConfig(Layer_Type &layer)
{
	// SET conv ip core parameter
	// #define XConvolution_WriteReg(BaseAddress, RegOffset, Data) Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_LAYER_ID_DATA, layer.layer_id);
	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_RC_BLOCK_NUM_DATA, layer.RC_block_num);
	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_RC_BLOCK_SIZE_DATA,layer.RC_block_size);
	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_RC_REAL_SIZE_DATA,layer.RC_real_size);
	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_RC_OUT_SIZE_DATA,layer.RC_out_size);

	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_CHI_BLOCK_NUM_DATA,layer.CHI_block_num);
	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_CHI_BLOCK_SIZE_DATA,layer.CHI_block_size);
	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_CHI_REAL_SIZE_DATA,layer.CHI_real_size);

	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_CHO_BLOCK_NUM_DATA,layer.CHO_block_num);
	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_CHO_BLOCK_SIZE_DATA,layer.CHO_block_size);
	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_CHO_REAL_SIZE_DATA,layer.CHO_real_size);
	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_KERNEL_DATA,layer.kernel);

	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_STRIDE_DATA,layer.stride);
	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_PSCALER_DATA,*(int *)(&layer.pscaler));
	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_IS_YOLO_DATA,layer.is_yolo);
	XConvolution_WriteReg(CONV_TOP_LITE_BASE, XCONVOLUTION_AXILITE_ADDR_LAYER_NSCALER_DATA,*(int *)(&layer.nscaler));

}

void XFPGA_read_LayerConfig() {
	//XFPGA_Set_layer_width_V(256);
	u32 data;

	data = XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_LAYER_ID_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_LAYER_ID_DATA        --> %d \n",data);
	  //XFPGA_Set_layer_height_V(256£©;
	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_RC_BLOCK_NUM_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_RC_BLOCK_NUM_DATA       --> %d \n",data);
	  //XFPGA_Set_layer_channels_in_V(3);
	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_RC_BLOCK_SIZE_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_RC_BLOCK_SIZE_DATA  --> %d \n",data);
	  //XFPGA_Set_layer_channels_out_V(64);
	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_RC_REAL_SIZE_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_RC_REAL_SIZE_DATA --> %d \n",data);
	  //XFPGA_Set_layer_kernel_V(3);
	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_RC_OUT_SIZE_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_RC_OUT_SIZE_DATA       --> %d \n",data);
	  //XFPGA_Set_layer_stride_V(2);
	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_CHI_BLOCK_NUM_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_CHI_BLOCK_NUM_DATA       --> %d \n",data);
	  //XFPGA_Set_layer_pad(1);
	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_CHI_BLOCK_SIZE_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_CHI_BLOCK_SIZE_DATA           --> %d \n",data);

	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_CHI_REAL_SIZE_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_CHI_REAL_SIZE_DATA            --> %d \n",data);
	  ///XFPGA_Set_layer_relu(1);
	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_CHO_BLOCK_NUM_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_CHO_BLOCK_NUM_DATA           --> %d \n",data);

	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_CHO_BLOCK_SIZE_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_CHO_BLOCK_SIZE_DATA           --> %d \n",data);

	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_CHO_REAL_SIZE_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_CHO_REAL_SIZE_DATA           --> %d \n",data);

	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_KERNEL_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_KERNEL_DATA           --> %d \n",data);

	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_STRIDE_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_STRIDE_DATA           --> %d \n",data);
	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_PSCALER_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_SCALER_DATA           --> %.15f \n", *(float *)(&data));

	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_IS_YOLO_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_IS_YOLO_DATA           --> %d \n",data);
	data =XConvolution_ReadReg(CONV_TOP_LITE_BASE,XCONVOLUTION_AXILITE_ADDR_LAYER_NSCALER_DATA);
	printf("XCONVOLUTION_AXILITE_ADDR_LAYER_NSCALER_DATA           --> %.15f \n", *(float *)(&data));
}


void XFPGA_setStartTime(void)
{
	XTime_GetTime(&(XTimeStruct.t_start));
}

void XFPGA_setEndTime(void)
{
	XTime_GetTime(&(XTimeStruct.t_end));
}

unsigned int XFPGA_getElapsedTime(void)
{
	return (XTimeStruct.t_start - XTimeStruct.t_end) * XTimeStruct.stamp / COUNTS_PER_SECOND;
}

