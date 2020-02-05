bool checkSD()
{
  if (!card.init(SPI_HALF_SPEED, sd_card_cs_pin)) {
    return false;
  } else {
    if (!SD.begin(sd_card_cs_pin))
      return false;
    else
      return true;
  }
}

uint32_t sd_volume()
{
  if (!checkSD())
  {
    //Serial.println("Initialization failed.");
    return 0;
  }

  if (!volume.init(card))
  {
    //Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return 0;
  }

  //Serial.print("Volume type is:    FAT");
  //Serial.println(volume.fatType(), DEC);
  uint32_t volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
  return volumesize;
}

String sd_getNextFileName()
{
  bool found = false;
  uint8_t i = 0;

  while (!found) {
    FileName = "LiT-";
    if (i < 100)
    {
      FileName += "0";
      if (i < 10)
      {
        FileName += "0";
      }
    }
    FileName += i;
    FileName += ".csv";
    if (!SD.exists(FileName))
    {
      found = true;
    }
    else
      i++;
  }
  return FileName;
}

void sd_logData()
{
  if (dataFile) {
    String dataString = "";
    dataString += String(elapsedTime);
    dataString += ";";
    dataString += String(amps);
    dataString += ";";
    dataString += String(volt);
    dataString += ";";
    dataString += String(Capacity);
    dataString += ";";
    switch (next_state)
    {
      case CHARGING:
        dataString += "CHARGING";
        break;
      case DISCHARGING:
        dataString += "DISCHARGING";
        break;
      default:
        break;
    }
    dataString += ";";
    dataFile.println(dataString);
  }
}
