#define S0 13
#define S1 12
#define S2 9
#define S3 11
#define sensorOut 5

unsigned int Htime_r;
float Freq_r;
float Ttime_r;
float F_array_r[20];
float F_Sum_r;
float F_r; 
int i_r=0;

// 

unsigned int Htime_g;
float Freq_g;
float Ttime_g;
float F_array_g[20];
float F_Sum_g;
float F_g; 
int i_g=0;

// 

unsigned int Htime_b;
float Freq_b;
float Ttime_b;
float F_array_b[20];
float F_Sum_b;
float F_b; 
int i_b=0;

// 

char c;
float Ftot;
float coeff_r;
float coeff_g;
float coeff_b;
float soglia;
bool calibration=false;
float R;
float G;
float B;
bool sat=false;

void setup() {
pinMode(S0, OUTPUT);
pinMode(S1, OUTPUT);
pinMode(S2, OUTPUT);
pinMode(S3, OUTPUT);
pinMode(6, OUTPUT);
pinMode(4, OUTPUT);
pinMode(sensorOut, INPUT);
digitalWrite(S0,LOW);
digitalWrite(S1,HIGH);                 //Scaling della frequenza di output al 2%
Serial.begin(9600);
}

void loop() {
  
  Measure();
  
  if(Serial.available()!=0){
  c=Serial.read();
  if (c=='c') Calibration();
  }
  
  if(calibration){
    if(!sat){
    R=F_Sum_r*coeff_r;
    G=F_Sum_g*coeff_g;
    B=F_Sum_b*coeff_b;
    Serial.print("R: ");
    Serial.print(R);
    Serial.print(" ");
    Serial.print("G: ");
    Serial.print(G);
    Serial.print(" ");
    Serial.print("B: ");
    Serial.println(B);
    }
    else{
      sat=false;
    }
  }
} 

void ChangeFilter(String filter){
  if (filter=="red"){
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
  }
  if (filter=="green"){
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
  }
  if (filter=="clear"){
    digitalWrite(S2, HIGH);
    digitalWrite(S3, LOW);
  }
  if (filter=="blue"){
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
  }
}

void Calibration() {
    int j;
    Serial.println("Metti il cartoncino bianco...");
    delay(3000);
    Serial.println("Calibrando..");
    delay(1000);
    Ftot=F_Sum_b+F_Sum_r+F_Sum_g;
    coeff_r=(33.33)/F_Sum_r;
    coeff_g=(33.33)/F_Sum_g;
    coeff_b=(33.33)/F_Sum_b;
    calibration=true;
}

void Measure() {
  
  noInterrupts();
  
  ChangeFilter("red");
  Htime_r = pulseIn(sensorOut,HIGH);      
  Ttime_r = Htime_r *2.0;                   
  Freq_r = 1000/Ttime_r;           
  F_array_r[i_r] = Freq_r;
  if (i_r < 20) i_r++;
  else i_r = 0;
  F_Sum_r=0;
  for (int j=0; j<20; j++) {
    F_Sum_r += F_array_r[j];
  }
  F_Sum_r = F_Sum_r / 20;
  if(F_Sum_r >12) {
    Serial.println("Condizione di saturazione, allontana il cartoncino dal sensore!");
    digitalWrite(4,HIGH);
    sat=true;
  }
  else{
    digitalWrite(4,LOW);
  }
  
  ChangeFilter("green");
  Htime_g = pulseIn(sensorOut,HIGH);      
  Ttime_g = Htime_g *2.0;                   
  Freq_g = 1000/Ttime_g;                
  F_array_g[i_g] = Freq_g;
  if (i_g < 20) i_g++;
  else i_g = 0;
  F_Sum_g=0;
  for (int j=0; j<20; j++) {
    F_Sum_g += F_array_g[j];
  }
  F_Sum_g = F_Sum_g / 20;
  if(F_Sum_g>12) {
    Serial.println("Condizione di saturazione, allontana il cartoncino dal sensore!");
    digitalWrite(4,HIGH);
    sat=true;
  }
  else{
    digitalWrite(4,LOW);
  }
  
  ChangeFilter("blue");
  Htime_b = pulseIn(sensorOut,HIGH);    
  Ttime_b = Htime_b *2.0;                   
  Freq_b = 1000/Ttime_b;         
  F_array_b[i_b] = Freq_b;
  if (i_b < 20) i_b++;
  else i_b = 0;
  F_Sum_b=0;
  for (int j=0; j<20; j++) {
    F_Sum_b += F_array_b[j];
  }
  F_Sum_b = F_Sum_b / 20;
  if(F_Sum_b>12) {
    Serial.println("Condizione di saturazione, allontana il cartoncino dal sensore!");
    digitalWrite(4,HIGH);
    sat=true;
  }
  else{
    digitalWrite(4,LOW);
  }
  
  interrupts();
  
}
