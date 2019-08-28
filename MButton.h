class MButton {
  public:
    MButton();
    int btnNum = 1;
    void attach(int pin[], int mode = 1);
    void setState(char state[]);
    void setActive(bool active);
    void setIncrement(char inc);
    void setDecrement(char dec);
    char getBtn();
    void setIndex(int num);
    int getIndex(int num);
    bool isPressed();
  private:
    int _index = 0;
    int *_pin;
    char *_state;
    bool _active = LOW;
    char _standBy = ' ';
    char _increment = '^';
    char _decrement = 'v';
};

MButton::MButton() {}

void MButton::attach(int *pin, int mode = 1) {
  for (int i = 0; i < btnNum; i++) {
    pinMode(pin[i], mode);
  }
  _pin = pin;
}

void MButton::setState(char *state) {
  _state = state;
}

void MButton::setActive(bool active) {
  _active = active;
}

char MButton::getBtn() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(_pin[i]) == _active) {
      while(digitalRead(_pin[i]) != _active);
      return _state[i];
    }
  }
  return _standBy;
}

bool MButton::isPressed() {
  if (getBtn() != _standBy) {
    return true;
  } else {
    return false;
  }
}

void MButton::setIncrement(char inc) {
  _increment = inc;
}

void MButton::setDecrement(char dec) {
  _decrement = dec;
}

int MButton::getIndex(int max) {
  if(getBtn() == _increment){
    _index--;
    delay(200);
  }
  if(getBtn() == _decrement){
    _index++;
    delay(200);
  }
  if(_index<0){
    _index = max-1;
  }
  if(_index>=max){
    _index = 0;
  }
  return _index;
}

void MButton::setIndex(int num){
  _index = num;
}

