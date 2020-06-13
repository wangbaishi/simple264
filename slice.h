/* definitions used in decording slice data */

#ifndef SLICE_H
#define SLICE_H

#define SLICE_TYPE_I	1
#define SLICE_TYPE_B	2
#define SLICE_TYPE_P	3

struct slice_hdr{
    int     first_mb_in_slice;
    int     slice_type;
    int     pps_id;
	int 	frame_num;
    char    field_pic_flag;
    char    bottom_field_flag;
    int     idr_pic_id;
    int     poc_lsb;
    int     delta_poc_bottom;
    int     delta_poc0;
    int     delta_poc1;
    int     redundant_pic_cnt;
    char    direct_spatial_mv_pred_flag;
    char    num_ref_idx_active_override;
    int     num_ref_idx_l0_active;
    int     num_ref_idx_l1_active;
    int     slice_qp_delta;
    int     disable_deblocking_filter_idc;
    int     slice_alpha_c0_offset;
    int     slice_beta_offset;
}

struct slice_hdr slice_hdr;

int process_slice_hdr(void);

void dump_slice_hdr(void);

int process_slice_data(void);

#endif