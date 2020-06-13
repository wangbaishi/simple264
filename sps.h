/* this header file contains definition of data structures and functions
   used in processing sequance parameter set. */

#ifndef SPS_H
#define SPS_H

#define  MAX_REF_FRAMES_IN_POC_CYCLE    64

struct sps{
    int     profile_idc;
    int     level_idc;
    char    constraint_set0_flag;
    char    constraint_set1_flag;
    char    constraint_set2_flag;
    char    constraint_set3_flag;
    int     sps_id;
    int     max_frame_num;
    char    poc_type;
    int     max_poc_lsb;
    char    delta_pic_order_always_zero;
    int     non_ref_pic_offset;
    int     top_bottom_field_offset;
    int     num_ref_frames_in_poc_cycle;
    int     offset_for_ref_frame[MAX_REF_FRAMES_IN_POC_CYCLE];
    int     num_ref_frames;
    char    gaps_in_frame_num_value_allowed;
    int     pic_width;
    int     pic_height;
    char    frame_mbs_only;
    char    mb_adaptive_frame_field;
	char 	direct_8x8_inference_flag;
    char    frame_cropping_flag;
    int     frame_crop_left;
    int     frame_crop_right;
    int     frame_crop_top;
    int     frame_crop_bottom;
    char    vui_param_present;
};

struct sps  sps;

int process_sps(void);

void dump_sps(void);

#endif