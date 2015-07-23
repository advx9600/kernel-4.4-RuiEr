#define VBP                     (4)
#define LCD_VBPD                VBP
#define VFP                     (8)
#define LCD_VFPD                VFP
#define VPW                     (4)
#define LCD_VSPW                VPW

#define HBP                     (48)
#define LCD_HBPD                HBP
#define HFP                     (16)
#define LCD_HFPD                HFP
#define HPW                     (4)
#define LCD_HSPW                HPW

#define LCD_XSIZE_TFT           (800)
#define LCD_YSIZE_TFT           (1281)
#define LCD_WIDTH               LCD_XSIZE_TFT
#define LCD_HEIGHT              LCD_YSIZE_TFT

// ¿¿¿¿¿¿¿¿SPI¿¿
#define LCD_DSI_CLCK_LOW                (0x4214)
#define LCD_DSI_CLCK_NORMAL             (0x8224)

// ssd2828 reset
static void Set_RST(U32 index)
{
	gpio_direction_output(PIN_SSD2828_RESET,index);
}

static void ssd2828_no_sleep(int index)
{
	gpio_direction_output(PIN_SSD2828_SLEEP,index);
}

void Init_SSD2805_step1(void)
{
	ssd2828_no_sleep(1);

	Set_RST(0);// ( rGPFDAT &= (~(1<<3))) ;
	Delay_ms(20);
	Set_RST(1);//  ( rGPFDAT |= (1<<3) ) ;
	Delay_ms(30);

 SPI_2825_WrCmd(0xb7);
			        SPI_WriteData(0x50);//50=TX_CLK 70=PCLK
			        SPI_WriteData(0x00);   //Configuration Register

			        SPI_2825_WrCmd(0xb8);
			        SPI_WriteData(0x00);
			        SPI_WriteData(0x00);   //VC(Virtual ChannelID) Control Register

			        SPI_2825_WrCmd(0xb9);
			        SPI_WriteData(0x00);//1=PLL disable
			        SPI_WriteData(0x00);
                               //TX_CLK/MS should be between 5Mhz to100Mhz
				SPI_2825_WrReg(0xBA,LCD_DSI_CLCK_LOW);
				
			       // SPI_2825_WrCmd(0xBA);//PLL=(TX_CLK/MS)*NS 8228=480M 4428=240M  061E=120M 4214=240M 821E=360M 8219=300M
			        //SPI_WriteData(0x14);//D7-0=NS(0x01 : NS=1)
			        //SPI_WriteData(0x42);//D15-14=PLL¿¿ 00=62.5-125 01=126-250 10=251-500 11=501-1000  DB12-8=MS(01:MS=1)

			        SPI_2825_WrCmd(0xBB);//LP Clock Divider LP clock = 400MHz / LPD / 8 = 240 / 8 / 4 = 7.5MHz
			        SPI_WriteData(0x03);//D5-0=LPD=0x1 ¿ Divide by 2
			        SPI_WriteData(0x00);

			        SPI_2825_WrCmd(0xb9);
			       	SPI_WriteData(0x01);//1=PLL disable
			       	SPI_WriteData(0x00);
			        //MIPI lane configuration
			            	SPI_2825_WrCmd(0xDE);//¿¿¿
			            	SPI_WriteData(0x03);//11=4LANE 10=3LANE 01=2LANE 00=1LANE
			            	SPI_WriteData(0x00);

			        SPI_2825_WrCmd(0xc9);
			        SPI_WriteData(0x02);
			        SPI_WriteData(0x23);   //p1: HS-Data-zero  p2: HS-Data- prepare  --> 8031 issue
			       // Delay(100);
 
 
//code


 
  
//SSD2825_Initial
SPI_2825_WrCmd(0xb7);
SPI_WriteData(0x50);
SPI_WriteData(0x00);   //Configuration Register

SPI_2825_WrCmd(0xb8);
SPI_WriteData(0x00);
SPI_WriteData(0x00);   //VC(Virtual ChannelID) Control Register

SPI_2825_WrCmd(0xb9);
SPI_WriteData(0x00);//1=PLL disable
SPI_WriteData(0x00);

SPI_2825_WrReg(0xBA,LCD_DSI_CLCK_NORMAL);
//SPI_2825_WrCmd(0xBA);//PLL=(TX_CLK/MS)*NS 8228=480M 4428=240M  061E=120M 4214=240M 821E=360M 8219=300M 8225=444M 8224=432
//SPI_WriteData(0x2b);//D7-0=NS(0x01 : NS=1)   //0X28
//SPI_WriteData(0x82);//D15-14=PLL¿¿ 00=62.5-125 01=126-250 10=251-500 11=501-1000  DB12-8=MS(01:MS=1) //0X82

SPI_2825_WrCmd(0xBB);//LP Clock Divider LP clock = 400MHz / LPD / 8 = 480 / 8/ 8 = 7.5MHz
SPI_WriteData(0x07);//D5-0=LPD=0x1 ¿ Divide by 2
SPI_WriteData(0x00);

SPI_2825_WrCmd(0xb9);
SPI_WriteData(0x01);//1=PLL disable
SPI_WriteData(0x00);

SPI_2825_WrCmd(0xc9);
SPI_WriteData(0x02);
SPI_WriteData(0x23);   //p1: HS-Data-zero  p2: HS-Data- prepare  --> 8031 issue
Delay(5);

SPI_2825_WrCmd(0xCA);
SPI_WriteData(0x01);//CLK Prepare
SPI_WriteData(0x23);//Clk Zero

SPI_2825_WrCmd(0xCB); //local_write_reg(addr=0xCB,data=0x0510)
SPI_WriteData(0x10); //Clk Post
SPI_WriteData(0x05); //Clk Per

SPI_2825_WrCmd(0xCC); //local_write_reg(addr=0xCC,data=0x100A)
SPI_WriteData(0x05); //HS Trail
SPI_WriteData(0x10); //Clk Trail
Delay(5);

SPI_2825_WrCmd(0xD0); //local_write_reg(addr=0xCC,data=0x100A)
SPI_WriteData(0x00); //HS Trail
SPI_WriteData(0x00); //Clk Trail
Delay(5);
}

void Init_SSD2805_step2(void)
{
//#include "orgin_config_2.c"
	
	/**************************************************/
 
	   SPI_2825_WrReg(0xb1,(LCD_VSPW<<8)|LCD_HSPW);	//Vertical sync and horizontal sync active period 
	SPI_2825_WrReg(0xb2,(LCD_VBPD<<8)|LCD_HBPD);	//Vertical and horizontal back porch period  
	SPI_2825_WrReg(0xb3,(LCD_VFPD<<8)|LCD_HFPD);	//Vertical and horizontal front porch period 
	SPI_2825_WrReg(0xb4, LCD_XSIZE_TFT);
	SPI_2825_WrReg(0xb5, LCD_YSIZE_TFT);
	SPI_2825_WrReg(0xb6, 0x0003);				//Video mode and video pixel format 
//MIPI lane configuration
SPI_2825_WrCmd(0xDE);//¿¿¿
SPI_WriteData(0x03);//11=4LANE 10=3LANE 01=2LANE 00=1LANE
SPI_WriteData(0x00);

SPI_2825_WrCmd(0xD6);//  05=BGR  04=RGB
SPI_WriteData(0x05);//D0=0=RGB 1:BGR D1=1=Most significant byte sent first
SPI_WriteData(0x00);

SPI_2825_WrCmd(0xB7);
SPI_WriteData(0x4B); //0X4B
SPI_WriteData(0x02);Delay(100);

SPI_2825_WrCmd(0x2C);
}

