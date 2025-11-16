#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int pinA = 6;
int pinB = 7;
int pinP = 8;
int pinALast;
unsigned long currentTime;
unsigned long loopTime;
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;
unsigned long blinkTimer = 0;
int clickCount = 0;
unsigned long lastClickTime = 0;
const unsigned long window = 350;
bool blinkState = false;
String name;



void setup()
{
  pinMode (pinA,INPUT);
  pinMode (pinB,INPUT);
  pinMode(pinP, INPUT_PULLUP);
  pinALast = digitalRead(pinA);
  Serial.begin (9600);
  currentTime = millis();
  loopTime = currentTime;
  lcd.init();
  lcd.backlight();

  byte eye[8] = {
    B11111,
    B10001,
    B10101,
    B10101,
    B10101,
    B10101,
    B10001,
    B11111
  };

  byte blink1[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11110,
    B01110,
    B00000
  };

  byte blink2[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B01111,
    B01110,
    B00000
  };

  byte mouse1[8] = {
    B00000,
    B00000,
    B00000,
    B11100,
    B11000,
    B11000,
    B11111,
    B11111
  };

  byte mouse2[8] = {
    B00000,
    B00000,
    B00000,
    B00111,
    B00011,
    B00011,
    B11111,
    B11111
  };

  lcd.createChar(0, eye);
  lcd.createChar(1, mouse1);
  lcd.createChar(2, mouse2);
  lcd.createChar(3, blink1);
  lcd.createChar(4, blink2);

  draw_face(0,0,1,2);
  showMessage("Hello!", 1500);
  showMessage("I'm B-12 !", 1500);
  name = entry("Who are you ?");
  String full = String("Nice to meet you ") + name + String(" !");
  showMessage(full.c_str(), 1500);
}

void (*f[5])() = {work, eat, sleep, sport, NULL};

void draw_face(int e1, int e2, int m1, int m2)
{
  lcd.clear();
  draw(e1, 0, 0);
  draw(e2, 1, 0);
  draw(m1, 0, 1);
  draw(m2, 1, 1);
}

void scrollMessage(const char* msg, unsigned long duration_ms)
{
    int len = strlen(msg);
    for (int i = 0; i <= len - 12; i++)
    {
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print(msg + i);
        unsigned long t = millis();
        while (millis() - t < 300)
          ;
        draw_face(0, 0, 1, 2);
    }
}

void showMessage(const char* msg, unsigned long duration_ms)
{
  if (strlen(msg) > 12)
    return (scrollMessage(msg, duration_ms));
  lcd.setCursor(3, 0);
  lcd.print(msg);
  unsigned long t = millis();
  while (millis() - t < duration_ms)
    ;
  lcd.clear();
  draw_face(0, 0, 1, 2);
}


void draw(byte c, int a, int b)
{
  if (a >= 0 && b >= 0)
    lcd.setCursor(a, b);
  else
    lcd.setCursor(3, 1);
  lcd.write(c);
}

void menu4(const char* a, const char* b, const char* c, const char* d)
{
    const char* opts[4] = {a,b,c,d};
    int pos = 0;
    int pA_prev = LOW;
    unsigned long t0 = millis();

    while (true)
    {
        int click = detectNClick(digitalRead(pinP));
        int A = digitalRead(pinA);
        int B = digitalRead(pinB);
        
        if (millis() - t0 > 5000)
          return;
        if (!A && pA_prev)
        {
            t0 = millis();
            if (B)
              pos++;
            else
              pos--;
            if (pos < 0)
              pos = 3;
            if (pos > 3)
              pos = 0;

            lcd.clear();
            draw_face(0,0,1,2);
            lcd.setCursor(2,0); lcd.write('?');

            lcd.setCursor(3,0);
            lcd.print(pos==0 ? "~" : " "); lcd.print(a);
            
            lcd.setCursor(10,0);
            lcd.print(pos==1 ? "~" : " "); lcd.print(b);
            
            lcd.setCursor(3,1);
            lcd.print(pos==2 ? "~" : " "); lcd.print(c);
            
            lcd.setCursor(10,1);
            lcd.print(pos==3 ? "~" : " "); lcd.print(d);
        }

        if (click == 1)
          return (f[pos]());
        pA_prev = A;
    }
}

