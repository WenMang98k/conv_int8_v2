#include "network.hpp"

/*********************************[get_network_config]***********************************************/
network_t *get_network_config()
{
	network_t *net = new network_t(10, 3561472);

	//                                1    2    3    4       1    2    3    4                         1    2    3    4    kernel,stride,pad,   relu,pool
	//VGG16:      layer_t(name[6],    w,   h,  ci,  co,     tr,  tc,  tn,  tm,    tri, tci, tni,    trn, tcn, tnn, tmn,      k,   s,   p,      r,   pl)


	addlayer(net, layer_t("conv_0",   416,   416,	3,		15,    		60,  	60,  	128,  	4,   		62,   	62,     	7,  	7,   	1,  	1,     	3,   1,   2,      1));
	//maxpool size2 stride2
	addlayer(net, layer_t("conv_1",   208,   208,	15,		31,    		60,   	60,  	128,   	4,     		62,  	62,  		4,    	4,   	4,   	1,    	3,   1,   2,      1));
	//maxpool size2 stride2
	addlayer(net, layer_t("conv_2",   104,   104,  	34, 	64,    		60,   	60,  	128,   	4,     		62,   	62,      	2,		2,  	9,  	1,     	3,   1,   2,      1));
	//maxpool size2 stride2
	addlayer(net, layer_t("conv_3",   52,   52,  	64, 	125,   		60,   	60,  	128,   	4,      	62,   	62,    		1,    	1,  	16,   	1,    	3,   1,   2,      1));  //
	//maxpool size2 stride2
	addlayer(net, layer_t("conv_4",   26,   26,  	125, 	253,   		60,   	60,  	128,   	4,     		62,  	62,       	1,    	1,   	32,   	2,     	3,   1,   2,      1));
	//maxpool size2 stride2
	addlayer(net, layer_t("conv_5",   13,   13, 	253,  	386,  		60,   	60,  	128,   	4,     	 	62,   	62,     	1,    	1,   	64,   	2,     	3,   1,   2,      1));
	//maxpool size2 stride1
	addlayer(net, layer_t("conv_6",   13,  	13,  	386, 	1024,  		60,   	60,  	128,   	4,      	62,   	62,     	1,    	1,   	97,   	8,    	3,   1,   2,      1));  //
	addlayer(net, layer_t("conv_7",   13,   13, 	1024,  	4,   		60,   	60,  	128,   	4,      	60,   	60,     	1,    	1,   	256,   	1,     	1,   1,   0,      1));
	addlayer(net, layer_t("conv_8",   13,   13,  	4, 		134,   		60,   	60,  	128,   	4,      	62,   	62,      	1,    	1,   	1,   	2,     	3,   1,   2,      1));  //
	addlayer(net, layer_t("conv_9",   13,   13, 	134, 	18,   		60,   	60,  	128,   	4,     		60,  	60,      	1,    	1,   	34,   	1,   	1,   1,   0,      1));
	return net;
};
