char sc(bool c) {
  if (c) {
    return '~';
  }
  else {
    return ' ';
  }
}

String normalize(String x){
  x.concat("               ");
  return x;
}

void showMenu(LiquidCrystal_I2C lcd, String t[], int idx) {  
  int a, b;
  bool c = false;
  if ((idx % 2) == 0) {
    a = idx;
    b = idx + 1;
    c = true;
  }
  else if ((idx % 2) == 1) {
    a = idx - 1;
    b = idx;
    c = false;
  }

  lcd.setCursor(0, 0);
  lcd.print(sc(c));
  lcd.setCursor(1, 0);
  lcd.print(normalize(t[a]));
  lcd.setCursor(0, 1);
  lcd.print(sc(!c));
  lcd.setCursor(1, 1);
  lcd.print(normalize(t[b]));
}
