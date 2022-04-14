#include "main.hpp"

void layer_7_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst)
{
	int index = 0;

	for(int k = 0; k < 8; k ++){
		for(int j = 0; j < 8; j ++){
			for(int i = 0; i < 32; i ++){
				p_dst[index] = p_org_0[8 * 32 * k + 32 * j + i];
				index ++;
			}

			for(int i = 0; i < 32; i ++){
				p_dst[index] = p_org_1[8 * 32 * k + 32 * j + i];
				index ++;
			}

			for(int i = 0; i < 11; i ++){
				p_dst[index] = p_org_0[8 * 8 * 32 + 8 * 32 * k + 32 * j + i];
				index ++;
			}
		}
	}
}

typedef struct
{
	dtype Data[4][8][8][32];
}Layer6BaseAddr;

typedef struct
{
	dtype Data[16][10][10][16];
}Layer7ImgAddr;

void layer_6_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst)
{
	Layer6BaseAddr *pB0;
	Layer6BaseAddr *pB1;
	Layer7ImgAddr *pL7;

	pB0 = (Layer6BaseAddr*)p_org_0;
	pB1 = (Layer6BaseAddr*)p_org_1;
	pL7 = (Layer7ImgAddr*)p_dst;

	for(int r=0; r<10; r++)
	{
		if((r==0)||(r==9))
		{
			for(int c=0; c<10; c++)
			{
				for(int m0=0; m0<16; m0++)
				{
					for(int m1=0; m1<16; m1++)
					{
						pL7->Data[m0][r][c][m1] = 0;
					}//end channel 1
				}//end channel 0
			}//end column
		}//end row padding
		else
		{
			//column left padding
			for(int m0=0; m0<16; m0++)
			{
				for(int m1=0; m1<16; m1++)
				{
					pL7->Data[m0][r][0][m1] = 0;
				}//end channel 1
			}//end channel 0

			for(int c=1; c<9; c++)
			{
				for(int m0=0; m0<16; m0+=4)
				{
					for(int m1=0; m1<16; m1++)
					{
						pL7->Data[m0+0][r][c][m1] = pB0->Data[m0>>2][r-1][c-1][m1];
					}//end channel 1
					for(int m1=0; m1<16; m1++)
					{
						pL7->Data[m0+1][r][c][m1] = pB0->Data[m0>>2][r-1][c-1][m1+16];
					}//end channel 1

					for(int m1=0; m1<16; m1++)
					{
						pL7->Data[m0+2][r][c][m1] = pB1->Data[m0>>2][r-1][c-1][m1];
					}//end channel 1
					for(int m1=0; m1<16; m1++)
					{
						pL7->Data[m0+3][r][c][m1] = pB1->Data[m0>>2][r-1][c-1][m1+16];
					}//end channel 1
				}//end channel 0
			}//end column


			//column right padding
			for(int m0=0; m0<16; m0++)
			{
				for(int m1=0; m1<16; m1++)
				{
					pL7->Data[m0][r][9][m1] = 0;
				}//end channel 1
			}//end channel 0
		}
	}//end row

	for(int iter=0; iter<1; iter++)
	{
		memcpy(p_dst+sizeof(Layer7ImgAddr), p_dst, sizeof(Layer7ImgAddr));
		p_dst += sizeof(Layer7ImgAddr);
	}
}








typedef struct
{
	dtype Data[8][8][8][32];
}Layer5BaseAddr;

typedef struct
{
	dtype Data[32][10][10][16];
}Layer6ImgAddr;

void layer_5_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst)
{
	Layer5BaseAddr *pB0, *pB1;
	Layer6ImgAddr *pL6;

	pB0 = (Layer5BaseAddr*)p_org_0;
	pB1 = (Layer5BaseAddr*)p_org_1;
	pL6 = (Layer6ImgAddr*)p_dst;

	for(int r=0; r<10; r++)
	{
		//row padding
		if((r==0)||(r==9))
		{
			for(int c=0; c<10; c++)
			{
				for(int m0=0; m0<32; m0++)
				{
					for(int m1=0; m1<16; m1++)
					{
						pL6->Data[m0][r][c][m1] = 0;
					}//end channel 1
				}//end channel 0
			}//end column
		}
		else
		{
			//column left padding
			for(int m0=0; m0<32; m0++)
			{
				for(int m1=0; m1<16; m1++)
				{
					pL6->Data[m0][r][0][m1] = 0;
				}//end channel 1
			}//end channel 0

			for(int c=1; c<9; c++)
			{
				for(int m0=0; m0<32; m0+=4)
				{
					for(int m1=0; m1<16; m1++)
					{
						pL6->Data[m0+0][r][c][m1] = pB0->Data[m0>>2][r-1][c-1][m1];
					}//end channel 1
					for(int m1=0; m1<16; m1++)
					{
						pL6->Data[m0+1][r][c][m1] = pB0->Data[m0>>2][r-1][c-1][m1+16];
					}//end channel 1

					for(int m1=0; m1<16; m1++)
					{
						pL6->Data[m0+2][r][c][m1] = pB1->Data[m0>>2][r-1][c-1][m1];
					}//end channel 1
					for(int m1=0; m1<16; m1++)
					{
						pL6->Data[m0+3][r][c][m1] = pB1->Data[m0>>2][r-1][c-1][m1+16];
					}//end channel 1
				}//end channel 0
			}//end column


			//column right padding
			for(int m0=0; m0<32; m0++)
			{
				for(int m1=0; m1<16; m1++)
				{
					pL6->Data[m0][r][9][m1] = 0;
				}//end channel 1
			}//end channel 0
		}
	}//end row

	for(int iter=0; iter<3; iter++)
	{
		memcpy(p_dst+sizeof(Layer6ImgAddr), p_dst, sizeof(Layer6ImgAddr));
		p_dst += sizeof(Layer6ImgAddr);
	}
}



typedef struct
{
	dtype Data[4][8][8][32];
}Layer4BaseAddr;

typedef struct
{
	dtype Data[16][11][11][16];
}Layer5ImgAddr;


void layer_4_result_data_reorg_1(dtype *p_org_0, dtype *p_org_1, dtype *p_dst)
{
	Layer4BaseAddr *pB0;
	Layer4BaseAddr *pB1;
	Layer5ImgAddr *pL5;

	pB0 = (Layer4BaseAddr*)p_org_0;
	pB1 = (Layer4BaseAddr*)p_org_1;
	pL5 = (Layer5ImgAddr*)p_dst;

	for(int r=0; r<11; r++)
	{
		if((r==0)||(r==9)||(r==10))
		{
			for(int c=0; c<11; c++)
			{
				for(int m0=0; m0<16; m0++)
				{
					for(int m1=0; m1<16; m1++)
					{
						pL5->Data[m0][r][c][m1] = 0;
					}//end channel 1
				}//end channel 0
			}//end column
		}//end row padding
		else
		{
			//column left padding
			for(int m0=0; m0<16; m0++)
			{
				for(int m1=0; m1<16; m1++)
				{
					pL5->Data[m0][r][0][m1] = 0;
				}//end channel 1
			}//end channel 0

			for(int c=1; c<9; c++)
			{
				for(int m0=0; m0<16; m0+=4)
				{
					for(int m1=0; m1<16; m1++)
					{
						pL5->Data[m0+0][r][c][m1] = pB0->Data[m0>>2][r-1][c-1][m1];
					}//end channel 1
					for(int m1=0; m1<16; m1++)
					{
						pL5->Data[m0+1][r][c][m1] = pB0->Data[m0>>2][r-1][c-1][m1+16];
					}//end channel 1

					for(int m1=0; m1<16; m1++)
					{
						pL5->Data[m0+2][r][c][m1] = pB1->Data[m0>>2][r-1][c-1][m1];
					}//end channel 1
					for(int m1=0; m1<16; m1++)
					{
						pL5->Data[m0+3][r][c][m1] = pB1->Data[m0>>2][r-1][c-1][m1+16];
					}//end channel 1
				}//end channel 0
			}//end column


			//column right padding
			for(int m0=0; m0<16; m0++)
			{
				for(int m1=0; m1<16; m1++)
				{
					pL5->Data[m0][r][9][m1] = 0;
					pL5->Data[m0][r][10][m1] = 0;
				}//end channel 1
			}//end channel 0
		}
	}//end row

	for(int iter=0; iter<7; iter++)
	{
		memcpy(p_dst+sizeof(Layer5ImgAddr), p_dst, sizeof(Layer5ImgAddr));
		p_dst += sizeof(Layer5ImgAddr);
	}
}


