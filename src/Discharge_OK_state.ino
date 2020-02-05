
void drawDischargeOk()
{
  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  char title[] = "Info";
  char message0 [] = "Descarga completa!";
  char message1 [] = "Ciclo completo!";
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
  if (event == CYCLE)
    u8g.drawStr(10, 14, message1);
  else
    u8g.drawStr(10, 14, message0);
  u8g.drawStr( 4, 26, "Tiempo:");
  u8g.setPrintPos(50, 26);
  u8g.print(timeToHuman(elapsedTime));
  u8g.drawStr( 4, 38, "Cap.[Ah]:");
  u8g.setPrintPos(64, 38);
  u8g.print(Capacity);

  //u8g.setFont(u8g_font_6x13);
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
  u8g.drawBox(w_aceptar - 1, 64 * 2 / 3 + 9, u8g.getStrWidth("Aceptar") + 1, h);
  u8g.setDefaultBackgroundColor();
  u8g.drawStr(w_aceptar, 64 * 2 / 3 + 9, "Aceptar");
  u8g.setDefaultForegroundColor();
}

void DischargeOk_buttons(uint8_t uiKeyCode)
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

void state_discharge_ok(void)
{
  digitalWrite(chargingRelay, LOW);
  digitalWrite(dischargingRelay, LOW);
}
