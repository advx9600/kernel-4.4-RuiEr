#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h> 
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/gpio.h>

#include <mach/platform.h>
#include <mach/devices.h>
#include <mach/soc.h>
#include <mach/gpio.h>
#include <linux/uaccess.h>
#include <linux/pci.h>

#define DEVICE_NAME "real_led"
static int led_gpios[] = {
	(PAD_GPIO_C + 1),
};
#define LED_NUM		ARRAY_SIZE(led_gpios)

static int __init realarm_led_init(void)
{
	int ret = 0;
	int i = 0;
	printk(DEVICE_NAME"\tinit start\n");
	for(i = 0;i < LED_NUM;i++)
	{
		ret = gpio_request(led_gpios[i], DEVICE_NAME);
		if (ret < 0) {
			printk(KERN_ERR DEVICE_NAME "gpio_%d request fail",
			led_gpios[i]);
			goto fail;
		}
		gpio_direction_output(led_gpios[i], 1);		
	}	
	printk(DEVICE_NAME"\tinitialized\n");
	return ret;
fail:
	gpio_free(led_gpios[i]);
	return ret;
} 
static int __init real4418_led_dev_init(void) {
	int ret;
	realarm_led_init();
	return 0;
}

static void __exit real4418_led_dev_exit(void) {
	
	int i;
	for(i = 0;i < LED_NUM;i++)
	{
		gpio_direction_output(led_gpios[i], 0);
		gpio_free(led_gpios[i]);
	}
	return 0;
}

module_init(real4418_led_dev_init);
module_exit(real4418_led_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kuangrenyu <hzzhangguofeng@gmail.com>");

