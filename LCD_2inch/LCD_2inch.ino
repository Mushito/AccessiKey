#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

void setup()
{
  Config_Init();
  LCD_Init();
  LCD_Clear(0xffff);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
  Paint_Clear(WHITE);

  // Apply horizontal mirroring to correct the flipped letters
  Paint_SetMirroring(MIRROR_HORIZONTAL);

  Paint_DrawCircle(160, 120, 40, BLACK, 2, 0);

  // Vertical and horizontal lines
  Paint_DrawLine(160, 160, 160, 180, BLACK, 2, LINE_STYLE_SOLID);
  Paint_DrawLine(160, 80, 160, 60, BLACK, 2, LINE_STYLE_SOLID);    
  Paint_DrawLine(200, 120, 220, 120, BLACK, 2, LINE_STYLE_SOLID);
  Paint_DrawLine(120, 120, 100, 120, BLACK, 2, LINE_STYLE_SOLID);

  // Diagonal lines
  Paint_DrawLine(130, 150, 110, 170, BLACK, 2, LINE_STYLE_SOLID);
  Paint_DrawLine(130, 90, 110, 70, BLACK, 2, LINE_STYLE_SOLID);
  Paint_DrawLine(190, 150, 210, 170, BLACK, 2, LINE_STYLE_SOLID);
  Paint_DrawLine(190, 90, 210, 70, BLACK, 2, LINE_STYLE_SOLID);

  // Add letters A-G to lines
  Paint_DrawString_EN(140, 190, "BASE", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(155, 40, "D", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(225, 115, "F", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(75, 115, "B", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(100, 170, "A", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(95, 55, "C", &Font20, WHITE, BLACK); 
  Paint_DrawString_EN(210, 170, "G", &Font20, WHITE, BLACK);       
  Paint_DrawString_EN(210, 60, "E", &Font20, WHITE, BLACK);        
}

void loop()
{
  
}