void layer_4_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst)
{
	Layer4BaseAddr *pB0;
	Layer4BaseAddr *pB1;
	Layer5ImgAddr *pL5;

	pB0 = (Layer4BaseAddr*)p_org_0;
	pB1 = (Layer4BaseAddr*)p_org_1;
	pL5 = (Layer5ImgAddr*)p_dst;

	for(int r=0; r<10; r++)
	{
		if((r==0)||(r==9))
		{
			for(int c=0; c<10; c++)
			{
				for(int m0=0; m0<16; m0++)
				{
					for(int m1=0; m1<16; m1++)
					{
						pL5->Data[m0][r][c][m1] = 0;
					}//end channel 1
				}//end channel 0
			}//end column
		}//end row padding
		else
		{
			//column left padding
			for(int m0=0; m0<16; m0++)
			{
				for(int m1=0; m1<16; m1++)
				{
					pL5->Data[m0][r][0][m1] = 0;
				}//end channel 1
			}//end channel 0

			for(int c=1; c<9; c++)
			{
				for(int m0=0; m0<16; m0+=4)
				{
					for(int m1=0; m1<16; m1++)
					{
						pL5->Data[m0+0][r][c][m1] = pB0->Data[m0>>2][r-1][c-1][m1];
					}//end channel 1
					for(int m1=0; m1<16; m1++)
					{
						pL5->Data[m0+1][r][c][m1] = pB0->Data[m0>>2][r-1][c-1][m1+16];
					}//end channel 1

					for(int m1=0; m1<16; m1++)
					{
						pL5->Data[m0+2][r][c][m1] = pB1->Data[m0>>2][r-1][c-1][m1];
					}//end channel 1
					for(int m1=0; m1<16; m1++)
					{
						pL5->Data[m0+3][r][c][m1] = pB1->Data[m0>>2][r-1][c-1][m1+16];
					}//end channel 1
				}//end channel 0
			}//end column


			//column right padding
			for(int m0=0; m0<16; m0++)
			{
				for(int m1=0; m1<16; m1++)
				{
					pL5->Data[m0][r][9][m1] = 0;
				}//end channel 1
			}//end channel 0
		}
	}//end row

	for(int iter=0; iter<7; iter++)
	{
		memcpy(p_dst+sizeof(Layer5ImgAddr), p_dst, sizeof(Layer5ImgAddr));
		p_dst += sizeof(Layer5ImgAddr);
	}
}


