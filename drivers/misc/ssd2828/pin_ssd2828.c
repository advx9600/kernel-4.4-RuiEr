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

#include <linux/proc_fs.h>

#define DEVICE_NAME "pin-ssd2828"

#define print(...) printk(DEVICE_NAME":"__VA_ARGS__)

#include "pin_ssd2828.h"

static void Delay_us(U32 us)
{
        udelay(us*50);
}

static void Delay_ms(U32 ms)
{
        Delay_us(ms*1000);
}

static void Delay(U32 us)
{
	Delay_us(us);
}

static void Delay_clk_time(void)
{
        Delay_us(1);
}

static void lcd_reset(void)
{
	gpio_direction_output(PIN_LCD_RESET,1);
	Delay_ms(10);
	gpio_direction_output(PIN_LCD_RESET,0);
	Delay_ms(20);
	gpio_direction_output(PIN_LCD_RESET,1);
	Delay_ms(30);
}

static void Set_2805_CS(U32 index)
{
	gpio_direction_output(SPI_CS,index);
}

static void Set_SCL(U32 index)
{
	gpio_direction_output(SPI_CLK,index);
}

static void Set_SDI(U32 index)
{
	gpio_direction_output(SPI_MOSI,index);
}

static void spi_init(void)
{
	gpio_direction_output(SPI_CLK,0);
	gpio_direction_output(SPI_CS,0);
	gpio_direction_input(SPI_MISO);
	gpio_direction_output(SPI_MOSI,0);

	Set_2805_CS(1);
        Set_SCL(0);
        Set_SDI(0);
}

static void SPI_3W_SET_Cmd(U8 cmd)
{
        U32 kk;

        Set_SDI(0);                     //Set DC=0, for writing to Command register
        Set_SCL(0);
        Delay_clk_time();
        Set_SCL(1);
        Delay_clk_time();


        Set_SCL(0);
        Delay_clk_time();
        for(kk=0;kk<8;kk++)
        {
                if((cmd&0x80)==0x80) Set_SDI(1);
                else         Set_SDI(0);
                Set_SCL(1);
                Delay_clk_time();
                Set_SCL(0);
                Delay_clk_time();
                cmd = cmd<<1;
	}
}

static void SPI_3W_SET_PAs(U8 value)
{
        U32 kk;

        Set_SDI(1);                     //Set DC=1, for writing to Data register
        Set_SCL(0);
        Delay_clk_time();
        Set_SCL(1);
        Delay_clk_time();
//      Delay_us(1);    
        Set_SCL(0);
        Delay_clk_time();
        for(kk=0;kk<8;kk++)
        {
                if((value&0x80)==0x80) Set_SDI(1);
                else         Set_SDI(0);
                Set_SCL(1);
                Delay_clk_time();
                Set_SCL(0);
                Delay_clk_time();
                value = value<<1;
        }
}

static void Write_com(U16 vv)
{
        Set_2805_CS(0);
        SPI_3W_SET_Cmd(vv&0xff);
}

static void SPI_2825_WrCmd(U8 cmd)
{
        U32 kk;

        Set_2805_CS(0);

        Set_SDI(0);                     //Set DC=0, for writing to Command register
        Set_SCL(0);
        Delay_clk_time();
        Set_SCL(1);
        Delay_clk_time();


        Set_SCL(0);
        Delay_clk_time();
        for(kk=0;kk<8;kk++)
        {
                if((cmd&0x80)==0x80) Set_SDI(1);
                else         Set_SDI(0);
		Set_SCL(1);
                Delay_clk_time();
                Set_SCL(0);
                Delay_clk_time();
                cmd = cmd<<1;
        }

        Set_2805_CS(1);
}

static void SPI_2825_WrReg(U8 c,U16 value)
{
        Set_2805_CS(0);
        SPI_3W_SET_Cmd(c);
        SPI_3W_SET_PAs(value&0xff);
        SPI_3W_SET_PAs((value>>8)&0xff);
        Set_2805_CS(1);
}

