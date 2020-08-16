#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "bs.h"
#include "sps.h"


void debug_sps(sps_t* sps)
{
	printf("======= SPS =======\n");
	printf(" profile_idc : %d \n", sps->profile_idc );
	printf(" constraint_set0_flag : %d \n", sps->constraint_set0_flag );
	printf(" constraint_set1_flag : %d \n", sps->constraint_set1_flag );
	printf(" constraint_set2_flag : %d \n", sps->constraint_set2_flag );
	printf(" constraint_set3_flag : %d \n", sps->constraint_set3_flag );
	printf(" constraint_set4_flag : %d \n", sps->constraint_set4_flag );
	printf(" constraint_set5_flag : %d \n", sps->constraint_set5_flag );
	printf(" reserved_zero_2bits : %d \n", sps->reserved_zero_2bits );
	printf(" level_idc : %d \n", sps->level_idc );
	printf(" seq_parameter_set_id : %d \n", sps->seq_parameter_set_id );
	printf(" chroma_format_idc : %d \n", sps->chroma_format_idc );
	printf(" separate_colour_plane_flag : %d \n", sps->separate_colour_plane_flag );
	printf(" bit_depth_luma_minus8 : %d \n", sps->bit_depth_luma_minus8 );
	printf(" bit_depth_chroma_minus8 : %d \n", sps->bit_depth_chroma_minus8 );
	printf(" qpprime_y_zero_transform_bypass_flag : %d \n", sps->qpprime_y_zero_transform_bypass_flag );
	printf(" seq_scaling_matrix_present_flag : %d \n", sps->seq_scaling_matrix_present_flag );
	//  int seq_scaling_list_present_flag[8];
	//  void* ScalingList4x4[6];
	//  int UseDefaultScalingMatrix4x4Flag[6];
	//  void* ScalingList8x8[2];
	//  int UseDefaultScalingMatrix8x8Flag[2];
	printf(" log2_max_frame_num_minus4 : %d \n", sps->log2_max_frame_num_minus4 );
	printf(" pic_order_cnt_type : %d \n", sps->pic_order_cnt_type );
	printf(" log2_max_pic_order_cnt_lsb_minus4 : %d \n", sps->log2_max_pic_order_cnt_lsb_minus4 );
	printf(" delta_pic_order_always_zero_flag : %d \n", sps->delta_pic_order_always_zero_flag );
	printf(" offset_for_non_ref_pic : %d \n", sps->offset_for_non_ref_pic );
	printf(" offset_for_top_to_bottom_field : %d \n", sps->offset_for_top_to_bottom_field );
	printf(" num_ref_frames_in_pic_order_cnt_cycle : %d \n", sps->num_ref_frames_in_pic_order_cnt_cycle );
	printf(" offset_for_ref_frame[0] : %d \n", sps->offset_for_ref_frame[0] ); //offset_for_ref_frame[256];
	
	printf(" max_num_ref_frames : %d \n", sps->max_num_ref_frames );
	printf(" gaps_in_frame_num_value_allowed_flag : %d \n", sps->gaps_in_frame_num_value_allowed_flag );
	printf(" pic_width_in_mbs_minus1 : %d \n", sps->pic_width_in_mbs_minus1 );
	printf(" pic_height_in_map_units_minus1 : %d \n", sps->pic_height_in_map_units_minus1 );
	printf(" frame_mbs_only_flag : %d \n", sps->frame_mbs_only_flag );
	printf(" mb_adaptive_frame_field_flag : %d \n", sps->mb_adaptive_frame_field_flag );
	printf(" direct_8x8_inference_flag : %d \n", sps->direct_8x8_inference_flag );
	printf(" frame_cropping_flag : %d \n", sps->frame_cropping_flag );
	printf(" frame_crop_left_offset : %d \n", sps->frame_crop_left_offset );
	printf(" frame_crop_right_offset : %d \n", sps->frame_crop_right_offset );
	printf(" frame_crop_top_offset : %d \n", sps->frame_crop_top_offset );
	printf(" frame_crop_bottom_offset : %d \n", sps->frame_crop_bottom_offset );
	printf(" vui_parameters_present_flag : %d \n", sps->vui_parameters_present_flag );

	printf("=== VUI ===\n");
	printf(" aspect_ratio_info_present_flag : %d \n", sps->vui.aspect_ratio_info_present_flag );
	printf(" aspect_ratio_idc : %d \n", sps->vui.aspect_ratio_idc );
	printf(" sar_width : %d \n", sps->vui.sar_width );
	printf("  sar_height : %d \n", sps->vui.sar_height );
	printf(" overscan_info_present_flag : %d \n", sps->vui.overscan_info_present_flag );
	printf(" overscan_appropriate_flag : %d \n", sps->vui.overscan_appropriate_flag );
	printf(" video_signal_type_present_flag : %d \n", sps->vui.video_signal_type_present_flag );
	printf(" video_format : %d \n", sps->vui.video_format );
	printf(" video_full_range_flag : %d \n", sps->vui.video_full_range_flag );
	printf(" colour_description_present_flag : %d \n", sps->vui.colour_description_present_flag );
	printf(" colour_primaries : %d \n", sps->vui.colour_primaries );
	printf(" transfer_characteristics : %d \n", sps->vui.transfer_characteristics );
	printf(" matrix_coefficients : %d \n", sps->vui.matrix_coefficients );
	printf(" chroma_loc_info_present_flag : %d \n", sps->vui.chroma_loc_info_present_flag );
	printf(" chroma_sample_loc_type_top_field : %d \n", sps->vui.chroma_sample_loc_type_top_field );
	printf(" chroma_sample_loc_type_bottom_field : %d \n", sps->vui.chroma_sample_loc_type_bottom_field );
	printf(" timing_info_present_flag : %d \n", sps->vui.timing_info_present_flag );
	printf(" num_units_in_tick : %d \n", sps->vui.num_units_in_tick );
	printf(" time_scale : %d \n", sps->vui.time_scale );
	printf(" fixed_frame_rate_flag : %d \n", sps->vui.fixed_frame_rate_flag );
	printf(" nal_hrd_parameters_present_flag : %d \n", sps->vui.nal_hrd_parameters_present_flag );
	printf(" vcl_hrd_parameters_present_flag : %d \n", sps->vui.vcl_hrd_parameters_present_flag );
	printf(" low_delay_hrd_flag : %d \n", sps->vui.low_delay_hrd_flag );
	printf(" pic_struct_present_flag : %d \n", sps->vui.pic_struct_present_flag );
	printf(" bitstream_restriction_flag : %d \n", sps->vui.bitstream_restriction_flag );
	printf(" motion_vectors_over_pic_boundaries_flag : %d \n", sps->vui.motion_vectors_over_pic_boundaries_flag );
	printf(" max_bytes_per_pic_denom : %d \n", sps->vui.max_bytes_per_pic_denom );
	printf(" max_bits_per_mb_denom : %d \n", sps->vui.max_bits_per_mb_denom );
	printf(" log2_max_mv_length_horizontal : %d \n", sps->vui.log2_max_mv_length_horizontal );
	printf(" log2_max_mv_length_vertical : %d \n", sps->vui.log2_max_mv_length_vertical );
	printf(" num_reorder_frames : %d \n", sps->vui.num_reorder_frames );
	printf(" max_dec_frame_buffering : %d \n", sps->vui.max_dec_frame_buffering );

	printf("=== HRD ===\n");
	printf(" cpb_cnt_minus1 : %d \n", sps->hrd.cpb_cnt_minus1 );
	printf(" bit_rate_scale : %d \n", sps->hrd.bit_rate_scale );
	printf(" cpb_size_scale : %d \n", sps->hrd.cpb_size_scale );
	int SchedSelIdx;
	for( SchedSelIdx = 0; SchedSelIdx <= sps->hrd.cpb_cnt_minus1; SchedSelIdx++ )
	{
		printf(" bit_rate_value_minus1[%d] : %d \n", SchedSelIdx, sps->hrd.bit_rate_value_minus1[SchedSelIdx] ); // up to cpb_cnt_minus1, which is <= 31
		printf(" cpb_size_value_minus1[%d] : %d \n", SchedSelIdx, sps->hrd.cpb_size_value_minus1[SchedSelIdx] );
		printf(" cbr_flag[%d] : %d \n", SchedSelIdx, sps->hrd.cbr_flag[SchedSelIdx] );
	}
	printf(" initial_cpb_removal_delay_length_minus1 : %d \n", sps->hrd.initial_cpb_removal_delay_length_minus1 );
	printf(" cpb_removal_delay_length_minus1 : %d \n", sps->hrd.cpb_removal_delay_length_minus1 );
	printf(" dpb_output_delay_length_minus1 : %d \n", sps->hrd.dpb_output_delay_length_minus1 );
	printf(" time_offset_length : %d \n", sps->hrd.time_offset_length );
}

