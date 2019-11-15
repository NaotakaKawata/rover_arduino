#include <EEPROM.h>
#include <fuzzy_table.h>
#include <PID_Beta6.h>
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#include <MotorWheel.h>
#include <Omni3WD.h>

// Motors
irqISR(irq1,isr1);
MotorWheel wheel1(9,8,6,7,&irq1);        // Pin9:PWM, Pin8:DIR, Pin6:PhaseA, Pin7:PhaseB
irqISR(irq2,isr2);
MotorWheel wheel2(10,11,14,15,&irq2);    // Pin10:PWM, Pin11:DIR, Pin14:PhaseA, Pin15:PhaseB
irqISR(irq3,isr3);
MotorWheel wheel3(3,2,4,5,&irq3);        // Pin3:PWM, Pin2:DIR, Pin4:PhaseA, Pin5:PhaseB
Omni3WD Omni(&wheel1,&wheel2,&wheel3);

void setup() {
    TCCR1B=TCCR1B&0xf8|0x01;    // Pin9,Pin10 PWM 31250Hz
    TCCR2B=TCCR2B&0xf8|0x01;    // Pin3,Pin11 PWM 31250Hz
    Omni.PIDEnable(0.26,0.02,0,10);
    Serial.begin(19200);
}

void loop() {
    int data;
    if(Serial.available()>0){
      data = Serial.read();
      switch(data){
        case 'r':
          Serial.println("received rotate right");
          Omni.setCarRotateRight(0);
          Omni.setCarSpeedMMPS(100,1000);
          Omni.delayMS(1000,true);
          Omni.setCarSlow2Stop(1000);
          for(int i=0;i<100;++i) {
          //motorのスピードを調整する
          Omni.PIDRegulate();
          delay(10);
          }
          break;
        case 'l':
          Serial.println("received rotate left");
          Omni.setCarRotateLeft(0);
          Omni.setCarSpeedMMPS(100,1000);
          Omni.delayMS(1500,false);
          Omni.setCarSlow2Stop(1000);
          for(int i=0;i<100;++i) {
          //motorのスピードを調整する
          Omni.PIDRegulate();
          delay(10);
          }
          break;
        case 'b':
          Serial.println("received back");
          Omni.setCarBackoff(0);
          Omni.setCarSpeedMMPS(100,1000);
          Omni.delayMS(1500,false);
          Omni.setCarSlow2Stop(1000);
          for(int i=0;i<100;++i) {
          Omni.PIDRegulate();
          delay(10);
          }
          break;
        case 'f':
          Serial.println("received forward");
          Omni.setCarAdvance(0);
          Omni.setCarSpeedMMPS(100,1000);
          Omni.delayMS(1500,false);
          Omni.setCarSlow2Stop(1000);
          for(int i=0;i<100;++i) {
          Omni.PIDRegulate();
          delay(10);
          }
          break;
        case 's':
          Serial.println("received stop");
          Omni.setCarStop();
          Omni.delayMS(3000,false);
          break;
        }
      }
}
