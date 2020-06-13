
#include <stdio.h>
#include "macroblock.h"
#include "stream.h"
#include "sps.h"
#include "pps.h"

int pcm_sample_luma[256];
int pcm_sample_chroma[512];

int process_macroblock()
{
	int mb_type = ue();
	int i;
	int mb_part_idx;
	int transform_size_8x8_flag;
	int coded_block_pattern;
	int mb_qp_delta;

	if (mb_type == MB_TYPE_I_PCM)
	{
		while (!stream_byte_aligned())
			stream_get_one_bit();

		for (i = 0; i < 256; i ++)
			pcm_sample_luma[i] = stream_get_bits();    // need confirmation

		for (i = 0; i < 2 * mb_width * mb_height; i ++)
			pcm_sample_chroma[i] = stream_get_bits();   // need confirmation

	}
	else
	{
		int no_sub_mb_part_size_less_than_8x8 = 1;

		if (mb_type != MB_TYPE_I_NxN && mb_part_pred_mode(mb_type, 0) != Intra_16x16 &&
										num_mb_part(mb_type) == 4)
		{
			sub_mb_pred(mb_type);
			for (mb_part_idx = 0; mb_part_idx < 4; mb_part_idx ++)
			{
				if (sub_mb_type[mb_part_idx] != B_Direct_8x8)
				{
					if (num_sub_mb_part(sub_mb_type[mb_part_idx]) > 1)
						no_sub_mb_part_size_less_than_8x8 = 0;
					else if(!sps.direct_8x8_inference_flag)
						no_sub_mb_part_size_less_than_8x8 = 0;
				}
			}
		}
		else
		{
			if (pps.transform_8x8_flag && mb_type == I_NxN)
				transform_size_8x8_flag = stream_get_one_bit();
			mb_pred(mb_type);
		}
		
		if (mb_part_pred_mode(mb_type, 0) != Intra_16x16)
		{
			coded_block_pattern = me();
		}

		if (coded_block_pattern_luma > 0 || coded_block_pattern_chroma > 0 ||
						mb_part_pred_mode(mb_type, 0) == Intra_16x16)
		{
			mb_qp_delta = se();
			residual();
		}

	}
		
}

int mb_pred(int mb_type)
{
                                         
}