int nal_to_rbsp(const int nal_header_size, const uint8_t* nal_buf, int* nal_size, uint8_t* rbsp_buf, int* rbsp_size)
{
    int i = 0;
    int j     = 0;
    int count = 0;
  
    for( i = nal_header_size; i < *nal_size; i++ )
    { 
        // in NAL unit, 0x000000, 0x000001 or 0x000002 shall not occur at any byte-aligned position
        if( ( count == 2 ) && ( nal_buf[i] < 0x03) ) 
        {
            return -1;
        }

        if( ( count == 2 ) && ( nal_buf[i] == 0x03) )
        {
            // check the 4th byte after 0x000003, except when cabac_zero_word is used, in which case the last three bytes of this NAL unit must be 0x000003
            if((i < *nal_size - 1) && (nal_buf[i+1] > 0x03))
            {
                return -1;
            }

            // if cabac_zero_word is used, the final byte of this NAL unit(0x03) is discarded, and the last two bytes of RBSP must be 0x0000
            if(i == *nal_size - 1)
            {
                break;
            }

            i++;
            count = 0;
        }

        if ( j >= *rbsp_size ) 
        {
            // error, not enough space
            return -1;
        }

        rbsp_buf[j] = nal_buf[i];
        if(nal_buf[i] == 0x00)
        {
            count++;
        }
        else
        {
            count = 0;
        }
        j++;
    }

    *nal_size = i;
    *rbsp_size = j;
    return j;
}

