#include <Keyboard.h>
#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

const int threshold = 30;
const unsigned long debounceDelay = 200;
unsigned long lastInputTime = 0;

enum Direction {
  CENTER,
  UP,
  UP_RIGHT,
  RIGHT,
  DOWN_RIGHT,
  DOWN,
  DOWN_LEFT,
  LEFT,
  UP_LEFT
};

enum Layer {
  BASE,
  LAYER_A_G,
  LAYER_H_N,
  LAYER_O_U,
  LAYER_V_2,
  LAYER_3_9,
  LAYER_MODIFIERS,
  LAYER_FNS1
};

bool shiftActive = false;
bool capsLock = false;

Layer currentLayer = BASE;

void setup() {
  Serial.begin(9600);
  draw_BASE();
}

void loop() {
  delay(10);
  
  int x = analogRead(A1) - 490;
  int y = analogRead(A0) - 500;

  double xOffset = (x/10) - 5.12;
  double yOffset = (y/10) - 5.12;
  
  double magnitude = sqrt((xOffset * xOffset) + (yOffset * yOffset));

  if (magnitude > threshold && (millis() - lastInputTime) > debounceDelay) {
    float angle = atan2(yOffset, xOffset) * (180.0 / PI);
    if (angle < 0) {
      angle += 360.0;
    }

    Direction direction = getDirection(angle, magnitude);

    handleInput(direction);
    
    lastInputTime = millis();
  }
}


Direction getDirection(float angle, int magnitude) {
  if (magnitude < threshold) {
    return CENTER;
  }
  if (angle >= 67.5 && angle < 112.5) {
    return UP;
  } else if (angle >= 22.5 && angle < 67.5) {
    return UP_RIGHT;
  } else if (angle >= 337.5 || angle < 22.5) {
    return RIGHT;
  } else if (angle >= 292.5 && angle < 337.5) {
    return DOWN_RIGHT;
  } else if (angle >= 247.5 && angle < 292.5) {
    return DOWN;
  } else if (angle >= 202.5 && angle < 247.5) {
    return DOWN_LEFT;
  } else if (angle >= 157.5 && angle < 202.5) {
    return LEFT;
  } else if (angle >= 112.5 && angle < 157.5) {
    return UP_LEFT;
  }
  return CENTER;
}

String directionToString(Direction dir) {
  switch (dir) {
    case CENTER:     return "Center";
    case UP:         return "Up";
    case UP_RIGHT:   return "Up-Right";
    case RIGHT:      return "Right";
    case DOWN_RIGHT: return "Down-Right";
    case DOWN:       return "Down";
    case DOWN_LEFT:  return "Down-Left";
    case LEFT:       return "Left";
    case UP_LEFT:    return "Up-Left";
    default:         return "Unknown";
  }
}