char *entry(const char* msg)
{
    static char answer[12];
    int idx = 0;
    int l = 0;
    bool finished = false;

    for (int i = 0; i < 12; i++)
      answer[i] = ' ';

    lcd.setCursor(3,0);
    lcd.print(msg);

    while (!finished)
    {
        int click = detectNClick(digitalRead(pinP));
        encoder_A = digitalRead(pinA);
        encoder_B = digitalRead(pinB);

        if (click == 3)
        {
            answer[idx + 1] = '\0';
            answer[12] = '\0';
            finished = true;
        }

        if (click == 1 && idx < 11)
        {
            idx++;
            l = answer[idx] - ' ';
        }

        if (click == 2 && idx > 0)
        {
            idx--;
            l = answer[idx] - ' ';
        }

        if (!encoder_A && encoder_A_prev)
        {
            if (encoder_B)
            {
              l++;
              if (l > 25)
                l = 0;
            }
            else
            {
              l--;
              if (l < 0)
                l = 25;
            }

            answer[idx] = 'a' + l;
            lcd.setCursor(3,1);
            for (int i = 0; i < 12; i++)
              lcd.write(answer[i]);
        }

        encoder_A_prev = encoder_A;
    }
    return (answer);
}

int detectNClick(bool pressed)
{
    static bool prev = true;
    int result = 0;

    if (prev && !pressed)
    {
        unsigned long t = millis();
        if (t - lastClickTime > window)
          clickCount = 0;
        clickCount++;
        lastClickTime = t;
    }

    if (millis() - lastClickTime > window && clickCount > 0)
    {
        result = clickCount;
        clickCount = 0;
    }

    prev = pressed;
    return result;
}

void eat()
{
    int n = 0;
    int pos = 3;
    int food = random(3,15);
    unsigned long t0 = millis();

    showMessage("  Catch the food!", 1500);
    while (millis() - t0 < 6000 && n < 9)
    {
        int A = digitalRead(pinA);
        int B = digitalRead(pinB);
        static int Ap = HIGH;

        if (!A && Ap)
        {
          if (B)
            pos++;
          else
            pos--;
          if (pos < 3)
            pos=15;
          if (pos>15)
            pos=3;
        }

        lcd.setCursor(3,1);
        for (int i=3;i<16;i++)
        {
            if (i==food)
              lcd.write('F');
            else if (i==pos)
              lcd.write('O');
            else
              lcd.write(' ');
        }

        Ap = A;
        if (pos == food)
        {
          n++;
          food = random(3,15);
          t0 = millis();
        }
    }

    lcd.setCursor(3,0);
    lcd.print("B-12 ate");
    lcd.setCursor(4,1);
    lcd.write('0' + n);
    lcd.print(" food!");
    delay(1500);
}



void sleep()
{
    unsigned long t0 = millis();

    showMessage("  Dont disturb me!", 1500);
    lcd.setCursor(3,1);
    while (millis() - t0 < 5000)
    {
        int A = digitalRead(pinA);
        static int Ap = HIGH;
        if (!A && Ap) 
        {
          showMessage("  Huh? Let me sleep!", 500);
          t0 = millis();
        } 
        Ap = A;
    }

    lcd.clear();
    showMessage("  Yaaaaaaawwwwwwnnnn", 1500);
    showMessage("Thank you...", 1500);
}

void work()
{
    int target = random(0,26);
    int cur = 0;
    static int Ap = HIGH;

    lcd.clear();
    draw_face(0,0,3,4);
    lcd.setCursor(3,0);
    lcd.print("Work");
    lcd.setCursor(3,1);
    lcd.print("Match: ");

    while (true)
    {
        int A = digitalRead(pinA);
        int B = digitalRead(pinB);

        if (!A && Ap)
        {
            if (B)
              cur++;
            else
              cur--;
            if (cur<0)
              cur=25;
            if (cur>25)
              cur=0;
        }
        Ap = A;

        lcd.setCursor(10,1);
        lcd.write('a' + cur);
        lcd.write(' ');
        lcd.write('(');
        lcd.write('a'+target);
        lcd.write(')');

        int c = detectNClick(digitalRead(pinP));
        if (c==1 && cur==target)
          break;
    }

    showMessage("Good Job!", 1500);
}


void sport()
{
    int reps = 0;
    static int Ap = HIGH;

    lcd.clear();
    lcd.print("Sport");
    lcd.setCursor(0,1);
    lcd.print("Spin fast!");

    unsigned long t0 = millis();
    while (millis() - t0 < 6000)
    {
        int A = digitalRead(pinA);
        int B = digitalRead(pinB);

        if (!A && Ap)
          reps++;
        Ap = A;

        lcd.setCursor(0,1);
        lcd.print("Reps:");
        lcd.print(reps);
        lcd.write(' ');
    }

    lcd.clear();
    lcd.print("Total:");
    lcd.print(reps);
    delay(1500);
}

void loop()
{
  int pA_prev = HIGH;
  int A = digitalRead(pinA);
  // get the current elapsed time
  currentTime = millis();

  if (millis() - blinkTimer > 5000)
  {
    blinkTimer = millis();

    if (blinkState)
    {
        draw_face(0, 0, 1, 2);
        blinkState = false;
    }
    else
    {
        draw_face(3, 4, 1, 2);
        blinkState = true;
    }
  }

  if (!A && pA_prev)
  {
    menu4("work", "eat", "sleep", "sport");
    pA_prev = A;
  }
}