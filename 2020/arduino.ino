#include <Servo.h> // 서보 라이브러리를 불러옵니다.

Servo horizontal; //가로축 서보를 설정합니다.
int servoh = 90; // 가로축 서보의 기본값은 90으로 설정합니다.
Servo vertical; // 세로축 서보를 설정합니다.
int servov = 90; // 세로축 서보의 기본값은 90으로 설정합니다.


// 조도센서의 이름 = 아날로그 핀번호;
int ldrlt = 3; //LDR 왼쪽 위
int ldrrt = 0; //LDR 오른쪽 위
int ldrld = 2; //LDR 왼쪽 아래
int ldrrd = 1; //ldr 오른쪽 아래

byte arr[2]={0,0};

unsigned long time1=0;

void calcul(int n){
   arr[0] = n/4;
   arr[1] = n%4;
 }

void setup() {
  Serial.begin(9600);
  horizontal.attach(9);
  vertical.attach(10);
}



void loop() {
  int lt = analogRead(ldrlt); // 왼쪽 탑
  int rt = analogRead(ldrrt); // 오른쪽 탑
  int ld = analogRead(ldrld); // 왼쪽 다운
  int rd = analogRead(ldrrd); // 오른쪽 다운

  int dtime = 15; 
  int tol = 10; 

  int avt = (lt + rt) / 2; // 위쪽의 평균값을 구합니다.
  int avd = (ld + rd) / 2; // 아래쪽의 평균값을 구합니다.
  int avl = (lt + ld) / 2; // 왼쪽의 평균값을 구합니다.
  int avr = (rt + rd) / 2; // 오른쪽의 평균값을 구합니다.

  int dvert = avt - avd; // 위쪽과 아래쪽의 차이를 구합니다.
  int dhoriz = avl - avr;// 왼쪽과 오른쪽의 차이를 구합니다.

 if(millis() - time1 > 2000){
   calcul(lt);
   Serial.write(arr,2); 
   delay(10);
   calcul(rt);
   Serial.write(arr,2); 
   delay(10);
   calcul(ld);
   Serial.write(arr,2); 
   delay(10);
   calcul(rd);
   Serial.write(arr,2); 
   time1=millis();
}


  if (-1 * tol > dvert || dvert > tol) { // check if the diffirence is in the tolerance else change vertical angle
    if (avt < avd) {
      servov = ++servov;
      if (servov > 180) {
        servov = 180;
      }
    }
    else if (avt > avd) {
      servov = --servov;
      if (servov < 0) {
        servov = 0;
      }
    }
    vertical.write(servov);
  }



  if (-1 * tol > dhoriz || dhoriz > tol) {// check if the diffirence is in the tolerance else change horizontal angle

    if (avl > avr) {

      servoh = --servoh;

      if (servoh < 0) {

        servoh = 0;

      }

    }

    else if (avl < avr) {

      servoh = ++servoh;

      if (servoh > 180) {

        servoh = 180;

      }

    }

    else if (avl == avr) {

      // nothing

    }

    horizontal.write(servoh);

  }

  delay(dtime);

}
