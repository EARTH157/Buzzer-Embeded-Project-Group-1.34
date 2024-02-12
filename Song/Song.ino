#define NOTE_C4  523
#define NOTE_D4  587
#define NOTE_E4  659
#define NOTE_F4  698
#define NOTE_G4  784
#define NOTE_A4  880
#define NOTE_B4  987
#define NOTE_C5  1047
#define NOTE_REST 1

int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_C4,
  NOTE_REST,
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_C4,
  NOTE_REST,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_D4,
  NOTE_REST,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_D4,
  NOTE_REST,
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_C4,
  NOTE_REST
};

int noteDurations[] = {
  4, 4, 4, 4,
  4, 4, 2,
  4, 4, 4, 4,
  4, 4, 2,
  2,
  4, 4, 4, 4,
  4, 4, 2,
  2,
  4, 4, 4, 4,
  4, 4, 2,
  2,
  4, 4, 4, 4,
  4, 4, 2,
  2,
  4, 4, 4, 4,
  4, 4, 2,
  4, 4, 4, 4,
  4, 4, 2,
  2
};

hw_timer_t *My_timer = NULL;

#define ledPin 14
float speedFactor = 1.0; // ค่าเริ่มต้นสำหรับความเร็วของเพลง
#define BUTTON 0

void IRAM_ATTR onButton() {
  speedFactor += 0.25;
  if(speedFactor > 2.0) { 
    speedFactor = 0.25;
  }
}

void IRAM_ATTR onTimer(){
  digitalWrite(32, !digitalRead(32));
}

double Feq(double t){
  return 1000000/(t*2);
}

void setup() {
  pinMode(BUTTON, INPUT); // เซ็ตขา 2 เป็นขา Input และเปิดใช้ Pull-up resistor
  pinMode(32, OUTPUT);
  Serial.begin(9600); // เริ่มต้นการสื่อสารทาง Serial
  attachInterrupt(BUTTON, &onButton, RISING); // ผูกการตั้งค่าลงในการเรียกใช้ฟังก์ชัน onButton เมื่อมีการคลิกปุ่ม
  ledcSetup(0, 5000, 8); // กำหนดขาของ LEDC, ความถี่ (Hz), ขนาดบิตข้อมูล
  ledcAttachPin(ledPin, 0);
  My_timer = timerBegin(0, 80, true); 
  timerAttachInterrupt(My_timer, &onTimer, true);
}

void loop() {
  for (int thisNote = 0; thisNote < (sizeof(melody) / sizeof(int)); thisNote++) {
    int noteDurationValue = (int)(1000 / noteDurations[thisNote] * speedFactor);
    timerAlarmWrite(My_timer, Feq(melody[thisNote]), true);

    int pauseBetweenNotes = (int)(noteDurationValue * 1.30);
    delay(pauseBetweenNotes);

    timerAlarmEnable(My_timer);
  }
  // ledcWrite(0, 128);
}


