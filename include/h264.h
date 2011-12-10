/*
 * Copyright (C) 2011 Marcin Kościelnicki <koriakin@0x04.net>
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef H264_H
#define H264_H

#include "vstream.h"

enum h264_nal_unit_type {
	H264_NAL_UNIT_TYPE_SLICE_NONIDR = 1,
	H264_NAL_UNIT_TYPE_SLICE_PART_A = 2,		/* main data */
	H264_NAL_UNIT_TYPE_SLICE_PART_B = 3,		/* I, SI residual blocks */
	H264_NAL_UNIT_TYPE_SLICE_PART_C = 4,		/* P, B residual blocks */
	H264_NAL_UNIT_TYPE_SLICE_IDR = 5,
	H264_NAL_UNIT_TYPE_SEI = 6,
	H264_NAL_UNIT_TYPE_SEQPARM = 7,
	H264_NAL_UNIT_TYPE_PICPARM = 8,
	H264_NAL_UNIT_TYPE_ACC_UNIT_DELIM = 9,
	H264_NAL_UNIT_TYPE_END_SEQ = 10,
	H264_NAL_UNIT_TYPE_END_STREAM = 11,
	H264_NAL_UNIT_TYPE_FILLER_DATA = 12,
	H264_NAL_UNIT_TYPE_SEQPARM_EXT = 13,
	H264_NAL_UNIT_TYPE_PREFIX_NAL_UNIT = 14,	/* SVC/MVC */
	H264_NAL_UNIT_TYPE_SUBSET_SEQPARM = 15,		/* SVC/MVC */
	H264_NAL_UNIT_TYPE_SLICE_AUX = 19,
	H264_NAL_UNIT_TYPE_SLICE_EXT = 20,		/* SVC/MVC */
};

enum h264_profile_idc {
	H264_PROFILE_CAVLC_444 = 44,
	H264_PROFILE_BASELINE = 66,
	H264_PROFILE_MAIN = 77,
	H264_PROFILE_SCALABLE_BASELINE = 83,
	H264_PROFILE_SCALABLE_HIGH = 86,
	H264_PROFILE_EXTENDED = 88,
	H264_PROFILE_HIGH = 100,
	H264_PROFILE_HIGH_10 = 110,
	H264_PROFILE_MULTIVIEW_HIGH = 118,
	H264_PROFILE_HIGH_422 = 122,
	H264_PROFILE_STEREO_HIGH = 128,
	H264_PROFILE_HIGH_444_PRED = 244,
};

enum h264_primary_pic_type {
	H264_PRIMARY_PIC_TYPE_I = 0,
	H264_PRIMARY_PIC_TYPE_P_I = 1,
	H264_PRIMARY_PIC_TYPE_P_B_I = 2,
	H264_PRIMARY_PIC_TYPE_SI = 3,
	H264_PRIMARY_PIC_TYPE_SP_SI = 4,
	H264_PRIMARY_PIC_TYPE_I_SI = 5,
	H264_PRIMARY_PIC_TYPE_P_I_SP_SI = 6,
	H264_PRIMARY_PIC_TYPE_P_B_I_SP_SI = 7,
};

struct h264_vui {
	/* XXX: fill me */
};

struct h264_seqparm {
	uint32_t profile_idc;
	uint32_t constraint_set;
	uint32_t level_idc;
	uint32_t seq_parameter_set_id;
	/* start of new profile only stuff */
	uint32_t chroma_format_idc;
	uint32_t separate_colour_plane_flag;
	uint32_t bit_depth_luma_minus8;
	uint32_t bit_depth_chroma_minus8;
	uint32_t qpprime_y_zero_transform_bypass_flag;
	uint32_t seq_scaling_matrix_present_flag;
	uint32_t seq_scaling_list_present_flag[12];
	uint32_t use_default_scaling_matrix_flag[12];
	uint32_t seq_scaling_list_4x4[6][16];
	uint32_t seq_scaling_list_8x8[6][64];
	/* end of new profile only stuff */
	uint32_t log2_max_frame_num_minus4;
	uint32_t pic_order_cnt_type;
	uint32_t log2_max_pic_order_cnt_lsb_minus4;
	uint32_t delta_pic_order_always_zero_flag;
	int32_t offset_for_non_ref_pic;
	int32_t offset_for_top_to_bottom_field;
	uint32_t num_ref_frames_in_pic_order_cnt_cycle;
	int32_t offset_for_ref_frame[255];
	uint32_t max_num_ref_frames;
	uint32_t gaps_in_frame_num_value_allowed_flag;
	uint32_t pic_width_in_mbs_minus1;
	uint32_t pic_height_in_map_units_minus1;
	uint32_t frame_mbs_only_flag;
	uint32_t mb_adaptive_frame_field_flag;
	uint32_t direct_8x8_inference_flag;
	uint32_t frame_cropping_flag;
	uint32_t frame_crop_left_offset;
	uint32_t frame_crop_right_offset;
	uint32_t frame_crop_top_offset;
	uint32_t frame_crop_bottom_offset;
	struct h264_vui *vui;
};

struct h264_picparm {
	struct h264_seqparm *seqparm;
	uint32_t entropy_coding_mode_flag;
	uint32_t num_ref_idx_l0_default_active_minus1;
	uint32_t num_ref_idx_l1_default_active_minus1;
	uint32_t pic_init_qp_minus26;
};

struct h264_sliceparm {
	struct h264_picparm *picparm;
	uint32_t slice_type;
	uint32_t num_ref_idx_l0_active_minus1;
	uint32_t num_ref_idx_l1_active_minus1;
	uint32_t slice_qp_delta;
	uint32_t cabac_init_idc;
	/* derived stuff starts here */
	uint32_t sliceqpy;
};

struct h264_pred_weight_table_entry {
	uint32_t luma_weight_flag;
	uint32_t luma_weight;
	uint32_t luma_offset;
	uint32_t chroma_weight_flag;
	uint32_t chroma_weight[2];
	uint32_t chroma_offset[2];
};

struct h264_pred_weight_table {
	uint32_t luma_log2_weight_denom;
	uint32_t chroma_log2_weight_denom;
	struct h264_pred_weight_table_entry l0[0x20];
	struct h264_pred_weight_table_entry l1[0x20];
};

struct h264_macroblock {
	uint32_t mb_skip_flag;
	uint32_t mb_field_decoding_flag;
	uint32_t mb_type;
	uint32_t coded_block_pattern;
	uint32_t transform_size_8x8_flag;
	uint32_t mb_qp_delta;
	uint32_t pcm_sample_luma[256];
	uint32_t pcm_sample_chroma[64];
};

int h264_pred_weight_table(struct bitstream *str, struct h264_sliceparm *slp, struct h264_pred_weight_table *table);
void h264_del_seqparm(struct h264_seqparm *seqparm);
int h264_seqparm(struct bitstream *str, struct h264_seqparm *seqparm);

void h264_print_seqparm(struct h264_seqparm *seqparm);

#endif