void handleInput(Direction direction) {
  switch (currentLayer) {
    case BASE:
    draw_BASE();
      switch (direction) {
        case UP:
          draw_A_G();
          currentLayer = LAYER_A_G;
          Serial.println("Current Layer: A-G");
          break;
        case UP_RIGHT:
          draw_H_N();
          currentLayer = LAYER_H_N;
          Serial.println("Current Layer: H-N");
          break;
        case RIGHT:
          draw_O_U();
          currentLayer = LAYER_O_U;
          Serial.println("Current Layer: O-U");
          break;
        case DOWN_RIGHT:
          draw_V_2();
          currentLayer = LAYER_V_2;
          Serial.println("Current Layer: V-2");
          break;
        case DOWN:
          draw_3_9();
          currentLayer = LAYER_3_9;
          Serial.println("Current Layer: 3-9");
          break;
        case DOWN_LEFT:
          currentLayer = LAYER_FNS1;
          Serial.println("Current Layer: FUNCTIONS-1");
          break;
        case LEFT:
          draw_MODS();
          currentLayer = LAYER_MODIFIERS;
          Serial.println("Current Layer: MODIFIERS");
          break;
        default:
          break;
      }
      break;

    case LAYER_A_G:
      switch (direction) {
        case DOWN_LEFT:
          sendKey('a');
          break;
        case LEFT:
          sendKey('b');
          break;
        case UP_LEFT:
          sendKey('c');
          break;
        case UP:
          sendKey('d');
          break;
        case UP_RIGHT:
          sendKey('e');
          break;
        case RIGHT:
          sendKey('f');
          break;
        case DOWN_RIGHT:
          sendKey('g');
          break;
        case DOWN:
          draw_BASE();
          currentLayer = BASE;
          Serial.println("Current Layer: Base");
          break;
        default:
          break;
      }
      break;

    case LAYER_H_N:
      switch (direction) {
        case DOWN_LEFT:
          sendKey('h');
          break;
        case LEFT:
          sendKey('i');
          break;
        case UP_LEFT:
          sendKey('j');
          break;
        case UP:
          sendKey('k');
          break;
        case UP_RIGHT:
          sendKey('l');
          break;
        case RIGHT:
          sendKey('m');
          break;
        case DOWN_RIGHT:
          sendKey('n');
          break;
        case DOWN:
          draw_BASE();
          currentLayer = BASE;
          Serial.println("Current Layer: Base");
          break;
        default:
          break;
      }
      break;
      
    case LAYER_O_U:
      switch (direction) {
        case DOWN_LEFT:
          sendKey('o');
          break;
        case LEFT:
          sendKey('p');
          break;
        case UP_LEFT:
          sendKey('q');
          break;
        case UP:
          sendKey('r');
          break;
        case UP_RIGHT:
          sendKey('s');
          break;
        case RIGHT:
          sendKey('t');
          break;
        case DOWN_RIGHT:
          sendKey('u');
          break;
        case DOWN:
          draw_BASE();
          currentLayer = BASE;
          Serial.println("Current Layer: Base");
          break;
        default:
          break;
      }
      break;
      
    case LAYER_V_2:
      switch (direction) {
        case DOWN_LEFT:
          sendKey('v');
          break;
        case LEFT:
          sendKey('w');
          break;
        case UP_LEFT:
          sendKey('x');
          break;
        case UP:
          sendKey('y');
          break;
        case UP_RIGHT:
          sendKey('z');
          break;
        case RIGHT:
          sendKey('1');
          break;
        case DOWN_RIGHT:
          sendKey('2');
          break;
        case DOWN:
          currentLayer = BASE;
          draw_BASE();
          Serial.println("Current Layer: Base");
          break;
        default:
          break;

      }
      break;

    case LAYER_3_9:
      switch (direction) {
        case DOWN_LEFT:
          sendKey('3');
          break;
        case LEFT:
          sendKey('4');
          break;
        case UP_LEFT:
          sendKey('5');
          break;
        case UP:
          sendKey('6');
          break;
        case UP_RIGHT:
          sendKey('7');
          break;
        case RIGHT:
          sendKey('8');
          break;
        case DOWN_RIGHT:
          sendKey('9');
          break;
        case DOWN:
          draw_BASE();
          currentLayer = BASE;
          break;
        default:
          break;

      }
    case LAYER_MODIFIERS:
      switch (direction) {
        case DOWN_LEFT:
          sendSpecialKey(KEY_BACKSPACE);
          break;
        case LEFT:
          shiftActive = true;
          Serial.println("Shift Activated");
          break;
        case UP_LEFT:
          Serial.println("NOT DECLARED");
          break;
        case UP:
          sendKey(' ');
          break;
        case UP_RIGHT:
          Serial.println("NOT DECLARED");
          break;
        case RIGHT:
          Serial.println("NOT DECLARED");
          break;
        case DOWN_RIGHT:
          Serial.println("NOT DECLARED");
          break;
        case DOWN:
          draw_BASE();
          currentLayer = BASE;
          Serial.println("Current Layer: Base");
          break;
        default:
          break;

      }
      break;

    default:
      currentLayer = BASE;
      draw_BASE();
      break;
  }
}

void sendKey(char key) {
  if (shiftActive || capsLock) {
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(key);
    delay(50);
    Keyboard.releaseAll();
    if (shiftActive) {
      Serial.println("Shift deactivated");
      shiftActive = false; // Turn off Shift after one key
    }
  } else {
    Keyboard.press(key);
    delay(50);
    Keyboard.release(key);
  }
}

void sendSpecialKey(uint8_t keycode) {
  Keyboard.press(keycode);
  delay(100);
  Keyboard.release(keycode);
}

