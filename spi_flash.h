#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f10x.h"
#include "stm32f10x_spi.h"

#define SPI_FLASH_SPI                           SPI1
#define SPI_FLASH_SPI_CLK                       RCC_APB2Periph_SPI1
#define SPI_FLASH_SPI_SCK_PIN                   GPIO_Pin_5                  /* PA.05 */
#define SPI_FLASH_SPI_SCK_GPIO_PORT             GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MISO_PIN                  GPIO_Pin_6                  /* PA.06 */
#define SPI_FLASH_SPI_MISO_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MOSI_PIN                  GPIO_Pin_7                  /* PA.07 */
#define SPI_FLASH_SPI_MOSI_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_FLASH_CS_PIN                        GPIO_Pin_6                  /* PB.06 */
#define SPI_FLASH_CS_GPIO_PORT                  GPIOB                       /* GPIOB */
#define SPI_FLASH_CS_GPIO_CLK                   RCC_APB2Periph_GPIOB

#define SPI_FLASH_HOLD_PIN                        GPIO_Pin_5                  /* PB.05 */
#define SPI_FLASH_HOLD_GPIO_PORT                  GPIOB                       /* GPIOB */
#define SPI_FLASH_HOLD_GPIO_CLK                   RCC_APB2Periph_GPIOB

#define SPI_FLASH_WP_PIN                        GPIO_Pin_7                  /* PB.07 */
#define SPI_FLASH_WP_GPIO_PORT                  GPIOB                       /* GPIOB */
#define SPI_FLASH_WP_GPIO_CLK                   RCC_APB2Periph_GPIOB


#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(SPI_FLASH_CS_GPIO_PORT, SPI_FLASH_CS_PIN)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(SPI_FLASH_CS_GPIO_PORT, SPI_FLASH_CS_PIN)

#define SPI_FLASH_WP_LOW()       GPIO_ResetBits(SPI_FLASH_WP_GPIO_PORT, SPI_FLASH_WP_PIN)
#define SPI_FLASH_WP_HIGH()      GPIO_SetBits(SPI_FLASH_WP_GPIO_PORT, SPI_FLASH_WP_PIN)

#define SPI_FLASH_HOLD_LOW()       GPIO_ResetBits(SPI_FLASH_HOLD_GPIO_PORT, SPI_FLASH_HOLD_PIN)
#define SPI_FLASH_HOLD_HIGH()      GPIO_SetBits(SPI_FLASH_HOLD_GPIO_PORT, SPI_FLASH_HOLD_PIN)




#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
#define  sFLASH_ID              0xEF4015
//#define  sFLASH_ID              0xEF3015

//wz用户定义
extern uint8_t para_Write_Buffer[200]; 
extern uint8_t para_Read_Buffer[200];

extern __IO uint32_t DeviceID ;
extern __IO uint32_t FlashID ;

void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

#endif /* __SPI_FLASH_H */

