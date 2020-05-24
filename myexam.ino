#include <Wire.h>
#include <LiquidCrystal_I2C_Spark.h>

// D0 and D1 connected to SDA and SCL
int andreasButton = D2;
int erikButton = D3;
int jennyButton = D4;
int sanderButton = D5;
int fellesButton = D6;
int led = D7; // NC
unsigned int activatedTime = 0;
unsigned int currentTime = 0;
bool done = true;
LiquidCrystal_I2C *lcd;

void setup() 
{
    // Buttons setup
    pinMode(led, OUTPUT);
    pinMode(andreasButton, INPUT_PULLUP);
    pinMode(erikButton, INPUT_PULLUP);
    pinMode(jennyButton, INPUT_PULLUP);
    pinMode(sanderButton, INPUT_PULLUP);
    pinMode(fellesButton, INPUT_PULLUP);
    // LCD setup
    lcd = new LiquidCrystal_I2C(0x27, 16, 2);
    lcd->init();
    lcd->backlight();
    lcd->clear();
    lcd->setCursor(2,0);
    lcd->print("Hvem vasker?"); // If loose power and reset itself.
}

void buttonPressed(String name)
{
    if(name == "ANDREAS")
    {
        Particle.publish("andreasState","Pressed",60,PRIVATE); // IFTTT + ttl 60s + private
    }
    if(name == "ERIK")
    {
        Particle.publish("erikState","Pressed",60,PRIVATE); // IFTTT + ttl 60s + private
    }
    if(name == "JENNY")
    {
        Particle.publish("jennyState","Pressed",60,PRIVATE); // IFTTT + ttl 60s + private
    }
    if(name == "SANDER")
    {
        Particle.publish("sanderState","Pressed",60,PRIVATE); // IFTTT + ttl 60s + private
    }
    if(name == "FELLES")
    {
        Particle.publish("fellesState","Pressed",60,PRIVATE); // IFTTT + ttl 60s + private
    }
    digitalWrite(led, HIGH);
    lcd->clear();
    lcd->setCursor(5,0);
    lcd->print(name);
    lcd->setCursor(5,1);
    lcd->print("VASKER");
    activatedTime = Time.now();
    done = false;
    delay(5000); // Delay 5s for not getting emailed spammed and led light on to give feedback to user.
}

void loop() 
{
    // More button setup
    int andreasState = digitalRead(andreasButton);
    int erikState = digitalRead(erikButton);
    int jennyState = digitalRead(jennyButton);
    int sanderState = digitalRead(sanderButton);
    int fellesState = digitalRead(fellesButton);
    
    if((currentTime-activatedTime >= 10800) && (done == false)) // 10800 = 3 hours
    {
            lcd->setCursor(5,1); // Replaces "VASKER"
            lcd->print("FERDIG");
            Particle.publish("VASK","FERDIG",60,PRIVATE); // IFTTT + ttl 60s + private
            done = true;
            delay(2000); // Delay 2s for not getting emailed spammed
    }
    if(andreasState == LOW)
    {
        buttonPressed("ANDREAS");
    }
    else if(erikState == LOW)
    {
        buttonPressed("ERIK");
    }
    else if(jennyState == LOW)
    {
        buttonPressed("JENNY");
    }
    else if(sanderState == LOW)
    {
        buttonPressed("SANDER");
    }
    else if(fellesState == LOW)
    {
        buttonPressed("FELLES");
    }
    else
    {
        digitalWrite(led, LOW);
        currentTime = Time.now(); // Always get time updated
    }
}