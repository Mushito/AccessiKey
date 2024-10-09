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
  // Add more layers as needed
};

Layer currentLayer = BASE;
unsigned long lastInputTime = 0;
bool shiftActive = false;
bool capsLock = false;

void setup() {
  Serial.begin(115200);

  while(!Serial) {
    delay(10);
  }

  Serial.println("Gamepad QT example!");
  
  if(!ss.begin(0x50)){
    Serial.println("ERROR! seesaw not found");
    while(1) delay(1);
  }
  Serial.println("seesaw started");
  uint32_t version = ((ss.getVersion() >> 16) & 0xFFFF);
  if (version != 5743) {
    Serial.print("Wrong firmware loaded? ");
    Serial.println(version);
    while(1) delay(10);
  }
  Serial.println("Found Product 5743");
  
  ss.pinModeBulk(button_mask, INPUT_PULLUP);
  ss.setGPIOInterrupts(button_mask, 1);

  Keyboard.begin();
  Serial.println("Current Layer: Base");
}

int last_x = 0, last_y = 0;

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

    String direction = getDirection(angle, magnitude);

    handleInput(direction);

    lastInputTime = millis();
  }

  uint32_t buttons = ss.digitalReadBulk(button_mask);

  if (! (buttons & (1UL << BUTTON_A))) {
    shiftActive = true;
    Serial.println("Shift Activated");
    
  }
  if (! (buttons & (1UL << BUTTON_B))) {
    sendKey(' ');
  }
  if (! (buttons & (1UL << BUTTON_Y))) {
    if (!capsLock){
        Serial.println("Caps Lock ON");
        capsLock = true; 
    }
    else{
        Serial.println("Caps Lock OFF");
        capsLock = false;
    }
  }
  if (! (buttons & (1UL << BUTTON_SELECT))) {
    Serial.println("Button SELECT pressed");
    sendKey(KEY_ESC);
  }
  if (! (buttons & (1UL << BUTTON_START))) {
    Serial.println("Button START pressed");
    sendKey(KEY_RETURN);
  }
}

String getDirection(float angle, int magnitude) {
  if (magnitude < threshold) {
    return "Center";
  }
  if (angle >= 67.5 && angle < 112.5) {
    return "Up";
  } else if (angle >= 22.5 && angle < 67.5) {
    return "Up-Right";
  } else if (angle >= 337.5 || angle < 22.5) {
    return "Right";
  } else if (angle >= 292.5 && angle < 337.5) {
    return "Down-Right";
  } else if (angle >= 247.5 && angle < 292.5) {
    return "Down";
  } else if (angle >= 202.5 && angle < 247.5) {
    return "Down-Left";
  } else if (angle >= 157.5 && angle < 202.5) {
    return "Left";
  } else if (angle >= 112.5 && angle < 157.5) {
    return "Up-Left";
  }
  return "Center";
}

void handleInput(String direction) {
  switch (currentLayer) {
    case BASE:
    
      switch(direction) {
        case "Up":
          currentLayer = LAYER_A_G:
          Serial.println("Current Layer: A-G");
          
        case "Up-Right"
          currentLayer = LAYER_H_N:
          Serial.println("Current Layer: H-N");
          
        case "Right"
          currentLayer = LAYER_O_U:
          Serial.println("Current Layer: O_U");

        case "Down-Right"
          currentLayer = LAYER_V_2:
          Serial.println("Current Layer: V_2");

        case "Down"
          currentLayer = LAYER_3_9:
          Serial.println("Current Layer: 3_9");

        case "Down-Left"
          currentLayer = LAYER_MODIFIERS:
          Serial.println("Current Layer: MODIFIERS");

        case "Left"
          currentLayer = LAYER_FNS1:
          Serial.println("Current Layer: FUNCTIONS-1"); //////////SHIFT TO ADD MORE FUNCTIONS////////////
      }
      
      if (direction == "Up") {
        currentLayer = LAYER_A_G;
        Serial.println("Current Layer: A-G");
      } else if (direction == "Down-Left") {
        shiftActive = true; // Activate Shift toggle
        Serial.println("Shift Activated");
      }
    case LAYER_A_G:
      switch(direction){
        case "Up":
          sendKey('d');
        case "Up-Right":
          sendKey('e');
        case "Right":
          sendKey('f');
        case "Down-Right":
          sendKey('g');
        case "Down":
          currentLayer = BASE;
        case "Down-Left":
          sendKey('a');
        case "Left":
          sendKey('b');
        case "Up-Left":
          sendKey('c');        
      }
    case LAYER H-N:
      if (direction == "Up") {
        currentLayer = LAYER_H-N;
        Serial.println("Current Layer: A-G");
      } else if (direction == "Down-Left") {
        shiftActive = true; // Activate Shift toggle
        Serial.println("Shift Activated");
      }
    case BASE:
      if (direction == "Up") {
        currentLayer = LAYER_A_G;
        Serial.println("Current Layer: A-G");
      } else if (direction == "Down-Left") {
        shiftActive = true; // Activate Shift toggle
        Serial.println("Shift Activated");
      }
    case BASE:
      if (direction == "Up") {
        currentLayer = LAYER_A_G;
        Serial.println("Current Layer: A-G");
      } else if (direction == "Down-Left") {
        shiftActive = true; // Activate Shift toggle
        Serial.println("Shift Activated");
      }
      // Handle other directions in the base layer if needed
      break;

      break;

    // Add more cases for additional layers

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
    if(shiftActive){
      Serial.print("Shift deactivated");
      shiftActive = false; // Turn off Shift after one key      
    }
  } else {
    Keyboard.press(key);
    delay(100);
    Keyboard.release(key);
  }
}
