#include <stdio.h>
#include <strint.h>

#include "pps.h"
#include "sps.h"
#include "rbsp.h"
#include "stream.h"
#include "exp_golomb.h"
#include "slice.h"
#include "macroblock.h"

int process_slice_hdr(void)
{
	memset(&slice_hdr, 0, sizeof(struct slice_hdr));
	
	slice_hdr.first_mb_in_slice = ue();
	slice_hdr.slice_type = ue();
	slice_hdr.pps_id = ue();
	slice_hdr.frame_num = stream_get_bits(sps.max_frame_num + 4);	// need further confirmation
	if (!sps.frame_mbs_only)
	{
		slice_hdr.field_pic_flag = stream_get_one_bit();
		if (slice_hdr.field_pic_flag)
			slice_hdr.bottom_field_flag = stream_get_one_bit();
	}

	if (nal_unit_type == 5)
		slice_hdr.idr_pic_id = ue();

	if (sps.poc_type == 0)
	{
		slice_hdr.poc_lsb = stream_get_bits(); 						// need further confirmation
		if (pps.pic_order_present && !slice_hdr.field_pic_flag)
			slice_hdr.delta_poc_bottom = se();
	}

	if (sps.poc_type == 1 && !sps.delta_pic_order_always_zero)
	{
		slice_hdr.delta_poc0 = se();
		if (pps.pic_order_present && !slice_hdr.field_pic_flag)
			slice_hdr.delta_poc1 = se();
	}

	if (pps.redundant_pic_cnt_present_flag)
		slice_hdr.redundant_pic_cnt = ue();

	if (slice_hdr.slice_type == P)  			// need further confirmation
	{
		slice_hdr.num_ref_idx_active_override = stream_get_one_bit();
		if (slice_hdr.num_ref_idx_active_override)
		{
			slice_hdr.num_ref_idx_l0_active = ue();

			if (slice_hdr.slice_type == B)
				slice_hdr.num_ref_idx_l1_active = ue();
		}
	}

	ref_pic_list_reordering();

	if (nal_ref_idc != 0)
		dec_ref_pic_marking();

	slice_hdr.slice_qp_delta = se();

	if (pps.dbk_filter_control_present)
	{
		slice_hdr.disable_deblocking_filter_idc = ue();
		if (slice_hdr.disable_deblocking_filter_idc != 1)
		{
			slice_hdr.slice_alpha_c0_offset = se();
			slice_hdr.slice_beta_offset = se();
		}
	}
}

void dump_slice_hdr(void)
{
	printf("first_mb_in_slice  : %d\n", slice_hdr.first_mb_in_slice );  
    printf("slice_type  : %d\n", slice_hdr.slice_type ); 
    printf("pps_id  : %d\n", slice_hdr.pps_id );   
	printf("frame_num  : %d\n", slice_hdr.frame_num ); 
    printf("field_pic_flag : %d\n", field_pic_flag);
	printf("bottom_field_flag : %d\n", bottom_field_flag);
    printf("idr_pic_id  : %d\n", slice_hdr.idr_pic_id );  
    printf("poc_lsb  : %d\n", slice_hdr.poc_lsb );   
    printf("delta_poc_bottom  : %d\n", slice_hdr.delta_poc_bottom ); 
    printf("delta_poc0  : %d\n", slice_hdr.delta_poc0 );  
    printf("delta_poc1  : %d\n", slice_hdr.delta_poc1 ); 
    printf("redundant_pic_cnt  : %d\n", slice_hdr.redundant_pic_cnt );  
	printf("direct_spatial_mv_pred_flag  : %d\n", slice_hdr.direct_spatial_mv_pred_flag );   
	printf("num_ref_idx_active_override  : %d\n", slice_hdr.num_ref_idx_active_override );   
    printf("num_ref_idx_l0_active  : %d\n", slice_hdr.num_ref_idx_l0_active ); 
    printf("num_ref_idx_l1_active  : %d\n", slice_hdr.num_ref_idx_l1_active );    
    printf("slice_qp_delta  : %d\n", slice_hdr.slice_qp_delta );     slice_qp_delta;
    printf("disable_deblocking_filter_idc  : %d\n", slice_hdr.disable_deblocking_filter_idc );
    printf("slice_alpha_c0_offset  : %d\n", slice_hdr.slice_alpha_c0_offset );  
    printf("slice_beta_offset  : %d\n", slice_hdr.slice_beta_offset );  
}

int process_slice_data(void)
{
	int more_data_flag = 1;
	int prev_mb_skipped = 0;

	int i;

	int curr_mb_addr = slice_hdr.first_mb_in_slice * (1 + sps.mb_adaptive_frame_field);

	int mb_field_decoding_flag;

	do{
		if (slice_hdr.slice_type != SLICE_TYPE_I)
		{
			if (!pps.entropy_mode)
			{
				int mb_skip_run = ue();

				prev_mb_skipped = (mb_skip_run > 0);

				for (i = 0; i < mb_skip_run; i ++)
					curr_mb_addr = next_mb_addr(curr_mb_addr);

				more_data_flag = more_rbsp_data();
			}
		}
		if (more_data_flag)
		{
			if (sps.mb_adaptive_frame_field && 
				(((curr_mb_addr % 2) == 0) || ((curr_mb_addr % 2 == 1) && prev_mb_skipped)))
			{
				mb_field_decoding_flag = stream_get_one_bit();
			}

			process_macroblock();
		}

		if (!pps.entropy_mode)
			more_data_flag = more_rbsp_data();
		
		curr_mb_addr = next_mb_addr(curr_mb_addr);
	}while(more_data_flag)

	return 0;
}


