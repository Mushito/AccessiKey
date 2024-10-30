#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

void setup()
{
  Config_Init();
  LCD_Init();
  LCD_Clear(0xffff);
  Paint_NewImage(LCD_HEIGHT, LCD_WIDTH, 90, WHITE);  // Width and height swapped
  Paint_Clear(WHITE);
  Paint_DrawCircle(160, 120, 40, BLACK, 2, 0);  // Swapped x and y values

  Paint_DrawLine(200, 120, 220, 120, BLACK, 2, LINE_STYLE_SOLID);  // Top line
  Paint_DrawLine(120, 120, 100, 120, BLACK, 2, LINE_STYLE_SOLID);  // Bottom line
  Paint_DrawLine(160, 80, 160, 60, BLACK, 2, LINE_STYLE_SOLID);    // Left line
  Paint_DrawLine(160, 160, 160, 180, BLACK, 2, LINE_STYLE_SOLID);  // Right line

  Paint_DrawLine(130, 90, 110, 70, BLACK, 2, LINE_STYLE_SOLID);    // Top-left diagonal
  Paint_DrawLine(130, 150, 110, 170, BLACK, 2, LINE_STYLE_SOLID);  // Top-right diagonal
  Paint_DrawLine(190, 90, 210, 70, BLACK, 2, LINE_STYLE_SOLID);    // Bottom-left diagonal
  Paint_DrawLine(190, 150, 210, 170, BLACK, 2, LINE_STYLE_SOLID);  // Bottom-right diagonal

  Paint_DrawString_EN(210, 120, "A", &Font20, WHITE, BLACK);  // Swapped x and y values
}

void loop()
{
  
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
