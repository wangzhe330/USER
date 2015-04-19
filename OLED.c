#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "OLED.h"
#include "SysTick.h"

/*
1: VCC3.3
2: VCC3.3
3: EN --*							15V供电使能信号，不接 
4: GND				
5: CS --PE8						片选信号。低有效
6: RES --PE9					复位信号　低有效
7: D/C --PE10					数据与命令控制线。高电平-数据，低电平-命令
8: GND		
9: SCLK	--PE11 SPI1SCK					SCK
10:SDIN	--PE12 SPI1MOSI				MOSI
*/

typedef  unsigned char uchar;
typedef uint16_t uint;

//全局变量声明
//unsigned  char jing[32];		//可以用const修饰
extern unsigned  char ao[32];
extern unsigned  char ke[32];
extern unsigned  char ji[32];
extern unsigned char  Logo[1500];		 
extern unsigned char  Ascii_1[241][5];
extern unsigned char  Ascii_2[108][5];
extern unsigned char  data_4byte[4];




void OLED_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	/* Enable the GPIO_LED Clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);

	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11| GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);

#ifdef 0
	//SPI1初始化
		/* Enable SPI1 and GPIO clocks */
	/*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
			 SPI_FLASH_SPI_MISO_GPIO, SPI_FLASH_SPI_DETECT_GPIO 
			 and SPI_FLASH_SPI_SCK_GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

	/*!< SPI_FLASH_SPI Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	/*!< AFIO Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);


	/*!< Configure OLED_SPI pins: SCK */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*!< Configure OLED_SPI pins: MISO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*!< Configure OLED_SPI pins: MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	OLED_CS_HIGH();
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);
#endif	

}


void Write_Command(unsigned char data)
{
	unsigned char i;
	OLED_CS_LOW();
	OLED_DC_LOW();

	
	//SPI_I2S_SendData( SPI1 , data);
	for (i=0; i<8; i++)
	{
		//SCLK=0;
		OLED_SCK_LOW();
		
		if((data&0x80)>>7)
			OLED_DIO_HIGH();
		else
			OLED_DIO_LOW();
		data = data << 1; 
//		uDelay(1);
		//SCLK=1;
		//Delay_us(1);
		OLED_SCK_HIGH();
		//Delay_us(1);
//		uDelay(1);
	}
	
	OLED_CS_HIGH();
	OLED_DC_HIGH();	
}

void Write_Data(unsigned char data)
{
	unsigned char i;
	OLED_CS_LOW();
	OLED_DC_HIGH();
	
	//SPI_I2S_SendData( SPI1 , data);
	for (i=0; i<8; i++)
	{
		//SCLK=0;
		OLED_SCK_LOW();
		
		if((data&0x80)>>7)
			OLED_DIO_HIGH();
		else
			OLED_DIO_LOW();
		data = data << 1; 
//		uDelay(1);
		//SCLK=1;
		//Delay_us(1);
		OLED_SCK_HIGH();
		//Delay_us(1);
//		uDelay(1);
	}
	
	OLED_CS_HIGH();
	OLED_CS_HIGH();	
}

