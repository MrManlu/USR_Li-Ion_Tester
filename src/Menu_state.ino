#define MENU_ITEMS 4
const char *menu_strings[MENU_ITEMS] = { "Atr\xe1s", "Carga", "Descarga", "Ciclo" };
uint8_t menu_current = 0;

const uint8_t derecha_bitmap[] = {0x10, 0x18, 0x1c, 0x1e, 0x1e, 0x1c, 0x18, 0x10};
const uint8_t izquierda_bitmap[] = {0x10, 0x30, 0x70, 0xF0, 0xF0, 0x70, 0x30, 0x10};

void drawMenu(void)
{
  uint8_t i, h;
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();

  h = u8g.getFontAscent() - u8g.getFontDescent();
  w = u8g.getWidth();
  for ( i = 0; i < MENU_ITEMS; i++ ) {
    d = 10;
    u8g.setDefaultForegroundColor();
    //u8g.setFont(u8g_font_m2icon_7);
    //(0, i*h, "\x4E");
    if ( i == menu_current ) {
      u8g.drawBox(0, i * h, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.setFont(u8g_font_6x13);
    u8g.drawStr(d, i * h, menu_strings[i]);
    //u8g.setFont(u8g_font_m2icon_7);
    //u8g.drawStr(0, i*h, "\x4E");
    if (i != 0)
      u8g.drawBitmap( 0, i * h + 2, 1, 8, derecha_bitmap);
    else
      u8g.drawBitmap( 0, i * h + 2, 1, 8, izquierda_bitmap);
  }
}

void Menu_nextEntry(void)
{
  menu_current++;
  if ( menu_current >= MENU_ITEMS )
    menu_current = 0;
}

void Menu_prevEntry(void)
{
  if ( menu_current == 0 )
    menu_current = MENU_ITEMS;
  menu_current--;
}

void Menu_select(void)
{
  switch (menu_current)
  {
    case 0:
      next_state = IDLE;
      event = STOP;
      break;
    case 1:
      next_state = DIALOG;
      event = CHARGE;
      break;
    case 2:
      next_state = DIALOG;
      event = DISCHARGE;
      break;
    case 3:
      next_state = DIALOG;
      event = CYCLE;
      break;
    //    case 0:
    //      next_state = IDLE;
    //      break;
    default:
      next_state = MENU;
      event = STOP;
      break;
  }
  //next_state = IDLE;
}

void Menu_buttons(uint8_t uiKeyCode)
{
  switch ( uiKeyCode ) {
    case KEY_NEXT:
      Menu_nextEntry();
      beep();
      break;
    case KEY_PREV:
      Menu_prevEntry();
      beep();
      break;
    case KEY_SELECT:
      Menu_select();
      beepOK();
      break;
  }
}

void state_menu(void)
{
  //Serial.println("MENU");

  digitalWrite(chargingRelay, LOW);
  digitalWrite(dischargingRelay, LOW);
}
