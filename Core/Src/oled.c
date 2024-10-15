
#include "OLED.h"
#include "OLED_FONT.h"
#define OLED_ADDRESS 0x7A
/*=========================内部函数============================*/
void _writeCMD(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1, OLED_DEV_ADDRESS, OLED_CMD_ADDRESS, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 0x100);
}

void _writeDATA(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1, OLED_DEV_ADDRESS, OLED_DATA_ADDRESS, I2C_MEMADD_SIZE_8BIT, &data, 1, 0x100);
}

// 设置光标位置
void _setPosition(uint8_t x, uint8_t page)
{
	_writeCMD(0x00 | (x & 0x0F));//得到x的低四位二进制数，并或上0x00，写入
	_writeCMD(0x10 | ((x & 0xF0) >> 4));//x与11110000并右移得到x的高四位二进制数，并或0x10，写入
	_writeCMD(0xB0 | page);//将页位置或上00100，写入
}

// 取数字位，取传入数字的每一位
void _getNumPost(unsigned int num, uint8_t length, uint8_t* array)//取位函数
{
	for(uint8_t i = 0; i < length; i++)
	{
		array[length - i - 1] = num % 10;//将传入数字的取模，获取最低位，并将最低位传给array[0],
		num = num / 10;//接着令数字=数字再除以10，循环继续
	}
}

/*=========================外部函数============================*/
// 初始化
void OLED_Init(void)
{
	uint8_t CMD_Data[]={
		0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40,0xA1, 0xC8, 0xDA,
		0x12, 0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6,0x8D, 0x14,
		0xAF};//启动oled每个需要配置的寄存器地址
	for(uint8_t i = 0; i < 23; i++)
	{
		_writeCMD(CMD_Data[i]);
	}
	OLED_Clear();//oled初始化后，将oled清屏。
}

// 清屏
void OLED_Clear(void)
{
	for(uint8_t i = 0; i < 128; i++)
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			_setPosition(i, j);//将坐标移到oled的每个像素点上。
			_writeDATA(0x00);//将64x128的每个像素点都设置为0。
		}
	}
}

// 显示字符
//charIndex为要显示的字符的索引
void OLED_ShowChar8X16(uint8_t x, uint8_t page, char charIndex)
{
	uint8_t index = charIndex - ' ';//用要显示的字符索引减去空字符，即可得到字符所对应的ascll码。
	_setPosition(x, page);//在oled初始化中采用水平寻址的方式，列数在写完数据后会自动加一
	for(uint8_t i = 0; i < 8; i++)//写前一页8行的数据
	{
		_writeDATA(char8X16[index * 2][i]);//char8X16[]为汉字数组名
	}
	_setPosition(x, page + 1);//写后一页8行的数据
	for(uint8_t i = 0; i < 8; i++)
	{
		_writeDATA(char8X16[index * 2 + 1][i]);
	}
}


//charIndex为要显示的字符的索引
void OLED_ShowChar6X8(uint8_t x, uint8_t page, char charIndex)
{
	uint8_t index = charIndex - ' ';
	_setPosition(x, page);
	for(uint8_t i = 0; i < 6; i++)
	{
		_writeDATA(char6X8[index][i]);
	}
}

// 显示汉字
void OLED_ShowChinese16X16(uint8_t x, uint8_t page, uint8_t chineseIndex)
{
	_setPosition(x, page);
	for(uint8_t i = 0; i < 16; i++)
	{
		_writeDATA(font16X16[chineseIndex * 2][i]);//font16X16[]为汉字数组名
	}
	_setPosition(x, page + 1);
	for(uint8_t i = 0; i < 16; i++)
	{
		_writeDATA(font16X16[chineseIndex * 2 + 1][i]);
	}
}

// 显示字符串
void OLED_ShowString8X16(uint8_t x, uint8_t page, char* string)//string为字符串数组
{
	for(uint8_t index = 0; string[index] != '\0'; index++)//判断是否检测到\0，如果没有则索引++
	{
		OLED_ShowChar8X16(x + index * 8, page, string[index]);
	}
}


void OLED_ShowString6X8(uint8_t x, uint8_t page, char* string)
{
	for(uint8_t index = 0; string[index] != '\0'; index++)
	{
		OLED_ShowChar6X8(x + index * 6, page, string[index]);
	}
}

// 显示图像
void OLED_ShowIMG(uint8_t x, uint8_t page, uint8_t imgWeight, uint8_t imgHeight)//设置图像的宽度与高度
{
	for(uint8_t i = 0; i < imgHeight; i++)
	{
		_setPosition(x, page + i);
		for(uint8_t j = 0; j < imgWeight; j++)
		{
			_writeDATA(image[i * imgWeight + j]);
		}
	}
}

// 显示数字
void OLED_ShowNum8X16(uint8_t x, uint8_t page, unsigned int num, uint8_t length)//传入x，y，显示的数字以及数字的长度
{
	uint8_t numArray[length];//定义length长度的指针，
	_getNumPost(num, length, &numArray[0]);//接着取传入数字的每一位
	for(uint8_t i = 0; i < length; i++)
	{
		OLED_ShowChar8X16(x + i * 8, page, numArray[i] + '0');//再调用ShowChar8X16将数字显示在屏幕上
	}
}


void OLED_ShowNum6X8(uint8_t x, uint8_t page, unsigned int num, uint8_t length)
{
	uint8_t numArray[length];
	_getNumPost(num, length, &numArray[0]);
	for(uint8_t i = 0; i < length; i++)
	{
		OLED_ShowChar6X8(x + i * 6, page, numArray[i] + '0');
	}
}

uint8_t GRAM[8][128];

void OLED_NewFrame()
{
	for(uint8_t i=0;i<8;i++)
	{
		for(uint8_t j=0;j<128;j++)
		{
			GRAM[i][j]=0;
		}
	}
}

void OLED_ShowFrame()
{
	uint8_t sendBuffer[129];
	sendBuffer[0]=0x40;
	for(uint8_t i=0;i<8;i++)
	{
		for(uint8_t j=0;j<128;j++)
		{
			sendBuffer[j+1]=GRAM[i][j];
		}	
		_writeCMD(0xB0+i);
		_writeCMD(0x02);
		_writeCMD(0x10);
		HAL_I2C_Master_Transmit(&hi2c1,OLED_ADDRESS,sendBuffer,sizeof(sendBuffer),HAL_MAX_DELAY);
	}
}

void OLED_SetPixel(uint8_t x,uint8_t y)
{
	if(x>=128||y>=64)  return;
	GRAM[y/8][x]|=0x01<<(y%8);
	
}


/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */

