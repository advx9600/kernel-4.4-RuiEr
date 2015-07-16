/*
 * (C) Copyright 2009
 * jung hyun kim, Nexell Co, <jhkim@nexell.co.kr>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/delay.h>	/* mdelay */
#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>

/* nexell soc headers */
#include <mach/platform.h>
#include <mach/devices.h>
#include <mach/pm.h>
#include <mach/s5p4418_bus.h>

#if (0)	/* default on */
#define DBGOUT(msg...)		{ printk("cpu: " msg); }
#else
#define DBGOUT(msg...)		do {} while (0)
#endif

#if (CFG_BUS_RECONFIG_ENB == 1)
void nxp_set_bus_config(void)
{
	u32 val;
	u32 num_si, num_mi;
	u32 i_slot;
#if CFG_BUS_RECONFIG_DREXQOS
	/* ------------- DREX QoS -------------- */
	for (i_slot = 0; i_slot < 2; i_slot++)
	{
		val = readl(NX_VA_BASE_REG_DREX + NX_DREX_QOS_OFFSET + (i_slot<<3));
		if (val != g_DrexQoS[i_slot])
			writel( g_DrexQoS[i_slot], (NX_VA_BASE_REG_DREX + NX_DREX_QOS_OFFSET + (i_slot<<3)) );
	}
#endif
#if CFG_BUS_RECONFIG_BOTTOMBUSSI
	/* ------------- Bottom BUS ------------ */
	num_si = readl(NX_VA_BASE_REG_PL301_BOTT + 0xFC0);
	num_mi = readl(NX_VA_BASE_REG_PL301_BOTT + 0xFC4);

	/* Set SI QoS */
	val = readl(NX_BASE_REG_PL301_BOTT_QOS_TRDMARK + 0x20);
	if (val != g_BottomQoSSI[0])
		writel(g_BottomQoSSI[0], (NX_BASE_REG_PL301_BOTT_QOS_TRDMARK + 0x20) );

	val = readl(NX_BASE_REG_PL301_BOTT_QOS_CTRL + 0x20);
	if (val != g_BottomQoSSI[1])
		writel(g_BottomQoSSI[1], (NX_BASE_REG_PL301_BOTT_QOS_CTRL + 0x20) );

	/* Set progamming for AR */
	// Master Interface
	for (i_slot = 0; i_slot < num_mi; i_slot++)
	{
		writel( (0xFF000000 | i_slot),  NX_BASE_REG_PL301_BOTT_AR );
		val = readl(NX_BASE_REG_PL301_BOTT_AR);
		if (val != i_slot)
			writel( (i_slot << SLOT_NUM_POS) | (i_slot << SI_IF_NUM_POS),  NX_BASE_REG_PL301_BOTT_AR );
	}

	// Slave Interface
	for (i_slot = 0; i_slot < num_si; i_slot++)
	{
		writel( (0xFF000000 | i_slot),  (NX_BASE_REG_PL301_BOTT_AR + 0x20) );
		val = readl(NX_BASE_REG_PL301_BOTT_AR + 0x20);
		if (val != g_BottomBusSI[i_slot])
			writel( (i_slot << SLOT_NUM_POS) | (g_BottomBusSI[i_slot] << SI_IF_NUM_POS),  (NX_BASE_REG_PL301_BOTT_AR + 0x20) );
	}

	/* Set progamming for AW */
	// Master Interface
	for (i_slot = 0; i_slot < num_mi; i_slot++)
	{
		writel( (0xFF000000 | i_slot),  NX_BASE_REG_PL301_BOTT_AW );
		val = readl(NX_BASE_REG_PL301_BOTT_AW);
		if (val != i_slot)
			writel( (i_slot << SLOT_NUM_POS) | (i_slot << SI_IF_NUM_POS),  NX_BASE_REG_PL301_BOTT_AW );
	}

	// Slave Interface
	for (i_slot = 0; i_slot < num_si; i_slot++)
	{
		writel( (0xFF000000 | i_slot),  (NX_BASE_REG_PL301_BOTT_AW + 0x20) );
		val = readl(NX_BASE_REG_PL301_BOTT_AW + 0x20);
		if (val != g_BottomBusSI[i_slot])
			writel( (i_slot << SLOT_NUM_POS) | (g_BottomBusSI[i_slot] << SI_IF_NUM_POS),  (NX_BASE_REG_PL301_BOTT_AW + 0x20) );
	}
#endif
#if CFG_BUS_RECONFIG_TOPBUSSI
	/* ------------- Top BUS ------------ */
	num_si = readl(NX_VA_BASE_REG_PL301_TOP + 0xFC0);
	num_mi = readl(NX_VA_BASE_REG_PL301_TOP + 0xFC4);

	/* Set progamming for AR */
	// Master Interface
	for (i_slot = 0; i_slot < num_mi; i_slot++)
	{
		writel( (0xFF000000 | i_slot),  NX_BASE_REG_PL301_TOP_AR );
		val = readl(NX_BASE_REG_PL301_TOP_AR);
		if (val != i_slot)
			writel( (i_slot << SLOT_NUM_POS) | (i_slot << SI_IF_NUM_POS),  NX_BASE_REG_PL301_TOP_AR );
	}

	// Slave Interface
	for (i_slot = 0; i_slot < num_si; i_slot++)
	{
		writel( (0xFF000000 | i_slot),  (NX_BASE_REG_PL301_TOP_AR + 0x20) );
		val = readl(NX_BASE_REG_PL301_TOP_AR + 0x20);
		if (val != g_TopBusSI[i_slot])
			writel( (i_slot << SLOT_NUM_POS) | (g_TopBusSI[i_slot] << SI_IF_NUM_POS),  (NX_BASE_REG_PL301_TOP_AR + 0x20) );
	}

	/* Set progamming for AW */
	// Master Interface
	for (i_slot = 0; i_slot < num_mi; i_slot++)
	{
		writel( (0xFF000000 | i_slot),  NX_BASE_REG_PL301_TOP_AW );
		val = readl(NX_BASE_REG_PL301_TOP_AW);
		if (val != i_slot)
			writel( (i_slot << SLOT_NUM_POS) | (i_slot << SI_IF_NUM_POS),  NX_BASE_REG_PL301_TOP_AW );
	}

	// Slave Interface
	for (i_slot = 0; i_slot < num_si; i_slot++)
	{
		writel( (0xFF000000 | i_slot),  (NX_BASE_REG_PL301_TOP_AW + 0x20) );
		val = readl(NX_BASE_REG_PL301_TOP_AW + 0x20);
		if (val != g_TopBusSI[i_slot])
			writel( (i_slot << SLOT_NUM_POS) | (g_TopBusSI[i_slot] << SI_IF_NUM_POS),  (NX_BASE_REG_PL301_TOP_AW + 0x20) );
	}
#endif
#if CFG_BUS_RECONFIG_DISPBUSSI
	/* ------------- Display BUS ----------- */
	num_si = readl(NX_VA_BASE_REG_PL301_DISP + 0xFC0);
	num_mi = readl(NX_VA_BASE_REG_PL301_DISP + 0xFC4);

	/* Set progamming for AR */
	// Slave Interface
	for (i_slot = 0; i_slot < num_si; i_slot++)
	{
		writel( (0xFF000000 | i_slot),  NX_BASE_REG_PL301_DISP_AR);
		val = readl(NX_BASE_REG_PL301_DISP_AR);
		if (val != g_DispBusSI[i_slot])
			writel( (i_slot << SLOT_NUM_POS) | (g_DispBusSI[i_slot] << SI_IF_NUM_POS),  NX_BASE_REG_PL301_DISP_AR );
	}

	/* Set progamming for AW */
	// Slave Interface
	for (i_slot = 0; i_slot < num_si; i_slot++)
	{
		writel( (0xFF000000 | i_slot),  NX_BASE_REG_PL301_DISP_AW);
		val = readl(NX_BASE_REG_PL301_DISP_AW);
		if (val != g_DispBusSI[i_slot])
			writel( (i_slot << SLOT_NUM_POS) | (g_DispBusSI[i_slot] << SI_IF_NUM_POS),  NX_BASE_REG_PL301_DISP_AW );
	}
#endif
	return;
}
#endif	/* #if (CFG_BUS_RECONFIG_ENB == 1) */

