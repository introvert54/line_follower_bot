#define rfor 3
#define rbac 4
#define lfor 5
#define lbac 6
#define rpwm 9
#define lpwm 10
#define startbutt 7
#define led 8

int LFSensor[6] = {0, 0, 0, 0, 0, 0};

char path[30];
int index = 0;
int traverse = 0;
int end = 0;
int nodeflag = 0;

int Kp = 1;
int Kd = 1;
int Ki = 1;
int P = 0;
int I = 0;
int D = 0;
int error = 0;
int prevError = 0;
int PIDvalue = 0;

int basespeed = 200;
int rspeed = 0;
int lspeed = 0;
int turnspeed = 150;
int inchDelay = 20;
int turnDelay = 700;
int uturnDelay = 1400;

void readSensors()
{
  LFSensor[0] = digitalRead(11);
  LFSensor[1] = digitalRead(12);
  LFSensor[2] = digitalRead(13);
  LFSensor[3] = digitalRead(0);
  LFSensor[4] = digitalRead(1);
  LFSensor[5] = digitalRead(2);
}

void checkNode()
{
  if(LFSensor[5] == 1)
  {
    path[index] = 'U';
    index++;
    uturn();
  }
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 )) 
  {
    runExtraInch();
    if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 ))
    {
      path[index] = 'S';
      index++;
    }
    else
    {
      turnRight();
    }
  }
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  {
    runExtraInch();
    if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 ))
    {
      path[index] = 'L';
      index++;
    }
    turnLeft();
  }
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 )) 
  {
    runExtraInch();
    if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 )) 
    {
      end = 1;
      digitalWrite(led, HIGH);
      stop();
    }
    else
    {
      path[index] = 'L';
      index++;
      turnLeft();
    }
  }
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  {
    path[index] = 'U';
    index++;
    uturn();
  }
  else
  {
    lineFollow();
  }
}


void runExtraInch()
{
  straight();
  delay(inchDelay);
  stop();
  readSensors();
}


void straight()
{
  digitalWrite(rfor, HIGH);
  digitalWrite(lfor, HIGH);
  digitalWrite(rbac, LOW);
  digitalWrite(lbac, LOW);
  analogWrite(rpwm, basespeed);
  analogWrite(lpwm, basespeed);
}

void stop()
{
  digitalWrite(rfor, LOW);
  digitalWrite(lfor, LOW);
  digitalWrite(rbac, LOW);
  digitalWrite(lbac, LOW);
  analogWrite(rpwm, 0);
  analogWrite(lpwm, 0);
}

void turnRight()
{
  digitalWrite(rfor, LOW);
  digitalWrite(lfor, HIGH);
  digitalWrite(rbac, HIGH);
  digitalWrite(lbac, LOW);
  analogWrite(rpwm, turnspeed);
  analogWrite(lpwm, turnspeed);
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
  analogWrite(rpwm, turnspeed);
  analogWrite(lpwm, turnspeed);
  delay(turnDelay);
  stop();
  readSensors();
  lineFollow();
}

void uturn()
{
  digitalWrite(lfor, LOW);
  digitalWrite(rfor, HIGH);
  digitalWrite(lbac, HIGH);
  digitalWrite(rbac, LOW);
  analogWrite(rpwm, turnspeed);
  analogWrite(lpwm, turnspeed);
  delay(uturnDelay);
  stop();
  readSensors();
  lineFollow();
}

void calerror()
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
  error = 0;
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  error =- 1;
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
}

void move()
{
  rspeed = basespeed - PIDvalue;
  lspeed = basespeed + PIDvalue;

  digitalWrite(rfor, HIGH);
  digitalWrite(lfor, HIGH);
  digitalWrite(rbac, LOW);
  digitalWrite(lbac, LOW);
  analogWrite(rpwm, rspeed);
  analogWrite(lpwm, lspeed);
}


void lineFollow()
{
  calerror();
  calculatePID();
  move();
}


int isnode()
{
  if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 )) 
  {
    runExtraInch();
    if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 ))
    {
      return 1;
    }
    else
    {
      turnRight();
    }
  }
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  {
    runExtraInch();
    if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 ))
    {
      return 1;
    }
    turnLeft();
  }
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 )) 
  {
    runExtraInch();
    if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(LFSensor[3]== 1 )&&(LFSensor[4]== 1 )) 
    {
      end = 0;
      stop();
    }
    else
    {
      return 1;
    }
  }
  // else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(LFSensor[3]== 0 )&&(LFSensor[4]== 0 )) 
  // {
  //   runInch();
  //   uturn();
  // }
  else
  {
    lineFollow();
  }
  return 0;
}


void adjustPath()
{
  if(index > 2 && path[index-2] == 'U')
  {
    if(path[index - 3] == 'L' && path[index - 1] == 'L')
    {
      path[index - 3] = 'S';
      index -= 2;
    }
    else if(path[index - 3] == 'L' && path[index - 1] == 'R')
    {
      path[index - 3] = 'U';
      index -= 2;
    }
    else if(path[index - 3] == 'L' && path[index - 1] == 'S')
    {
      path[index - 3] = 'R';
      index -= 2;
    }
    else if(path[index - 3] == 'S' && path[index - 1] == 'L')
    {
      path[index - 3] = 'R';
      index -= 2;
    }
    else if(path[index - 3] == 'S' && path[index - 1] == 'S')
    {
      path[index - 3] = 'U';
      index -= 2;
    }
    else if(path[index - 3] == 'S' && path[index - 1] == 'R')
    {
      path[index - 3] = 'L';
      index -= 2;
    }
    else if(path[index - 3] == 'R' && path[index - 1] == 'L')
    {
      path[index - 3] = 'U';
      index -= 2;
    }
    else if(path[index - 3] == 'R' && path[index - 1] == 'S')
    {
      path[index - 3] = 'L';
      index -= 2;
    }
    else if(path[index - 3] == 'R' && path[index - 1] == 'R')
    {
      path[index - 3] = 'S';
      index -= 2;
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(startbutt) == 1)
  {
    //delay(200);
    if(end == 0)
    {
      readSensors();
      //lineFollow();
      checkNode();
      adjustPath();
    }
    else
    {
      readSensors();
      nodeflag = isnode();
      if(nodeflag == 1)
      {
        if(path[traverse] == 'L')
        {
          turnLeft();
          traverse++;
        }
        else if(path[traverse] == 'R')
        {
          turnRight();
          traverse++;
        }
        else
        {
          traverse++;
        }
        lineFollow();
      }
    }
  }
  else{}
}
