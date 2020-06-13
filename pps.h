/* definition of data structures and functions
   used in processing sequance parameter set. */

#ifndef PPS_H
#define PPS_H

struct pps{
    int     pps_id;
    int     sps_id;
    char    entropy_mode;
    char    pic_order_present;
    int     num_slice_group;
    int     num_ref_idx0;
    int     num_ref_idx1;
    char    weighted_pred_flag;
    int     weighted_bipred_idc;
    int     init_qp;
    int     init_qs;
    int     chroma_qp_index_offset;
    char    dbk_filter_control_present;
    char    constrained_intra_pred_flag;
    char    redundant_pic_cnt_present_flag;
	char 	transform_8x8_flag;
	char 	pic_scaling_matrix_present;
	int 	second_chroma_qp_idx_offset;
}

struct pps  pps;

int process_pps(void);

#endif