//OLED基本操作

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Initialization
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED_Init()
{
	unsigned char i;

	//RES=0;
	OLED_RES_LOW();
	for(i=0;i<200;i++)
	{
		Delay_us(200);
	}
	//RES=1;
	OLED_RES_HIGH();

	Set_Display_On_Off(0x00);		// Display Off (0x00/0x01)
	Set_Display_Clock(0x91);		// Set Clock as 135 Frames/Sec
	Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x5F)
	Set_Display_Offset(0x4C);		// Shift Mapping RAM Counter (0x00~0x5F)
	Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x5F)
	Set_Master_Config(0x00);		// Disable Embedded DC/DC Converter (0x00/0x01)
	Set_Remap_Format(0x50);			// Set Column Address 0 Mapped to SEG0
						//     Disable Nibble Remap
						//     Horizontal Address Increment
						//     Scan from COM[N-1] to COM0
						//     Enable COM Split Odd Even
	Set_Current_Range(0x02);		// Set Full Current Range
	Set_Gray_Scale_Table();			// Set Pulse Width for Gray Scale Table
	Set_Contrast_Current(Brightness);	// Set Scale Factor of Segment Output Current Control
	Set_Frame_Frequency(0x51);		// Set Frame Frequency
	Set_Phase_Length(0x55);			// Set Phase 1 as 5 Clocks & Phase 2 as 5 Clocks
	Set_Precharge_Voltage(0x10);		// Set Pre-Charge Voltage Level
	Set_Precharge_Compensation(0x20,0x02);	// Set Pre-Charge Compensation
	Set_VCOMH(0x1C);			// Set High Voltage Level of COM Pin
	Set_VSL(0x0D);				// Set Low Voltage Level of SEG Pin
	Set_Display_Mode(0x00);			// Normal Display Mode (0x00/0x01/0x02/0x03)

	Fill_RAM(0x00);				// Clear Screen

	Set_Display_On_Off(0x01);		// Display On (0x00/0x01)
}



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Column_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x15);			// Set Column Address
	Write_Command(a);			//   Default => 0x00
	Write_Command(b);			//   Default => 0x3F (Total Columns Devided by 2)
}


void Set_Row_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x75);			// Set Row Address
	Write_Command(a);			//   Default => 0x00
	Write_Command(b);			//   Default => 0x4F
}


void Set_Contrast_Current(unsigned char d)
{
	Write_Command(0x81);			// Set Contrast Value
	Write_Command(d);			//   Default => 0x40
}


void Set_Current_Range(unsigned char d)
{
	Write_Command(0x84|d);			// Set Current Range
						//   Default => 0x84
						//     0x84 (0x00) => Quarter Current Range
						//     0x85 (0x01) => Half Current Range
						//     0x86 (0x02) => Full Current Range
}


void Set_Remap_Format(unsigned char d)
{
	Write_Command(0xA0);			// Set Re-Map & Data Format
	Write_Command(d);			//   Default => 0x00
						//     Column Address 0 Mapped to SEG0
						//     Disable Nibble Re-Map
						//     Horizontal Address Increment
						//     Scan from COM0 to COM[N-1]
						//     Disable COM Split Odd Even
}


void Set_Start_Line(unsigned char d)
{
	Write_Command(0xA1);			// Set Display Start Line
	Write_Command(d);			//   Default => 0x00
}


void Set_Display_Offset(unsigned char d)
{
	Write_Command(0xA2);			// Set Display Offset
	Write_Command(d);			//   Default => 0x00
}


void Set_Display_Mode(unsigned char d)
{
	Write_Command(0xA4|d);			// Set Display Mode
						//   Default => 0xA4
						//     0xA4 (0x00) => Normal Display
						//     0xA5 (0x01) => Entire Display On, All Pixels Turn On at GS Level 15
						//     0xA6 (0x02) => Entire Display Off, All Pixels Turn Off
						//     0xA7 (0x03) => Inverse Display
}


void Set_Multiplex_Ratio(unsigned char d)
{
	Write_Command(0xA8);			// Set Multiplex Ratio
	Write_Command(d);			//   Default => 0x5F
}


void Set_Master_Config(unsigned char d)
{
	Write_Command(0xAD);			// Set Master Configuration
	Write_Command(0x02|d);			//   Default => 0x03
						//     0x02 (0x00) => Select External VCC Supply
						//     0x03 (0x01) => Select Internal DC/DC Voltage Converter
}


void Set_Display_On_Off(unsigned char d)
{
	Write_Command(0xAE|d);			// Set Display On/Off
						//   Default => 0xAE
						//     0xAE (0x00) => Display Off
						//     0xAF (0x01) => Display On
}