static void cpu_base_init(void)
{
	U32 tie_reg, val;
	int i = 0;

	NX_RSTCON_Initialize();
	NX_RSTCON_SetBaseAddress((U32)IO_ADDRESS(NX_RSTCON_GetPhysicalAddress()));

	NX_TIEOFF_Initialize();
	NX_TIEOFF_SetBaseAddress((U32)IO_ADDRESS(NX_TIEOFF_GetPhysicalAddress()));

	NX_GPIO_Initialize();
	for (i = 0; NX_GPIO_GetNumberOfModule() > i; i++) {
		NX_GPIO_SetBaseAddress(i, (U32)IO_ADDRESS(NX_GPIO_GetPhysicalAddress(i)));
		NX_GPIO_OpenModule(i);
	}

	NX_ALIVE_Initialize();
	NX_ALIVE_SetBaseAddress((U32)IO_ADDRESS(NX_ALIVE_GetPhysicalAddress()));
	NX_ALIVE_OpenModule();

	NX_CLKPWR_Initialize();
	NX_CLKPWR_SetBaseAddress((U32)IO_ADDRESS(NX_CLKPWR_GetPhysicalAddress()));
	NX_CLKPWR_OpenModule();

	NX_ECID_Initialize();
	NX_ECID_SetBaseAddress((U32)IO_ADDRESS(NX_ECID_GetPhysicalAddress()));

	/*
	 * NOTE> ALIVE Power Gate must enable for RTC register access.
	 * 		 must be clear wfi jump address
	 */
	NX_ALIVE_SetWriteEnable(CTRUE);
	__raw_writel(0xFFFFFFFF, SCR_ARM_SECOND_BOOT);

	/*
	 * NOTE> Control for ACP register access.
	 */
	tie_reg = (U32)IO_ADDRESS(NX_TIEOFF_GetPhysicalAddress());

	val = __raw_readl(tie_reg + 0x70) & ~((3 << 30) | (3 << 10));
	writel(val, (tie_reg + 0x70));

	val = __raw_readl(tie_reg + 0x80) & ~(3 << 3);
	writel(val, (tie_reg + 0x80));
}