void read_hrd_parameters(sps_t* sps , bs_t* b)
{
    int SchedSelIdx;

    sps->hrd.cpb_cnt_minus1 = bs_read_ue(b);
    sps->hrd.bit_rate_scale = bs_read_u(b,4);
    sps->hrd.cpb_size_scale = bs_read_u(b,4);
    for( SchedSelIdx = 0; SchedSelIdx <= sps->hrd.cpb_cnt_minus1; SchedSelIdx++ )
    {
        sps->hrd.bit_rate_value_minus1[ SchedSelIdx ] = bs_read_ue(b);
        sps->hrd.cpb_size_value_minus1[ SchedSelIdx ] = bs_read_ue(b);
        sps->hrd.cbr_flag[ SchedSelIdx ] = bs_read_u1(b);
    }
    sps->hrd.initial_cpb_removal_delay_length_minus1 = bs_read_u(b,5);
    sps->hrd.cpb_removal_delay_length_minus1 = bs_read_u(b,5);
    sps->hrd.dpb_output_delay_length_minus1 = bs_read_u(b,5);
    sps->hrd.time_offset_length = bs_read_u(b,5);
}

void read_vui_parameters(sps_t* sps, bs_t* b)
{
    sps->vui.aspect_ratio_info_present_flag = bs_read_u1(b);
    if( sps->vui.aspect_ratio_info_present_flag )
    {
        sps->vui.aspect_ratio_idc = bs_read_u8(b);
        if( sps->vui.aspect_ratio_idc == SAR_Extended )
        {
            sps->vui.sar_width = bs_read_u(b,16);
            sps->vui.sar_height = bs_read_u(b,16);
        }
    }
    sps->vui.overscan_info_present_flag = bs_read_u1(b);
    if( sps->vui.overscan_info_present_flag )
    {
        sps->vui.overscan_appropriate_flag = bs_read_u1(b);
    }
    sps->vui.video_signal_type_present_flag = bs_read_u1(b);
    if( sps->vui.video_signal_type_present_flag )
    {
        sps->vui.video_format = bs_read_u(b,3);
        sps->vui.video_full_range_flag = bs_read_u1(b);
        sps->vui.colour_description_present_flag = bs_read_u1(b);
        if( sps->vui.colour_description_present_flag )
        {
            sps->vui.colour_primaries = bs_read_u8(b);
            sps->vui.transfer_characteristics = bs_read_u8(b);
            sps->vui.matrix_coefficients = bs_read_u8(b);
        }
    }
    sps->vui.chroma_loc_info_present_flag = bs_read_u1(b);
    if( sps->vui.chroma_loc_info_present_flag )
    {
        sps->vui.chroma_sample_loc_type_top_field = bs_read_ue(b);
        sps->vui.chroma_sample_loc_type_bottom_field = bs_read_ue(b);
    }
    sps->vui.timing_info_present_flag = bs_read_u1(b);
    if( sps->vui.timing_info_present_flag )
    {
        sps->vui.num_units_in_tick = bs_read_u(b,32);
        sps->vui.time_scale = bs_read_u(b,32);
        sps->vui.fixed_frame_rate_flag = bs_read_u1(b);
    }
    sps->vui.nal_hrd_parameters_present_flag = bs_read_u1(b);
    if( sps->vui.nal_hrd_parameters_present_flag )
    {
        read_hrd_parameters(sps, b);
    }
    sps->vui.vcl_hrd_parameters_present_flag = bs_read_u1(b);
    if( sps->vui.vcl_hrd_parameters_present_flag )
    {
        read_hrd_parameters(sps, b);
    }
    if( sps->vui.nal_hrd_parameters_present_flag || sps->vui.vcl_hrd_parameters_present_flag )
    {
        sps->vui.low_delay_hrd_flag = bs_read_u1(b);
    }
    sps->vui.pic_struct_present_flag = bs_read_u1(b);
    sps->vui.bitstream_restriction_flag = bs_read_u1(b);
    if( sps->vui.bitstream_restriction_flag )
    {
        sps->vui.motion_vectors_over_pic_boundaries_flag = bs_read_u1(b);
        sps->vui.max_bytes_per_pic_denom = bs_read_ue(b);
        sps->vui.max_bits_per_mb_denom = bs_read_ue(b);
        sps->vui.log2_max_mv_length_horizontal = bs_read_ue(b);
        sps->vui.log2_max_mv_length_vertical = bs_read_ue(b);
        sps->vui.num_reorder_frames = bs_read_ue(b);
        sps->vui.max_dec_frame_buffering = bs_read_ue(b);
    }
}

