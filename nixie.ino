#include <Arduino.h>
#include <Wire.h>
#include <DS1307.h>
#include <PCF8574.h>


PCF8574 pcf20(0x20);
DS1307 clock;
RTCDateTime dt;

int j;
int k;
int zmienna1 = 0;
int zmienna2 = 0;
int mode_counter = 0;
int godziny = 0;
int minuty = 0 ;
int mode = 0;
int blink = 0;


bool min = false;
bool hr = false;
bool i = true;
bool y = false;
bool up;
bool down;
bool prog;
bool first;
bool second;

unsigned long zapamietanyCzas = 0;
unsigned long aktualnyCzas = 0;

const int q = 14;
const int w = 15;
const int e = 17;
const int nixie_0 = 2;
const int nixie_1 = 3;
const int nixie_2 = 4;
const int nixie_3 = 5;
const int nixie_4 = 6;
const int nixie_5 = 7;
const int nixie_6 = 8;
const int nixie_7 = 9;
const int nixie_8 = 10;
const int nixie_9 = 11;
const int time_on = 2;



void setup() {

Wire.begin();

pcf20.begin(OUTPUT);

pinMode(e, INPUT_PULLUP);
pinMode(nixie_0, OUTPUT);
pinMode(nixie_1, OUTPUT);
pinMode(nixie_2, OUTPUT);
pinMode(nixie_3, OUTPUT);
pinMode(nixie_4, OUTPUT);
pinMode(nixie_5, OUTPUT);
pinMode(nixie_6, OUTPUT);
pinMode(nixie_7, OUTPUT);
pinMode(nixie_8, OUTPUT);
pinMode(nixie_9, OUTPUT);
pinMode(w, INPUT_PULLUP);
pinMode(q, INPUT_PULLUP);


clock.begin();

}

void on_number(int row, int nixie){
pcf20.write(row, HIGH);
digitalWrite(nixie, HIGH);
}


void off_all(){
pcf20.write(0, LOW);
pcf20.write(1, LOW);
pcf20.write(2, LOW);
pcf20.write(3, LOW);
digitalWrite(nixie_0, LOW);
digitalWrite(nixie_1, LOW);
digitalWrite(nixie_2, LOW);
digitalWrite(nixie_3, LOW);
digitalWrite(nixie_4, LOW);
digitalWrite(nixie_5, LOW);
digitalWrite(nixie_6, LOW);
digitalWrite(nixie_7, LOW);
digitalWrite(nixie_8, LOW);
digitalWrite(nixie_9, LOW);


}

void loop()
{
dt = clock.getDateTime();

if(i == true || y == true)
{

clock.setDateTime(2014, 4, 13, godziny, minuty , 00);
i = false;
y = false;
mode = 0;
}
if(min==false)
{

j = dt.minute/10;
k = dt.minute%10;
off_all();
on_number(3,k+2);
delay(time_on);

off_all();
on_number(2,j+2);
delay(time_on);
}
else{
if(blink < 8 && first == true){
off_all();
delay(time_on);
blink++;
}
else{
j = minuty/10;
k = minuty%10;
off_all();
on_number(3,k+2);
delay(time_on);

off_all();
on_number(2,j+2);
delay(time_on);
if (blink < 16) blink++;
else blink = 0;
}
}

if(hr ==false)
{

j = dt.hour/10;
k = dt.hour%10;
off_all();
on_number(1,k+2);
delay(time_on);

off_all();
on_number(0,j+2);
delay(time_on);
}

else{
if(blink < 8 && second == true){
off_all();
delay(time_on);
blink++;
}
else{
j = godziny/10;
k = godziny%10;

off_all();
on_number(1,k+2);
delay(time_on);

off_all();
on_number(0,j+2);
delay(time_on);
}
}

up = digitalRead(q);
down = digitalRead(w);
prog = digitalRead(e);

if(up == 0 && down == 0 && mode == 0)
{
mode = 1;

}

if(mode==1){
if (mode_counter < 200){
mode_counter++;
}
else{
mode_counter=0;
mode=2;
}
min= true;
hr = false;
first = true;
second = false;
}

if(mode==2){
if (mode_counter < 200){
mode_counter++;
}
else{
mode_counter=0;
mode=3;
}
min = true;
hr = true;
first = false;
second = true;
}

if(mode==3){
min = false;
hr = false;
y = true;
first = false;
second = false;
}

if(up == 0 && mode == 1)
{
aktualnyCzas = millis();
if (aktualnyCzas - zapamietanyCzas >= 200UL) {
zapamietanyCzas = aktualnyCzas;
zmienna1++;
}
mode_counter = 0;
if(zmienna1 > 59){ zmienna1 = 0;}
minuty = zmienna1;
}

if(down == 0 && mode == 1)
{
aktualnyCzas = millis();
if (aktualnyCzas - zapamietanyCzas >= 200UL) {
zapamietanyCzas = aktualnyCzas;
zmienna1--;
}
mode_counter = 0;
if(zmienna1 < 0){ zmienna1 = 59;}
minuty = zmienna1;
}

if(up == 0 && mode ==2)
{
aktualnyCzas = millis();
if (aktualnyCzas - zapamietanyCzas >= 200UL) {
zapamietanyCzas = aktualnyCzas;
zmienna2++;
}
mode_counter = 0;
if(zmienna2 > 24){ zmienna2 = 0;}
godziny= zmienna2;
}

if(down == 0 && mode == 2)
{

aktualnyCzas = millis();
if (aktualnyCzas - zapamietanyCzas >= 200UL) {
zapamietanyCzas = aktualnyCzas;
zmienna2--;
}
mode_counter = 0;
if(zmienna2 < 0){ zmienna2 = 24;}
godziny = zmienna2;
}

}