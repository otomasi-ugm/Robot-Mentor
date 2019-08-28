void parsing(int *setData, int numData = 1)
{
  int endData = numData + 1;
  static unsigned char data[255] = {0};
  int i, msg;
  msg = Serial.available();

  if (msg > 255) msg = 0; //protection buffer
  if (msg > 0) {
    for (i = 0; i < msg; i++) {
      data[i] = Serial.read();
    }
  }

  if (data[0] == 255 && data[endData] == 254) //packet data
  {    
    for (i = 0; i < numData; i++) {
      setData[i] = data[i+1];
    }
  }

  Serial.flush();
}