void read_rbsp_trailing_bits( bs_t* b)
{
    int rbsp_stop_one_bit = bs_read_u1( b ); // equal to 1

    while( !bs_byte_aligned(b) )
    {
        int rbsp_alignment_zero_bit = bs_read_u1( b ); // equal to 0
    }
}

void read_scaling_list(bs_t* b, int* scalingList, int sizeOfScalingList, int useDefaultScalingMatrixFlag )
{
    int j;
    if(scalingList == NULL)
    {
        return;
    }

    int lastScale = 8;
    int nextScale = 8;
    for( j = 0; j < sizeOfScalingList; j++ )
    {
        if( nextScale != 0 )
        {
            int delta_scale = bs_read_se(b);
            nextScale = ( lastScale + delta_scale + 256 ) % 256;
            useDefaultScalingMatrixFlag = ( j == 0 && nextScale == 0 );
        }
        scalingList[ j ] = ( nextScale == 0 ) ? lastScale : nextScale;
        lastScale = scalingList[ j ];
    }
}

void read_seq_parameter_set_rbsp(bs_t* b,  sps_t* sps )
{
    int i;
    int profile_idc = bs_read_u8(b);
    int constraint_set0_flag = bs_read_u1(b);
    int constraint_set1_flag = bs_read_u1(b);
    int constraint_set2_flag = bs_read_u1(b);
    int constraint_set3_flag = bs_read_u1(b);
    int constraint_set4_flag = bs_read_u1(b);
    int constraint_set5_flag = bs_read_u1(b);
    int reserved_zero_2bits  = bs_read_u(b,2);  /* all 0's */
    int level_idc = bs_read_u8(b);
    int seq_parameter_set_id = bs_read_ue(b);

    memset(sps, 0, sizeof(sps_t));
    sps->chroma_format_idc = 1; 
    sps->profile_idc = profile_idc; // bs_read_u8(b);
    sps->constraint_set0_flag = constraint_set0_flag;//bs_read_u1(b);
    sps->constraint_set1_flag = constraint_set1_flag;//bs_read_u1(b);
    sps->constraint_set2_flag = constraint_set2_flag;//bs_read_u1(b);
    sps->constraint_set3_flag = constraint_set3_flag;//bs_read_u1(b);
    sps->constraint_set4_flag = constraint_set4_flag;//bs_read_u1(b);
    sps->constraint_set5_flag = constraint_set5_flag;//bs_read_u1(b);
    sps->reserved_zero_2bits = reserved_zero_2bits;//bs_read_u(b,2);
    sps->level_idc = level_idc; //bs_read_u8(b);
	//printf("seq_parameter_set_id:[%d]\n",   seq_parameter_set_id);
    sps->seq_parameter_set_id = seq_parameter_set_id; // bs_read_ue(b);
    if( sps->profile_idc == 100 || sps->profile_idc == 110 ||
        sps->profile_idc == 122 || sps->profile_idc == 144 )
    {
        sps->chroma_format_idc = bs_read_ue(b);
        sps->ChromaArrayType = sps->chroma_format_idc;
        if( sps->chroma_format_idc == 3 )
        {
            sps->separate_colour_plane_flag = bs_read_u1(b);
            if (sps->separate_colour_plane_flag) sps->ChromaArrayType = 0;
        }
        sps->bit_depth_luma_minus8 = bs_read_ue(b);
        sps->bit_depth_chroma_minus8 = bs_read_ue(b);
        sps->qpprime_y_zero_transform_bypass_flag = bs_read_u1(b);
        sps->seq_scaling_matrix_present_flag = bs_read_u1(b);
        if( sps->seq_scaling_matrix_present_flag )
        {
            for( i = 0; i < ((sps->chroma_format_idc!=3) ? 8 : 12); i++ )
            {
                sps->seq_scaling_list_present_flag[ i ] = bs_read_u1(b);
                if( sps->seq_scaling_list_present_flag[ i ] )
                {
                    if( i < 6 )
                    {
                        read_scaling_list( b, &sps->ScalingList4x4[ i ], 16,
                                      sps->UseDefaultScalingMatrix4x4Flag[ i ]);
                    }
                    else
                    {
                        read_scaling_list( b, &sps->ScalingList8x8[ i - 6 ], 64,
                                      sps->UseDefaultScalingMatrix8x8Flag[ i - 6 ] );
                    }
                }
            }
        }
    }
		
    sps->log2_max_frame_num_minus4 = bs_read_ue(b);
    sps->pic_order_cnt_type = bs_read_ue(b);

    if( sps->pic_order_cnt_type == 0 )
    {
        sps->log2_max_pic_order_cnt_lsb_minus4 = bs_read_ue(b);
    }
    else if( sps->pic_order_cnt_type == 1 )
    {
        sps->delta_pic_order_always_zero_flag = bs_read_u1(b);
        sps->offset_for_non_ref_pic = bs_read_se(b);
        sps->offset_for_top_to_bottom_field = bs_read_se(b);
        sps->num_ref_frames_in_pic_order_cnt_cycle = bs_read_ue(b);
	
        for( i = 0; i < sps->num_ref_frames_in_pic_order_cnt_cycle; i++ )
        {
            sps->offset_for_ref_frame[ i ] = bs_read_se(b);
        }
    }
	
    sps->max_num_ref_frames = bs_read_ue(b);
    sps->gaps_in_frame_num_value_allowed_flag = bs_read_u1(b);
	
    sps->pic_width_in_mbs_minus1 = bs_read_ue(b);
    sps->pic_height_in_map_units_minus1 = bs_read_ue(b);
	sps->frame_mbs_only_flag = bs_read_u1(b);
    if( !sps->frame_mbs_only_flag )
    {
        sps->mb_adaptive_frame_field_flag = bs_read_u1(b);
    }
	
    sps->direct_8x8_inference_flag = bs_read_u1(b);
    sps->frame_cropping_flag = bs_read_u1(b);
    if( sps->frame_cropping_flag )
    {
        sps->frame_crop_left_offset = bs_read_ue(b);
        sps->frame_crop_right_offset = bs_read_ue(b);
        sps->frame_crop_top_offset = bs_read_ue(b);
        sps->frame_crop_bottom_offset = bs_read_ue(b);
    }
    sps->vui_parameters_present_flag = bs_read_u1(b);
    if( sps->vui_parameters_present_flag )
    {
        read_vui_parameters(sps, b);
        /* 注：这里的帧率计算还有问题，x264编码25fps，time_scale为50，num_units_in_tick为1，计算得50fps
        网上说法，当nuit_field_based_flag为1时，再除以2，又说x264将该值设置为0.
        地址：http://forum.doom9.org/showthread.php?t=153019
        */
    }

	//debug_sps(sps);
    read_rbsp_trailing_bits( b);
}