void layer_3_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst)
{
	int index = 0;

	// 0
	for(int l = 0; l < 2; l ++){
		for(int i = 0; i < 18 * 16; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int k = 0; k < 16; k ++){
			for(int i = 0; i < 16; i ++){
				p_dst[index] = 0;
				index ++;
			}

			for(int j = 0; j < 16; j ++){
				for(int i = 0; i < 16; i ++){
					p_dst[index] = p_org_0[i + 32 * j + 32 * 16 * k + 16 * l];
					index ++;
				}
			}

			for(int i = 0; i < 16; i ++){
				p_dst[index] = 0;
				index ++;
			}
		}

		for(int i = 0; i < 18 * 16; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	// 1
	for(int l = 0; l < 2; l ++){
		for(int i = 0; i < 18 * 16; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int k = 0; k < 16; k ++){
			for(int i = 0; i < 16; i ++){
				p_dst[index] = 0;
				index ++;
			}

			for(int j = 0; j < 16; j ++){
				for(int i = 0; i < 16; i ++){
					p_dst[index] = p_org_1[i + 32 * j + 32 * 16 * k + 16 * l];
					index ++;
				}
			}

			for(int i = 0; i < 16; i ++){
				p_dst[index] = 0;
				index ++;
			}
		}

		for(int i = 0; i < 18 * 16; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	// 2
	for(int l = 0; l < 2; l ++){
		for(int i = 0; i < 18 * 16; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int k = 0; k < 16; k ++){
			for(int i = 0; i < 16; i ++){
				p_dst[index] = 0;
				index ++;
			}

			for(int j = 0; j < 16; j ++){
				for(int i = 0; i < 16; i ++){
					p_dst[index] = p_org_0[i + 32 * j + 32 * 16 * k + 16 * l + 16 * 16 * 32];
					index ++;
				}
			}

			for(int i = 0; i < 16; i ++){
				p_dst[index] = 0;
				index ++;
			}
		}

		for(int i = 0; i < 18 * 16; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	// 3
	for(int l = 0; l < 2; l ++){
		for(int i = 0; i < 18 * 16; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int k = 0; k < 16; k ++){
			for(int i = 0; i < 16; i ++){
				p_dst[index] = 0;
				index ++;
			}

			for(int j = 0; j < 16; j ++){
				for(int i = 0; i < 16; i ++){
					p_dst[index] = p_org_1[i + 32 * j + 32 * 16 * k + 16 * l + 16 * 16 * 32];
					index ++;
				}
			}

			for(int i = 0; i < 16; i ++){
				p_dst[index] = 0;
				index ++;
			}
		}

		for(int i = 0; i < 18 * 16; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	memcpy(p_dst + index, p_dst, index);
	memcpy(p_dst + 2 * index, p_dst, 2 * index);
}

void layer_2_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst)
{
	int index = 0;


	for(int i = 0; i < 34 * 16; i ++){
		p_dst[index] = 0;
		index ++;
	}

	for(int k = 0; k < 32; k ++){
		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int j = 0; j < 32; j ++){
			for(int i = 0; i < 16; i ++){
				p_dst[index] = p_org_0[32 * 32 * k + 32 * j + i];
				index ++;
			}
		}

		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	for(int i = 0; i < 34 * 16; i ++){
		p_dst[index] = 0;
		index ++;
	}









	for(int i = 0; i < 34 * 16; i ++){
		p_dst[index] = 0;
		index ++;
	}

	for(int k = 0; k < 32; k ++){
		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int j = 0; j < 32; j ++){
			for(int i = 0; i < 16; i ++){
				p_dst[index] = p_org_0[32 * 32 * k + 32 * j + i + 16];
				index ++;
			}
		}

		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	for(int i = 0; i < 34 * 16; i ++){
		p_dst[index] = 0;
		index ++;
	}




	for(int i = 0; i < 34 * 16; i ++){
		p_dst[index] = 0;
		index ++;
	}

	for(int k = 0; k < 32; k ++){
		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int j = 0; j < 32; j ++){
			for(int i = 0; i < 16; i ++){
				p_dst[index] = p_org_1[32 * 32 * k + 32 * j + i];
				index ++;
			}
		}

		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	for(int i = 0; i < 34 * 16; i ++){
		p_dst[index] = 0;
		index ++;
	}









	for(int i = 0; i < 34 * 16; i ++){
		p_dst[index] = 0;
		index ++;
	}

	for(int k = 0; k < 32; k ++){
		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int j = 0; j < 32; j ++){
			for(int i = 0; i < 16; i ++){
				p_dst[index] = p_org_1[32 * 32 * k + 32 * j + i + 16];
				index ++;
			}
		}

		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	for(int i = 0; i < 34 * 16; i ++){
		p_dst[index] = 0;
		index ++;
	}

	memcpy(p_dst + index, p_dst, index);
}

void layer_1_result_data_reorg(dtype *p_org_0, dtype *p_org_1, dtype *p_dst)
{
	int index = 0;

	for(int i = 0; i < 66 * 16; i ++){
		p_dst[index] = 0;
		index ++;
	}

	for(int j = 0; j < 32; j ++){
		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int i = 0; i < 16 * 32; i ++){
			p_dst[index] = p_org_0[32 * 16 * j + i];
			index ++;
		}

		for(int i = 0; i < 16 * 32; i ++){
			p_dst[index] = p_org_0[32 * 32 * 16 + 32 * 16 * j + i];
			index ++;
		}

		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	for(int j = 0; j < 32; j ++){
		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int i = 0; i < 32 * 16; i ++){
			p_dst[index] = p_org_0[32 * 32 * 16 * 2 + 32 * 16 * j + i];
			index ++;
		}

		for(int i = 0; i < 32 * 16; i ++){
			p_dst[index] = p_org_0[32 * 32 * 16 * 3 + 32 * 16 * j + i];
			index ++;
		}

		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	for(int i = 0; i < 66 * 16; i ++){
		p_dst[index] = 0;
		index ++;
	}

	////////////////////////////////

	for(int i = 0; i < 66 * 16; i ++){
		p_dst[index] = 0;
		index ++;
	}

	for(int j = 0; j < 32; j ++){
		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int i = 0; i < 16 * 32; i ++){
			p_dst[index] = p_org_1[32 * 16 * j + i];
			index ++;
		}

		for(int i = 0; i < 16 * 32; i ++){
			p_dst[index] = p_org_1[32 * 32 * 16 + 32 * 16 * j + i];
			index ++;
		}

		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	for(int j = 0; j < 32; j ++){
		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int i = 0; i < 32 * 16; i ++){
			p_dst[index] = p_org_1[32 * 32 * 16 * 2 + 32 * 16 * j + i];
			index ++;
		}

		for(int i = 0; i < 32 * 16; i ++){
			p_dst[index] = p_org_1[32 * 32 * 16 * 3 + 32 * 16 * j + i];
			index ++;
		}

		for(int i = 0; i < 16; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	for(int i = 0; i < 66 * 16; i ++){
		p_dst[index] = 0;
		index ++;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 假设上一层rc_out_siz和下一层rc_real_size一致，并且上一层的cho_bn * cho_real_size 是64的倍数
///
void layer_result_data_reorg_stride1(dtype *p_org_0, dtype *p_org_1, dtype *p_dst, Layer_Type &layer_last,Layer_Type &layer_next)
{
	int index = 0;
	bool padding = (layer_next.kernel==3)? true : false;
	printf("layer_%d padding %d",layer_next.layer_id,padding);
	for(int row_num = 0;row_num<layer_next.RC_block_num;row_num++){
		for(int col_num = 0;col_num<layer_next.RC_block_num;col_num++){
			for(int tm_num=0;tm_num<layer_next.CHO_block_num;tm_num++){
				for(int tn_num=0;tn_num<layer_next.CHI_block_num;tn_num++){
					//block
					for(int blk_row = row_num*layer_next.RC_real_size-int(padding);blk_row<(row_num + 1)*layer_next.RC_real_size+int(padding);blk_row++){
						for(int blk_col = col_num*layer_next.RC_real_size-int(padding);blk_col<(col_num + 1)*layer_next.RC_real_size+int(padding);blk_row++){
							for(int str_out_num = 0;str_out_num<int(TM/TN);str_out_num++){
								for(int blk_tn=0;blk_tn<16;blk_tn++){
									int i = row_num *layer_next.RC_real_size + blk_row;
									int j = col_num * layer_next.RC_real_size  + blk_col;
									int tn = tn_num * layer_next.CHI_block_num + str_out_num * TN + blk_tn;
									if(i<0||i>(layer_next.RC_block_num * layer_next.RC_real_size)||j<0||j>(layer_next.RC_block_num * layer_next.RC_real_size)){
										p_dst[index] = 0;
										index++;
									}
									else{
										p_dst[index] = p_org_0[row_num*layer_last.RC_block_num*layer_next.RC_real_size*(int)(TM/2)  ];
										index++;
									}
								}
							}
						}
					}

				}
			}
		}
	}

}

void layer_1_result_data_transfer(dtype *p_org_0, dtype *p_org_1, dtype *p_dst){
	int index = 0;

	for(int i = 0; i < 66 * 32; i ++){
		p_dst[index] = 0;
		index ++;
	}

	for(int k = 0; k < 32; k ++){
		for(int i = 0; i < 32; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int j = 0; j < 32; j ++){
			for(int i = 0; i < 16; i ++){
				p_dst[index] = p_org_0[32 * 16 * k + i + 16 * j];
				index ++;
			}

			for(int i = 0; i < 16; i ++){
				p_dst[index] = p_org_1[32 * 16 * k + i + 16 * j];
				index ++;
			}
		}

		for(int j = 0; j < 32; j ++){
			for(int i = 0; i < 16; i ++){
				p_dst[index] = p_org_0[32 * 32 * 16 + 32 * 16 * k + i + 16 * j];
				index ++;
			}

			for(int i = 0; i < 16; i ++){
				p_dst[index] = p_org_1[32 * 32 * 16 + 32 * 16 * k + i + 16 * j];
				index ++;
			}
		}

		for(int i = 0; i < 32; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	for(int k = 0; k < 32; k ++){
		for(int i = 0; i < 32; i ++){
			p_dst[index] = 0;
			index ++;
		}

		for(int j = 0; j < 32; j ++){
			for(int i = 0; i < 16; i ++){
				p_dst[index] = p_org_0[32 * 32 * 16 * 2 + 32 * 16 * k + 16 * j + i];
				index ++;
			}

			for(int i = 0; i < 16; i ++){
				p_dst[index] = p_org_1[32 * 32 * 16 * 2 + 32 * 16 * k + 16 * j + i];
				index ++;
			}
		}

		for(int j = 0; j < 32; j ++){
			for(int i = 0; i < 16; i ++){
				p_dst[index] = p_org_0[32 * 32 * 16 * 3 + 32 * 16 * k + 16 * j + i];
				index ++;
			}

			for(int i = 0; i < 16; i ++){
				p_dst[index] = p_org_1[32 * 32 * 16 * 3 + 32 * 16 * k + 16 * j + i];
				index ++;
			}
		}

		for(int i = 0; i < 32; i ++){
			p_dst[index] = 0;
			index ++;
		}
	}

	for(int i = 0; i < 66 * 32; i ++){
		p_dst[index] = 0;
		index ++;
	}
}
void compare_test(dtype *a, dtype *b){
	int count = 0;
	for(int i =0;i<64 * 64 * 16;i++){
		if((int)a[i]!= (int)b[i]) {
			count++;
		};
	}
	printf("error number: %d",count);
}

void layer_0_result_data_reorg(dtype *p_org, dtype *p_org_1, dtype *p_dst ,Layer_Type &layer_cur, Layer_Type &layer_next)
{
	int index = 0;
	// 66 * 66 * 16, tile 0 start
	int tile = 0;
////////////////////////////////////////////////////////////////第0块   012//////////////////////////////////////////////
	for(int i = 0; i < layer_next.RC_block_size * layer_next.CHI_block_size; i ++){
		p_dst[index] = 0;
		index ++;
	}
	//////////////////////////////////345///////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int j = 0; j < layer_next.CHI_block_size; j ++){ ////////// 3
			p_dst[index] = 0;
			index ++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){   ///////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2        /////////////////////5
			p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	/////////////////////////////678//////////////////////////////////////////////////
	for(int j = 0; j < layer_next.CHI_block_size; j ++){ // 6
		p_dst[index] = 0;
		index ++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){    //////////////7
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2  /////////////////////8
		p_dst[index] = p_org[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}////////第0块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////////////////////////////第0块   012//////////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_block_size * layer_next.CHI_block_size; i ++){
			p_dst[index] = 0;
			index ++;
		}
		//////////////////////////////////345///////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int j = 0; j < layer_next.CHI_block_size; j ++){ ////////// 3
				p_dst[index] = 0;
				index ++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){   ///////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l =0;l<16;l++){// TM/2        /////////////////////5
				p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		/////////////////////////////678//////////////////////////////////////////////////
		for(int j = 0; j < layer_next.CHI_block_size; j ++){ // 6
			p_dst[index] = 0;
			index ++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){    //////////////7
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  /////////////////////8
			p_dst[index] = p_org[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}////////第0块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;

////////////////////////////////////////第1块 012///////////////////////////////////////////////////////////
	for(int i = 0; i < layer_next.RC_block_size * layer_next.CHI_block_size; i ++){
		p_dst[index] = 0;
		index ++;
	}
	//////////////////////////////////中间层345////////////////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int l =0;l<16;l++){// TM/2                       ///////////// 3
			p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){    ////////////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////////////////5
			p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	///////////////////////////////////////////最下层678//////////////////////////
	for(int l =0;l<16;l++){// TM/2           ///////6
		p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){ ///////////////7
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2 //////////////////////////8
		p_dst[index] = p_org[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}////////第1块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////第1块 012///////////////////////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_block_size * layer_next.CHI_block_size; i ++){
			p_dst[index] = 0;
			index ++;
		}
		//////////////////////////////////中间层345////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int l =0;l<16;l++){// TM/2                       ///////////// 3
				p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){    ////////////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l =0;l<16;l++){// TM/2  ///////////////////////////5
				p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		///////////////////////////////////////////最下层678//////////////////////////
		for(int l =0;l<16;l++){// TM/2           ///////6
			p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){ ///////////////7
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2 //////////////////////////8
			p_dst[index] = p_org[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}////////第1块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;

////////////////////////////////////////第2块 012///////////////////////////////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_block_size * layer_next.CHI_block_size; i ++){
			p_dst[index] = 0;
			index ++;
		}
		//////////////////////////////////中间层////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int l =0;l<16;l++){// TM/2   //////////////////////////3
				p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l =0;l<16;l++){// TM/2  ///////////////5
				p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		///////////////////////////////////////////最下层678//////////////////////////
		for(int l =0;l<16;l++){// TM/2           ///////6
			p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){/////////////7
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2   //////////////////////8
			p_dst[index] = p_org[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}////////第2块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
		////////////////////////////////////////第2块 012///////////////////////////////////////////////////////////////////
				for(int i = 0; i < layer_next.RC_block_size * layer_next.CHI_block_size; i ++){
					p_dst[index] = 0;
					index ++;
				}
				//////////////////////////////////中间层////////////////////////////////////////
				for(int i = 0; i < layer_next.RC_real_size; i ++){
					for(int l =0;l<16;l++){// TM/2   //////////////////////////3
						p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
						index++;
					}
					for(int l =0;l<16;l++){// TM/2
						p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
						index++;
					}
					for(int l=0;l<32;l++){
						p_dst[index] = 0;
						index++;
					}
					for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
						for(int l =0;l<16;l++){// TM/2
							p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
							index++;
						}
						for(int l =0;l<16;l++){// TM/2
							p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
							index++;
						}
						for(int l=0;l<32;l++){
							p_dst[index] = 0;
							index++;
						}
					}
					for(int l =0;l<16;l++){// TM/2  ///////////////5
						p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
						index++;
					}
					for(int l =0;l<16;l++){// TM/2
						p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
						index++;
					}
					for(int l=0;l<32;l++){
						p_dst[index] = 0;
						index++;
					}
				}
				///////////////////////////////////////////最下层678//////////////////////////
				for(int l =0;l<16;l++){// TM/2           ///////6
					p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
				for(int j=0;j<layer_next.RC_real_size;j++){/////////////7
					for(int l =0;l<16;l++){// TM/2
						p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
						index++;
					}
					for(int l =0;l<16;l++){// TM/2
						p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
						index++;
					}
					for(int l=0;l<32;l++){
						p_dst[index] = 0;
						index++;
					}
				}
				for(int l =0;l<16;l++){// TM/2   //////////////////////8
					p_dst[index] = p_org[64*64*16*(tile+5) + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}////////第2块填充完毕
				printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
		tile++;
////////////////////////////////////////第3块 012///////////////////////////////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_block_size * layer_next.CHI_block_size; i ++){
			p_dst[index] = 0;
			index ++;
		}
		//////////////////////////////////中间层////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int l =0;l<16;l++){// TM/2   //////////////////////////////// 3
				p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l=0;l<64;l++){    /////////////////////////////////////5
				p_dst[index] = 0;
				index++;
			}
		}
		///////////////////////////////////////////最下层678//////////////////////////
		for(int l =0;l<16;l++){// TM/2           ///////6
			p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}///
		for(int j=0;j<layer_next.RC_real_size;j++){//////////7
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l=0;l<64;l++){
			p_dst[index] = 0;
			index++;
		}////////第3块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
		////////////////////////////////////////第3块 012///////////////////////////////////////////////////////////////////
				for(int i = 0; i < layer_next.RC_block_size * layer_next.CHI_block_size; i ++){
					p_dst[index] = 0;
					index ++;
				}
				//////////////////////////////////中间层////////////////////////////////////////
				for(int i = 0; i < layer_next.RC_real_size; i ++){
					for(int l =0;l<16;l++){// TM/2   //////////////////////////////// 3
						p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
						index++;
					}
					for(int l =0;l<16;l++){// TM/2
						p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
						index++;
					}
					for(int l=0;l<32;l++){
						p_dst[index] = 0;
						index++;
					}
					for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
						for(int l =0;l<16;l++){// TM/2
							p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
							index++;
						}
						for(int l =0;l<16;l++){// TM/2
							p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
							index++;
						}
						for(int l=0;l<32;l++){
							p_dst[index] = 0;
							index++;
						}
					}
					for(int l=0;l<64;l++){    /////////////////////////////////////5
						p_dst[index] = 0;
						index++;
					}
				}
				///////////////////////////////////////////最下层678//////////////////////////
				for(int l =0;l<16;l++){// TM/2           ///////6
					p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}///
				for(int j=0;j<layer_next.RC_real_size;j++){//////////7
					for(int l =0;l<16;l++){// TM/2
						p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
						index++;
					}
					for(int l =0;l<16;l++){// TM/2
						p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
						index++;
					}
					for(int l=0;l<32;l++){
						p_dst[index] = 0;
						index++;
					}
				}
				for(int l=0;l<64;l++){
					p_dst[index] = 0;
					index++;
				}////////第3块填充完毕
				printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
		tile++;
////////////////////////////////////////////////////////////////第4块   012//////////////////////////////////////////////
		/////////////////////////012///////////////////////////////////////////
		for(int l=0;l<64;l++){  ////////////////////////////0
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){//////////1
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////////2
			p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	//////////////////////////////////345///////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int j = 0; j < layer_next.CHI_block_size; j ++){ ////////// 3
			p_dst[index] = 0;
			index ++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){   ///////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2        /////////////////////5
			p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	/////////////////////////////678//////////////////////////////////////////////////
	for(int j = 0; j < layer_next.CHI_block_size; j ++){ // 6
		p_dst[index] = 0;
		index ++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){    //////////////7
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2  /////////////////////8
		p_dst[index] = p_org[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}////////第4块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////////////////////////////第4块   012//////////////////////////////////////////////
			/////////////////////////012///////////////////////////////////////////
			for(int l=0;l<64;l++){  ////////////////////////////0
				p_dst[index] = 0;
				index++;
			}
			for(int j=0;j<layer_next.RC_real_size;j++){//////////1
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l =0;l<16;l++){// TM/2  ///////////////////2
				p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		//////////////////////////////////345///////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int j = 0; j < layer_next.CHI_block_size; j ++){ ////////// 3
				p_dst[index] = 0;
				index ++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){   ///////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l =0;l<16;l++){// TM/2        /////////////////////5
				p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		/////////////////////////////678//////////////////////////////////////////////////
		for(int j = 0; j < layer_next.CHI_block_size; j ++){ // 6
			p_dst[index] = 0;
			index ++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){    //////////////7
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  /////////////////////8
			p_dst[index] = p_org[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}////////第4块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;
////////////////////////////////////////第5块 ///////////////////////////////////////////////////////////
	/////////////////////////012///////////////////////////////////////////
	for(int l =0;l<16;l++){// TM/2  ///////////////////0
		p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){//////////1
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2  ///////////////////2
		p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	//////////////////////////////////中间层345////////////////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int l =0;l<16;l++){// TM/2                       ///////////// 3
			p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){    ////////////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////////////////5
			p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	///////////////////////////////////////////最下层678//////////////////////////
	for(int l =0;l<16;l++){// TM/2           ///////6
		p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){ ///////////////7
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2 //////////////////////////8
		p_dst[index] = p_org[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}////////第5块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////第5块 ///////////////////////////////////////////////////////////
		/////////////////////////012///////////////////////////////////////////
		for(int l =0;l<16;l++){// TM/2  ///////////////////0
			p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){//////////1
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////////2
			p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		//////////////////////////////////中间层345////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int l =0;l<16;l++){// TM/2                       ///////////// 3
				p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){    ////////////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l =0;l<16;l++){// TM/2  ///////////////////////////5
				p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		///////////////////////////////////////////最下层678//////////////////////////
		for(int l =0;l<16;l++){// TM/2           ///////6
			p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){ ///////////////7
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2 //////////////////////////8
			p_dst[index] = p_org[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}////////第5块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;

////////////////////////////////////////第6块 ///////////////////////////////////////////////////////////////////
	/////////////////////////012///////////////////////////////////////////
	for(int l =0;l<16;l++){// TM/2  ///////////////////0
		p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){//////////1
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2  ///////////////////2
		p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	//////////////////////////////////中间层////////////////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int l =0;l<16;l++){// TM/2   //////////////////////////3
			p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////5
			p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	///////////////////////////////////////////最下层678//////////////////////////
	for(int l =0;l<16;l++){// TM/2           ///////6
		p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){/////////////7
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2   //////////////////////8
		p_dst[index] = p_org[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}////////第6块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////第6块 ///////////////////////////////////////////////////////////////////
		/////////////////////////012///////////////////////////////////////////
		for(int l =0;l<16;l++){// TM/2  ///////////////////0
			p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){//////////1
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////////2
			p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		//////////////////////////////////中间层////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int l =0;l<16;l++){// TM/2   //////////////////////////3
				p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l =0;l<16;l++){// TM/2  ///////////////5
				p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		///////////////////////////////////////////最下层678//////////////////////////
		for(int l =0;l<16;l++){// TM/2           ///////6
			p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){/////////////7
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2   //////////////////////8
			p_dst[index] = p_org[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}////////第6块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;
////////////////////////////////////////第7块 012///////////////////////////////////////////////////////////////////
	/////////////////////////012///////////////////////////////////////////
	for(int l =0;l<16;l++){// TM/2  ///////////////////0
		p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){//////////1
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<64;l++){// TM/2  ///////////////////2
		p_dst[index] = 0;
		index++;
	}
	//////////////////////////////////中间层////////////////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int l =0;l<16;l++){// TM/2   //////////////////////////////// 3
			p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l=0;l<64;l++){    /////////////////////////////////////5
			p_dst[index] = 0;
			index++;
		}
	}
	///////////////////////////////////////////最下层678//////////////////////////
	for(int l =0;l<16;l++){// TM/2           ///////6
		p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}///
	for(int j=0;j<layer_next.RC_real_size;j++){//////////7
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l=0;l<64;l++){           ///////////8
		p_dst[index] = 0;
		index++;
	}////////第7块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////第7块 012///////////////////////////////////////////////////////////////////
		/////////////////////////012///////////////////////////////////////////
		for(int l =0;l<16;l++){// TM/2  ///////////////////0
			p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){//////////1
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<64;l++){// TM/2  ///////////////////2
			p_dst[index] = 0;
			index++;
		}
		//////////////////////////////////中间层////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int l =0;l<16;l++){// TM/2   //////////////////////////////// 3
				p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l=0;l<64;l++){    /////////////////////////////////////5
				p_dst[index] = 0;
				index++;
			}
		}
		///////////////////////////////////////////最下层678//////////////////////////
		for(int l =0;l<16;l++){// TM/2           ///////6
			p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}///
		for(int j=0;j<layer_next.RC_real_size;j++){//////////7
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l=0;l<64;l++){           ///////////8
			p_dst[index] = 0;
			index++;
		}////////第7块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;
////////////////////////////////////////////////////////////////第8块   012//////////////////////////////////////////////
	/////////////////////////012///////////////////////////////////////////
	for(int l=0;l<64;l++){  ////////////////////////////0
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){//////////1
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2  ///////////////////2
		p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	//////////////////////////////////345///////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int j = 0; j < layer_next.CHI_block_size; j ++){ ////////// 3
			p_dst[index] = 0;
			index ++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){   ///////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2        /////////////////////5
			p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	/////////////////////////////678//////////////////////////////////////////////////
	for(int j = 0; j < layer_next.CHI_block_size; j ++){ // 6
		p_dst[index] = 0;
		index ++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){    //////////////7
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2  /////////////////////8
		p_dst[index] = p_org[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}////////第8块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////////////////////////////第8块   012//////////////////////////////////////////////
		/////////////////////////012///////////////////////////////////////////
		for(int l=0;l<64;l++){  ////////////////////////////0
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){//////////1
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////////2
			p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		//////////////////////////////////345///////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int j = 0; j < layer_next.CHI_block_size; j ++){ ////////// 3
				p_dst[index] = 0;
				index ++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){   ///////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l =0;l<16;l++){// TM/2        /////////////////////5
				p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		/////////////////////////////678//////////////////////////////////////////////////
		for(int j = 0; j < layer_next.CHI_block_size; j ++){ // 6
			p_dst[index] = 0;
			index ++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){    //////////////7
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  /////////////////////8
			p_dst[index] = p_org[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}////////第8块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;
////////////////////////////////////////第9块 ///////////////////////////////////////////////////////////
	/////////////////////////012///////////////////////////////////////////
	for(int l =0;l<16;l++){// TM/2  ///////////////////0
		p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){//////////1
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2  ///////////////////2
		p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	//////////////////////////////////中间层345////////////////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int l =0;l<16;l++){// TM/2                       ///////////// 3
			p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){    ////////////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////////////////5
			p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	///////////////////////////////////////////最下层678//////////////////////////
	for(int l =0;l<16;l++){// TM/2           ///////6
		p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){ ///////////////7
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2 //////////////////////////8
		p_dst[index] = p_org[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}////////第9块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////第9块 ///////////////////////////////////////////////////////////
		/////////////////////////012///////////////////////////////////////////
		for(int l =0;l<16;l++){// TM/2  ///////////////////0
			p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){//////////1
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////////2
			p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		//////////////////////////////////中间层345////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int l =0;l<16;l++){// TM/2                       ///////////// 3
				p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){    ////////////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l =0;l<16;l++){// TM/2  ///////////////////////////5
				p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		///////////////////////////////////////////最下层678//////////////////////////
		for(int l =0;l<16;l++){// TM/2           ///////6
			p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){ ///////////////7
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2 //////////////////////////8
			p_dst[index] = p_org[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}////////第9块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;

////////////////////////////////////////第10块 ///////////////////////////////////////////////////////////////////
	/////////////////////////012///////////////////////////////////////////
	for(int l =0;l<16;l++){// TM/2  ///////////////////0
		p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){//////////1
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2  ///////////////////2
		p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	//////////////////////////////////中间层////////////////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int l =0;l<16;l++){// TM/2   //////////////////////////3
			p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////5
			p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	///////////////////////////////////////////最下层678//////////////////////////
	for(int l =0;l<16;l++){// TM/2           ///////6
		p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){/////////////7
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2   //////////////////////8
		p_dst[index] = p_org[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}////////第10块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////第10块 ///////////////////////////////////////////////////////////////////
		/////////////////////////012///////////////////////////////////////////
		for(int l =0;l<16;l++){// TM/2  ///////////////////0
			p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){//////////1
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////////2
			p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		//////////////////////////////////中间层////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int l =0;l<16;l++){// TM/2   //////////////////////////3
				p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l =0;l<16;l++){// TM/2  ///////////////5
				p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		///////////////////////////////////////////最下层678//////////////////////////
		for(int l =0;l<16;l++){// TM/2           ///////6
			p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){/////////////7
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2   //////////////////////8
			p_dst[index] = p_org[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+5) + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}////////第10块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;
////////////////////////////////////////第11块 012///////////////////////////////////////////////////////////////////
	/////////////////////////012///////////////////////////////////////////
	for(int l =0;l<16;l++){// TM/2  ///////////////////0
		p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){//////////1
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<64;l++){// TM/2  ///////////////////2
		p_dst[index] = 0;
		index++;
	}
	//////////////////////////////////中间层////////////////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int l =0;l<16;l++){// TM/2   //////////////////////////////// 3
			p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l=0;l<64;l++){    /////////////////////////////////////5
			p_dst[index] = 0;
			index++;
		}
	}
	///////////////////////////////////////////最下层678//////////////////////////
	for(int l =0;l<16;l++){// TM/2           ///////6
		p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}///
	for(int j=0;j<layer_next.RC_real_size;j++){//////////7
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l=0;l<64;l++){           ///////////8
		p_dst[index] = 0;
		index++;
	}////////第11块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////第11块 012///////////////////////////////////////////////////////////////////
		/////////////////////////012///////////////////////////////////////////
		for(int l =0;l<16;l++){// TM/2  ///////////////////0
			p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){//////////1
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<64;l++){// TM/2  ///////////////////2
			p_dst[index] = 0;
			index++;
		}
		//////////////////////////////////中间层////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int l =0;l<16;l++){// TM/2   //////////////////////////////// 3
				p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l=0;l<64;l++){    /////////////////////////////////////5
				p_dst[index] = 0;
				index++;
			}
		}
		///////////////////////////////////////////最下层678//////////////////////////
		for(int l =0;l<16;l++){// TM/2           ///////6
			p_dst[index] = p_org[64*64*16*(tile+3) + 63*16+l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+3) +63*16+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}///
		for(int j=0;j<layer_next.RC_real_size;j++){//////////7
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile + 4) + j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+4)+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l=0;l<64;l++){           ///////////8
			p_dst[index] = 0;
			index++;
		}////////第11块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;
////////////////////////////////////////////////////////////////第12块   012//////////////////////////////////////////////
	/////////////////////////012///////////////////////////////////////////
	for(int l=0;l<64;l++){  ////////////////////////////0
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){//////////1
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2  ///////////////////2
		p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	//////////////////////////////////345///////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int j = 0; j < layer_next.CHI_block_size; j ++){ ////////// 3
			p_dst[index] = 0;
			index ++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){   ///////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2        /////////////////////5
			p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	/////////////////////////////678//////////////////////////////////////////////////
	for(int j = 0; j < layer_next.CHI_block_size*layer_next.RC_block_size; j ++){ // 678
		p_dst[index] = 0;
		index ++;
	}////////第12块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////////////////////////////第12块   012//////////////////////////////////////////////
		/////////////////////////012///////////////////////////////////////////
		for(int l=0;l<64;l++){  ////////////////////////////0
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){//////////1
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////////2
			p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		//////////////////////////////////345///////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int j = 0; j < layer_next.CHI_block_size; j ++){ ////////// 3
				p_dst[index] = 0;
				index ++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){   ///////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l =0;l<16;l++){// TM/2        /////////////////////5
				p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		/////////////////////////////678//////////////////////////////////////////////////
		for(int j = 0; j < layer_next.CHI_block_size*layer_next.RC_block_size; j ++){ // 678
			p_dst[index] = 0;
			index ++;
		}////////第12块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;
////////////////////////////////////////第13块 ///////////////////////////////////////////////////////////
	/////////////////////////012///////////////////////////////////////////
	for(int l =0;l<16;l++){// TM/2  ///////////////////0
		p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){//////////1
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2  ///////////////////2
		p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	//////////////////////////////////中间层345////////////////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int l =0;l<16;l++){// TM/2                       ///////////// 3
			p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){    ////////////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////////////////5
			p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	///////////////////////////////////////////最下层678//////////////////////////
	for(int j = 0; j < layer_next.CHI_block_size*layer_next.RC_block_size; j ++){ // 678
		p_dst[index] = 0;
		index ++;
	}////////第13块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////第13块 ///////////////////////////////////////////////////////////
		/////////////////////////012///////////////////////////////////////////
		for(int l =0;l<16;l++){// TM/2  ///////////////////0
			p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){//////////1
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////////2
			p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		//////////////////////////////////中间层345////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int l =0;l<16;l++){// TM/2                       ///////////// 3
				p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){    ////////////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l =0;l<16;l++){// TM/2  ///////////////////////////5
				p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		///////////////////////////////////////////最下层678//////////////////////////
		for(int j = 0; j < layer_next.CHI_block_size*layer_next.RC_block_size; j ++){ // 678
			p_dst[index] = 0;
			index ++;
		}////////第13块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;

////////////////////////////////////////第14块 ///////////////////////////////////////////////////////////////////
	/////////////////////////012///////////////////////////////////////////
	for(int l =0;l<16;l++){// TM/2  ///////////////////0
		p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){//////////1
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<16;l++){// TM/2  ///////////////////2
		p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	//////////////////////////////////中间层////////////////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int l =0;l<16;l++){// TM/2   //////////////////////////3
			p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////5
			p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	///////////////////////////////////////////最下层678//////////////////////////
	for(int j = 0; j < layer_next.CHI_block_size*layer_next.RC_block_size; j ++){ // 678
		p_dst[index] = 0;
		index ++;
	}////////第14块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////第14块 ///////////////////////////////////////////////////////////////////
		/////////////////////////012///////////////////////////////////////////
		for(int l =0;l<16;l++){// TM/2  ///////////////////0
			p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){//////////1
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<16;l++){// TM/2  ///////////////////2
			p_dst[index] = p_org[64*64*16*(tile-3) +63*64*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-3) + 63*64*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		//////////////////////////////////中间层////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int l =0;l<16;l++){// TM/2   //////////////////////////3
				p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l =0;l<16;l++){// TM/2  ///////////////5
				p_dst[index] = p_org[64*64*16*(tile+1) + i* 16*64 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile+1)+i * 16 *64+ l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		///////////////////////////////////////////最下层678//////////////////////////
		for(int j = 0; j < layer_next.CHI_block_size*layer_next.RC_block_size; j ++){ // 678
			p_dst[index] = 0;
			index ++;
		}////////第14块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;
////////////////////////////////////////第15块 012///////////////////////////////////////////////////////////////////
	/////////////////////////012///////////////////////////////////////////
	for(int l =0;l<16;l++){// TM/2  ///////////////////0
		p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
		index++;
	}
	for(int l =0;l<16;l++){// TM/2
		p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
		index++;
	}
	for(int l=0;l<32;l++){
		p_dst[index] = 0;
		index++;
	}
	for(int j=0;j<layer_next.RC_real_size;j++){//////////1
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
	}
	for(int l =0;l<64;l++){// TM/2  ///////////////////2
		p_dst[index] = 0;
		index++;
	}
	//////////////////////////////////中间层////////////////////////////////////////
	for(int i = 0; i < layer_next.RC_real_size; i ++){
		for(int l =0;l<16;l++){// TM/2   //////////////////////////////// 3
			p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l=0;l<64;l++){    /////////////////////////////////////5
			p_dst[index] = 0;
			index++;
		}
	}
	///////////////////////////////////////////最下层678//////////////////////////
	for(int j = 0; j < layer_next.CHI_block_size*layer_next.RC_block_size; j ++){ // 678
		p_dst[index] = 0;
		index ++;
	}////////第15块填充完毕
	printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	////////////////////////////////////////第15块 012///////////////////////////////////////////////////////////////////
		/////////////////////////012///////////////////////////////////////////
		for(int l =0;l<16;l++){// TM/2  ///////////////////0
			p_dst[index] = p_org[64*64*16*(tile-5) +63*64*16+63*16+ l];
			index++;
		}
		for(int l =0;l<16;l++){// TM/2
			p_dst[index] = p_org_1[64*64*16*(tile-5) + 63*64*16+63*16+l];
			index++;
		}
		for(int l=0;l<32;l++){
			p_dst[index] = 0;
			index++;
		}
		for(int j=0;j<layer_next.RC_real_size;j++){//////////1
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org[64*64*16*(tile - 4) + 63*64*16+j* 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-4)+63*64*16+j * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
		}
		for(int l =0;l<64;l++){// TM/2  ///////////////////2
			p_dst[index] = 0;
			index++;
		}
		//////////////////////////////////中间层////////////////////////////////////////
		for(int i = 0; i < layer_next.RC_real_size; i ++){
			for(int l =0;l<16;l++){// TM/2   //////////////////////////////// 3
				p_dst[index] = p_org[64*64*16*(tile-1)+i * 64 * 16 + 63 * 16 + l];
				index++;
			}
			for(int l =0;l<16;l++){// TM/2
				p_dst[index] = p_org_1[64*64*16*(tile-1)+i * 64 * 16 +63 * 16 + l];
				index++;
			}
			for(int l=0;l<32;l++){
				p_dst[index] = 0;
				index++;
			}
			for(int j = 0;j<layer_next.RC_real_size;j++){    ///////////4
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l =0;l<16;l++){// TM/2
					p_dst[index] = p_org_1[64*64*16*tile+i * 64 * 16 + j * 16 + l];
					index++;
				}
				for(int l=0;l<32;l++){
					p_dst[index] = 0;
					index++;
				}
			}
			for(int l=0;l<64;l++){    /////////////////////////////////////5
				p_dst[index] = 0;
				index++;
			}
		}
		///////////////////////////////////////////最下层678//////////////////////////
		for(int j = 0; j < layer_next.CHI_block_size*layer_next.RC_block_size; j ++){ // 678
			p_dst[index] = 0;
			index ++;
		}////////第15块填充完毕
		printf("length_index in layer_%d: tile_%d:%d\n",layer_cur.layer_id,tile,index);
	tile++;
}
int ifm_len(struct Layer_Type * layer)
{
	return layer->RC_block_num * layer->RC_block_num * layer->CHO_block_num * layer->CHI_block_num
			* (layer->RC_block_size) * (layer->RC_block_size) * TN;
}

int weights_len(struct Layer_Type * layer)
{
	return layer->RC_block_num * layer->RC_block_num * layer->CHO_block_num * layer->CHI_block_num
			* (TM * 4 + layer->kernel * layer->kernel * TN * TM);
}

int ofm_len(struct Layer_Type * layer)
{
	return layer->RC_block_num * layer->RC_block_num * layer->CHO_block_num
			* layer->RC_out_size * layer->RC_out_size * TM / 2;
}

void data_check_ifm(dtype * p0, dtype * p1, Layer_Type &layer)
{
	int counter = 0;
	for(int i = 0; i < layer.RC_block_num*layer.RC_block_num*layer.CHI_block_num*layer.CHO_block_num*layer.RC_block_size*layer.RC_block_size*layer.CHI_block_size; i ++){
		int r0 = (int)p0[i];
		int r1 = (int)p1[i];
		int res = r0 - r1;
		if(res > 1 || res < -1)
			counter++;
	}
	printf("check ifm over!Error number:%d\n",counter);
}
void data_check_bw(dtype * p0, dtype * p1, Layer_Type &layer)
{
	int counter = 0;
	for(int i = 0; i < (layer.CHO_block_size *4+layer.CHO_block_size*layer.CHI_block_size*layer.kernel*layer.kernel)*layer.CHO_block_num*layer.CHI_block_num*layer.RC_block_num*layer.RC_block_num; i ++){
		int r0 = (int)p0[i];
		int r1 = (int)p1[i];
		int res = r0 - r1;
		if(res > 1 || res < -1){
			printf("W_%d :%d\n",((i- layer.CHO_block_size *4)/ (layer.CHO_block_size*layer.CHI_block_size*layer.kernel*layer.kernel)),p1[i]);
			counter++;
		}
			//printf("%d %d\n", i, res);
	}
	printf("check w over!Error number:%d\n",counter);
}
void data_check_ofm(dtype * p0, dtype * p1, Layer_Type &layer)
{
	int counter = 0;
	for(int i = 0; i < layer.RC_block_num*layer.RC_block_num*layer.CHO_block_num*layer.RC_out_size*layer.RC_out_size*layer.CHO_block_size/2; i ++){
		int r0 = (int)p0[i];
		int r1 = (int)p1[i];
		int res = r0 - r1;
		if(res >= 1 || res <= -1){
			printf("%dth ofm r0:%d r1:%d\n",i,r0,r1);
			counter++;
		}
	}
	printf("check ofm over!Error number:%d\n",counter);
}
void data_check(dtype * p0, dtype * p1,int length)
{
	int counter = 0;
	for(int i = 0; i < length; i ++){
		int r0 = (int)p0[i];
		int r1 = (int)p1[i];
		int res = r0 - r1;
		if(res >= 1 || res <= -1){
			printf("%d %dth tile : r0:%d r1:%d\n",i,i/(66*66*64),r0,r1);
			counter++;
		}
	}
	printf("check ofm over!Error number:%d\n",counter);
}
void data_print(dtype *pixcels, int offset, int length)
{
	for(int i = 0; i < length; i ++)
	{
		printf("data[%d] = %d\n", i, pixcels[offset + i]);
	}
}





/***************************************[orig_block_transfer_in]**********************************************************/
void orig_block_transfer_in(float *org_input, float *blk_input, layer_t layer)
{
	int index_in = 0;

/////////////////////////////////block////////////////////////////////////////////////////
	for(int row = 0; row < layer.width; row += layer.Tr * layer.stride  )
	{
		for(int col = 0; col < layer.height; col += layer.Tc * layer.stride )
		{
			for(int to = 0; to < layer.channels_out; to += layer.Tm )
			{
				for(int ti = 0; ti < layer.channels_in; ti += layer.Tn )
				{

///////////////////////////////input orig to blk//////////////////////////
					for(int trr = 0; trr < layer.Tri; trr++ )
					{
						for(int tcc = 0; tcc < layer.Tci; tcc++ )
						{
							for(int tii = 0; tii < layer.Tn; tii += 2 )
							{

								int row_total = (layer.kernel == 1) ? int(row + trr) : int(row + trr - 1) ;
								int col_total = (layer.kernel == 1) ? int(col + tcc) : int(col + tcc - 1) ;


								int ti_total = ti + tii;
								int ti_total_1 = ti + tii + 1;

								if( (row_total < 0) || (col_total < 0) || (row_total >= layer.width) || (col_total >= layer.height) || (ti_total >= layer.channels_in) )
								{
									blk_input[index_in] = 0;
									blk_input[index_in + layer.num_input_blk/2] = 0;
								}

								else if(ti_total == layer.channels_in - 1 )
								{
									blk_input[index_in] = org_input[row_total * layer.height * layer.channels_in + col_total * layer.channels_in + ti_total];
									blk_input[index_in + layer.num_input_blk/2] = 0;
								}

								else
								{
									blk_input[index_in] = org_input[row_total * layer.height * layer.channels_in + col_total * layer.channels_in + ti_total];
									blk_input[index_in + layer.num_input_blk/2] = org_input[row_total * layer.height * layer.channels_in + col_total * layer.channels_in + ti_total_1];
								}

								index_in++;
							}
						}
					}
				}
			}
		}
	}
};	//end function  [orig_block_transfer_in]







/***************************************[orig_block_transfer_w]**********************************************************/
void orig_block_transfer_w(float *org_weights, float *blk_weights, layer_t layer)
{
	int index_w = 0;

/////////////////////////////////block////////////////////////////////////////////////////
	for(int row = 0; row < layer.width; row += layer.Tr * layer.stride )
	{
		for(int col = 0; col < layer.height; col += layer.Tc * layer.stride )
		{
			for(int to = 0; to < layer.channels_out; to += layer.Tm )
			{
				for(int ti = 0; ti < layer.channels_in; ti += layer.Tn )
				{

///////////////////////////weights orig to blk/////////////////////////
					for(int i = 0; i < layer.kernel; i++ )
					{
						for(int j = 0; j < layer.kernel; j++ )
						{
							for(int tii = 0; tii < (layer.Tn + 1); tii++ )
							{
								for(int too = 0; too < layer.Tm; too += 2 )
								{
									int ti_total = ti + tii;

									int to_total = to + too;
									int to_total_1 = to + too + 1;

									if( ((ti_total >= layer.channels_in) && (tii < layer.Tn))  ||  (to_total >= layer.channels_out) )
									{
										blk_weights[index_w] = 0;
										blk_weights[index_w + layer.num_weights_blk/2] = 0;
									}

									else if(to_total == layer.channels_out - 1 )
									{
										blk_weights[index_w] = org_weights[i * layer.kernel * layer.channels_in * layer.channels_out + j * layer.channels_in * layer.channels_out + ti_total * layer.channels_out + to_total ];
										blk_weights[index_w + layer.num_weights_blk/2] = 0;
									}

									else if(tii == layer.Tn)  //bias
									{
										blk_weights[index_w] = org_weights[layer.kernel * layer.kernel * layer.channels_in * layer.channels_out + to_total];
										blk_weights[index_w + layer.num_weights_blk/2] = org_weights[layer.kernel * layer.kernel * layer.channels_in * layer.channels_out + to_total_1];
									}

									else
									{
										blk_weights[index_w] = org_weights[i * layer.kernel * layer.channels_in * layer.channels_out + j * layer.channels_in * layer.channels_out + ti_total * layer.channels_out + to_total ];
										blk_weights[index_w + layer.num_weights_blk/2] = org_weights[i * layer.kernel * layer.channels_in * layer.channels_out + j * layer.channels_in * layer.channels_out + ti_total * layer.channels_out + to_total_1 ];
									}


									/*
									if( index_w < 1000 )
									{
										printf("blk_weights[%d] = %f \n", index_w, blk_weights[index_w] );
										printf("blk_weights[%d] = %f \n", index_w + layer.num_weights_blk/2 , blk_weights[index_w + layer.num_weights_blk/2] );
									}
									*/

									index_w++;
								}
							}
						}
					}
				}
			}
		}
	}
};	//end function  [orig_block_transfer_w]







/***************************************[block_orig_transfer_out]**********************************************************/
void block_orig_transfer_out(float *blk_output, float *org_output, layer_t layer)
{
	int index_out = 0;

	int width_out =  std::floor((float)(layer.width + layer.pad - layer.kernel)/layer.stride) + 1;
	int height_out = std::floor((float)(layer.height + layer.pad - layer.kernel)/layer.stride) + 1;

	/*
	int blk_width_out = (layer.pool == true) ?  layer.Tr/2 : layer.Tr;
	int blk_height_out = (layer.pool == true) ?  layer.Tc/2 : layer.Tc;
	*/

/////////////////////////////////block/////////////////////////////////////////////////////
	for(int row = 0; row < width_out; row += layer.Tr )
	{
		for(int col = 0; col < height_out; col += layer.Tc)
		{
			for(int to = 0; to < layer.channels_out; to += layer.Tm)
			{

//////////////////////////output blk to org///////////////////////
				for(int trr = 0; trr < layer.Tr; trr++ )
				{
					for(int tcc = 0; tcc < layer.Tc; tcc++ )
					{
						for(int too = 0; too < layer.Tm; too += 4)
						{

							int to_total = to + too;
							int to_total_1 = to + too + 1;
							int to_total_2 = to + too + 2;
							int to_total_3 = to + too + 3;

							int row_total = row + trr;
							int col_total = col + tcc;


							if( (row_total >= width_out) || (col_total >= height_out) || (to_total >= layer.channels_out) )
							{
								



							}

							else if(to_total == layer.channels_out - 1 )
							{
								org_output[row_total * height_out * layer.channels_out + col_total * layer.channels_out + to_total] = blk_output[index_out];
								


							}

							else if(to_total == layer.channels_out - 2 )
							{
								org_output[row_total * height_out * layer.channels_out + col_total * layer.channels_out + to_total] = blk_output[index_out];
								org_output[row_total * height_out * layer.channels_out + col_total * layer.channels_out + to_total_1] = blk_output[index_out + layer.num_output_blk/4];
								

							}

							else if(to_total == layer.channels_out - 3 )
							{
								org_output[row_total * height_out * layer.channels_out + col_total * layer.channels_out + to_total] = blk_output[index_out];
								org_output[row_total * height_out * layer.channels_out + col_total * layer.channels_out + to_total_1] = blk_output[index_out + layer.num_output_blk/4];
								org_output[row_total * height_out * layer.channels_out + col_total * layer.channels_out + to_total_2] = blk_output[index_out + layer.num_output_blk/2];
								
							}

							else 
							{
								org_output[row_total * height_out * layer.channels_out + col_total * layer.channels_out + to_total] = blk_output[index_out];
								org_output[row_total * height_out * layer.channels_out + col_total * layer.channels_out + to_total_1] = blk_output[index_out + layer.num_output_blk/4];
								org_output[row_total * height_out * layer.channels_out + col_total * layer.channels_out + to_total_2] = blk_output[index_out + layer.num_output_blk/2];
								org_output[row_total * height_out * layer.channels_out + col_total * layer.channels_out + to_total_3] = blk_output[index_out + layer.num_output_blk*3/4];
							}


							/*
							if( index_out < 1000 )
							{
								printf("org_output[%d] = %f \n", index_out, org_output[index_out] );
							}
							*/

							index_out++;
						}
					}
				}
			}
		}
	}
};	//end function  [block_orig_transfer_out]


/////////////////////////   maxpool
void MaxPool(float *Input, float *Output,int PoolStride,layer_t layer)
{

	int goal1 = 0;
	int goal2 = 0;
	int goal3 = 0;
	int goal4 = 0;
	int count = 0;

	float Max1,Max2,Max3;

	for(int row = 0; row < layer.width;row+=PoolStride){
		for(int col = 0; col < layer.height;col+=PoolStride){
			for(int too = 0; too <layer.channels_out;too++){
				goal1 = row*layer.height*layer.channels_out + col*layer.channels_out + too;
				if((row < (layer.width-1))&&(col < (layer.height-1))){
					goal2 =((row + 1)*layer.height*layer.channels_out + col*layer.channels_out + too);
					goal3 = (row *layer.height*layer.channels_out + (col+1)*layer.channels_out + too);
					goal4 = ((row + 1)*layer.height*layer.channels_out + (col+1)*layer.channels_out + too);
				}
				else if((row == (layer.width-1))&&(col < (layer.height-1))){
					goal2 =goal1;
					goal3 = (row *layer.height*layer.channels_out + (col+1)*layer.channels_out + too);
					goal4 = goal3;
				}
				else if((row < (layer.width-1))&&(col == (layer.height-1))){
					goal2 =((row + 1)*layer.height*layer.channels_out + col*layer.channels_out + too);
					goal3 = goal1;
					goal4 = goal2;
				}
				else if((row == (layer.width-1))&&(col == (layer.height-1))){
					goal2 =goal1;
					goal3 =goal1;
					goal4 = goal1;
				}
				if(Input[goal1]>Input[goal2])	Max1 = Input[goal1];	else Max1 = Input[goal2];
				if(Input[goal3]>Input[goal4])	Max2 = Input[goal3];	else Max2 = Input[goal4];
				if(Max1>Max2)	Max3 = Max1;	else Max3 = Max2;
				Output[count] = Max3;

				count++;
			}

		}

	}
	layer.num_output_org = (layer.width / PoolStride )*(layer .height /PoolStride)*layer.channels_out;
};// end function [MaxPool]

/////////////////////////   upsample
void UpSample(float *Input, float *Output,int UpSize,layer_t layer)
{

	int org,goal1,goal2,goal3,goal4;

	for(int row = 0; row < layer.width; row++){
		for(int col = 0; col < layer.height;col++){
			for(int too = 0; too <layer.channels_out;too++){
				org = row*layer.height*layer.channels_out + col*layer.channels_out + too;

				goal1 = row*UpSize*layer.height*layer.channels_out + col*UpSize*layer.channels_out + too;
				goal2 = (row*UpSize	+	1)*layer.height*layer.channels_out + col*UpSize*layer.channels_out + too;
				goal3 = row*UpSize*layer.height*layer.channels_out + (col+1)*UpSize*layer.channels_out + too;
				goal4 = (row+1)*UpSize*layer.height*layer.channels_out +( col+1)*UpSize*layer.channels_out + too;
				Output[goal1] = Input[org];
				Output[goal2] = Input[org];
				Output[goal3] = Input[org];
				Output[goal4] = Input[org];

			}

		}

	}
	layer.height = layer.height * UpSize;
	layer.width = layer.width * UpSize;
	layer.num_output_org = layer.width * layer.height * layer.channels_out;
};// end function [MaxPool]

////Concate
void Concate(float *Input1, float *Input2,float *Output,int Ch1,int Ch2,layer_t layer)
{
	int FinalCh;
	FinalCh = Ch1 + Ch2;
	for(int row = 0; row < layer.width; row++){
		for(int col = 0; col < layer.height;col++){
			for(int too = 0; too <FinalCh;too++){
				if( too < Ch1){
					Output[row*layer.height*FinalCh	+ col*FinalCh + too] = Input1[ row*layer.height*Ch1	+ col*Ch1 + too];
				}
				else {
					Output[row*layer.height*FinalCh	+ col*FinalCh + too] = Input2[ row*layer.height*Ch2	+ col*Ch2 + (too-Ch1)];
				}
			}

		}

	}
	layer.num_output_org = layer.width*layer.height*FinalCh;
};

