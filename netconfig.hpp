

#ifndef SRC_NETCONFIG_HPP_
#define SRC_NETCONFIG_HPP_

#include <string>
#include <cmath>
#include <cstdlib>
#include <stdio.h>


const int NET_NAME_MAX_LEN = 6;   //max length of layer names
const int MEMORY_ALIGNMENT = 4 * 1024;    //align data in DRAM to 4KB borders


/************************float data_t**************************************/
typedef float data_type;


/************************struct AXI_VAL*****************************/
#ifndef AXI_VAL_DEF
struct AXI_VAL
{
	data_type data;
	bool last;
};
#define AXI_VAL_DEF
#endif



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
};







/**********************struct layer_t************************/
struct layer_t
{
	char name[NET_NAME_MAX_LEN + 1];

	int width;
	int height;
	int channels_in;
	int channels_out;

	int Tr;
	int Tc;
	int Tm;
	int Tn;
    int Tri;    //Tri=S*Tr+2
    int Tci;

    int Tr_num;
    int Tc_num;
    int Tn_num;
    int Tm_num;

    int kernel;
    int stride;
    int pad;

    bool leakyrelu;

    int addr_input_org;
    int addr_output_org;
    int addr_weights_org;

    int addr_input_blk;
    int addr_output_blk;
    int addr_weights_blk;

    int num_input_blk;
    int num_output_blk;
    int num_output_org;
    int num_weights_blk;

    layer_t(
    	const char *N,
		int w, int h, int ci, int co,
    	int tr, int tc, int tm, int tn, int tri, int tci,
    	int trn, int tcn, int tnn, int tmn,
    	int k, int s, int p,
    	bool r,
    	int org_mem_i=0, int org_mem_o=0, int org_mem_w=0,
    	int blk_mem_i=0, int blk_mem_o=0, int blk_mem_w=0,
    	int blk_num_i=0, int blk_num_o=0, int org_num_o=0, int blk_num_w=0
    	):

	    width(w),
	    height(h),
	    channels_in(ci),
	    channels_out(co),

	    Tr(tr),
	    Tc(tc),
	    Tm(tm),
	    Tn(tn),
        Tri(tri),
        Tci(tci),

        Tr_num(trn),
        Tc_num(tcn),
        Tn_num(tnn),
        Tm_num(tmn),

        kernel(k),
        stride(s),
        pad(p),

		leakyrelu(r),

        addr_input_org(org_mem_i),
        addr_output_org(org_mem_o),
        addr_weights_org(org_mem_w),

        addr_input_blk(blk_mem_i),
        addr_output_blk(blk_mem_o),
        addr_weights_blk(blk_mem_w),

        num_input_blk(blk_num_i),
        num_output_blk(blk_num_o),
        num_output_org(org_num_o),
        num_weights_blk(blk_num_w)
        {
            for(int i = 0; i < NET_NAME_MAX_LEN; i++)
            {
                name[i] = N[i];
                if(N[i] == 0) break;
            }
            name[5] = 0;
        };


    layer_t():
        width(0),
	    height(0),
	    channels_in(0),
	    channels_out(0),

	    Tr(0),
	    Tc(0),
	    Tm(0),
	    Tn(0),
        Tri(0),
        Tci(0),

        Tr_num(0),
        Tc_num(0),
        Tn_num(0),
        Tm_num(0),

        kernel(0),
        stride(0),
        pad(0),

		leakyrelu(0),

        addr_input_org(0),
        addr_output_org(0),
        addr_weights_org(0),

        addr_input_blk(0),
        addr_output_blk(0),
        addr_weights_blk(0),

        num_input_blk(0),
        num_output_blk(0),
        num_output_org(0),
        num_weights_blk(0)
        {
        name[0] = 0;
        };
};



/************************struct network_t***********************************/
struct network_t
{
    layer_t *layers;
    int num_layers;
    float *weights;
    int num_weights;
    int total_pixel_mem;

    network_t(int max_layers, int max_weights):
    num_layers(0),
    num_weights(0),
    total_pixel_mem(0)
    {
        layers = (layer_t *)malloc((sizeof(layer_t)) * max_layers);
        weights = (float*)malloc((sizeof(float)) * max_weights);
    };
	
};



/*****************************************************************************/
void addlayer(network_t *net, layer_t layer);

void print_layer(layer_t *layer);

void print_layers(network_t *net);

void loadWeightsFromFile(network_t *net, const char *filename);



#endif //SRC_NETCONFIG_HPP_
