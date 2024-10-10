#include "Adafruit_seesaw.h"
#include <Keyboard.h>

Adafruit_seesaw ss;

#define BUTTON_X         6
#define BUTTON_Y         2
#define BUTTON_A         5
#define BUTTON_B         1
#define BUTTON_SELECT    0
#define BUTTON_START    16

uint32_t button_mask = (1UL << BUTTON_X) | (1UL << BUTTON_Y) | (1UL << BUTTON_START) |
                       (1UL << BUTTON_A) | (1UL << BUTTON_B) | (1UL << BUTTON_SELECT);

const int threshold = 200; // Adjust based on joystick sensitivity
const unsigned long debounceDelay = 300; // Milliseconds

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

Layer currentLayer = BASE;
unsigned long lastInputTime = 0;
bool shiftActive = false;
bool capsLock = false;

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

void setup() {
  Serial.begin(115200);

  while (!Serial) {
    delay(10);
  }

  Serial.println("Gamepad QT example!");

  if (!ss.begin(0x50)) {
    Serial.println("ERROR! seesaw not found");
    while (1) delay(1);
  }
  Serial.println("seesaw started");
  uint32_t version = ((ss.getVersion() >> 16) & 0xFFFF);
  if (version != 5743) {
    Serial.print("Wrong firmware loaded? ");
    Serial.println(version);
    while (1) delay(10);
  }
  Serial.println("Found Product 5743");

  ss.pinModeBulk(button_mask, INPUT_PULLUP);
  ss.setGPIOInterrupts(button_mask, 1);

  Keyboard.begin();
  Serial.println("Current Layer: Base");
}

