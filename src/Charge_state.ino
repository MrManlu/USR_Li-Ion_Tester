#define sd_width 10
#define sd_height 12
static unsigned char sd_bits[] = {
  0xff, 0x00, 0xff, 0x01, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03,
  0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0x01, 0x02, 0x01, 0x02, 0xff, 0x03
};

#define nosd_width 10
#define nosd_height 12
static unsigned char nosd_bits[] = {
  0xff, 0x00, 0xff, 0x01, 0xff, 0x03, 0xfd, 0x02, 0x7b, 0x03, 0xb7, 0x03,
  0xcf, 0x03, 0xcf, 0x03, 0xb5, 0x02, 0x4b, 0x03, 0x85, 0x02, 0xff, 0x03
};

void drawCharge()
{
  //u8g.setFont(u8g_font_unifont);
  u8g.setFont(u8g_font_6x10);
  u8g.setPrintPos(80, 32);
  u8g.print(timeToHuman(elapsedTime));
  u8g.drawStr(0, 32, "Cargando...");
  u8g.setPrintPos(13, 44);
  if (logging)
  {
    u8g.drawXBM(0, 34, sd_width, sd_height, sd_bits);
    u8g.print(FileName);
  }
  else
  {
    u8g.drawXBM(0, 34, nosd_width, nosd_height, nosd_bits);
    u8g.print("Sin SD");
  }

  u8g.drawFrame(0, 53, 100, 10);
  uint8_t x_percentage = 0;
  uint8_t percentage = 0;
  if (volt >= dch_min_volt && volt <= chg_max_volt)
  {
    x_percentage = (int)mapfloat(volt, 2.5, 4.2, 1, 96);
    percentage = (int)mapfloat(volt, 2.5, 4.2, 0, 100);
  }
  else
  {
    x_percentage = 0;
    percentage = 0;
  }
  u8g.drawBox(2, 56, x_percentage, 4);

  u8g.setPrintPos(105, 62);
  u8g.print(String(percentage) + "%");

  draw_ampBox(0, 0, amps);
  draw_voltBox(64, 0, volt);
}

void Charge_buttons(uint8_t uiKeyCode)
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

void state_charge(void)
{
  digitalWrite(dischargingRelay, LOW);
  digitalWrite(chargingRelay, HIGH);
  elapsedTime = millis() - millisAtStart;
  millisPassed = millis() - previousMillis;
  previousMillis = millis();
  if (logging)
    if ((elapsedTime - prev_elapsedTime) >= seconds_to_log * 1000)
    {
      sd_logData();
      prev_elapsedTime = elapsedTime;
    }

  //if (elapsedTime >= 6000)
  if (amps <= chg_min_amps || volt >= chg_max_volt)
  {
    digitalWrite(chargingRelay, LOW);
    digitalWrite(dischargingRelay, LOW);
    switch (event)
    {
      case CYCLE:
        next_state = DISCHARGING;
        delay(250);
        break;
      case CHARGE:
        if (logging)
          dataFile.close();
        song();
        next_state = CHARGE_OK;
        break;
      default:
        next_state = IDLE;
        break;
    }
  }

  //  else
  //  {
  //    // LOGIN SD
  //    break;
  //  }

}
