
#include <stdio.h>
#include <string.h>

#include "pps.h"
#include "sps.h"
#include "rbsp.h"
#include "stream.h"
#include "exp_golomb.h"

int process_pps(void)
{
	memset((char *)&pps, 0, sizeof (struct pps));
	
	pps.pps_id = ue();
	pps.sps_id = ue();
	pps.entropy_mode = stream_get_one_bit();
	pps.pic_order_present = stream_get_one_bit();
	pps.num_slice_group = ue();
	pps.num_ref_idx0 = ue();
	pps.num_ref_idx1 = ue();
	pps.weighted_pred_flag = stream_get_one_bit();
	pps.weighted_bipred_idc = stream_get_bits(2);
	pps.init_qp = se();
	pps.init_qs = se();
	pps.chroma_qp_index_offset = se();
	pps.dbk_filter_control_present = stream_get_one_bit();
	pps.constrained_intra_pred_flag = stream_get_one_bit();
	pps.redundant_pic_cnt_present_flag = stream_get_one_bit();
	pps.transform_8x8_flag = stream_get_one_bit();
	pps.pic_scaling_matrix_present = stream_get_one_bit();
	pps.second_chroma_qp_idx_offset = se();	

	rbsp_trailing_bits();
}

void dump_pps(void)
{
	printf("pps_id : %d\n", pps.pps_id);
	printf("sps_id : %d\n", pps.sps_id);
	printf("entropy_mode : %d\n", pps.entropy_mode);
	printf("pic_order_present : %d\n", pps.pic_order_present);
	printf("num_slice_group : %d\n", pps.num_slice_group);
	printf("num_ref_idx0 : %d\n", pps.num_ref_idx0);
	printf("num_ref_idx1 : %d\n", pps.num_ref_idx1);
	printf("weighted_pred_flag : %d\n", pps.weighted_pred_flag);
	printf("weighted_bipred_idc : %d\n", pps.weighted_bipred_idc);
	printf("init_qp : %d\n", pps.init_qp);
	printf("init_qs : %d\n", pps.init_qs);
	printf("chroma_qp_index_offset : %d\n", pps.chroma_qp_index_offset);
	printf("dbk_filter_control_present : %d\n", pps.dbk_filter_control_present);
	printf("constrained_intra_pred_flag : %d\n", pps.constrained_intra_pred_flag);
	printf("redundant_pic_cnt_present_flag : %d\n", pps.redundant_pic_cnt_present_flag);
	printf("transform_8x8_flag : %d\n", pps.transform_8x8_flag);
	printf("pic_scaling_matrix_present : %d\n", pps.pic_scaling_matrix_present);
	printf("second_chroma_qp_idx_offset : %d\n", pps.second_chroma_qp_idx_offset);
	
}

