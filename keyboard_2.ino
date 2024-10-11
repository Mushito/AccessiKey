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
          Serial.println("Current Layer: Base");
          break;
        default:
          break;

      }
      break;