static void cpu_bus_init(void)
{
	/* MCUS for Static Memory. */
	NX_MCUS_Initialize();
	NX_MCUS_SetBaseAddress((U32)IO_ADDRESS(NX_MCUS_GetPhysicalAddress()));
	NX_MCUS_OpenModule();

	/*
	 * MCU-Static config: Static Bus #0 ~ #1
	 */
	#define STATIC_BUS_CONFIGUTATION( _n_ )								\
	NX_MCUS_SetStaticBUSConfig											\
	( 																	\
		NX_MCUS_SBUSID_STATIC ## _n_, 									\
		CFG_SYS_STATIC ## _n_ ## _BW, 									\
		CFG_SYS_STATIC ## _n_ ## _TACS, 								\
		CFG_SYS_STATIC ## _n_ ## _TCAH, 								\
		CFG_SYS_STATIC ## _n_ ## _TCOS, 								\
		CFG_SYS_STATIC ## _n_ ## _TCOH, 								\
		CFG_SYS_STATIC ## _n_ ## _TACC, 								\
		CFG_SYS_STATIC ## _n_ ## _TSACC,								\
		(NX_MCUS_WAITMODE ) CFG_SYS_STATIC ## _n_ ## _WAITMODE, 		\
		(NX_MCUS_BURSTMODE) CFG_SYS_STATIC ## _n_ ## _RBURST, 			\
		(NX_MCUS_BURSTMODE) CFG_SYS_STATIC ## _n_ ## _WBURST			\
	);

	STATIC_BUS_CONFIGUTATION( 0);
	STATIC_BUS_CONFIGUTATION( 1);
}

/*
 * 	cpu core shutdown/reset
 */
void (*nxp_board_shutdown)(void) = NULL;
void (*nxp_board_reset)(char str, const char *cmd) = NULL;

static unsigned int core_power[][2] = {
	[0] = { TIEOFFINDEX_OF_CORTEXA9MP_TOP_QUADL2C_CLAMPCPU0,
		    TIEOFFINDEX_OF_CORTEXA9MP_TOP_QUADL2C_CPU0PWRDOWN },
	[1] = { TIEOFFINDEX_OF_CORTEXA9MP_TOP_QUADL2C_CLAMPCPU1,
		    TIEOFFINDEX_OF_CORTEXA9MP_TOP_QUADL2C_CPU1PWRDOWN },
	[2] = { TIEOFFINDEX_OF_CORTEXA9MP_TOP_QUADL2C_CLAMPCPU2,
		    TIEOFFINDEX_OF_CORTEXA9MP_TOP_QUADL2C_CPU2PWRDOWN },
	[3] = { TIEOFFINDEX_OF_CORTEXA9MP_TOP_QUADL2C_CLAMPCPU3,
		    TIEOFFINDEX_OF_CORTEXA9MP_TOP_QUADL2C_CPU3PWRDOWN },
};