int h264_get_width(sps_t *sps)  
{  
	printf("w[%d]", sps->pic_width_in_mbs_minus1);

	int tmp = (sps->pic_width_in_mbs_minus1 + 1) * 16;  
	if(sps->frame_cropping_flag)
	{
		unsigned int crop_unit_x;
		if (0 == sps->chroma_format_idc) // monochrome
			crop_unit_x = 1;
		else if (1 == sps->chroma_format_idc) // 4:2:0
			crop_unit_x = 2;
		else if (2 == sps->chroma_format_idc) // 4:2:2
			crop_unit_x = 2;
		else // 3 == sps.chroma_format_idc	 // 4:4:4
			crop_unit_x = 1;
		
		tmp	-= crop_unit_x * (sps->frame_crop_left_offset + sps->frame_crop_right_offset);
	}
	
    return tmp;  
}
  
int h264_get_height(sps_t* sps)  
{  
	printf("h[%d]", sps->pic_height_in_map_units_minus1);
	int tmp = (sps->pic_height_in_map_units_minus1 + 1) * 16 * (2 - sps->frame_mbs_only_flag);
	
	if(sps->frame_cropping_flag)
	{
		unsigned int crop_unit_y;
		if (0 == sps->chroma_format_idc) // monochrome
			crop_unit_y = 2 - sps->frame_mbs_only_flag;
		else if (1 == sps->chroma_format_idc) // 4:2:0
			crop_unit_y = 2 * (2 - sps->frame_mbs_only_flag);
		else if (2 == sps->chroma_format_idc) // 4:2:2
			crop_unit_y = 2 - sps->frame_mbs_only_flag;
		else // 3 == sps.chroma_format_idc	 // 4:4:4
			crop_unit_y = 2 - sps->frame_mbs_only_flag;
		
		tmp	 -= crop_unit_y * (sps->frame_crop_top_offset  + sps->frame_crop_bottom_offset);
	}	
	
    return  tmp;
}  

