/*
 * (C) Copyright 2009
 * jung hyun kim, Nexell Co, <jhkim@nexell.co.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/delay.h>
#include <mach/platform.h>

#ifdef CONFIG_ARM_NXP_CPUFREQ_DEBUG
#define	DBGOUT(msg...)		printk(msg)
#else
#define	DBGOUT(msg...)
#endif

struct pll_pms {
	long rate;	/* unint Khz */
	int	P;
	int	M;
	int	S;
};

// PLL 0,1
static struct pll_pms pll0_1_pms [] =
{
    [ 0] = { .rate = 2000000, .P = 3, .M = 4000, .S = 4, },
    [ 1] = { .rate = 1900000, .P = 3, .M = 3800, .S = 4, },
    [ 2] = { .rate = 1800000, .P = 3, .M = 3600, .S = 4, },
    [ 3] = { .rate = 1700000, .P = 3, .M = 3400, .S = 4, },
    [ 4] = { .rate = 1600000, .P = 3, .M =  400, .S = 1, },
    [ 5] = { .rate = 1500000, .P = 3, .M =  375, .S = 1, },
    [ 6] = { .rate = 1400000, .P = 3, .M =  350, .S = 1, },
    [ 7] = { .rate = 1300000, .P = 3, .M =  325, .S = 1, },
    [ 8] = { .rate = 1200000, .P = 6, .M =  600, .S = 1, },
    [ 9] = { .rate = 1100000, .P = 6, .M =  550, .S = 1, },
    [10] = { .rate = 1000000, .P = 6, .M =  500, .S = 1, },
    [11] = { .rate =  900000, .P = 6, .M =  450, .S = 1, },
    [12] = { .rate =  800000, .P = 6, .M =  400, .S = 1, },
    [13] = { .rate =  700000, .P = 6, .M =  350, .S = 1, },
    [14] = { .rate =  666000, .P = 6, .M =  333, .S = 1, },
    [15] = { .rate =  600000, .P = 6, .M =  600, .S = 2, },
    [16] = { .rate =  533000, .P = 6, .M =  533, .S = 2, },
    [17] = { .rate =  500000, .P = 6, .M =  500, .S = 2, },
    [18] = { .rate =  400000, .P = 6, .M =  400, .S = 2, },
    [19] = { .rate =  333000, .P = 6, .M =  333, .S = 2, },
    [20] = { .rate =  300000, .P = 6, .M =  600, .S = 3, },
    [21] = { .rate =  266000, .P = 6, .M =  532, .S = 3, },
    [22] = { .rate =  200000, .P = 6, .M =  400, .S = 3, },
    [23] = { .rate =  166000, .P = 6, .M =  332, .S = 3, },
    [24] = { .rate =  147500, .P = 6, .M =  590, .S = 4, },  // 147.456000
    [25] = { .rate =  133000, .P = 6, .M =  532, .S = 4, },
    [26] = { .rate =  100000, .P = 6, .M =  400, .S = 4, },
};

// PLL 2,3
static struct pll_pms pll2_3_pms [] =
{
    [ 0] = { .rate = 2000000, .P = 3, .M = 500, .S = 1, },
    [ 1] = { .rate = 1900000, .P = 3, .M = 475, .S = 1, },
    [ 2] = { .rate = 1800000, .P = 3, .M = 450, .S = 1, },
    [ 3] = { .rate = 1700000, .P = 3, .M = 425, .S = 1, },
    [ 4] = { .rate = 1600000, .P = 3, .M = 400, .S = 1, },
    [ 5] = { .rate = 1500000, .P = 3, .M = 375, .S = 1, },
    [ 6] = { .rate = 1400000, .P = 3, .M = 350, .S = 1, },
    [ 7] = { .rate = 1300000, .P = 3, .M = 325, .S = 1, },
    [ 8] = { .rate = 1200000, .P = 3, .M = 300, .S = 1, },
    [ 9] = { .rate = 1100000, .P = 3, .M = 275, .S = 1, },
    [10] = { .rate = 1000000, .P = 3, .M = 250, .S = 1, },
    [11] = { .rate =  900000, .P = 3, .M = 225, .S = 1, },
    [12] = { .rate =  800000, .P = 3, .M = 200, .S = 1, },
    [13] = { .rate =  700000, .P = 3, .M = 175, .S = 1, },
    [14] = { .rate =  666000, .P = 4, .M = 222, .S = 1, },
    [15] = { .rate =  600000, .P = 3, .M = 150, .S = 1, },
    [16] = { .rate =  533000, .P = 4, .M = 355, .S = 2, },
    [17] = { .rate =  500000, .P = 3, .M = 250, .S = 2, },
    [18] = { .rate =  400000, .P = 3, .M = 200, .S = 2, },
    [19] = { .rate =  333000, .P = 4, .M = 222, .S = 2, },
    [20] = { .rate =  300000, .P = 3, .M = 150, .S = 2, },
    [21] = { .rate =  266000, .P = 3, .M = 266, .S = 3, },
    [22] = { .rate =  200000, .P = 3, .M = 200, .S = 3, },
    [23] = { .rate =  166000, .P = 3, .M = 166, .S = 3, },
    [24] = { .rate =  147500, .P = 4, .M = 197, .S = 3, },  // 147.456000
    [25] = { .rate =  133000, .P = 3, .M = 266, .S = 4, },
    [26] = { .rate =  100000, .P = 3, .M = 200, .S = 4, },
};

