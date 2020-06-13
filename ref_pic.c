
#include "stream.h"
#include "ref_pic_list.h"

int ref_pic_list_reordering(void)
{
	int flag_l0;
	int pic_nums_idc;
	
	if ((slice_hdr.slice_type != SLICE_TYPE_I) && 
				(slice_hdr.slice_type != SLICE_TYPE_SI))
	{
		flag_l0 = stream_get_one_bit();
		if (flag_l0)
		{
			do{
				pic_nums_idc = ue();
				if ((pic_nums_idc == 0) || (pic_nums_idc == 1))
				{
					int abs_diff_pic_num = ue();
					
				}
				else if (pic_num_idc == 2)
				{
					int long_term_pic_num = ue();
					
				}
			}while(pic_nums_idc != 3)
		}
	}

	return 0;
}

int dec_ref_pic_marking(void)
{
	if (nal_unit_type == 5)
	{
		int no_output_of_prior_pics_flag = stream_get_one_bit();
		int long_term_reference_flag = stream_get_one_bit();

	}
	else
	{
		int adaptive_ref_pic_marking_mode = stream_get_one_bit();
		int mem_mng_ctrl_op;
		if (adaptive_ref_pic_marking_mode)
		{
			do{
				mem_mng_ctrl_op = ue();
				if ((mem_mng_ctrl_op == 1) ||
						(mem_mng_ctrl_op == 3))
				{
					int diff_pic_nums = ue();
				}
				if (mem_mng_ctrl_op == 2)
				{
					int long_term_pic_num = ue();
				}
				if ((mem_mng_ctrl_op == 3) ||
						(mem_mng_ctrl_op == 6))
				{
					int long_term_fram_idx = ue();
				}
				if (mem_mng_ctrl_op == 4)
				{
					int long_term_fram_idx = ue();
				}
			}while(mem_mng_ctrl_op != 0)
		}
	}
}
	
