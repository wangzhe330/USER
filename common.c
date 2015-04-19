/********************  ********************
* File Name          : common.c
* Author             : u-easytech
* Version            : v2.0
* Design By          : CQ_RIC
* Date               : 10/22/2008
* Description        : .Five serials functions transmit with strings and so on

********************************************************************************

*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "common.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction Jump_To_Application;
u32 JumpAddress;
u32 BlockNbr = 0, UserMemoryMask = 0;
bool FlashProtection = FALSE;
extern u32 FlashDestination;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Int2Str
* Description    : Convert an Integer to a string
* Input          : - str: The string
*                  - intnum: The intger to be converted
* Output         : None
* Return         : None
*******************************************************************************/
void Int2Str(u8* str, s32 intnum)
{
  u32 i, Div = 1000000000, j = 0, Status = 0;

  for (i = 0; i < 10; i++)
  {
    str[j++] = (intnum / Div) + 48;

    intnum = intnum % Div;
    Div /= 10;
    if ((str[j-1] == '0') & (Status == 0))
    {
      j = 0;
    }
    else
    {
      Status++;
    }
  }
}

/*******************************************************************************
* Function Name  : Str2Int
* Description    : Convert a string to an integer
* Input 1        : - inputstr: The string to be converted
*                  - intnum: The intger value
* Output         : None
* Return         : 1: Correct
*                  0: Error
*******************************************************************************/
u32 Str2Int(u8 *inputstr, s32 *intnum)
{
  u32 i = 0, res = 0;
  u32 val = 0;

  if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
  {
    if (inputstr[2] == '\0')
    {
      return 0;
    }
    for (i = 2; i < 11; i++)
    {
      if (inputstr[i] == '\0')
      {
        *intnum = val;
        /* return 1; */
        res = 1;
        break;
      }
      if (ISVALIDHEX(inputstr[i]))
      {
        val = (val << 4) + CONVERTHEX(inputstr[i]);
      }
      else
      {
        /* return 0, Invalid input */
        res = 0;
        break;
      }
    }
    /* over 8 digit hex --invalid */
    if (i >= 11)
    {
      res = 0;
    }
  }
  else /* max 10-digit decimal input */
  {
    for (i = 0;i < 11;i++)
    {
      if (inputstr[i] == '\0')
      {
        *intnum = val;
        /* return 1 */
        res = 1;
        break;
      }
      else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
      {
        val = val << 10;
        *intnum = val;
        res = 1;
        break;
      }
      else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
      {
        val = val << 20;
        *intnum = val;
        res = 1;
        break;
      }
      else if (ISVALIDDEC(inputstr[i]))
      {
        val = val * 10 + CONVERTDEC(inputstr[i]);
      }
      else
      {
        /* return 0, Invalid input */
        res = 0;
        break;
      }
    }
    /* Over 10 digit decimal --invalid */
    if (i >= 11)
    {
      res = 0;
    }
  }

  return res;
}

/*******************************************************************************
* Function Name  : GetIntegerInput
* Description    : Get an integer from the HyperTerminal
* Input          : - num: The inetger
* Output         : None
* Return         : 1: Correct
*                  0: Error
*******************************************************************************/
u32 GetIntegerInput(s32 * num)
{
  u8 inputstr[16];

  while (1)
  {
    GetInputString(inputstr);
    if (inputstr[0] == '\0') continue;
    if ((inputstr[0] == 'a' || inputstr[0] == 'A') && inputstr[1] == '\0')
    {
      Serial1PutString("User Cancelled \r\n");
      return 0;
    }

    if (Str2Int(inputstr, num) == 0)
    {
      Serial1PutString("Error, Input again: \r\n");
    }
    else
    {
      return 1;
    }
  }
}

/*******************************************************************************
* Function Name  : SerialGetChar
* Description    : Test to see if a key has been pressed on the HyperTerminal
* Input          : - key: The key pressed
* Output         : None
* Return         : 1: Correct
*                  0: Error
*******************************************************************************/
u32 SerialKeyPressed(u8 *key)
{

  if ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
  {
    *key = (u8)USART1->DR;
    return 1;
  }
  else
  {
    return 0;
  }
}

/*******************************************************************************
* Function Name  : Serial1GetChar
* Description    : Test to see if a key has been pressed on the HyperTerminal
* Input          : - key: The key pressed
* Output         : None
* Return         : 1: Correct
*                  0: Error
*******************************************************************************/
u8 Serial1GetChar(void)
{

  if ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
  {
   return ( (u8)USART1->DR);
    
  }
  return 0;
  
}