#define	PLL0_1_SIZE		ARRAY_SIZE(pll0_1_pms)
#define	PLL2_3_SIZE		ARRAY_SIZE(pll2_3_pms)

#define	PMS_RATE(p, i)	((&p[i])->rate)
#define	PMS_P(p, i)		((&p[i])->P)
#define	PMS_M(p, i)		((&p[i])->M)
#define	PMS_S(p, i)		((&p[i])->S)

#define PLL_S_BITPOS	0
#define PLL_M_BITPOS    8
#define PLL_P_BITPOS    18

static void core_pll_change(int PLL, int P, int M, int S)
{
	struct NX_CLKPWR_RegisterSet *clkpwr =
	(struct NX_CLKPWR_RegisterSet*)IO_ADDRESS(PHY_BASEADDR_CLKPWR_MODULE);

	// 1. change PLL0 clock to Oscillator Clock
	clkpwr->PLLSETREG[PLL] &= ~(1 << 28); 	// pll bypass on, xtal clock use
	clkpwr->CLKMODEREG0 = (1 << PLL); 		// update pll

	while(clkpwr->CLKMODEREG0 & (1<<31)); 		// wait for change update pll

	// 2. PLL Power Down & PMS value setting
	clkpwr->PLLSETREG[PLL] =((1UL << 29)			| // power down
							 (0UL << 28)    		| // clock bypass on, xtal clock use
							 (S   << PLL_S_BITPOS) 	|
							 (M   << PLL_M_BITPOS) 	|
							 (P   << PLL_P_BITPOS));

	clkpwr->CLKMODEREG0 = (1 << PLL); 				// update pll

	while(clkpwr->CLKMODEREG0 & (1<<31)); 			// wait for change update pll

	udelay(1);

	// 3. Update PLL & wait PLL locking
	clkpwr->PLLSETREG[PLL] &= ~((U32)(1UL<<29)); // pll power up

	clkpwr->CLKMODEREG0 = (1 << PLL); 			// update pll

	while(clkpwr->CLKMODEREG0 & (1<<31)); 		// wait for change update pll

	udelay(10);	// 1000us

	// 4. Change to PLL clock
	clkpwr->PLLSETREG[PLL] |= (1<<28); 			// pll bypass off, pll clock use
	clkpwr->CLKMODEREG0 = (1<<PLL); 				// update pll

	while(clkpwr->CLKMODEREG0 & (1<<31)); 				// wait for change update pll
}

static DEFINE_SPINLOCK(_pll_lock);
static unsigned long lock_flags;

static inline void core_pll_change_lock(bool lock)
{
	if (lock) {
		preempt_disable();
		spin_lock_irqsave(&_pll_lock, lock_flags);
	} else {
		spin_unlock_irqrestore(&_pll_lock, lock_flags);
		preempt_enable();
	}
}

unsigned long nxp_cpu_pll_round_frequency(int pllno, unsigned long rate, int *p, int *m, int *s)
{
	struct pll_pms *pms;
	int len, i = 0, n = 0, l = 0;
	long freq = 0;

	rate /= 1000;
	DBGOUT("PLL.%d, %ld", pllno, rate);

	switch (pllno) {
	case 0 :
	case 1 : pms = pll0_1_pms; len = PLL0_1_SIZE; break;
	case 2 :
	case 3 : pms = pll2_3_pms; len = PLL2_3_SIZE; break;
	default: printk(KERN_ERR "Not support pll.%d (0~3)\n", pllno);
		return 0;
	}

	i = len/2;

	while (1) {
		l = n + i;
		freq = PMS_RATE(pms, l);
		if (freq == rate)
			break;

		if (rate > freq)
			len -= i, i >>= 1;
		else
			n += i, i = (len-n-1)>>1;

		if (0 == i) {
			int k = l;
			if (abs(rate - freq) > abs(rate - PMS_RATE(pms, k+1)))
				k += 1;
			if (abs(rate - PMS_RATE(pms, k)) >= abs(rate - PMS_RATE(pms, k-1)))
				k -= 1;
			l = k;
			break;
		}
	}

	if (p) *p = PMS_P(pms, l);
	if (m) *m = PMS_M(pms, l);
	if (s) *s = PMS_S(pms, l);

	DBGOUT("(real %ld Khz, P=%d ,M=%3d, S=%d)\n",
		PMS_RATE(pms, l), PMS_P(pms, l), PMS_M(pms, l), PMS_S(pms, l));

	return PMS_RATE(pms, l);
}

unsigned long nxp_cpu_pll_change_frequency(int pllno, unsigned long rate)
{
	int p = 0, m = 0, s = 0;
	unsigned long freq;

#ifdef CONFIG_NXP_DFS_BCLK
    mdelay(1);
#endif

	freq = nxp_cpu_pll_round_frequency(pllno, rate, &p, &m, &s);

    preempt_disable();
    local_irq_disable();

	core_pll_change(pllno, p, m, s);

    preempt_enable();
    local_irq_enable();

#ifdef CONFIG_NXP_DFS_BCLK
    mdelay(1);
#endif

	return freq;
}