void nxp_cpu_core_shutdown(int core)
{
	printk(KERN_INFO "cpu.%d shutdown ...\n", core);
	NX_TIEOFF_Set(core_power[core][0], 1);
	NX_TIEOFF_Set(core_power[core][1], 1);
}

void nxp_cpu_shutdown(void)
{
	int cpu, cur = smp_processor_id();

	if (nxp_board_shutdown)
		nxp_board_shutdown();

	for_each_present_cpu(cpu) {
		if (cpu == cur)
			continue;
		nxp_cpu_core_shutdown(cpu);
	}

	printk(KERN_INFO "cpu.%d shutdown ...\n", cur);
	NX_ALIVE_SetWriteEnable(CTRUE);			/* close alive gate */
	NX_ALIVE_SetVDDPWRON(CFALSE, CFALSE);	/* Core power down */
	NX_ALIVE_SetWriteEnable(CFALSE);			/* close alive gate */
	nxp_cpu_core_shutdown(cur);
	halt();
}

void nxp_cpu_reset(char str, const char *cmd)
{
	printk(KERN_INFO "system reset: %s ...\n", cmd);

	if (nxp_board_reset)
		nxp_board_reset(str, cmd);

	__raw_writel((-1UL), SCR_RESET_SIG_RESET);
	if (cmd && !strcmp(cmd, "recovery")) {
		__raw_writel(RECOVERY_SIGNATURE, SCR_RESET_SIG_SET);
		__raw_readl (SCR_RESET_SIG_READ);	/* verify */
		printk("recovery sign [0x%x:0x%x] \n", SCR_RESET_SIG_READ, readl(SCR_RESET_SIG_READ));
	}

	NX_ALIVE_SetWriteEnable(CFALSE);	/* close alive gate */
	NX_CLKPWR_SetSoftwareResetEnable(CTRUE);
	NX_CLKPWR_DoSoftwareReset();
}

/*
 * Notify cpu version
 *
 * /sys/devices/platform/cpu/version
 */
unsigned int cpu_vers_no = -1;

static ssize_t version_show(struct device *pdev,
			struct device_attribute *attr, char *buf)
{
	char *s = buf;
	s += sprintf(s, "%d\n", nxp_cpu_version());
	if (s != buf)
		*(s-1) = '\n';

	return (s - buf);
}

/*
 * Notify cpu GUID
 * HEX value
 * /sys/devices/platform/cpu/guid
 */
static ssize_t guid_show(struct device *pdev,
			struct device_attribute *attr, char *buf)
{
	char *s = buf;
	unsigned long start = jiffies;
	int timeout = 1;
	u32 guid[4];

	while (!NX_ECID_GetKeyReady()) {
		if (time_after(jiffies, start + timeout)) {
			if (NX_ECID_GetKeyReady())
				break;
			printk("Error: %s not key ready for CHIP GUID ...\n", __func__);
			return -ETIMEDOUT;
		}
		cpu_relax();
	}

	NX_ECID_GetGUID((NX_GUID*)guid);
	s += sprintf(s, "%08x:%08x:%08x:%08x\n", guid[0], guid[1], guid[2], guid[3]);
	if (s != buf)
		*(s-1) = '\n';

	return (s - buf);
}

/*
 * Notify cpu UUID
 * HEX value
 * /sys/devices/platform/cpu/guid
 */
static ssize_t uuid_show(struct device *pdev,
			struct device_attribute *attr, char *buf)
{
	char *s = buf;
	unsigned long start = jiffies;
	int timeout = 1;
	u32 euid[4];

	while (!NX_ECID_GetKeyReady()) {
		if (time_after(jiffies, start + timeout)) {
			if (NX_ECID_GetKeyReady())
				break;
			printk("Error: %s not key ready for CHIP UUID ...\n", __func__);
			return -ETIMEDOUT;
		}
		cpu_relax();
	}

	NX_ECID_GetECID(euid);
	s += sprintf(s, "%08x:%08x:%08x:%08x\n", euid[0], euid[1], euid[2], euid[3]);
	if (s != buf)
		*(s-1) = '\n';

	return (s - buf);
}

/*
 * Notify cpu chip name
 * HEX value
 * /sys/devices/platform/cpu/name
 */
static ssize_t name_show(struct device *pdev,
			struct device_attribute *attr, char *buf)
{
	char *s = buf;
	unsigned long start = jiffies;
	int timeout = 1;
	u8 name[64];