void loop() {
  delay(10); // delay in loop to slow serial output

  // Reverse x/y values to match joystick orientation
  int x = 1023 - ss.analogRead(14);
  int y = 1023 - ss.analogRead(15);

  int xOffset = x - 512;
  int yOffset = y - 512;

  int magnitude = sqrt(xOffset * xOffset + yOffset * yOffset);

  if (magnitude > threshold && (millis() - lastInputTime) > debounceDelay) {
    float angle = atan2(yOffset, xOffset) * (180.0 / PI);
    if (angle < 0) {
      angle += 360.0;
    }

    Direction direction = getDirection(angle, magnitude);

    handleInput(direction);

    // Optionally, print the direction
    Serial.println(directionToString(direction));

    lastInputTime = millis();
  }

  uint32_t buttons = ss.digitalReadBulk(button_mask);

  if (!(buttons & (1UL << BUTTON_A))) {
    shiftActive = true;
    Serial.println("Shift Activated");
  }
  if (!(buttons & (1UL << BUTTON_B))) {
    sendKey(' ');
  }
  if (!(buttons & (1UL << BUTTON_Y))) {
    if (!capsLock) {
      Serial.println("Caps Lock ON");
      capsLock = true;
    } else {
      Serial.println("Caps Lock OFF");
      capsLock = false;
    }
  }
  if (!(buttons & (1UL << BUTTON_SELECT))) {
    Serial.println("Button SELECT pressed");
    sendKey(KEY_ESC);
  }
  if (!(buttons & (1UL << BUTTON_START))) {
    Serial.println("Button START pressed");
    sendKey(KEY_RETURN);
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
      switch (direction) {
        case UP:
          currentLayer = LAYER_A_G;
          Serial.println("Current Layer: A-G");
          break;
        case UP_RIGHT:
          currentLayer = LAYER_H_N;
          Serial.println("Current Layer: H-N");
          break;
        case RIGHT:
          currentLayer = LAYER_O_U;
          Serial.println("Current Layer: O-U");
          break;
        case DOWN_RIGHT:
          currentLayer = LAYER_V_2;
          Serial.println("Current Layer: V-2");
          break;
        case DOWN:
          currentLayer = LAYER_3_9;
          Serial.println("Current Layer: 3-9");
          break;
        case DOWN_LEFT:
          currentLayer = LAYER_MODIFIERS;
          Serial.println("Current Layer: MODIFIERS");
          break;
        case LEFT:
          currentLayer = LAYER_FNS1;
          Serial.println("Current Layer: FUNCTIONS-1");
          break;
        default:
          // Handle CENTER or other directions if needed
          break;
      }
      break;

    case LAYER_A_G:
      switch (direction) {
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
          currentLayer = BASE;
          Serial.println("Returning to Base Layer");
          break;
        case DOWN_LEFT:
          sendKey('a');
          break;
        case LEFT:
          sendKey('b');
          break;
        case UP_LEFT:
          sendKey('c');
          break;
        default:
          break;
      }
      break;

    case LAYER_H_N:
      switch (direction) {
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
          currentLayer = BASE;
          Serial.println("Returning to Base Layer");
          break;
        case DOWN_LEFT:
          sendKey('h');
          break;
        case LEFT:
          sendKey('i');
          break;
        case UP_LEFT:
          sendKey('j');
          break;
        default:
          break;
      }
      break;

    case LAYER_O_U:
      switch (direction) {
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
          currentLayer = BASE;
          Serial.println("Returning to Base Layer");
          break;
        case DOWN_LEFT:
          sendKey('o');
          break;
        case LEFT:
          sendKey('p');
          break;
        case UP_LEFT:
          sendKey('q');
          break;
        default:
          break;
      }
      break;

    case LAYER_V_2:
      switch (direction) {
        case UP:
          sendKey('x');
          break;
        case UP_RIGHT:
          sendKey('y');
          break;
        case RIGHT:
          sendKey('z');
          break;
        case DOWN_RIGHT:
          sendKey('2');
          break;
        case DOWN:
          currentLayer = BASE;
          Serial.println("Returning to Base Layer");
          break;
        case DOWN_LEFT:
          sendKey('v');
          break;
        case LEFT:
          sendKey('w');
          break;
        case UP_LEFT:
          sendKey('1');
          break;
        default:
          break;
      }
      break;

    case LAYER_3_9:
      switch (direction) {
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
          currentLayer = BASE;
          Serial.println("Returning to Base Layer");
          break;
        case DOWN_LEFT:
          sendKey('3');
          break;
        case LEFT:
          sendKey('4');
          break;
        case UP_LEFT:
          sendKey('5');
          break;
        default:
          break;
      }
      break;

    case LAYER_MODIFIERS:
      switch (direction) {
        case UP:
          sendSpecialKey(KEY_LEFT_CTRL);
          break;
        case UP_RIGHT:
          sendSpecialKey(KEY_LEFT_ALT);
          break;
        case RIGHT:
          sendSpecialKey(KEY_LEFT_GUI); // Windows or Command key
          break;
        case DOWN_RIGHT:
          sendSpecialKey(KEY_LEFT_SHIFT);
          break;
        case DOWN:
          currentLayer = BASE;
          Serial.println("Returning to Base Layer");
          break;
        // Add more modifiers if needed
        default:
          break;
      }
      break;

    case LAYER_FNS1:
      switch (direction) {
        case UP:
          sendSpecialKey(KEY_F5);
          break;
        case UP_RIGHT:
          sendSpecialKey(KEY_F6);
          break;
        case RIGHT:
          sendSpecialKey(KEY_F7);
          break;
        case DOWN_RIGHT:
          sendSpecialKey(KEY_F8);
          break;
        case DOWN:
          currentLayer = BASE;
          Serial.println("Returning to Base Layer");
          break;
        case DOWN_LEFT:
          sendSpecialKey(KEY_F1);
          break;
        case LEFT:
          sendSpecialKey(KEY_F2);
          break;
        case UP_LEFT:
          sendSpecialKey(KEY_F3);
          break;
        default:
          break;
      }
      break;

    default:
      currentLayer = BASE;
      break;
  }
}

void sendKey(char key) {
  if (shiftActive || capsLock) {
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(key);
    delay(100);
    Keyboard.releaseAll();
    if (shiftActive) {
      Serial.println("Shift deactivated");
      shiftActive = false; // Turn off Shift after one key
    }
  } else {
    Keyboard.press(key);
    delay(100);
    Keyboard.release(key);
  }
}

void sendSpecialKey(uint8_t keycode) {
  Keyboard.press(keycode);
  delay(100);
  Keyboard.release(keycode);
}
