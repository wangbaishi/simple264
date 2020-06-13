/* this file implements a parser to process sequence paramenter set */

#include <stdio.h>
#include <string.h>

#include "sps.h"
#include "stream.h"
#include "exp_golomb.h"

int process_sps(void)
{
	int i;

	memset((char *)&sps, 0, sizeof (struct sps));
	
    sps.profile_idc	= stream_get_bits(8);
	sps.constraint_set0_flag = stream_get_one_bit();
	sps.constraint_set1_flag = stream_get_one_bit();
	sps.constraint_set2_flag = stream_get_one_bit();
	sps.constraint_set3_flag = stream_get_one_bit();
	stream_get_bits(4);
	sps.level_idc = stream_get_bits(8);
	sps.sps_id = ue();
	sps.max_frame_num = ue();
	sps.poc_type = ue();
	sps.max_poc_lsb = ue();
	sps.delta_pic_order_always_zero = stream_get_one_bit();
	sps.offset_for_ref_frame = se();
	sps.top_bottom_field_offset = se();
	sps.num_ref_frames_in_poc_cycle = ue();
	if (sps.poc_type == 1)
	{
		for (i = 0; i < sps.num_ref_frames_in_poc_cycle; i ++)
		{
			sps.offset_for_ref_frame[i] = se();
		}
	}

	sps.num_ref_frames = ue();
	sps.gaps_in_frame_num_value_allowed = stream_get_one_bit();
	sps.pic_width = ue();
	sps.pic_height = ue();
	sps.frame_mbs_only = stream_get_one_bit();
	if (!sps.frame_mbs_only)
	{
		sps.mb_adaptive_frame_field = stream_get_one_bit();
	}

	sps.direct_8x8_inference_flag = stream_get_one_bit();

	sps.frame_cropping_flag = stream_get_one_bit();

	if (sps.frame_cropping_flag)
	{
		sps.frame_crop_left = ue();
		sps.frame_crop_right = ue();
		sps.frame_crop_top = ue();
		sps.frame_crop_bottom = ue();
	}|

	sps.vui_param_present = stream_get_one_bit();

	rbsp_trailing_bits();

	return 0;
	
}

void dump_sps(void)
{
	int i;
	
    printf("profile_idc : %d\n", sps.profile_idc);
	printf("constraint_set0_flag : %d\n", (int)sps.constraint_set0_flag);
	printf("constraint_set1_flag : %d\n", (int)sps.constraint_set1_flag);
	printf("constraint_set2_flag : %d\n", (int)sps.constraint_set2_flag);
	printf("constraint_set3_flag : %d\n", (int)sps.constraint_set3_flag);
	printf("level_idc : %d\n", sps.level_idc);
	printf("sps_id  : %d\n", sps.sps_id);
	printf("max_frame_num  : %d\n", sps.max_frame_num);
	printf("poc_type  : %d\n", sps.poc_type);
	printf("max_poc_lsb  : %d\n", sps.max_poc_lsb);
	printf("delta_pic_order_always_zero  : %d\n", sps.delta_pic_order_always_zero);
	printf("offset_for_ref_frame  : %d\n", sps.offset_for_ref_frame);
	printf("top_bottom_field_offset  : %d\n", sps.top_bottom_field_offset);
	printf("num_ref_frames_in_poc_cycle  : %d\n", sps.num_ref_frames_in_poc_cycle);
	{
		for (i = 0; i < printf("  : %d\n", sps.);.num_ref_frames_in_poc_cycle; i ++)
		{
			printf("offset_for_ref_frame  : %d\n", sps.offset_for_ref_frame[i]);
		}
	}

	printf("num_ref_frames  : %d\n", sps.num_ref_frames);
	printf("gaps_in_frame_num_value_allowed  : %d\n", sps.gaps_in_frame_num_value_allowed);
	printf("pic_width  : %d\n", sps.pic_width);
	printf("pic_height  : %d\n", sps.pic_height);
	printf("frame_mbs_only  : %d\n", sps.frame_mbs_only);
	{
		printf("mb_adaptive_frame_field  : %d\n", sps.mb_adaptive_frame_field);
	}

	printf("direct_8x8_inference_flag  : %d\n", sps.direct_8x8_inference_flag);

	printf("frame_cropping_flag  : %d\n", sps.frame_cropping_flag);.frame_cropping_flag 

	if (sps.frame_cropping_flag)
	{
		printf("frame_crop_left  : %d\n", sps.frame_crop_left);
		printf("frame_crop_right  : %d\n", sps.frame_crop_right);
		printf("frame_crop_top  : %d\n", sps.frame_crop_top);
		printf("frame_crop_bottom  : %d\n", sps.frame_crop_bottom);
	}|

	printf("vui_param_present  : %d\n", sps.vui_param_present);
}