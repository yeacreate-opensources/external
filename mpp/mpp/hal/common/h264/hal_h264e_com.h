/*
 * Copyright 2015 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __HAL_H264E_H__
#define __HAL_H264E_H__

#include "mpp_log.h"

#include "mpp_hal.h"
#include "mpp_rc.h"
#include "mpp_device.h"

#include "h264e_syntax.h"
#include "hal_h264e_debug.h"

#define HAL_H264E_DBG_RKV_DPB           0x00001000
#define HAL_H264E_DBG_RKV_HEADER        0x00008000
#define HAL_H264E_DBG_RKV_RC            0x00080000

#define H264E_HAL_CLIP3(v, min, max)    ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))

#define H264E_HAL_SET_REG(reg, addr, val)                                    \
    do {                                                                     \
        reg[(addr)>>2] = (RK_U32)(val);                                      \
        if (hal_h264e_debug & 0/*H264E_HAL_LOG_INFO*/)                              \
            mpp_log("line(%d) set reg[%03d/%03x]: %08x", __LINE__, (addr)>>2, addr, val); \
    } while (0)

#define H264E_HAL_SPRINT(s, len, ...)  \
    do {                               \
        if (len > 0) { \
            RK_S32 n = snprintf(s, len, ## __VA_ARGS__); \
            s += n; \
            len -= n; \
        } \
    } while (0)

#define H264E_UUID_LENGTH           16

#define H264E_REF_MAX               16

#define H264E_SPSPPS_BUF_SIZE       512  //sps + pps
#define H264E_SEI_BUF_SIZE          1024 //unit in byte, may not be large enough in the future
#define H264E_EXTRA_INFO_BUF_SIZE   (H264E_SPSPPS_BUF_SIZE + H264E_SEI_BUF_SIZE)

#define H264E_NUM_REFS              1
#define H264E_LONGTERM_REF_EN       0
#define H264E_CQM_FLAT              0
#define H264E_CQM_JVT               1
#define H264E_CQM_CUSTOM            2
#define H264E_B_PYRAMID_NONE        0
#define H264E_B_PYRAMID_STRICT      1
#define H264E_B_PYRAMID_NORMAL      2


typedef enum H264eRkvCsp_e {
    H264E_RKV_CSP_BGRA8888,         // 0
    H264E_RKV_CSP_BGR888,           // 1
    H264E_RKV_CSP_BGR565,           // 2
    H264E_RKV_CSP_NONE,             // 3
    H264E_RKV_CSP_YUV422SP,         // 4
    H264E_RKV_CSP_YUV422P,          // 5
    H264E_RKV_CSP_YUV420SP,         // 6
    H264E_RKV_CSP_YUV420P,          // 7
    H264E_RKV_CSP_YUYV422,          // 8
    H264E_RKV_CSP_UYVY422,          // 9
    H264E_RKV_CSP_BUTT,             // 10
} H264eRkvCsp;

/* transplant from vpu_api.h:EncInputPictureType */
typedef enum H264VpuCsp_e {
    H264E_VPU_CSP_YUV420P   = 0,    // YYYY... UUUU... VVVV
    H264E_VPU_CSP_YUV420SP  = 1,    // YYYY... UVUVUV...
    H264E_VPU_CSP_YUYV422   = 2,    // YUYVYUYV...
    H264E_VPU_CSP_UYVY422   = 3,    // UYVYUYVY...
    H264E_VPU_CSP_RGB565    = 4,    // 16-bit RGB
    H264E_VPU_CSP_BGR565    = 5,    // 16-bit RGB
    H264E_VPU_CSP_RGB555    = 6,    // 15-bit RGB
    H264E_VPU_CSP_BGR555    = 7,    // 15-bit RGB
    H264E_VPU_CSP_ARGB8888  = 7,
    H264E_VPU_CSP_RGB444    = 8,    // 12-bit RGB
    H264E_VPU_CSP_BGR444    = 9,    // 12-bit RGB
    H264E_VPU_CSP_RGB888    = 10,   // 24-bit RGB
    H264E_VPU_CSP_BGR888    = 11,   // 24-bit RGB
    H264E_VPU_CSP_RGB101010 = 12,   // 30-bit RGB
    H264E_VPU_CSP_BGR101010 = 13,   // 30-bit RGB
    H264E_VPU_CSP_NONE,
    H264E_VPU_CSP_BUTT,
} H264eVpuCsp;

typedef enum H264eCqm4_e {
    H264E_CQM_4IY = 0,
    H264E_CQM_4PY = 1,
    H264E_CQM_4IC = 2,
    H264E_CQM_4PC = 3
} H264eCqm4;

typedef enum H264eCqm8_e {
    H264E_CQM_8IY = 0,
    H264E_CQM_8PY = 1,
    H264E_CQM_8IC = 2,
    H264E_CQM_8PC = 3,
} H264eCqm8;

typedef enum H264eOsdPltType_e {
    H264E_OSD_PLT_TYPE_NONE     = -1,
    H264E_OSD_PLT_TYPE_USERDEF  = 0,
    H264E_OSD_PLT_TYPE_DEFAULT  = 1,
} H264eOsdPltType;

typedef struct H264eSps_e {
    RK_S32 i_id;

    RK_S32 i_profile_idc;
    RK_S32 i_level_idc;

    RK_S32 b_constraint_set0;
    RK_S32 b_constraint_set1;
    RK_S32 b_constraint_set2;
    RK_S32 b_constraint_set3;

    RK_S32 i_log2_max_frame_num;

    RK_S32 i_poc_type;
    /* poc 0 */
    RK_S32 i_log2_max_poc_lsb;

    RK_S32 i_num_ref_frames;
    RK_S32 b_gaps_in_frame_num_value_allowed;
    RK_S32 i_mb_width;
    RK_S32 i_mb_height;
    RK_S32 b_frame_mbs_only;
    RK_S32 b_mb_adaptive_frame_field;
    RK_S32 b_direct8x8_inference;

    RK_S32 b_crop;
    struct {
        RK_S32 i_left;
        RK_S32 i_right;
        RK_S32 i_top;
        RK_S32 i_bottom;
    } crop;

    MppEncH264VuiCfg vui;

    RK_S32 b_qpprime_y_zero_transform_bypass;
    RK_S32 i_chroma_format_idc;

    /* only for backup, excluded in read SPS*/
    RK_S32 keyframe_max_interval;
} H264eSps;

typedef struct H264ePps_t {
    RK_S32 i_id;
    RK_S32 i_sps_id;

    RK_S32 b_cabac;

    RK_S32 b_pic_order;
    RK_S32 i_num_slice_groups;

    RK_S32 i_num_ref_idx_l0_default_active;
    RK_S32 i_num_ref_idx_l1_default_active;

    RK_S32 b_weighted_pred;
    RK_S32 i_weighted_bipred_idc;

    RK_S32 i_pic_init_qp;
    RK_S32 i_pic_init_qs;

    RK_S32 i_chroma_qp_index_offset;
    RK_S32 i_second_chroma_qp_index_offset;

    RK_S32 b_deblocking_filter_control;
    RK_S32 b_constrained_intra_pred;
    RK_S32 b_redundant_pic_cnt;

    RK_S32 b_transform_8x8_mode;

    RK_S32 b_cqm_preset; //pic_scaling_list_present_flag
    const RK_U8 *scaling_list[8]; /* could be 12, but we don't allow separate Cb/Cr lists */
} H264ePps;

typedef struct H264eVuiParam_t {
    /* they will be reduced to be 0 < x <= 65535 and prime */
    RK_S32         i_sar_height;
    RK_S32         i_sar_width;

    RK_S32         i_overscan;    /* 0=undef, 1=no overscan, 2=overscan */

    /* see h264 annex E for the values of the following */
    RK_S32         i_vidformat;
    RK_S32         b_fullrange;
    RK_S32         i_colorprim;
    RK_S32         i_transfer;
    RK_S32         i_colmatrix;
    RK_S32         i_chroma_loc;    /* both top & bottom */
} H264eVuiParam;

typedef struct H264eSei_t {
    RK_U32                       frame_cnt;
    h264e_control_extra_info_cfg extra_info_cfg;
    MppEncRcCfg                  rc_cfg;
} H264eSei;

typedef struct H264eRefParam_t {
    RK_S32         i_frame_reference;  /* Maximum number of reference frames */
    RK_S32         i_ref_pos;
    RK_S32         i_long_term_en;
    RK_S32         i_long_term_internal;
    RK_S32         hw_longterm_mode;
    RK_S32         i_dpb_size;         /* Force a DPB size larger than that implied by B-frames and reference frames.
                                        * Useful in combination with interactive error resilience. */
} H264eRefParam;

typedef struct H264eHalParam_t {
    RK_S32 hw_type; // 0:rkv, 1:vpu
    RK_S32 constrained_intra;

    H264eVuiParam vui;
    H264eRefParam ref;
} H264eHalParam;

typedef struct H264eMbRcCtx_t {
    /*
     * rc_mode - rate control mode
     * Mpp balances quality and bit rate by the mode index
     * Mpp provide 5 level of balance mode of quality and bit rate
     * 0 - only quality mode: only quality parameter takes effect
     * 1 - more quality mode: quality parameter takes more effect
     * 2 - balance mode     : balance quality and bitrate 50 to 50
     * 3 - more bitrate mode: bitrate parameter takes more effect
     * 4 - only bitrate mode: only bitrate parameter takes effect
     */
    RK_S32 mode;

    /*
     * quality - quality parameter
     * mpp does not give the direct parameter in different protocol.
     * mpp provide total 5 quality level 0 ~ 4
     * 0 - worst
     * 1 - worse
     * 2 - medium
     * 3 - better
     * 4 - best
     */
    RK_S32  quality;
} H264eMbRcCtx;

typedef struct H264eHalContext_t {
    MppHalApi                       api;
    IOInterruptCB                   int_cb;
    MppDevCtx                       dev_ctx;
    h264e_feedback                  feedback;
    void                            *regs;
    void                            *ioctl_input;
    void                            *ioctl_output;
    void                            *buffers;
    void                            *extra_info;
    void                            *dpb_ctx;
    void                            *dump_files;
    RK_U32                          frame_cnt_gen_ready;
    RK_U32                          frame_cnt_send_ready;
    RK_U32                          num_frames_to_send;
    /* @frame_cnt starts from ZERO */
    RK_U32                          frame_cnt;
    H264eHalParam                   param;
    RK_U32                          enc_mode;
    RK_U32                          frame_size;
    RK_U32                          alloc_flg;

    void                            *param_buf;
    MppPacket                       packeted_param;

    RK_S32                          osd_plt_type; //-1:invalid, 0:user define, 1:default
    MppEncOSDData                   osd_data;
    MppEncROICfg                    roi_data;
    MppEncSeiMode                   sei_mode;

    MppEncCfgSet                    *cfg;
    MppEncCfgSet                    *set;
    RK_U32                          idr_pic_id;
    RK_U32                          qp_scale; /* can be 1 or 2 */

    H264eHwCfg                      hw_cfg;
    MppLinReg                       *inter_qs;
    MppLinReg                       *intra_qs;
    MppLinReg                       *mad;
    MppData                         *qp_p;
    MppData                         *sse_p;
    H264eMbRcCtx                    mb_rc;
} H264eHalContext;

MPP_RET h264e_set_sps(H264eHalContext *ctx, H264eSps *sps);
MPP_RET h264e_set_pps(H264eHalContext *ctx, H264ePps *pps, H264eSps *sps);
void h264e_set_param(H264eHalParam *p, RK_S32 hw_type);

const RK_U8 * const h264e_cqm_jvt[8];
const RK_U8 h264e_zigzag_scan4[2][16];
const RK_U8 h264e_zigzag_scan8[2][64];
void h264e_rkv_set_format(H264eHwCfg *hw_cfg, MppEncPrepCfg *prep_cfg);
void h264e_vpu_set_format(H264eHwCfg *hw_cfg, MppEncPrepCfg *prep_cfg);
void h264e_sei_pack2str(char *str, H264eHalContext *ctx, RcSyntax *rc_syn);

#endif