	while (!NX_ECID_GetKeyReady()) {
		if (time_after(jiffies, start + timeout)) {
			if (NX_ECID_GetKeyReady())
				break;
			printk("Error: %s not key ready for CHIP NAME ...\n", __func__);
			return -ETIMEDOUT;
		}
		cpu_relax();
	}

	NX_ECID_GetChipName(name);
	s += sprintf(s, "%s\n", name);
	if (s != buf)
		*(s-1) = '\n';

	return (s - buf);
}

/*
 * Notify board mem phsical size
 * HEX value
 * /sys/devices/platform/cpu/mem_size
 */
extern struct meminfo meminfo;
static ssize_t mem_size_show(struct device *pdev,
			struct device_attribute *attr, char *buf)
{
	struct meminfo *mi = &meminfo;
	char *s = buf;
	int len = (int)(mi->bank[0].size/SZ_1M);

	s += sprintf(s, "%d\n", len);	/* unit is Mbyte */
	if (s != buf)
		*(s-1) = '\n';

	return (s - buf);
}

/*
 * Notify board mem clock frequency
 * HEX value
 * /sys/devices/platform/cpu/mem_clock
 */
static ssize_t mem_clock_show(struct device *pdev,
			struct device_attribute *attr, char *buf)
{
	unsigned int mclk = nxp_cpu_clock_hz(5);
	int khz = (int)(mclk/1000);
	char *s = buf;

	s += sprintf(s, "%d\n", khz);	/* unit is Khz */
	if (s != buf)
		*(s-1) = '\n';

	return (s - buf);
}

static struct device_attribute vers_attr = __ATTR(version, 0664, version_show, NULL);
static struct device_attribute guid_attr = __ATTR(guid, 0664, guid_show, NULL);
static struct device_attribute uuid_attr = __ATTR(uuid, 0664, uuid_show, NULL);
static struct device_attribute name_attr = __ATTR(name, 0664, name_show, NULL);
static struct device_attribute mem_size_attr = __ATTR(mem_size, 0664, mem_size_show, NULL);
static struct device_attribute mem_clock_attr = __ATTR(mem_clock, 0664, mem_clock_show, NULL);

static struct attribute *attrs[] = {
	&vers_attr.attr,
	&guid_attr.attr,
	&uuid_attr.attr,
	&name_attr.attr,
	&mem_size_attr.attr,
	&mem_clock_attr.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = (struct attribute **)attrs,
};

static int __init cpu_sys_init(void)
{
	struct kobject *kobj = NULL;
	int ret = 0;

	/* create attribute interface */
	kobj = kobject_create_and_add("cpu", &platform_bus.kobj);
	if (! kobj) {
		printk(KERN_ERR "Fail, create kobject for cpu\n");
		return -ret;
	}

	ret = sysfs_create_group(kobj, &attr_group);
	if (ret) {
		printk(KERN_ERR "Fail, create sysfs group for cpu\n");
		kobject_del(kobj);
		return -ret;
	}
	return ret;
}
module_init(cpu_sys_init);

unsigned int nxp_cpu_version(void)
{
	return cpu_vers_no;
}

/*
 * 	cpu func.
 */
#if defined(CONFIG_PROTOTYPE_RELEASE)
#define	DEBUG_PROTOTYPE		0
#else
#define	DEBUG_PROTOTYPE		1
#endif

void nxp_cpu_base_init(void)
{
	unsigned int  rev, ver = 0;

	cpu_base_init();
	cpu_bus_init();

#ifdef CONFIG_SMP
	//writel(0x0018, __PB_IO_MAP_REGS_VIRT + 0x11080);	// ACP Bus Enable
	writel(0x0000, __PB_IO_MAP_REGS_VIRT + 0x11080);	// ACP Bus Disable
	writel(0xffff, __PB_IO_MAP_MPPR_VIRT + 0x0c);		// SCU
	writel(0x0009, __PB_IO_MAP_MPPR_VIRT + 0x00);		// SCU L2 Spec... Enable.
#endif

#if (CFG_BUS_RECONFIG_ENB == 1)
	nxp_set_bus_config();
#endif

	/* Check version */
	if (-1 != cpu_vers_no)
		return;

	rev = __raw_readl(__PB_IO_MAP_IROM_VIRT + 0x0100);
	switch(rev) {
	case 0xe153000a:	ver = 1; break;
	default:			ver = 0; break;
	}
	cpu_vers_no = ver;
	printk(KERN_INFO "CPU : VERSION = %u (0x%X)\n", cpu_vers_no, rev);
}