u8 Serial2GetChar(void)
{

  if ( USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)
  {
    return (u8)USART2->DR;
    
  }
  return 0;
 
}
u8 Serial3GetChar(void)
{

  if ( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET)
  {
    return (u8)USART3->DR;
    
  }
  return 0;
  
}
u8 Serial4GetChar(void)
{

  if ( USART_GetFlagStatus(UART4, USART_FLAG_RXNE) != RESET)
  {
    return(u8)UART4->DR;
    
  }
  return 0;
 
}
u8 Serial5GetChar(void)
{

  if ( USART_GetFlagStatus(UART5, USART_FLAG_RXNE) != RESET)
  {
    return (u8)UART5->DR;
    
  }
  return 0 ;
  
}


/*******************************************************************************
* Function Name  : GetKey
* Description    : Get a key from the HyperTerminal
* Input          : None
* Output         : None
* Return         : The Key Pressed
*******************************************************************************/
u8 GetKey(void)
{
  u8 key = 0;

  /* Waiting for user input */
  while (1)
  {
    if (SerialKeyPressed((u8*)&key)) break;
  }
  return key;

}

/*******************************************************************************
* Function Name  : SerialPutChar
* Description    : Print a character on the HyperTerminal
* Input          : - c: The character to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void Serial1PutChar(u8 c)
{
  USART_SendData(USART1, c);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
/*******************************************************************************
* Function Name  : SerialPutChar
* Description    : Print a character on the HyperTerminal
* Input          : - c: The character to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void Serial2PutChar(u8 c)
{
  USART_SendData(USART2, c);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

 /*******************************************************************************
* Function Name  : SerialPutChar
* Description    : Print a character on the HyperTerminal
* Input          : - c: The character to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void Serial3PutChar(u8 c)
{
  USART_SendData(USART3, c);
  while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}
/*******************************************************************************
* Function Name  : SerialPutChar
* Description    : Print a character on the HyperTerminal
* Input          : - c: The character to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void Serial4PutChar(u8 c)
{
  USART_SendData(UART4, c);
  while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
}

/*******************************************************************************
* Function Name  : SerialPutChar
* Description    : Print a character on the HyperTerminal
* Input          : - c: The character to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void Serial5PutChar(u8 c)
{
  USART_SendData(UART5, c);
  while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
}


/*******************************************************************************
* Function Name  : Serial_PutString
* Description    : Print a string on the HyperTerminal
* Input          : - s: The string to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void Serial1_PutString(u8 *s)
{
  while (*s != '\0')
  {
    Serial1PutChar(*s);
    s ++;
  }
}
/*******************************************************************************
* Function Name  : Serial_PutString
* Description    : Print a string on the HyperTerminal
* Input          : - s: The string to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void Serial2_PutString(u8 *s)
{
  while (*s != '\0')
  {
    Serial2PutChar(*s);
    s ++;
  }
}
/*******************************************************************************
* Function Name  : Serial_PutString
* Description    : Print a string on the HyperTerminal
* Input          : - s: The string to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void Serial3_PutString(u8 *s)
{
  while (*s != '\0')
  {
    Serial3PutChar(*s);
    s ++;
  }
}

/*******************************************************************************
* Function Name  : Serial_PutString
* Description    : Print a string on the HyperTerminal
* Input          : - s: The string to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void Serial4_PutString(u8 *s)
{
  while (*s != '\0')
  {
    Serial4PutChar(*s);
    s ++;
  }
}

/*******************************************************************************
* Function Name  : Serial_PutString
* Description    : Print a string on the HyperTerminal
* Input          : - s: The string to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void Serial5_PutString(u8 *s)
{
  while (*s != '\0')
  {
    Serial5PutChar(*s);
    s ++;
  }
}



/*******************************************************************************
* Function Name  : GetInputString
* Description    : Get Input string from the HyperTerminal
* Input          : - buffP: The input string
* Output         : None
* Return         : None
*******************************************************************************/
void GetInputString (u8 * buffP)
{
  u32 bytes_read = 0;
  u8 c = 0;
  do
  {
    c = GetKey();
    if (c == '\r')
      break;
    if (c == '\b') /* Backspace */
    {
      if (bytes_read > 0)
      {
        Serial1PutString("\b \b");
        bytes_read --;
      }
      continue;
    }
    if (bytes_read >= CMD_STRING_SIZE )
    {
      Serial1PutString("Command string size overflow\r\n");
      bytes_read = 0;
      continue;
    }
    if (c >= 0x20 && c <= 0x7E)
    {
      buffP[bytes_read++] = c;
      Serial1PutChar(c);
    }
  }
  while (1);
  Serial1PutString(("\n\r"));
  buffP[bytes_read] = '\0';
}
