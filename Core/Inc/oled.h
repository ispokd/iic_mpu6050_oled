/*
 * OLED.h
 *
 *  Created on: Aug 22, 2024
 *      Author: ������
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "main.h"
extern I2C_HandleTypeDef hi2c1;

#define OLED_DEV_ADDRESS 	0x78
#define OLED_CMD_ADDRESS 	0x00
#define OLED_DATA_ADDRESS	0x40

// �ڲ�����
void _writeCMD(uint8_t cmd);
void _writeDATA(uint8_t data);
void _setPosition(uint8_t x, uint8_t page);
void _getNumPost(unsigned int num, uint8_t length, uint8_t* array);

// �ⲿ����
// ��ʼ��
void OLED_Init(void);
// ����
void OLED_Clear(void);
// ��ʾ�ַ� ռ��8��2ҳ
void OLED_ShowChar8X16(uint8_t x, uint8_t page, char charIndex);
// ��ʾ�ַ� ռ��6��1ҳ
void OLED_ShowChar6X8(uint8_t x, uint8_t page, char charIndex);
// ��ʾ����
void OLED_ShowChinese16X16(uint8_t x, uint8_t page, uint8_t chineseIndex);
// ��ʾ�ַ���
void OLED_ShowString8X16(uint8_t x, uint8_t page, char* string);
void OLED_ShowString6X8(uint8_t x, uint8_t page, char* string);
// ��ʾͼƬ
void OLED_ShowIMG(uint8_t x, uint8_t page, uint8_t imgWeight, uint8_t imgHeight);
// ��ʾ����
void OLED_ShowNum8X16(uint8_t x, uint8_t page, unsigned int num, uint8_t length);
void OLED_ShowNum6X8(uint8_t x, uint8_t page, unsigned int num, uint8_t length);

void OLED_NewFrame();
void OLED_ShowFrame();
void OLED_SetPixel(uint8_t x,uint8_t y);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
//void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
#endif /* INC_OLED_H_ */
