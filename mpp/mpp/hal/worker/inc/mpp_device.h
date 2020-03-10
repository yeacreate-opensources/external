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

#ifndef __MPP_DEVICE_H__
#define __MPP_DEVICE_H__

#include "rk_type.h"
#include "mpp_err.h"

/* define flags for mpp_request */

#define MPP_FLAGS_MULTI_MSG         (0x00000001)
#define MPP_FLAGS_LAST_MSG          (0x00000002)
#define MPP_FLAGS_REG_FD_NO_TRANS   (0x00000004)
#define MPP_FLAGS_SCL_FD_NO_TRANS   (0x00000008)
#define MPP_FLAGS_SECURE_MODE       (0x00010000)

/* mpp service capability description */
typedef enum MppDevCmd_e {
    MPP_DEV_GET_START = 0,
    MPP_DEV_GET_MAX_WIDTH,
    MPP_DEV_GET_MAX_HEIGHT,
    MPP_DEV_GET_MIN_WIDTH,
    MPP_DEV_GET_MIN_HEIGHT,
    MPP_DEV_GET_MMU_STATUS,

    MPP_DEV_SET_START = 0x01000000,
    MPP_DEV_SET_HARD_PLATFORM, // set paltform by user
    MPP_DEV_ENABLE_POSTPROCCESS,

    MPP_DEV_PROP_BUTT,
} MppDevCmd;

enum MPP_DEV_COMMAND_TYPE {
    MPP_CMD_QUERY_BASE              = 0,
    MPP_CMD_PROBE_HW_SUPPORT        = MPP_CMD_QUERY_BASE + 0,
    MPP_CMD_PROBE_IOMMU_STATUS      = MPP_CMD_QUERY_BASE + 1,

    MPP_CMD_INIT_BASE = 0x100,
    MPP_CMD_INIT_CLIENT_TYPE        = MPP_CMD_INIT_BASE + 0,
    MPP_CMD_INIT_DRIVER_DATA        = MPP_CMD_INIT_BASE + 1,
    MPP_CMD_INIT_TRANS_TABLE        = MPP_CMD_INIT_BASE + 2,

    MPP_CMD_SEND_BASE               = 0x200,
    MPP_CMD_SET_REG                 = MPP_CMD_SEND_BASE + 0,
    MPP_CMD_SET_VEPU22_CFG          = MPP_CMD_SEND_BASE + 1,
    MPP_CMD_SET_RKVENC_OSD_PLT      = MPP_CMD_SEND_BASE + 2,
    MPP_CMD_SET_RKVENC_L2_REG       = MPP_CMD_SEND_BASE + 3,
    MPP_CMD_SET_REG_ADDR_OFFSET     = MPP_CMD_SEND_BASE + 4,

    MPP_CMD_POLL_BASE               = 0x300,
    MPP_CMD_GET_REG                 = MPP_CMD_POLL_BASE + 0,

    MPP_CMD_CONTROL_BASE            = 0x400,
    MPP_CMD_RESET_SESSION           = MPP_CMD_CONTROL_BASE + 0,
    MPP_CMD_TRANS_FD_TO_IOVA        = MPP_CMD_CONTROL_BASE + 1,

    MPP_CMD_BUTT,
};

typedef struct MppDevCfg_t {
    // input
    MppCtxType      type;
    MppCodingType   coding;
    RK_U32          platform;
    RK_U32          pp_enable;
} MppDevCfg;

typedef void*   MppDevCtx;

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * hardware device open function
 * coding and type for device name detection
 */
MPP_RET mpp_device_init(MppDevCtx *ctx, MppDevCfg *cfg);
MPP_RET mpp_device_deinit(MppDevCtx ctx);

/*
 * control function for set or get device property
 */
RK_S32 mpp_device_control(MppDevCtx ctx, MppDevCmd cmd, void *param);

/*
 * register access interface
 */
MPP_RET mpp_device_send_reg(MppDevCtx ctx, RK_U32 *regs, RK_U32 nregs);
MPP_RET mpp_device_wait_reg(MppDevCtx ctx, RK_U32 *regs, RK_U32 nregs);
MPP_RET mpp_device_send_reg_with_id(MppDevCtx ctx, RK_S32 id, void *param, RK_S32 size);

#ifdef __cplusplus
}
#endif

#endif /* __MPP_DEVICE_H__ */
