#define rfor 27
#define rbac 14
#define lfor 26
#define lbac 25
#define rpwm 23
#define lpwm 5
#define led 10
#define buzz 11

int LFSensor[5] = {0, 0, 0, 0, 0};
char path[20] = "LLSS";

int Kp = 30;
int Kd = 1;
int Ki = 0;
int P = 0;
int I = 0;
int D = 0;
int error = 0;
int prevError = 0;
int PIDvalue = 0;

int basespeed = 200;
int rspeed = 0;
int lspeed = 0;

int turnspeed = 190;
int inchDelay = 30;
int turnDelay = 700;

int flag = 0;
int ind = 3;

const int freq = 5000;
const int rchannel = 0;
const int lchannel = 1;
const int resolution = 8;

void readSensors()
{
  LFSensor[0] = digitalRead(22);
  LFSensor[1] = digitalRead(19);
  LFSensor[2] = digitalRead(21);
  LFSensor[3] = digitalRead(18);
  LFSensor[4] = digitalRead(15);

  Serial.println("ir read");
  delay(2000);
}

void straight()
{
  digitalWrite(rfor, HIGH);
  digitalWrite(lfor, HIGH);
  digitalWrite(rbac, LOW);
  digitalWrite(lbac, LOW);
  ledcWrite(rchannel, basespeed);
  ledcWrite(lchannel, basespeed);
}

void stop()
{
  digitalWrite(rfor, LOW);
  digitalWrite(lfor, LOW);
  digitalWrite(rbac, LOW);
  digitalWrite(lbac, LOW);
  ledcWrite(rchannel, 0);
  ledcWrite(lchannel, 0);
}

void runExtraInch()
{
  straight();
  delay(inchDelay);
  stop();
  readSensors();
}

void turnRight()
{
  digitalWrite(rfor, LOW);
  digitalWrite(lfor, HIGH);
  digitalWrite(rbac, HIGH);
  digitalWrite(lbac, LOW);
  ledcWrite(rchannel, turnspeed);
  ledcWrite(lchannel, turnspeed);
  delay(turnDelay);
  stop();
  readSensors();
  lineFollow();
}

void turnLeft()
{
  digitalWrite(lfor, LOW);
  digitalWrite(rfor, HIGH);
  digitalWrite(lbac, HIGH);
  digitalWrite(rbac, LOW);
  ledcWrite(rchannel, turnspeed);
  ledcWrite(lchannel, turnspeed);
  delay(turnDelay);
  stop();
  readSensors();
  lineFollow();
}

void itIsNode()
{
    runExtraInch();
    digitalWrite(led, HIGH);
    digitalWrite(buzz, HIGH);
    delay(1000);
  if(path[ind] == 'L')
  {
    turnLeft();
    ind--;
  }
  else if(path[ind] == 'R')
  {
    turnRight();
    ind--;
  }
  else if(path[ind] == 'S')
  {
    straight();
    ind--;
  }
}

void checkNode()
{
  // if((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 1 )) 
  // {
  //   if(flag == 0)
  //   {
  //     flag = 1;
  //   }
  //   else 
  //   {
  //     flag = 0;
  //   }
  //     readSensors();
  //     while(((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 1 )) || ((LFSensor[0]== 0 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 0 )))
  //     {
  //       readSensors();
  //     }
  // }
  // else if((LFSensor[0]== 0 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 0 ))
  // {
  //   if(flag == 0)
  //   {
  //     flag = 1;
  //   }
  //   else 
  //   {
  //     flag = 0;
  //   }
  //     readSensors();
  //     while(((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 1 )) || ((LFSensor[0]== 0 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 0 )))
  //     {
  //       readSensors();
  //     }
  // }
  if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 ))
  {
    itIsNode();

    Serial.println("left turn");
  }
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 ))
  {
    itIsNode();

    Serial.println("right turn");
  }
  // else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 ))
  // {
  //   if(flag == 0)
  //   {
  //     stop();
  //   }
  // }
  else
  {
    Serial.println("no turn");
    delay(2000);
    
    lineFollow();
  }
}

void calerror1()
{
  if((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 1 )) 
  error = 0;
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 1 )) 
  error = -1;
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 )) 
  error = -2;
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 0 )) 
  error = -3;
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 0 )) 
  error = -4;
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  error = 1;
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  error = 2;
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  error = 3;
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  error = 4;
}  

void calerror2()
{
  if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 1 )) 
  error = 4;
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 )) 
  error = 3; 
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 0 )) 
  error = 2;
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 0 )) 
  error = 1;
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  {
  error = 0;
  Serial.println("error 0");
  delay(2000);
  }
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  error = -1;
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  error = -2;
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  error = -3;
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  error = -4;
}

void calculatePID()
{
  P = error;
  I = I + error;
  D = error-prevError;
  PIDvalue = (Kp*P) + (Ki*I) + (Kd*D);
  prevError = error;

  Serial.println(error);
  delay(2000);
}


void move()
{
  rspeed = basespeed - PIDvalue;
  lspeed = basespeed + PIDvalue;

  digitalWrite(rfor, HIGH);
  digitalWrite(lfor, HIGH);
  digitalWrite(rbac, LOW);
  digitalWrite(lbac, LOW);
  ledcWrite(rchannel, rspeed);
  ledcWrite(lchannel, lspeed);

  Serial.println(rspeed);
  Serial.println(lspeed);
  delay(2000);
}

void lineFollow()
{
  // if(flag == 0)
  //   calerror2();
  // else
  //   calerror1();

  calerror2();

  calculatePID();
  move();
}

void setup() {
  // put your setup code here, to run once:
  pinMode(23, INPUT);
  pinMode(22, INPUT);
  pinMode(21, INPUT);
  pinMode(2, INPUT);
  pinMode(15, INPUT);

  pinMode(rfor, OUTPUT);
  pinMode(rbac, OUTPUT);
  pinMode(lfor, OUTPUT);
  pinMode(lbac, OUTPUT);
  pinMode(rpwm, OUTPUT);
  pinMode(lpwm, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(buzz, OUTPUT);

  Serial.begin(9600);

  ledcSetup(rchannel, freq, resolution);
  ledcSetup(lchannel, freq, resolution);
  ledcAttachPin(rpwm, rchannel);
  ledcAttachPin(lpwm, lchannel);

  digitalWrite(rfor, HIGH);
  digitalWrite(lfor, HIGH);
  digitalWrite(rbac, LOW);
  digitalWrite(lbac, LOW);
  ledcWrite(rchannel, basespeed);
  ledcWrite(lchannel, basespeed);
  Serial.println("setup complete");

  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  readSensors();
  checkNode();
  Serial.println("first loop complete");
  delay(2000);
}