void Set_Phase_Length(unsigned char d)
{
	Write_Command(0xB1);			// Phase 1 & 2 Period Adjustment
	Write_Command(d);			//   Default => 0x53 (5 Display Clocks [Phase 2] / 3 Display Clocks [Phase 1])
						//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
						//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}


void Set_Frame_Frequency(unsigned char d)
{
	Write_Command(0xB2);			// Set Frame Frequency (Row Period)
	Write_Command(d);			//   Default => 0x25 (37 Display Clocks)
}


void Set_Display_Clock(unsigned char d)
{
	Write_Command(0xB3);			// Display Clock Divider/Osciallator Frequency
	Write_Command(d);			//   Default => 0x41
						//     D[3:0] => Display Clock Divider
						//     D[7:4] => Oscillator Frequency
}


void Set_Precharge_Compensation(unsigned char a, unsigned char b)
{
	Write_Command(0xB4);			// Set Pre-Charge Compensation Level
	Write_Command(b);			//   Default => 0x00 (No Compensation)

	if(a == 0x20)
	{
		Write_Command(0xB0);		// Set Pre-Charge Compensation Enable
		Write_Command(0x08|a);		//   Default => 0x08
						//     0x08 (0x00) => Disable Pre-Charge Compensation
						//     0x28 (0x20) => Enable Pre-Charge Compensation
	}
}


void Set_Precharge_Voltage(unsigned char d)
{
	Write_Command(0xBC);			// Set Pre-Charge Voltage Level
	Write_Command(d);			//   Default => 0x10 (Connect to VCOMH)
}


void Set_VCOMH(unsigned char d)
{
	Write_Command(0xBE);			// Set Output Level High Voltage for COM Signal
	Write_Command(d);			//   Default => 0x1D (0.81*VREF)
}


void Set_VSL(unsigned char d)
{
	Write_Command(0xBF);			// Set Segment Low Voltage Level
	Write_Command(0x02|d);			//   Default => 0x0E
						//     0x02 (0x00) => Keep VSL Pin Floating
						//     0x0E (0x0C) => Connect a Capacitor between VSL Pin & VSS
}


void Set_NOP()
{
	Write_Command(0xE3);			// Command for No Operation
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Graphic Acceleration
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void GA_Option(unsigned char d)
{
	Write_Command(0x23);		// Graphic Acceleration Command Options
	Write_Command(d);			//   Default => 0x01
						//     Enable Fill Rectangle
						//     Disable Wrap around in Horizontal Direction During Copying & Scrolling
						//     Disable Reverse Copy
}


void Draw_Rectangle(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
{
	Write_Command(0x24);			// Draw Rectangle
	Write_Command(a);			//   Column Address of Start
	Write_Command(c);			//   Row Address of Start
	Write_Command(b);			//   Column Address of End (Total Columns Devided by 2)
	Write_Command(d);			//   Row Address of End
	Write_Command(e);			//   Gray Scale Level
	Delay_us(200);
}


void Copy(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f)
{
	Write_Command(0x25);			// Copy
	Write_Command(a);			//   Column Address of Start
	Write_Command(c);			//   Row Address of Start
	Write_Command(b);			//   Column Address of End (Total Columns Devided by 2)
	Write_Command(d);			//   Row Address of End
	Write_Command(e);			//   Column Address of New Start
	Write_Command(f);			//   Row Address of New Start
	Delay_us(200);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//
//    a: Two Pixels Data
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_RAM(unsigned char a)
{
	GA_Option(0x01);
	Draw_Rectangle(0x00,0x3F,0x00,0x5F,a);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 2)
//    c: Row Address of Start
//    d: Row Address of End
//    e: Two Pixels Data
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
{
	GA_Option(0x01);
	Draw_Rectangle(a,b,c,d,e);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Checkboard (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Checkerboard()
{
unsigned char i,j;
	
	Set_Column_Address(0x00,0x3F);
	Set_Row_Address(0x00,0x5F);

	for(i=0;i<40;i++)
	{
		for(j=0;j<64;j++)
		{
			Write_Data(0xF0);
		}
		for(j=0;j<64;j++)
		{
			Write_Data(0x0F);
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Gray Scale Bar (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Grayscale()
{
	//   Level 16 => Column 1~8
		Fill_Block(0x00,0x03,0x00,0x3F,0xFF);

	//   Level 15 => Column 9~16
		Fill_Block(0x04,0x07,0x00,0x3F,0xEE);

	//   Level 14 => Column 17~24
		Fill_Block(0x08,0x0B,0x00,0x3F,0xDD);

	//   Level 13 => Column 25~32
		Fill_Block(0x0C,0x0F,0x00,0x3F,0xCC);

	//   Level 12 => Column 33~40
		Fill_Block(0x10,0x13,0x00,0x3F,0xBB);

	//   Level 11 => Column 41~48
		Fill_Block(0x14,0x17,0x00,0x3F,0xAA);

	//   Level 10 => Column 49~56
		Fill_Block(0x18,0x1B,0x00,0x3F,0x99);

	//   Level 9 => Column 57~64
		Fill_Block(0x1C,0x1F,0x00,0x3F,0x88);

	//   Level 8 => Column 65~72
		Fill_Block(0x20,0x23,0x00,0x3F,0x77);

	//   Level 7 => Column 73~80
		Fill_Block(0x24,0x27,0x00,0x3F,0x66);

	//   Level 6 => Column 81~88
		Fill_Block(0x28,0x2B,0x00,0x3F,0x55);

	//   Level 5 => Column 89~96
		Fill_Block(0x2C,0x2F,0x00,0x3F,0x44);

	//   Level 4 => Column 97~104
		Fill_Block(0x30,0x33,0x00,0x3F,0x33);

	//   Level 3 => Column 105~112
		Fill_Block(0x34,0x37,0x00,0x3F,0x22);

	//   Level 2 => Column 113~120
		Fill_Block(0x38,0x3B,0x00,0x3F,0x11);

	//   Level 1 => Column 121~128
		Fill_Block(0x3C,0x3F,0x00,0x3F,0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Frame (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Frame()
{
	GA_Option(0x00);
	Draw_Rectangle(0x00,0x3F,0x00,0x3F,0xFF);
	Fill_Block(0x00,0x00,0x01,0x3E,0x0F);
	Fill_Block(0x3F,0x3F,0x01,0x3E,0xF0);
}


 

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Character (5x7)
//
//    a: Database
//    b: Ascii
//    c: Start X Address
//    d: Start Y Address
//    k:0，表正常显示，1表示反显
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Font57(unsigned char a, unsigned char b, unsigned char c, unsigned char d,unsigned char k)
{
unsigned char *Src_Pointer;
unsigned char i,Font,MSB,LSB;

	switch(a)
	{
		case 1:
			Src_Pointer=&Ascii_1[(b-1)][i];
			break;
		case 2:
			Src_Pointer=&Ascii_2[(b-1)][i];
			break;
	}

	Set_Remap_Format(0x54);
	Src_Pointer--;

	switch(k)
       {
		case 0x00:
			for(i=0;i<=4;i+=2)
			{
				LSB=*Src_Pointer;
				Src_Pointer++;
				if(i == 4)
				{
					MSB=0x00;
				}
				else
				{
					MSB=*Src_Pointer;
					Src_Pointer++;
				}
		 		Set_Column_Address(c,c);
				Set_Row_Address(d,d+7);
		
				Font=((MSB&0x01)<<4)|(LSB&0x01);
				Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
				Write_Data(Font);
		
				Font=((MSB&0x02)<<3)|((LSB&0x02)>>1);
				Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
				Write_Data(Font);
		
				Font=((MSB&0x04)<<2)|((LSB&0x04)>>2);
				Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
				Write_Data(Font);
		
				Font=((MSB&0x08)<<1)|((LSB&0x08)>>3);
				Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
				Write_Data(Font);
		
				Font=((MSB&0x10)<<3)|((LSB&0x10)>>1);
				Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
				Write_Data(Font);
		
				Font=((MSB&0x20)<<2)|((LSB&0x20)>>2);
				Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
				Write_Data(Font);
		
				Font=((MSB&0x40)<<1)|((LSB&0x40)>>3);
				Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
				Write_Data(Font);
		
				Font=(MSB&0x80)|((LSB&0x80)>>4);
				Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
				Write_Data(Font);
				c++;
			}
			Set_Remap_Format(0x50);
			break;

			case 0x01:
			for(i=0;i<=4;i+=2)
			{
				LSB=*Src_Pointer;
				Src_Pointer++;
				if(i == 4)
				{
					MSB=0x00;
				}
				else
				{
					MSB=*Src_Pointer;
					Src_Pointer++;
				}
		 		Set_Column_Address(c,c);
				Set_Row_Address(d,d+7);
		
				Font=((MSB&0x01)<<4)|(LSB&0x01);
				Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
				Write_Data(~Font);
		
				Font=((MSB&0x02)<<3)|((LSB&0x02)>>1);
				Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
				Write_Data(~Font);
		
				Font=((MSB&0x04)<<2)|((LSB&0x04)>>2);
				Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
				Write_Data(~Font);
		
				Font=((MSB&0x08)<<1)|((LSB&0x08)>>3);
				Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
				Write_Data(~Font);
		
				Font=((MSB&0x10)<<3)|((LSB&0x10)>>1);
				Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
				Write_Data(~Font);
		
				Font=((MSB&0x20)<<2)|((LSB&0x20)>>2);
				Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
				Write_Data(~Font);
		
				Font=((MSB&0x40)<<1)|((LSB&0x40)>>3);
				Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
				Write_Data(~Font);
		
				Font=(MSB&0x80)|((LSB&0x80)>>4);
				Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
				Write_Data(~Font);
				c++;
			}
			Set_Remap_Format(0x50);
			break;
		}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show String
//
//    a: Database
//    b: Start X Address
//    c: Start Y Address
//    k:0，表正常显示，1表示反显
//    * Must write "0" in the end...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_String(unsigned char a, unsigned char *Data_Pointer, unsigned char b, unsigned char c,unsigned char k)
{
unsigned char *Src_Pointer;

	Src_Pointer=Data_Pointer;
	Show_Font57(1,96,b,c,k);			// No-Break Space
						//   Must be written first before the string start...

	while(1)
	{
		Show_Font57(a,*Src_Pointer,b,c,k);
		Src_Pointer++;
		b+=3;
		if(*Src_Pointer == 0) break;
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 2)
//    c: Row Address of Start
//    d: Row Address of End
//    k:0，表正常显示，1表示反显
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d,uchar k)
{
unsigned char *Src_Pointer;
unsigned char i,j;
	
	Src_Pointer=Data_Pointer;
	Set_Column_Address(a,b);
	Set_Row_Address(c,d);

	switch(k)
       {
		case 0x00:
			for(i=0;i<(d-c+1);i++)
			{
				for(j=0;j<(b-a+1);j++)
				{
					Write_Data(*Src_Pointer);
					Src_Pointer++;
				}
			}
			break;
		case 0x01:
			for(i=0;i<(d-c+1);i++)
			{
				for(j=0;j<(b-a+1);j++)
				{
					Write_Data(~(*Src_Pointer));
					Src_Pointer++;
				}
			}
			break;
	}

}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Vertical Scrolling (Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Upward)
//       "0x01" (Downward)
//    b: Set Numbers of Row Scroll per Step
//    c: Set Time Interval between Each Scroll Step
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c)
{
unsigned int i,j;	

	switch(a)
	{
		case 0:
			for(i=0;i<80;i+=b)
			{
				Set_Start_Line(i);
				for(j=0;j<c;j++)
				{
					Delay_us(200);
				}
			}
			break;
		case 1:
			for(i=0;i<80;i+=b)
			{
				Set_Start_Line(80-i);
				for(j=0;j<c;j++)
				{
					Delay_us(200);
				}
			}
			break;
	}
	Set_Start_Line(0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Continuous Horizontal Scrolling (Partial or Full Screen)
//
//    a: Set Numbers of Column Scroll per Step
//    b: Set Numbers of Row to Be Scrolled
//    c: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
//    d: Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Horizontal_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	GA_Option(0x03);
	Write_Command(0x26);			// Horizontal Scroll Setup
	Write_Command(a);
	Write_Command(b);
	Write_Command(c);
	Write_Command(0x2F);			// Activate Scrolling
	Delay_ms(d);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Continuous Horizontal Fade Scrolling (Partial or Full Screen)
//
//    a: Set Numbers of Column Scroll per Step
//    b: Set Numbers of Row to Be Scrolled
//    c: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
//    d: Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	GA_Option(0x01);
	Write_Command(0x26);			// Horizontal Scroll Setup
	Write_Command(a);
	Write_Command(b);
	Write_Command(c);
	Write_Command(0x2F);			// Activate Scrolling
	Delay_ms(d);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Deactivate Scrolling (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Deactivate_Scroll()
{
	Write_Command(0x2E);			// Deactivate Scrolling
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade In (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_In()
{
unsigned int i;	

	Set_Display_On_Off(0x01);
	for(i=0;i<(Brightness+1);i++)
	{
		Set_Contrast_Current(i);
		Delay_us(200);
		Delay_us(200);
		Delay_us(200);
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade Out (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Out()
{
unsigned int i;	

	for(i=(Brightness+1);i>0;i--)
	{
		Set_Contrast_Current(i-1);
		Delay_us(200);
		Delay_us(200);
		Delay_us(200);
	}
	Set_Display_On_Off(0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Sleep Mode
//
//    "0x01" Enter Sleep Mode
//    "0x00" Exit Sleep Mode
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Sleep(unsigned char a)
{
	switch(a)
	{
		case 0:
			Set_Display_On_Off(0x00);
			Set_Display_Mode(0x01);
			break;
		case 1:
			Set_Display_Mode(0x00);
			Set_Display_On_Off(0x01);
			break;
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Connection Test
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Test()
{
unsigned char i;

	//RES=0;
	OLED_RES_LOW();
	for(i=0;i<200;i++)
	{
		Delay_us(200);
	}
	//RES=1;
	OLED_CS_HIGH();

	Set_Display_Mode(0x01);			// Entire Display On Mode (0x00/0x01/0x02/0x03)

	while(1)
	{
		Set_Display_On_Off(0x01);	// Display On (0x00/0x01)
		Delay_ms(2);
		Set_Display_On_Off(0x00);	// Display Off (0x00/0x01)
		Delay_ms(2);
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Gray Scale Table Setting (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Gray_Scale_Table()
{
	Write_Command(0xB8);			// Set Gray Scale Table
	Write_Command(0x01);			//   Gray Scale Level 1
	Write_Command(0x11);			//   Gray Scale Level 3 & 2
	Write_Command(0x22);			//   Gray Scale Level 5 & 4
	Write_Command(0x32);			//   Gray Scale Level 7 & 6
	Write_Command(0x43);			//   Gray Scale Level 9 & 8
	Write_Command(0x54);			//   Gray Scale Level 11 & 10
	Write_Command(0x65);			//   Gray Scale Level 13 & 12
	Write_Command(0x76);			//   Gray Scale Level 15 & 14
}



/**************************************
  数据转换程序：将2位分成1个字节存入显存
  uchar DATA：取模来的字模数据
****************************************/
void con_4_byte(uchar DATA)
{
	uchar data_4byte[4];
	uchar i;
	uchar d,k;
	d=DATA;
	
	for(i=0;i<4;i++)   // 一两位的方式写入  2*4=8位
	{
		k=d&0xc0;     //当i=0时 为D7,D6位 当i=1时 为D5,D4位
		
		/****有4种可能，16级灰度,一个字节数据表示两个像素，一个像素对应一个字节的4位***/
		//wz 去掉了下面的延时20us
		switch(k)
		{
			case 0x00:
			   	data_4byte[i]=0x00;
			   	Delay_us(20);
				break;
			case 0x40:  // 0100,0000
			   	data_4byte[i]=0x0f;
			   	Delay_us(20);
			 	break;	
			case 0x80:  //1000,0000
			   	data_4byte[i]=0xf0;
			   	Delay_us(20);
			 	break;
			case 0xc0:   //1100,0000
			   	data_4byte[i]=0xff;
			   	Delay_us(20);
			 	break;	 
			default:
			 	break;
		}
		d=d<<2;                                //左移两位

		Write_Data(data_4byte[i]);                /* 8 column  a nibble of command is a dot*/
	}

}
/****************************************************
   写入一个16*16汉字
   x:列坐标，1个字节数据表示2列
   y: 行坐标
   a[32]：16点阵字模数据
   k:0，表正常显示，1表示反显
******************************************************/
void hz_1616(uchar x,uchar y,uchar a[32],uchar k)//write chinese word of1616
{
	uint i,j;
	Set_Remap_Format(0x52);
	Set_Column_Address(x,x+7); //设置列地址
	Set_Row_Address(y,y+15); //设置行地址

	switch(k)
    {
		case 0x00:
	  	for(j=0;j<16;j++)
		{
			for (i=0;i<2;i++) /* 2*8 column , a nibble of command is a dot*/
			{
	 		  con_4_byte(a[(j<<1)+i]);
			}
		}
	 	Set_Remap_Format(0x50);
	    break;

	   	case 0x01:
	  	for(j=0;j<16;j++)
		{
 			for (i=0;i<2;i++) /* 2*8 column , a nibble of command is a dot*/
    		{
	 			con_4_byte(~(a[(j<<1)+i]));
    		}
		}
	 	Set_Remap_Format(0x50);
	    break;
	} 
}

void hz_1616_clear(uchar x,uchar y)
{
	uint i,j;
	Set_Remap_Format(0x52);
	Set_Column_Address(x,x+7); //设置列地址
	Set_Row_Address(y,y+15); //设置行地址

  	for(j=0;j<16;j++)
	{
		for (i=0;i<2;i++) /* 2*8 column , a nibble of command is a dot*/
		{
 		  con_4_byte(0x00);
		}
	}
 	Set_Remap_Format(0x50);
}

void area_clear(void)
{
	GA_Option(0x01);
	Draw_Rectangle(0x00,0x3F,0x00,0x5F,0x00);//右边一半屏幕清除掉了
	//Draw_Rectangle(0x00,0x3F,0x00,0x5F,0x00);//全部屏幕清除掉了
}
 
 
 

 /****************************************************
   写入一个16*8 ascii
   x:列坐标，1个字节数据表示2列
   y: 行坐标
   a[16]：16点阵字模数据
   k:0，表正常显示，1表示反显
******************************************************/


void ascii_1608(uchar x,uchar y,uchar a[16],uchar k)
{
    

	uint j;
	Set_Remap_Format(0x52);
	Set_Column_Address(x,x+3); //设置列地址
	Set_Row_Address(y,y+15); //设置行地址

	switch(k)
	{
		case 0x00:
			for(j=0;j<16;j++)
			{
				con_4_byte(a[j]);
			}
			Set_Remap_Format(0x50);
			break;
		case 0x01:
			for(j=0;j<16;j++)
			{
				con_4_byte(~(a[j]));
			}
			Set_Remap_Format(0x50);
			break;
	}
}



 /*
 //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern_mono (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 2)
//    c: Row Address of Start
//    d: Row Address of End
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void Show_Pattern_mono(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
unsigned char *Src_Pointer;
unsigned char i,j;
	
	Src_Pointer=Data_Pointer;
	Set_Remap_Format(0x52);
	Set_Column_Address(a,b);
	Set_Row_Address(c,d);

	for(i=0;i<(d-c+1);i++)
	{
		for(j=0;j<(b-a+1);j++)
		{
			con_4_byte(*Src_Pointer);
			Src_Pointer++;
		}
	}
	Set_Remap_Format(0x50);
} 
  */
	
void Show_dot(unsigned char x, unsigned char y)
{	
	Draw_Rectangle(x,x,y,y,0xFF);
}
 



