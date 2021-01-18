#define mx1 13
#define mx2 12
#define mz1 11 //Top Motor A
#define mz2 10 //Top Motor B
#define in1 8 //Bottom Motor A FLIPPED
#define in2 9 //Bottom Motor B FLIPPED
#define vert_a 3
#define vert_b 2
#define trigger_top A0
#define echo_top A1
#define trigger_bottom 6
#define echo_bottom 7

String serialInput;
String moveDirection;
int z = 0;
int zl;
int zm;
int chc;
int calibrate_time;
int target_top=40;
int target_top_min=20;
int target_top_max=88;
int target_bottom=100;
float duration_top, distance_top;
float duration_bottom, distance_bottom;
String task[20];
int current_task = 0;
boolean task_input_mode = false;
boolean enable_bottom_motor = false;

void setup() {
  Serial.begin (9600);
  pinMode(mx1, OUTPUT);
  pinMode(mx2, OUTPUT);
  pinMode(mz1, OUTPUT);
  pinMode(mz2, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(vert_a, OUTPUT);
  pinMode(vert_b, OUTPUT);

  pinMode(trigger_top, OUTPUT);
  pinMode(echo_top, INPUT);
  pinMode(trigger_bottom, OUTPUT);
  pinMode(echo_bottom, INPUT);

  //task[0] = "12";
  
  Serial.println("Solanum Systems | v0.2");
  Serial.println("================================");
  Serial.print("Bottom motor enaled : ");
  Serial.println(enable_bottom_motor);
  Serial.println("Status              : READY");
  Serial.println("================================");
}

void loop() {
  if(task[current_task] != "" && task_input_mode == false){
    Serial.println(current_task);
    if(task[current_task].length() == 11){
      Serial.print("Moving to : ");
      Serial.println(task[current_task]);
      if(enable_bottom_motor == true){
        target_bottom = task[current_task].substring(0,3).toInt();
        if(readPositionBottom() != target_bottom){
          while(!moveBottom()){
            moveBottom();
          }
        }
      }
      target_top = task[current_task].substring(4,7).toInt();
      if(readPositionTop() != target_top){
        while(!moveTop()){
          moveTop();
        }
      }
    }
    else if(task[current_task].length() == 2){
      switch(task[current_task].toInt()){
        case 12:
          water();
          Serial.println("Watering");
        break;
        case 10:
          waterToggle(true);
          Serial.println("Water Pump ON");
        break;
        case 11:
          waterToggle(false);
          Serial.println("Water Pump OFF");
        break;
      }
    }
    else if(task[current_task] == ""){
      Serial.println("No task");
    }
    task[current_task] = "";
    current_task++;
  }

  if(Serial.available() > 0){
    serialInput = readSerial();
    serialInput.trim();

    if(task_input_mode == true){
      if(serialInput.toInt() == 8){
        task_input_mode = false;
        current_task = 0;
        Serial.println("Task input ended");
      }
      else{
        task[current_task] = serialInput;
        Serial.print("Task added : ");
        Serial.println(serialInput);
        current_task++;
      }
    }
    else{
      if(serialInput.length() == 2){
        chc = serialInput.toInt();
        //Serial.print(chc);
        switch(chc){
          case 10:
            digitalWrite(mx1, HIGH);
            digitalWrite(mx2, LOW);
          break;
          case 11:
            digitalWrite(mx1, LOW);
            digitalWrite(mx2, HIGH);
          break;
          case 20:
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
          break;
          case 21:
            digitalWrite(in1, LOW);
            digitalWrite(in2, HIGH);
          break;
          case 30:
            digitalWrite(mz1, HIGH);
            digitalWrite(mz2, LOW);
          break;
          case 31:
            digitalWrite(mz1, LOW);
            digitalWrite(mz2, HIGH);
          break;
          case 40:
            digitalWrite(vert_a, LOW);
            digitalWrite(vert_b, HIGH);
          break;
          case 41:
            digitalWrite(vert_a, HIGH);
            digitalWrite(vert_b, LOW);
          break;
          case 50:
            enable_bottom_motor = false;
            Serial.println("Bottom motor disabled");
          break;
          case 51:
            enable_bottom_motor = true;
            Serial.println("Bottom motor enabled");
          break;
          default:
            digitalWrite(mx1, LOW);
            digitalWrite(mx2, LOW);
            digitalWrite(mz1, LOW);
            digitalWrite(mz2, LOW);
            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
            digitalWrite(vert_a, LOW);
            digitalWrite(vert_b, LOW);
          break;
        }
      }
      if(serialInput.length() == 1){
        chc = serialInput.toInt();
        //Serial.print(chc);
        switch(chc){
          case 1:
            readPositionTop();
            Serial.print("TOP : ");
            Serial.println(distance_top);
          break;
          case 2:
            readPositionBottom();
            Serial.print("BOTTOM : ");
            Serial.println(distance_bottom);
          break;
          case 4:
            Serial.println(task[current_task]);
          break;
          case 9:
            task_input_mode = true;
            Serial.println("Task input mode");
            current_task = 0;
          break;
        }
      }
      else{
        Serial.println(serialInput.substring(0,3));
        target_bottom = serialInput.substring(0,3).toInt();
        Serial.println(serialInput.substring(4,7));
        target_top = serialInput.substring(4,7).toInt();
        Serial.println(serialInput.substring(8,11));
        
      }
    }
    
  }
}

String readSerial(){
  return Serial.readString();
}

int readPositionTop(){
  digitalWrite(trigger_top, LOW); 
  delayMicroseconds(2);
 
  digitalWrite(trigger_top, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_top, LOW);
  
  duration_top = pulseIn(echo_top, HIGH);
  distance_top = round((duration_top / 2) * 0.0344);
  return distance_top;
  //Serial.print("TOP : ");
  //Serial.println(distance_top);
}

int readPositionBottom(){
  digitalWrite(trigger_bottom, LOW); 
  delayMicroseconds(2);
 
  digitalWrite(trigger_bottom, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_bottom, LOW);
  
  duration_bottom = pulseIn(echo_bottom, HIGH);
  distance_bottom = round((duration_bottom / 2) * 0.0344);
  return distance_bottom;
  //Serial.print("BOT : ");
  //Serial.println(distance_bottom);
}

boolean moveTop(){
  if(distance_top!=target_top){
    readPositionTop();
    if(distance_top==target_top){
      digitalWrite(mz1, LOW);
      digitalWrite(mz2, LOW);
      delay(300);
      if(distance_top==target_top){
        return true;
      }
    }
    else if(distance_top>target_top){
      digitalWrite(mz1, LOW);
      digitalWrite(mz2, HIGH);
    }
    else if(distance_top<target_top){
      digitalWrite(mz1, HIGH);
      digitalWrite(mz2, LOW);
    }
  }
  return false;
}

boolean moveBottom(){
  if(distance_bottom != target_bottom){
    readPositionBottom();
    if(distance_bottom == target_bottom){
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      delay(300);
      if(distance_bottom == target_bottom){
        return true;
      }
    }
    else if(distance_bottom > target_bottom){
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
    else if(distance_bottom < target_bottom){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }
  }
  return false;
}

void water(){
  digitalWrite(mx1, HIGH);
  delay(5000);
  digitalWrite(mx1, LOW);
}

void waterToggle(boolean state){
  if(state == true){
    digitalWrite(mx1, HIGH);
  }
  else if(state == false){
    digitalWrite(mx1, LOW);
  }
}
