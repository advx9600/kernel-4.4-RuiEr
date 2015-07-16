#ifndef __WIFI_PM_H__
#define __WIFI_PM_H__

int ap6210_pm_gpio_ctrl(int state);
int ap6210_wifi_on_ctrl(int state);
int get_host_wake_irq(void);
#endif	/* __WIFI_PM_H__ */
