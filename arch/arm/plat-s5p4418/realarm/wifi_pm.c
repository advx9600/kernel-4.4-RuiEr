/*
 * esp8089 sdio wifi power management API
 */
#include <linux/device.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>

#include <mach/platform.h>
#include <mach/devices.h>
#include <mach/soc.h>

extern int __gpio_to_irq(unsigned gpio);
extern int gpio_direction_input(unsigned gpio);
int ap6210_pm_gpio_ctrl(int state)
{
    nxp_soc_gpio_set_out_value(CFG_WIFI_POWER_IO, state);
    printk("WiFi Power %s\n", state ? "Open":"Close");
	return 0;
}
EXPORT_SYMBOL_GPL(ap6210_pm_gpio_ctrl);
int ap6210_wifi_on_ctrl(int state)
{
	nxp_soc_gpio_set_out_value(CFG_WIFI_ON_IO, state);
	printk("WiFi On %s\n", state ? "Open":"Close");
	return 0;
}
EXPORT_SYMBOL_GPL(ap6210_wifi_on_ctrl);
int get_host_wake_irq(void)
{
	int  host_oob_irq = 0;
	host_oob_irq = __gpio_to_irq(CFG_WL_HOST_WAKE);
	gpio_direction_input(CFG_WL_HOST_WAKE);
	printk("host_oob_irq :%d\r\n", host_oob_irq);
	return host_oob_irq;
}
EXPORT_SYMBOL_GPL(get_host_wake_irq);