static U16   SPI_READ(U8 cmd)
{
        U8  rdT;
        U16 reValue=0;
        U32 kk;

        Set_2805_CS(0);

        Set_SDI(0);                     //Set DC=0, for writing to Command register
        Set_SCL(0);
        Delay_clk_time();
        Set_SCL(1);
        Delay_clk_time();


        Set_SCL(0);
        Delay_clk_time();
        for(kk=0;kk<8;kk++)
        {
                if((cmd&0x80)==0x80) Set_SDI(1);
	else         Set_SDI(0);
                Set_SCL(1);
                Delay_clk_time();
                Set_SCL(0);
                Delay_clk_time();
                cmd = cmd<<1;
        }

        Set_SDI(0);                     //Set DC=0, for writing to Command register
        Set_SCL(0);
        Delay_clk_time();
        Set_SCL(1);
        Delay_clk_time();


        cmd = 0xFA;
        Set_SCL(0);
        Delay_clk_time();
        for(kk=0;kk<8;kk++)
        {
                if((cmd&0x80)==0x80) Set_SDI(1);
	 else         Set_SDI(0);
                Set_SCL(1);
                Delay_clk_time();
                Set_SCL(0);
                Delay_clk_time();
                cmd = cmd<<1;
        }

        rdT=0;
        for(kk=0;kk<8;kk++)
        {
                rdT = rdT<<1;
                Set_SCL(1);
                if(gpio_get_value(SPI_MISO)) rdT |= 0x01;
                Delay_clk_time();
                Set_SCL(0);
                Delay_clk_time();
        }
	reValue = rdT;

	rdT=0;
        for(kk=0;kk<8;kk++)
        {
                rdT = rdT<<1;
                Set_SCL(1);
                if(gpio_get_value(SPI_MISO)) rdT |= 0x01;
                Delay_clk_time();
                Set_SCL(0);
                Delay_clk_time();
        }

        reValue += (rdT<<8);

        Set_2805_CS(1);

        return reValue;
}

static U16  SPI_READ_ID(U8 addr)
{
        SPI_2825_WrReg(0xd4, 0x00FA);
        return SPI_READ(addr);
}


static void SPI_WriteData(U8 value)
{
//      printf("-%2x",value);
        Set_2805_CS(0);
        SPI_3W_SET_PAs(value);
        Set_2805_CS(1);
}

static void GP_COMMAD_PA(U16 num)
{

	SPI_2825_WrReg(0xbc, num);
	Write_com(0x00bd);
		SPI_2825_WrReg(0xbe, num);
	Write_com(0x00bf);
	Set_2805_CS(1);
}

static U16 GP_COMMAD_PA_READ(void)
{
        U32 i,count;
        SPI_2825_WrReg(0xB7,0x382);
        SPI_2825_WrReg(0xBC,0x01);
        SPI_2825_WrReg(0xBF,0x0a);

        Set_2805_CS(1);
        Delay_ms(10);
        printf("C6:0x%X\n",SPI_READ_ID(0xC6));
        if ((SPI_READ_ID(0xC6)&0x19) !=0x19){
           printf("read 0xC6 not ready failed C3:0x%X\n",SPI_READ_ID(0xC3));
           return 0xFFFF;
        }

        count = SPI_READ_ID(0xC2);
        printf("readnum:0x%X ",count);
        if (count >100) count =100;
        for (i=0;i<count;i++)
	 {
            printf("data[%d]:0x%X ",i,SPI_READ_ID(0xFF));
        } printf("\n");

        return 0xFFFF;
//      return SPI_READ_ID(cmd);
}

#include "ssd2828.c"

static void write_lcd_register(void)
{
	#include "lcd_bp070wx1.c"
}

static int __init dev_init(void)
{
	int ret =0;
	print (" initialized\n");

	lcd_reset();

	spi_init();

	Init_SSD2805_step1();

	write_lcd_register();

	printf("2828:0x%X\n",SPI_READ_ID(0xb0));
	GP_COMMAD_PA_READ();

	
	Init_SSD2805_step2();
//	ret=misc_register(&misc);
	return ret;
}

static void __exit dev_exit(void)
{
	print(" removed\n");
//	misc_deregister(&misc);
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