float h264_get_framerate(sps_t *sps_ptr)  
{  
	float framerate;
		if (sps_ptr->vui.num_units_in_tick != 0)
            framerate = (float)(sps_ptr->vui.time_scale) / (float)(sps_ptr->vui.num_units_in_tick);
     return framerate;  
}  

int spare_sps(char *date, int sz)
{
	int m_nWidth;  
    int m_nHeight;  
    double m_nFrameRate;  
	float fps = 0.0; 
	sps_t _sps; 
	char tmpbuf[40] ={'\0'};
	int len = sz;
	
	int rc = nal_to_rbsp(0, date, &sz, tmpbuf, &len );
    bs_t* b = bs_new(tmpbuf, len);

	//从sps pps中获取信息  
	read_seq_parameter_set_rbsp(b, &_sps); 

	m_nWidth = h264_get_width(&_sps);  
	m_nHeight = h264_get_height(&_sps); 
	fps = h264_get_framerate(&_sps);   

	bs_free(b);
	printf("\nw:[%d] h:[%d] fps:[%f]\n", m_nWidth, m_nHeight, m_nFrameRate);
	
	return 0;
}

//char date_p[] = {0x00, 0x00, 0x00, 0x01, 0x28, 0xCE, 0x32, 0x48, 0x36, 0x31, 0x34, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int main()
{
	//以下是nal 去掉nalu header的 sps 帧信息.	
	#if  1
		//848-480.264:       848*480
		char date_s[] = { 0x42, 0x00, 0x1F, 0x8D, 0x68, 0x0D, 0x43, 0xDA, 0x10,
									0x00, 0x00, 0x03, 0x00, 0x10, 0x00, 0x00, 0x03, 0x01, 0xE0, 0xF1, 0x07, 0xA8};			  
		spare_sps(date_s,  sizeof(date_s));

		//src.h264:      720*480 
		char date_s1[] = {0x4D, 0x00, 0x1E, 0x8D, 0x94, 0xC0, 0x5A, 0x1E, 0x88};			  
		spare_sps(date_s1,  sizeof(date_s1));
		//nat.h264       704*576
		char date_s2[] = {0x4D, 0x40, 0x1E, 0x95, 0x15, 0x18, 0x80, 0x58, 0x09, 0x32};			  
		spare_sps(date_s2,  sizeof(date_s2));

		//720P25.h264
		char date_s3[] = {0x64, 0x00, 0x1F, 0xAD, 0x84, 0x01, 0x0C, 0x20, 0x08, 0x61, 0x00, 0x43, 0x08, 0x02, 0x18, 0x40,
					  0x10, 0xC2, 0x00, 0x84, 0x2B, 0x50, 0x28, 0x02, 0xDD, 0x37, 0x01, 0x01, 0x01, 0x40, 0x00, 0x00,
					  0xFA, 0x00, 0x00, 0x3A, 0x98, 0x21};
		spare_sps(date_s3,  sizeof(date_s3));
	
	#else    
	

	#endif
	
	return 0;
} 

