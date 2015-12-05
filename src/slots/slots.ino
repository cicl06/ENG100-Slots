//Arduino Slot Machine Program
//For authors and licensing information see READNME.md

#define NSLOTS 4

#define SLOTMAXVAL 9

#define DRAWINTERVAL 2

#define BUTTONPIN 12

const int slotUpdateTimes[4] = {101, 107, 117, 127};

const boolean digits [10][8] = {
  {1, 1, 1, 0, 1, 1, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {1, 0, 1, 1, 1, 0, 1, 0},
  {1, 0, 1, 1, 0, 1, 1, 0},
  {0, 1, 1, 1, 0, 1, 0, 0},
  {1, 1, 0, 1, 0, 1, 1, 0},
  {1, 1, 0, 1, 1, 1, 1, 0},
  {1, 0, 1, 0, 0, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 0, 1, 1, 0}
};

const int selectorPins[NSLOTS] = {3, 13, 11, 10};
const int displayPins[8] = {8, 6, 9, 7, 2, 5, 4, 0};

int currentSlot; //Current slot to be drawn

struct Slot
{
    unsigned int currentVal;
    unsigned long lastUpdate;
    bool stopped;
};

unsigned long lastDrawUpdate;

bool pressed;

struct Slot slots[NSLOTS]; 

void slotNextValue (int i);
void processSlot(int i);
void initSlot(int i);
void handleDisplay();

void setup () {
  for (int i = 0; i < NSLOTS; i++)
    {
        initSlot(i);
        pinMode(selectorPins[i], OUTPUT);
        digitalWrite(selectorPins[i], HIGH);
    }
    
    for (int i = 0; i < 8; i++)
    {
      pinMode(displayPins[i], OUTPUT);
      digitalWrite(displayPins[i], HIGH);
    }
    lastDrawUpdate = millis();
    
    pressed = false;
    
    currentSlot = 0;
    
    pinMode(BUTTONPIN, INPUT);
  }
    

void loop () {
  for (int i = 0; i < NSLOTS; i++)
  {
    processSlot(i);
  }
  
  if (buttonPressed())
  {
    stopNextSlot();
  }
  
  handleDisplay();
}


void initSlot(int i)
{
  slots[i].currentVal = 0;
  slots[i].lastUpdate = millis();
  slots[i].stopped = false;
}

void processSlot(int i)
{
  unsigned long time = millis();
  if (((time - slots[i].lastUpdate) >= slotUpdateTimes[i]) && !slots[i].stopped)
  {
    slotNextValue(i);
    slots[i].lastUpdate = time;
  }
}

void slotNextValue(int i)
{
  slots[i].currentVal++;
  if (slots[i].currentVal > 9)
    slots[i].currentVal = 0;
}

void stopNextSlot()
{
  for (int i = 0; i < NSLOTS; i++)
  {
    if (!slots[i].stopped)
    {
      slots[i].stopped = true;
      return;
    }
  }
  
  for (int i = 0; i < NSLOTS; i++)
  {
    slots[i].stopped = false;
  }
}

boolean buttonPressed()
{
  if (digitalRead(BUTTONPIN) == HIGH)
  {
    if (!pressed)
    {
      pressed = true;
      return true;
    }  
    return false;
  }
  pressed = false;
  return false;
}

void handleDisplay()
{
  unsigned long time = millis();
  if (time - lastDrawUpdate > DRAWINTERVAL)
  {
    lastDrawUpdate = time;
    
    digitalWrite(selectorPins[currentSlot], HIGH);
    
    currentSlot++;
    if (currentSlot > 3)
      currentSlot = 0;
    
    digitalWrite(selectorPins[currentSlot], LOW);
    
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(displayPins[i], digits[slots[currentSlot].currentVal][i] ? LOW : HIGH);
    }
  }
}
