#define y_bars 3

static unsigned char Close1_bitmap[] =
{
  0xff, 0xbd, 0xdb, 0xe7, 0xe7, 0xdb, 0xbd, 0xff
};

void drawChargeOK()
{
  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  char title[] = "Info";
  char message [] = "Carga completa!";
  // graphic commands to redraw the complete screen should be placed here
  uint8_t h = u8g.getFontAscent() - u8g.getFontDescent();
  uint8_t w = u8g.getWidth();
  uint8_t w_bars = w - (u8g.getStrWidth(title) + 2 * 3 + 10 + 1);
  int x_bars = u8g.getStrWidth(title) + 4;

  u8g.setDefaultForegroundColor();
  u8g.drawBox(0, 0, w, 13);
  u8g.drawLine(0, 13, 0, 64);
  u8g.drawLine(1, 63, 127, 63);
  u8g.drawLine(127, 13, 127, 63);

  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  u8g.drawStr( 4, 16, "\xa1");
  u8g.drawStr(10, 16, message);
  u8g.drawStr( 4, 28, "Tiempo:");
  u8g.setPrintPos(50, 28);
  u8g.print(timeToHuman(elapsedTime));

  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  u8g.setDefaultBackgroundColor();
  u8g.drawStr(2, 1, title);
  u8g.drawBox(x_bars, y_bars, w_bars, 2);
  u8g.drawBox(x_bars, y_bars + 5, w_bars, 2);
  u8g.drawBox(w - 11, 1, 10, 10);
  u8g.setDefaultForegroundColor();
  u8g.drawBitmap(w - 10, 2, 1, 8, Close1_bitmap);

  uint8_t w_aceptar = (w - u8g.getStrWidth("Aceptar")) / 2;
  u8g.setDefaultForegroundColor();
  u8g.drawBox(w_aceptar - 1, 64 * 2 / 3 + 5, u8g.getStrWidth("Aceptar") + 1, h + 2);
  u8g.setDefaultBackgroundColor();
  u8g.drawStr(w_aceptar, 64 * 2 / 3 + 5, "Aceptar");
  u8g.setDefaultForegroundColor();
}

void ChargeOk_buttons(uint8_t uiKeyCode)
{
  switch ( uiKeyCode ) {
    case KEY_NEXT:
      beep();
      break;
    case KEY_PREV:
      beep();
      break;
    case KEY_SELECT:
      beepOK();
      next_state = IDLE;
      event = STOP;
      break;
    default:
      break;
  }
}

void state_charge_ok(void)
{
  digitalWrite(chargingRelay, LOW);
  digitalWrite(dischargingRelay, LOW);
}
