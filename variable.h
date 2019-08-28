#define BASE 0
#define SHOULDER 1
#define ELBOW 2
#define WLEFT 3
#define WRIGHT 4

const int btnPin[] = {47, 49, 51, 53};

char OK = 'O', CANCEL = 'X', UP = '^', DOWN = 'v';
char btnState[] = {OK, CANCEL, UP, DOWN};

String nameAxis[] = {"Base", "Shoulder", "Elbow", "Wrist Left", "Wrist Right", ""};

const int numAxis = 5;
/*                    BASE | SHOULDER | ELBOW | WLEFT | WRIGHT */
const int potPin[] = { A5,      A4,       A3,    A2,     A1};
const int dirPin[] = {  6,      10,        8,     4,     2};
const int pwmPin[] = {  7,      11,        9,     5,     3};

int minAngle[]     = {  0,       0,        0,     0,      0};
int maxAngle[]     = {210,     180,      210,   320,    320};

int minPot[]       = { 10,      47,        0,     0,      0};
int maxPot[]       = {935,     662,     1024,  1024,    1024};

//int minPot[]       = {  0,       0,        0,     0,      0};
//int maxPot[]       = {1024,    1024,     1024,  1024,    1024};

struct AxisData {
  int minPot;
  int maxPot;
};

// EEPROM Address Allocation
const int axisDataAddr = 50; // Start at 50; End at (50 + (AxisData * numAxis))

AxisData axisData[numAxis];

int data[numAxis];