void draw_circle()
{
  LCD_Clear(0xffff);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
  Paint_Clear(WHITE);

  Paint_SetMirroring(MIRROR_HORIZONTAL);

  Paint_DrawCircle(160, 120, 40, BLACK, 2, 0);

  Paint_DrawLine(160, 160, 160, 180, BLACK, 2, LINE_STYLE_SOLID);
  Paint_DrawLine(160, 80, 160, 60, BLACK, 2, LINE_STYLE_SOLID);    
  Paint_DrawLine(200, 120, 220, 120, BLACK, 2, LINE_STYLE_SOLID);
  Paint_DrawLine(120, 120, 100, 120, BLACK, 2, LINE_STYLE_SOLID);

  Paint_DrawLine(130, 150, 110, 170, BLACK, 2, LINE_STYLE_SOLID);
  Paint_DrawLine(130, 90, 110, 70, BLACK, 2, LINE_STYLE_SOLID);
  Paint_DrawLine(190, 150, 210, 170, BLACK, 2, LINE_STYLE_SOLID);
  Paint_DrawLine(190, 90, 210, 70, BLACK, 2, LINE_STYLE_SOLID);

}

void draw_BASE()
{
  draw_circle();
  Paint_DrawString_EN(140, 190, "3-9", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(155, 40, "A-G", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(225, 115, "O-U", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(75, 115, "MOD", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(100, 170, "N/A", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(95, 55, "N/A", &Font20, WHITE, BLACK); 
  Paint_DrawString_EN(210, 170, "V-2", &Font20, WHITE, BLACK);       
  Paint_DrawString_EN(210, 60, "H-N", &Font20, WHITE, BLACK);  
}

void draw_A_G()
{
  draw_circle();
  Paint_DrawString_EN(100, 170, "A", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(75, 115, "B", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(95, 55, "C", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(155, 40, "D", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(210, 60, "E", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(225, 115, "F", &Font20, WHITE, BLACK); 
  Paint_DrawString_EN(210, 170, "G", &Font20, WHITE, BLACK);       
  Paint_DrawString_EN(140, 190, "BASE", &Font20, WHITE, BLACK);
}

void draw_H_N()
{
  draw_circle();
  Paint_DrawString_EN(100, 170, "H", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(75, 115, "I", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(95, 55, "J", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(155, 40, "K", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(210, 60, "L", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(225, 115, "M", &Font20, WHITE, BLACK); 
  Paint_DrawString_EN(210, 170, "N", &Font20, WHITE, BLACK);       
  Paint_DrawString_EN(140, 190, "BASE", &Font20, WHITE, BLACK);  
}

void draw_O_U()
{
  draw_circle();
  Paint_DrawString_EN(100, 170, "O", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(75, 115, "P", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(95, 55, "Q", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(155, 40, "R", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(210, 60, "S", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(225, 115, "T", &Font20, WHITE, BLACK); 
  Paint_DrawString_EN(210, 170, "U", &Font20, WHITE, BLACK);       
  Paint_DrawString_EN(140, 190, "BASE", &Font20, WHITE, BLACK);  
}

void draw_V_2()
{
  draw_circle();
  Paint_DrawString_EN(100, 170, "V", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(75, 115, "W", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(95, 55, "X", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(155, 40, "Y", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(210, 60, "Z", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(225, 115, "1", &Font20, WHITE, BLACK); 
  Paint_DrawString_EN(210, 170, "2", &Font20, WHITE, BLACK);       
  Paint_DrawString_EN(140, 190, "BASE", &Font20, WHITE, BLACK);  
}

void draw_3_9()
{
  draw_circle();
  Paint_DrawString_EN(100, 170, "3", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(75, 115, "4", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(95, 55, "5", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(155, 40, "6", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(210, 60, "7", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(225, 115, "8", &Font20, WHITE, BLACK); 
  Paint_DrawString_EN(210, 170, "9", &Font20, WHITE, BLACK);       
  Paint_DrawString_EN(140, 190, "BASE", &Font20, WHITE, BLACK);  
}

void draw_MODS()
{
  draw_circle();
  Paint_DrawString_EN(100, 170, "H", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(75, 115, "I", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(95, 55, "J", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(155, 40, "SPACE", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(210, 60, "L", &Font20, WHITE, BLACK);
  Paint_DrawString_EN(225, 115, "M", &Font20, WHITE, BLACK); 
  Paint_DrawString_EN(210, 170, "N", &Font20, WHITE, BLACK);       
  Paint_DrawString_EN(140, 190, "BASE", &Font20, WHITE, BLACK);  
}