#ifndef _OLED_H_ 
#define _OLED_H_

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

#define OLED_CS_LOW() GPIO_ResetBits(GPIOE, GPIO_Pin_8)
#define OLED_CS_HIGH() GPIO_SetBits(GPIOE, GPIO_Pin_8)

#define OLED_RES_LOW() GPIO_ResetBits(GPIOE, GPIO_Pin_9)
#define OLED_RES_HIGH() GPIO_SetBits(GPIOE, GPIO_Pin_9)

#define OLED_DC_LOW() GPIO_ResetBits(GPIOE, GPIO_Pin_10)
#define OLED_DC_HIGH() GPIO_SetBits(GPIOE, GPIO_Pin_10)

#define OLED_SCK_LOW() GPIO_ResetBits(GPIOE, GPIO_Pin_11)
#define OLED_SCK_HIGH() GPIO_SetBits(GPIOE, GPIO_Pin_11)

#define OLED_DIO_LOW() GPIO_ResetBits(GPIOE, GPIO_Pin_12)
#define OLED_DIO_HIGH() GPIO_SetBits(GPIOE, GPIO_Pin_12)

#define Dummy_Byte                0xFF

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define XLevelL		0x02
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)

#define Max_Column	0x3F			// 128/2-1 (Total Columns Devided by 2)
#define Max_Row		0x3F			// 64-1
#define	Brightness	0xfF

//全局变量声明
extern unsigned  char fang[32];
extern unsigned  char jia[32];
extern unsigned  char xiao[32];
extern unsigned  char xiao1[16];
extern unsigned  char xiao2[16];
extern unsigned  char xiao3[16];
extern unsigned  char jing[32];
extern unsigned  char ao[32];
extern unsigned  char ke[32];
extern unsigned  char jiji[32];
extern unsigned char  Logo[1500];		 
extern unsigned char  Ascii_1[241][5];
extern unsigned char  Ascii_2[108][5];
extern unsigned char data_4byte[4];
extern unsigned char lcdsoc_p05_g16[195];




//阀门要用的字
extern  unsigned  char can[32]; 
extern  unsigned  char shu[32]; 
extern  unsigned  char she[32]; 
extern  unsigned  char zhi[32]; 
//就(0) 地(1)
extern  unsigned  char jiu[32];
extern  unsigned  char di[32];
//脉(0) 冲(1) 式(2)
extern  unsigned  char mai[32];
extern  unsigned  char chong[32];
extern  unsigned  char shi[32];

// 浏(2) 览(3) 基(4) 本(5) 控(6) 制(7) 故(8) 障(9) 其(10) 他(11)
extern unsigned  char liu[32];
extern unsigned  char lan[32];
extern unsigned  char ji[32];
extern unsigned  char ben[32];
extern unsigned  char kong[32];
extern unsigned  char zhi2[32];
extern unsigned  char gu[32];
extern unsigned  char zhang[32];
extern unsigned  char qi[32];
extern unsigned  char ta[32];
extern unsigned char chinese[168][32];
extern unsigned  char english[43][16];

extern unsigned char op[32];
extern unsigned char opin[32];


void OLED_GPIO_Init(void);
void Write_Command(unsigned char Data);
void Write_Data(unsigned char Data);

//oled初始化函数
void OLED_Init(void);
void Set_Start_Column(unsigned char d);
void Set_Addressing_Mode(unsigned char d);
void Set_Column_Address(unsigned char a, unsigned char b);
void Set_Page_Address(unsigned char a, unsigned char b);
void Set_Start_Line(unsigned char d);
void Set_Contrast_Control(unsigned char d);
void Set_Area_Brightness(unsigned char d);
void Set_Segment_Remap(unsigned char d);
void Set_Entire_Display(unsigned char d);
void Set_Inverse_Display(unsigned char d);
void Set_Multiplex_Ratio(unsigned char d);
void Set_Dim_Mode(unsigned char a, unsigned char b);
void Set_Master_Config(unsigned char d);
void Set_Display_On_Off(unsigned char d);
void Set_Start_Page(unsigned char d);
void Set_Common_Remap(unsigned char d);
void Set_Display_Offset(unsigned char d);
void Set_Display_Clock(unsigned char d);
void Set_Area_Color(unsigned char d);
void Set_Precharge_Period(unsigned char d);
void Set_Common_Config(unsigned char d);
void Set_VCOMH(unsigned char d);
void Set_Read_Modify_Write(unsigned char d);
void Set_NOP(void);

//显示命令函数
void Fill_RAM(unsigned char Data);
void Fill_Block(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Checkerboard(void);
void Frame(void);
void Show_Font57(unsigned char a, unsigned char b, unsigned char c, unsigned char d,unsigned char k);
void Show_String(unsigned char a, unsigned char *Data_Pointer, unsigned char b, unsigned char c,unsigned char k);
void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d,unsigned char k);
//void Continuous_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h, unsigned char i);
void Deactivate_Scroll(void);
void Fade_In(void);
void Fade_Out(void);
void Sleep(unsigned char a);
//void Test();
//void Set_LUT(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
//void Set_Bank_Color();
 //1325
void GA_Option(unsigned char d);
void Set_Remap_Format(unsigned char d);
void Set_Current_Range(unsigned char d);
void Set_Gray_Scale_Table(void);
void Set_Contrast_Current(unsigned char d);
void Set_Frame_Frequency(unsigned char d);
void Set_Phase_Length(unsigned char d);
void Set_Precharge_Voltage(unsigned char d);
void Set_Precharge_Compensation(unsigned char a, unsigned char b);
void Set_VSL(unsigned char d);
void Set_Display_Mode(unsigned char d);
void Set_Row_Address(unsigned char a, unsigned char b);
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c);
void Horizontal_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Fade_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Grayscale(void);
void Draw_Rectangle(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e);//
//void Show_Pattern_mono(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Copy(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f);	//显存区域复制

void con_4_byte(unsigned char DATA);								 //字节转换程序
void hz_1616(unsigned char x,unsigned char y,unsigned char *Data_Pointer,unsigned char k);	//16*16汉字
void Show_dot(unsigned char x, unsigned char y);		   //画点
void ascii_1608(unsigned char x,unsigned char y,unsigned char a[16],unsigned char k);	  //16*8ascii
void hz_1616_clear(unsigned char x,unsigned char y);
void area_clear(void);

#endif

