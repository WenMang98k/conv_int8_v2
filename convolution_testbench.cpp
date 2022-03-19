#include "convolution.hpp"

int main()
{
	bool check_flag=true;
	Layer_Type layer = {
			0,		// layer_id

			1,		// RC_block_num
			66,		// RC_block_size 与kernel有关系
			64,		// RC_real_size
			64,		// RC_out_size 与stride有关

			1,		// CHI_block_num
			32,		// CHI_block_size
			32,		// CHI_real_size

			1,		// CHO_block_num
			64,		// CHO_block_size
			64,		// CHO_real_size

			3,		// kernel
			1,		// stride
			5.818701693028935e-07,		// scaler
			0		// is_yolo
	};
	cout<< "hahha" <<endl;
	int ifm_len = layer.RC_block_num * layer.RC_block_num * layer.CHO_block_num * layer.CHI_block_num
			* (layer.RC_real_size + 2) * (layer.RC_real_size + 2);

	int weights_len = layer.RC_block_num * layer.RC_block_num * layer.CHO_block_num * layer.CHI_block_num
			* (layer.CHO_real_size * 32 / 128 + layer.kernel * layer.kernel * layer.CHO_real_size);

	int ofm_len = layer.RC_block_num * layer.RC_block_num * layer.CHO_block_num
			* layer.RC_real_size * layer.RC_real_size * layer.CHO_real_size / 16 / 2;

	// int ifm_len = (66 * 66 * 16) / 16;
	// int weights_len = (32 * 4 + 3 * 3 * 16 * 32) / 16;
	// int ofm_len = 32 * 32 * 32 / 16 / 2;

	AXI_VAL_I input_0[ifm_len];
	AXI_VAL_I input_1[weights_len];

	AXI_VAL_O output_0[ofm_len];
	AXI_VAL_O output_1[ofm_len];

	FILE * FP_0 = fopen("E:/0_compacting_FPGA/compact/IP_testbech/img66_int8_2.bin", "rb");
	fseek(FP_0, 0, SEEK_END);//将文件指针FP_0的位置设置到离文件末尾0字节处。
	int len_0 = ftell(FP_0);
	// cout << len_0 << endl;
	dtype * pBuf_0 = new dtype[len_0];
	rewind(FP_0);
	fread(pBuf_0, 1, len_0, FP_0);
	fclose(FP_0);

	int index_0 = 0;
	for(int i = 0; i < ifm_len; i ++){
		for(int j = 0; j < 16; j ++){
			input_0[i].data[j] = pBuf_0[index_0];
			index_0 ++;
			cout << int(pBuf_0[index_0]) << " ";
		}
	}

	FILE * FP_1 = fopen("E:/0_compacting_FPGA/compact/IP_testbech/w0_24.bin", "rb");
	fseek(FP_1, 0, SEEK_END);
	int len_1 = ftell(FP_1);
	// cout << len_1 << endl;
	dtype * pBuf_1 = new dtype[len_1];
	rewind(FP_1);
	fread(pBuf_1, 1, len_1, FP_1);
	fclose(FP_1);

	int index_1 = 0;
	for(int i = 0; i < weights_len; i ++){
		for(int j = 0; j < 16; j ++){
			input_1[i].data[j] = pBuf_1[index_1];
			index_1 ++;
		}
	}

	AXI_VAL_I *str_in_0 = &input_0[0];
	AXI_VAL_I *str_in_1 = &input_1[0];

	AXI_VAL_O *str_out_0 = &output_0[0];
	AXI_VAL_O *str_out_1 = &output_1[0];

	convolution(layer, str_in_0, str_in_1, str_out_0, str_out_1);

	FILE * FP_2 = fopen("E:/0_compacting_FPGA/compact/IP_testbech/ofm_int8_2.bin", "rb");
	fseek(FP_2, 0, SEEK_END);
	int len_2 = ftell(FP_2);
	// cout << len_2 << endl;
	dtype * pBuf_2 = new dtype[len_2];
	rewind(FP_2);
	fread(pBuf_2, 1, len_2, FP_2);
	fclose(FP_2);

	AXI_VAL_O output[32 * 32];
	int index_2 = 0;
	for(int i = 0; i < 32 * 32; i ++){
		for(int j = 0; j < 16; j ++){
			output[i].data[j] = pBuf_2[index_2];
			index_2 ++;
		}
	}
	cout<< "check" <<endl;
	for(int i = 0; i < 32 * 32; i ++){
		for(int j = 0; j < 16; j ++){
			int res = output_0[i].data[j] - output[i].data[j];
			if(res != 0)
			{
				check_flag = false;
				cout << (int)res << endl;
			}
		}
	}

	for(int i = 0; i < 1; i ++){
		for(int j = 0; j < 16; j ++){
			cout << int(output_0[i].data[j]) << " ";
		}
		cout << endl;
	}
	if(check_flag == false)
		cout << "sorry,the result is wrong "<<endl;
	else
		cout << "Success!Congratulation "<<endl;

	return 0;
}
