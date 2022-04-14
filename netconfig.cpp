

#include "netconfig.hpp"


/******************************[addlayer]***********************************/
void addlayer(network_t *net, layer_t layer)
{

//    static int current_input_addr_org = 0;
    static int current_output_addr_org = 0;
    static int current_weights_addr_org = 0;

//    static int current_input_addr_blk = 0;
    static int current_output_addr_blk = 0;
//    static int current_weights_addr_blk = 0;

    float mem_border = MEMORY_ALIGNMENT / sizeof(data_type);

/*
 *  int addr_input_org;         0
    int addr_output_org;		0
    int addr_weights_org;		0

    int addr_input_blk;			0
    int addr_weights_blk;		0
    int addr_output_blk;		ceil(num_input_blk / mem_border) * mem_border;

    int num_input_blk;			(layer.Tri * layer.Tci * layer.Tn) * (layer.Tr_num * layer.Tc_num * layer.Tn_num * layer.Tm_num);
    int num_output_blk;			向下取整std::floor((float)((layer.Tr * layer.Tc * layer.Tm) * (layer.Tr_num * layer.Tc_num * layer.Tm_num)));
    							向上取整std::ceil
    int num_output_org;---------width_out * height_out * layer.channels_out;
    int num_weights_blk;-------(layer.kernel * layer.kernel * (layer.Tn + 1) * layer.Tm ) * (layer.Tr_num * layer.Tc_num * layer.Tn_num * layer.Tm_num);
 * */

////////////origional data size and address offset ///////////
    int width_out =  std::floor((float)(layer.width + layer.pad - layer.kernel)/layer.stride) + 1;
    int height_out = std::floor((float)(layer.height + layer.pad - layer.kernel)/layer.stride) + 1;

//    int num_input_org = layer.width * layer.height * layer.channels_in;
    int num_output_org = width_out * height_out * layer.channels_out;
        layer.num_output_org = num_output_org;
   // int num_weights_org = (layer.kernel * layer.kernel * layer.channels_in * layer.channels_out) + (layer.channels_out);



	layer.addr_input_org = 0;

	layer.addr_output_org = 0;

	layer.addr_weights_org = current_weights_addr_org;
//	current_weights_addr_org += num_weights_org;



 /////////block data size and address offset ///////////
    int num_input_blk = (layer.Tri * layer.Tci * layer.Tn) * (layer.Tr_num * layer.Tc_num * layer.Tn_num * layer.Tm_num);
        layer.num_input_blk = num_input_blk;
    int num_output_blk = std::floor((float)((layer.Tr * layer.Tc * layer.Tm) * (layer.Tr_num * layer.Tc_num * layer.Tm_num)));
        layer.num_output_blk = num_output_blk;
    int num_weights_blk = (layer.kernel * layer.kernel * (layer.Tn + 1) * layer.Tm ) * (layer.Tr_num * layer.Tc_num * layer.Tn_num * layer.Tm_num);
        layer.num_weights_blk = num_weights_blk;



        layer.addr_input_blk = 0;

        current_output_addr_blk = 0;
        current_output_addr_blk += num_input_blk;
        current_output_addr_blk = ceil(current_output_addr_blk / mem_border) * mem_border;
        layer.addr_output_blk = current_output_addr_blk;

        layer.addr_weights_blk = 0;


 

    net->layers[net->num_layers] = layer;
    net->num_layers++;
    net->total_pixel_mem = current_output_addr_org + num_output_org;

};



#define use_KB 0

#if use_KB
#define uint "k"
#define divi 1024
#else
#define unit ""
#define divi 4
#endif




/******************************[print_layer]***********************************/
void print_layer(layer_t *layer)
{
    int width_out = std::floor((float)(layer->width + layer->pad - layer->kernel)/layer->stride) + 1;
    int height_out = std::floor((float)(layer->height + layer->pad - layer->kernel)/layer->stride) + 1;

    int num_input_org = layer->width * layer->height * layer->channels_in;
    int num_output_org = width_out * height_out * layer->channels_out;
    int num_weights_org = (layer->kernel * layer->kernel * layer->channels_in * layer->channels_out) + (layer->channels_out);

////////////////////////////////////////////////////////////
    printf("%6s: %3d x %-3d x %3d > %3d x %-3d x %3d, ", layer->name, (int)layer->width, (int)layer->height, (int)layer->channels_in,
                                                                      (int)width_out,    (int)height_out,    (int)layer->channels_out);
    printf("\n");

    printf("CONV (%d x %d) / %d %s, ", (int)layer->kernel, (int)layer->kernel, (int)layer->stride,
                                          (layer->leakyrelu == true) ? "+ leakyReLU" : "      ");

    printf("\n");

    printf("CONV BLOCK NUM: %d x %d x %d x %d, ", (int)layer->Tr_num, (int)layer->Tc_num, (int)layer->Tn_num, (int)layer->Tm_num);

    printf("\n");

///////////////////////////////////////////////////////////////
    printf("num_input_blk: %d    num_output_blk: %d     num_weights_blk: %d , ", (int)layer->num_input_blk, (int)layer->num_output_blk, (int)layer->num_weights_blk);
    
    printf("num_input_org: %d    num_output_org: %d     num_weights_org: %d , ", (int)num_input_org, (int)num_output_org, (int)num_weights_org);

    printf("\n");

////////////////////////////////////////////////////////////////
    printf("IN @mem(%8lu" unit "B), ", long(layer->addr_input_org * sizeof(float) / divi));

    printf("OUT @mem(%8lu" unit "B), ", long(layer->addr_output_org * sizeof(float) / divi));

    printf("WEIGHTS @mem(%8lu -> %8lu" unit "B)", long(layer->addr_weights_org * sizeof(float) / divi),
                                                  long((layer->addr_weights_org + num_weights_org) * sizeof(float) / divi));
    printf("\n");
};

/******************************[print_layers]***********************************/
void print_layers(network_t *net)
{
    for(int i = 0; i < net->num_layers; i++)
    {
        layer_t *layer = &net->layers[i];
        print_layer(layer);

    }
};
