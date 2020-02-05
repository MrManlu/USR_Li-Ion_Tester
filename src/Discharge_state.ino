void drawDischarge()
{
  //u8g.setFont(u8g_font_unifont);
  u8g.setFont(u8g_font_6x10);
  u8g.setPrintPos(80, 32);
  u8g.print(timeToHuman(elapsedTime));
  u8g.drawStr(0, 32, "Descarga...");
  u8g.setPrintPos(13, 44);
  if(logging)
  {
    u8g.drawXBM(0,34, sd_width,sd_height,sd_bits);
    u8g.print(FileName);
  }
  else
  { 
    u8g.drawXBM(0,34, nosd_width,nosd_height,nosd_bits);
    u8g.print("Sin SD");
  }
  u8g.setPrintPos(0, 63);
  u8g.print("Cap. [Ah]=");
  u8g.setPrintPos(64, 63);
  u8g.print(Capacity);
  draw_ampBox(0, 0, amps);
  draw_voltBox(64, 0, volt);
}

void Discharge_buttons(uint8_t uiKeyCode)
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

void state_discharge(void)
{
  digitalWrite(chargingRelay, LOW);
  digitalWrite(dischargingRelay, HIGH);
  elapsedTime = millis() - millisAtStart;
  millisPassed = millis() - previousMillis;
  Capacity += (-amps) * (millisPassed / 3600000.0); // 1 Hour = 3600000ms
  previousMillis = millis();
  if(logging)
    if ((elapsedTime - prev_elapsedTime) >= 1000)
    {
      sd_logData();
      prev_elapsedTime = elapsedTime;
    }

  //if (elapsedTime >= 12000)
  if(volt <= dch_min_volt)
  {
    digitalWrite(dischargingRelay, LOW);
    digitalWrite(chargingRelay, LOW);
    if (logging)
      dataFile.close();
    EEPROM.put(0,Capacity);
    song();
    next_state = DISCHARGE_OK;
  }
}
