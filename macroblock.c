
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
	int pred_mode = mb_part_pred_mode(mb_type, 0);
	int idx;
	char prev_intra4x4_pred_mode_flag[16];
	char rem_intra4x4_pred_mode[16];
	char prev_intra8x8_pred_mode_flag[4];
	char rem_intra8x8_pred_mode[4];
	int intra_chroma_pred_mode;
	int ref_idx_l0[4];
	int ref_idx_l1[4];
	int mvd_l0[4][2];
	int mvd_l1[4][2];
	
	if (pred_mode == Intra_4x4 ||
			pred_mode == Intra_8x8 ||
			pred_mode == Intra_16x16)
	{
		if (pred_mode == Intra_4x4)
		{
			for (idx == 0; idx < 16; idx ++)
			{
				prev_intra4x4_pred_mode_flag[idx] = stream_get_one_bit();
				if (!prev_intra4x4_pred_mode_flag[idx])
					rem_intra4x4_pred_mode[idx] = stream_get_bits(3);
			}
		}
		if (pred_mode == Intra_8x8)
		{
			for (idx = 0; idx < 4; idx ++)
			{
				prev_intra8x8_pred_mode_flag[idx] = stream_get_one_bit();
				if (!prev_intra8x8_pred_mode_flag[idx])
					rem_intra8x8_pred_mode[idx] = stream_get_bits(3);
			}
		}
		if (chroma_format_idc != 0)
			intra_chroma_pred_mode = ue();
	}
	else if (pred_mode != Direct)
	{
		for (idx = 0; idx < num_mb_part(mb_type); idx ++)
		{
			if ((num_ref_idx_l0_active_minus1 > 0 || mb_field_decoding_flag) &&
					mb_part_pred_mode(mb_type, idx) != Pred_L1)
					ref_idx_l0[idx] = te();
		}

		for (idx = 0; idx < num_mb_part(mb_type); idx ++)
		{
			if ((num_ref_idx_l1_active_minus1 > 0 || mb_field_decoding_flag) &&
					mb_part_pred_mode(mb_type, idx) != Pred_L0)
					ref_idx_l1[idx] = te();
		}

		for (idx = 0; idx < num_mb_part(mb_type); idx ++)
		{
			if (mb_part_pred_mode(mb_type, idx) != Pred_L1)
				for (jdx = 0; jdx < 2; jdx ++)
					mvd_l0[idx][jdx] = se();
		}

		for (idx = 0; idx < num_mb_part(mb_type); idx ++)
		{
			if (mb_part_pred_mode(mb_type, idx) != Pred_L0)
				for (jdx = 0; jdx < 2; jdx ++)
					mvd_l1[idx][jdx] = se();
		}
	}
			
}

int sub_mb_pred(int mb_type)
{
	int sub_mb_type[4];
	int idx;
	int ref_idx_l0[4];
	int ref_idx_l1[4];
	int mvd_l0[4][4][2];
	int mvd_l1[4][4][2];

	for (idx = 0; idx < 4; idx ++)
		sub_mb_type = ue();

	for (idx = 0; idx < 4; idx ++)
	{
		if ((num_ref_idx_l0_active_minus1 > 0 || mb_field_decoding_flag) &&
				mb_type != P_8x8ref0 && sub_mb_type[idx] != B_Direct_8x8 &&
				sub_mb_pred_mode(mb_type, idx) != Pred_L1)
					ref_idx_l0[idx] = te();
	}

	for (idx = 0; idx < 4; idx ++)
	{
		if ((num_ref_idx_l1_active_minus1 > 0 || mb_field_decoding_flag) &&
				mb_type != B_Direct_8x8 && sub_mb_type[idx] != B_Direct_8x8 &&
				sub_mb_pred_mode(mb_type, idx) != Pred_L0)
					ref_idx_l1[idx] = te();
	}

	for (idx = 0; idx < 4; idx ++)
	{
		if (sub_mb_type[idx] != B_Direct_8x8 &&
				sub_mb_pred_mode(mb_type, idx) != Pred_L1)
				for (jdx = 0; jdx < num_sub_mb_part(sub_mb_type[idx]))
					 for (kdx = 0; kdx < 2; kdx ++)
					 	mvd_l0[idx][jdx][kdx] = se();
	}

	for (idx = 0; idx < 4; idx ++)
	{
		if (sub_mb_type[idx] != B_Direct_8x8 &&
				sub_mb_pred_mode(mb_type, idx) != Pred_L0)
				for (jdx = 0; jdx < num_sub_mb_part(sub_mb_type[idx]))
					 for (kdx = 0; kdx < 2; kdx ++)
					 	mvd_l1[idx][jdx][kdx] = se();
	}
}
					 