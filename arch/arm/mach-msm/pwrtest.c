/* linux/arch/arm/mach-msm/pwrtest.c
 *
 *  * Copyright (C) 2009 HTC Corporation.
 * Author:
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/gpio.h>
#include <linux/module.h>
#include "devices.h"
#include "proc_comm.h"
#ifdef CONFIG_MACH_VALENTE_WX
#include "board-valente_wx.h"
#endif

/* gpio info for PCOM_RPC_GPIO_TLMM_CONFIG_EX */
#define GPIO_ENABLE	0
#define GPIO_DISABLE	1

#define GPIO_INPUT	0
#define GPIO_OUTPUT	1

#define GPIO_NO_PULL	0
#define GPIO_PULL_DOWN	1
#define GPIO_KEEPER	2
#define GPIO_PULL_UP	3

#define GPIO_2MA	0
#define GPIO_4MA	1
#define GPIO_6MA	2
#define GPIO_8MA	3
#define GPIO_10MA	4
#define GPIO_12MA	5
#define GPIO_14MA	6
#define GPIO_16MA	7

#define PCOM_GPIO_CFG(gpio, func, dir, pull, drvstr) \
		((((gpio) & 0x3FF) << 4)	| \
		((func) & 0xf)			| \
		(((dir) & 0x1) << 14)		| \
		(((pull) & 0x3) << 15)		| \
		(((drvstr) & 0xF) << 17))
enum CPLD_REG_ID {
	CPLD_REG_MISC2 = 0,
	CPLD_REG_MISC3,
	CPLD_REG_MISC4,
	CPLD_REG_MISC5,
	CPLD_REG_INT2,
	CPLD_REG_MISC1,
	CPLD_REG_INT3,
	CPLD_REG_INT1,
	CPLD_REG_NUM,
};

typedef struct _tagSUSPEND_PIN_CONFIG {
    unsigned char arGpio[200]; /* the maximum should be 1024 bytes */
} SUSPEND_PIN_CONFIG, *PSUSPEND_PIN_CONFIG;

#ifdef CONFIG_MACH_VALENTE_WX
static uint32_t msm_gpio_wa[] = {
        GPIO_CFG(VALENTE_WX_AUD_WCD_SB_DATA, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, 0),
        GPIO_CFG(VALENTE_WX_WCN_CMD_DATA2, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, 0),
        GPIO_CFG(VALENTE_WX_WCN_CMD_DATA0, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, 0),
};
static void msm_set_sleep_gpio_wa(void)
{
	int i;
	for(i = 0;i < 3;i++)
		gpio_tlmm_config(msm_gpio_wa[i], GPIO_CFG_ENABLE);
}
#endif
void gpio_set_diag_gpio_table(unsigned long *dwMFG_gpio_table)
{
	int i = 0;
	unsigned cfg;
	PSUSPEND_PIN_CONFIG pSuspendPinConfig = 0;

	pSuspendPinConfig = (PSUSPEND_PIN_CONFIG)(dwMFG_gpio_table);
#if defined(CONFIG_ARCH_MSM7X00A)
	for (i = 0; i <= 121; i++)
#elif defined(CONFIG_ARCH_MSM7225)
	for (i = 0; i < 132; i++)
#elif defined(CONFIG_ARCH_MSM7227)
	for (i = 0; i < 132; i++)
#elif defined(CONFIG_ARCH_QSD8X50)
	for (i = 0; i <= 164; i++)
#elif defined(CONFIG_ARCH_MSM7X30)
	for (i = 0; i <= 181; i++)
#elif defined(CONFIG_ARCH_MSM8X60)
	for (i = 0; i < 173; i++)
#elif defined(CONFIG_ARCH_MSM8960)
	for (i = 0; i < 151; i++)
#endif
	{
		unsigned char tempGpio = pSuspendPinConfig->arGpio[i];

		if (tempGpio & 0x1) {
			/* configure by the settings from DIAG */
			unsigned long dwGpioKind, dwGpioConfig, dwOutputLevel;
			if (tempGpio & 0x2) { /* GPIO INPUT PIN */
				dwGpioKind = GPIO_INPUT;
				dwOutputLevel = 0;
			} else { /* GPIO_OUTPUT_PIN */
				dwGpioKind = GPIO_OUTPUT;
				if (tempGpio & 0x4)
					dwOutputLevel = 1;
				else
					dwOutputLevel = 0;
			}

			/* config GpioPullStatus */
			if ((tempGpio & 0x10) && (tempGpio & 0x08))
				dwGpioConfig = GPIO_PULL_UP;
			else if (tempGpio & 0x08)
				dwGpioConfig = GPIO_PULL_DOWN;
			else if (tempGpio & 0x10)
				dwGpioConfig = GPIO_KEEPER;
			else
				dwGpioConfig = GPIO_NO_PULL;

#if defined(CONFIG_ARCH_MSM8X60) || defined(CONFIG_ARCH_MSM8960)
			cfg = GPIO_CFG(i, 0, dwGpioKind, dwGpioConfig, GPIO_2MA);
			gpio_tlmm_config(cfg, GPIO_CFG_ENABLE);
#else
			cfg = PCOM_GPIO_CFG(i, 0, dwGpioKind, dwGpioConfig, GPIO_2MA);

			msm_proc_comm(PCOM_RPC_GPIO_TLMM_CONFIG_EX, &cfg, 0);
#endif
			if (dwGpioKind == GPIO_OUTPUT)
				gpio_direction_output(i, dwOutputLevel);
		} else {
			/* DIAG does not want to config this GPIO */
			continue;
		}
	}
#ifdef CONFIG_MACH_VALENTE_WX
	if (board_mfg_mode() == 4)
		msm_set_sleep_gpio_wa();
#endif
}
EXPORT_SYMBOL(gpio_set_diag_gpio_table);
