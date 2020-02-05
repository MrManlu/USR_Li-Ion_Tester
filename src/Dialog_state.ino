#define DIALOG_ITEMS 2
#define y_bars 3
const char *dialog_strings[DIALOG_ITEMS] = {"Aceptar", "Cancelar"};
uint8_t dialog_current = 0;

static unsigned char Close_bitmap[] =
{
  0xff, 0xbd, 0xdb, 0xe7, 0xe7, 0xdb, 0xbd, 0xff
};

void drawDialog() {
  String title_s = "";
  String message_s = "";
  switch (event)
  {
    case CYCLE:
      title_s = "Ciclo";
      message_s = "Comenzar ciclo?";
      break;
    case CHARGE:
      title_s = "Carga";
      message_s = "Comenzar carga?";
      break;
    case DISCHARGE:
      title_s = "Descarga";
      message_s = "Comenzar descarga?";
      break;
    default:
      title_s = "STOP";
      message_s = "STOP";
      break;
  }

  char title [20];
  char message [20];
  title_s.toCharArray(title, 20);
  message_s.toCharArray(message, 20);
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_6x13);
  uint8_t h = u8g.getFontAscent() - u8g.getFontDescent();
  uint8_t w = u8g.getWidth();
  uint8_t w_bars = w - (u8g.getStrWidth(title) + 2 * 3 + 10 + 1);
  int x_bars = u8g.getStrWidth(title) + 4;

  u8g.setDefaultForegroundColor();
  u8g.drawBox(0, 0, w, 13);
  u8g.drawLine(0, 13, 0, 64);
  u8g.drawLine(1, 63, 127, 63);
  u8g.drawLine(127, 13, 127, 63);
  u8g.drawStr( 4, 25, message);
  u8g.setDefaultBackgroundColor();
  //u8g.setFont(u8g_font_6x13);
  u8g.drawStr(2, 0, title);
  u8g.drawBox(x_bars, y_bars, w_bars, 2);
  u8g.drawBox(x_bars, y_bars + 5, w_bars, 2);
  u8g.drawBox(w - 11, 1, 10, 10);
  u8g.setDefaultForegroundColor();
  u8g.drawBitmap(w - 10, 2, 1, 8, Close_bitmap);

  int8_t buttons_space = (w - (u8g.getStrWidth(dialog_strings[1]) + u8g.getStrWidth(dialog_strings[1]))) / 3;
  uint8_t spaces[] = {0, u8g.getStrWidth(dialog_strings[0])};
  for (uint8_t i = 0; i < 2; i++)
  {
    u8g.setDefaultForegroundColor();
    if ( i == dialog_current)
    {
      u8g.drawBox(buttons_space * (1 + i) - 1 + spaces[i], 64 * 2 / 3, u8g.getStrWidth(dialog_strings[i]) + 1, h + 2);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(buttons_space * (1 + i) + spaces[i], 64 * 2 / 3, dialog_strings[i]);
  }
}

void dialog_nextEntry(void)
{
  dialog_current++;
  if ( dialog_current >= DIALOG_ITEMS )
    dialog_current = 0;
  Serial.print(dialog_current);
}

void dialog_prevEntry(void)
{
  if ( dialog_current == 0 )
    dialog_current = DIALOG_ITEMS;
  dialog_current--;
  Serial.print(dialog_current);
}

void dialog_select(void)
{
  if (dialog_current == 0)
  {
    Capacity = 0;
    u8g.firstPage();
    do  {
      u8g.drawStr(4,18, "Comprobando SD...");
    } while ( u8g.nextPage() );
    if(checkSD())
    {
      FileName = sd_getNextFileName();
      dataFile = SD.open(FileName, FILE_WRITE);
      dataFile.print("Time (s);Current (A);Voltage (V);Capacity (Ah);State;");
      dataFile.println("fw version: " + String(version) + ";");
      logging = true;
    }
    else
    {
      FileName = "";
      logging = false;
    }

    millisAtStart = 0;
    elapsedTime = 0;
    previousMillis = 0;
    millisPassed = 0;
    prev_elapsedTime = 0;
    
    switch (event)
    {
      case CYCLE:
        next_state = CHARGING;
        event = CYCLE;        
        digitalWrite(dischargingRelay, LOW);
        digitalWrite(chargingRelay, HIGH);
        delay(1000);
        readSensors();
        millisAtStart = millis();
        break;
      case CHARGE:
        next_state = CHARGING;
        event = CHARGE;
        digitalWrite(dischargingRelay, LOW);
        digitalWrite(chargingRelay, HIGH);
        delay(1000);
        readSensors();
        millisAtStart = millis();
        break;
      case DISCHARGE:
        next_state = DISCHARGING;
        event = DISCHARGE;
        digitalWrite(chargingRelay, LOW);
        digitalWrite(dischargingRelay, HIGH);
        delay(1000);
        readSensors();
        millisAtStart = millis();
        break;
      default:
        next_state = MENU;
        break;
    }
  }
  else
  {
    next_state = MENU;
    event = STOP;
  }
}

void Dialog_buttons(uint8_t uiKeyCode)
{
  switch ( uiKeyCode ) {
    case KEY_NEXT:
      dialog_nextEntry();
      beep();
      break;
    case KEY_PREV:
      dialog_prevEntry();
      beep();
      break;
    case KEY_SELECT:
      dialog_select();
      beepOK();
      break;
    default:
      break;
  }
}

void state_dialog(void)
{
  //Serial.println("dialog");
}
