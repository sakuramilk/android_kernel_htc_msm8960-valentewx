/* Copyright (c) 2010-2012, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef VIDC_TYPE_H
#define VIDC_TYPE_H

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/time.h>
#include <linux/dma-mapping.h>
#include <linux/android_pmem.h>

/*HTC_START*/
extern u32 vidc_msg_debug;
/*HTC_END*/
#define VIDC_ENABLE_DBGFS

#define USE_RES_TRACKER
#endif
