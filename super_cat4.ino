#include <Wire.h>
#include "adxl.h"
#include "l3g.h"

#define X1 2
#define X2 3
#define Y1 5
#define Y2 6

#define C1 10
#define C2 11
#define C3 12
#define C3_h 2035
#define C3_l 1220
#define C4 13

void setup() {
  analogWriteResolution(16);
  pinMode(X1, OUTPUT);pinMode(X2, OUTPUT);
  pinMode(Y1, OUTPUT);pinMode(Y2, OUTPUT);
  analogWrite(X1,0);analogWrite(X2,0);
  analogWrite(Y1,0);analogWrite(Y2,0);
  
  pinMode(C1, INPUT);pinMode(C2, INPUT);
  pinMode(C3, INPUT);pinMode(C4, INPUT);
  
  pinMode(22, OUTPUT);
  digitalWrite(22, LOW);delay(100);
  digitalWrite(22, HIGH);delay(100);
  
  Wire.begin();
  initADXL();
  initL3G4200D();
  
  Serial.begin(19200);delay(300);
  Serial.println("------------------");
}

void adxl2angle(int arr[]){
  double sin_x = arr[0] / 236.173; // 167 * sqrt(2)
  double sin_y = arr[1] / 264.4579; // 187 * sqrt(2)
  sin_x = constrain(sin_x, -1.0, 1.0);
  sin_y = constrain(sin_y, -1.0, 1.0);
  arr[1] = (int)(asin(sin_x) * 57.2957795);
  arr[0] = (int)(asin(sin_y) * 57.2957795);
}

void kalman(int arr[]){
  static double kalman_q = 0.01;
  static double kalman_r = 0.1;
  static double kalman_p[] = {1.0,1.0,1.0};
  static double kalman_last[] = {1.0,1.0,1.0};
  
  for (int i;i<3;i++){
    double d = arr[i];
    double _p = kalman_p[i] + kalman_q;
    
    double k = _p / (_p + kalman_r);
    kalman_last[i] = kalman_last[i] + k * (d - kalman_last[i]);
    kalman_p[i] = (1-k) * _p;
    
    arr[i] = (int) kalman_last[i];
  }
}

int getC3(){
  int n = pulseIn(C3, HIGH);
  return map(constrain(n,C3_l, C3_h), C3_l, C3_h, 1000, 0);
//  Serial.print(n, DEC);
//  Serial.print(',');
//  Serial.println( 
//    map(constrain(n,C3_l, C3_h), C3_l, C3_h, 1000, 0), 
//    DEC);
}

void get_pose(int arr[], long int larr[]) {
  static int i = 0;
  i++;
  
  L3G4200D_read(arr);
  larr[0] += arr[0];
  larr[1] += arr[1];
  larr[2] += arr[2];
  
  if (i % 40 == 0) {
    ADXL_read(arr);
    kalman(arr);
    
    int ax = (int)(larr[0] / 52000.0 /2.0);
    int ay = (int)(larr[1] / 52000.0 /2.0);
    
    adxl2angle(arr);
    if (ax != arr[0]){
      larr[0] += (ax > arr[0]) ? -50000 : 50000;
    }
    if (ay != arr[1]){
      larr[1] += (ay > arr[1]) ? -50000 : 50000;
    }
  }
  
  if (false){//i % 40 == 0) {
    Serial.print(  (int)(larr[0] / 52000.0 /2.0), DEC);Serial.print(", ");
    Serial.print(  (int)(larr[1] / 52000.0 /2.0), DEC);Serial.print(", ");
    Serial.println((int)(larr[2] / 52000.0 /2.0), DEC);
//    adxl2angle(arr);
//    Serial.print(  arr[0], DEC);Serial.print(", ");
//    Serial.print(  arr[1], DEC);Serial.print(", ");
//    Serial.println(i, DEC);
  }
  //delay(5);
}

void loop(){
  static int val = 0;
  static int i=0;
  int arr[3] = {0};
  static long larr[3] = {0};
  int delta;
  
  get_pose(arr, larr);
  
  delta = constrain(larr[1] / 10000.0, -1000, 1000);
  
  if (i%20 == 0) {
    val = getC3() * 10;
    Serial.println(delta);
  }
  
  analogWrite(X1,constrain(val-delta,0,20000));
  analogWrite(X2,constrain(val+delta,0,20000));
  i++;
}

void loop4(){
  int val = getC3() * 10;
  Serial.println(val);
  analogWrite(X1,val);analogWrite(X2,val);
  //analogWrite(Y1,val);analogWrite(Y2,val);
  delay(100);
}

void loop3(){
  static char buf[64] = {'\0'};
  static int buf_idx = 0;
  static int val = 0;

  if (Serial.available()) {
    char c = Serial.read();
    if (c >= '0' && c <= '9'){
      buf[buf_idx] = c;
      buf_idx++;
    }else{
      buf[buf_idx] = '\0';
      if (buf_idx > 0){
        val = 0;
        for (int i=0;i<buf_idx;i++)
          val = val * 10 + (buf[i] - '0');
        buf_idx = 0;
        
        Serial.println(val);
      }
    }
  }
  analogWrite(X1,val);analogWrite(X2,val);
  analogWrite(Y1,val);analogWrite(Y2,val);
}
