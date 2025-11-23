const int button = 2;
const int R = 9;
const int G = 10;
const int B = 11;

int mode = 0;                // 0~7 前7單色，第8快速閃
bool pressed = false;
unsigned long pressStartTime = 0;
bool breathing = false;       // 長按呼吸燈模式
unsigned long lastBreatheTime = 0;
float brightness = 0;         // 呼吸燈亮度
float fadeAmount = 5;
unsigned long lastFlashTime = 0;

void setup() {
  pinMode(button, INPUT_PULLUP);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  setColor(mode); // 初始顏色
}

void loop() {
  unsigned long currentMillis = millis();
  int state = digitalRead(button);

  // ⭐ 按鈕剛被按下
  if (state == LOW && !pressed) {
    pressed = true;
    pressStartTime = currentMillis;

    if(!breathing){
      mode++;
      if(mode > 7) mode = 0; // 循環到第1顏色

      if(mode < 7) setColor(mode);  // 0~6單色
      else lastFlashTime = currentMillis; // 第8下快速閃爍初始化
    }

    delay(20); // 去彈跳
  }

  // ⭐ 長按觸發呼吸燈
  if(state == LOW && pressed && !breathing){
    if(currentMillis - pressStartTime >= 1500){
      breathing = true;
      brightness = 0;
      fadeAmount = 5;
    }
  }

  // ⭐ 放開按鈕 → 結束呼吸燈
  if(state == HIGH){
    pressed = false;
    if(breathing){
      breathing = false;
      if(mode < 7) setColor(mode);
      else lastFlashTime = currentMillis; // 回到快速閃爍模式
    }
  }

  // ===== 模式控制 =====
  if(mode == 7 && !breathing){
    // 第8下快速閃爍
    if(currentMillis - lastFlashTime >= 200){ // 每200ms切下一顏色
      lastFlashTime = currentMillis;
      modeColorFlash();
    }
  }

  if(breathing){
    // 呼吸燈效果（當前單色）
    if(currentMillis - lastBreatheTime >= 30){
      lastBreatheTime = currentMillis;
      brightness += fadeAmount;
      if(brightness <= 0 || brightness >= 255) fadeAmount = -fadeAmount;

      int r=0,g=0,b=0;
      if(mode < 7){
        switch(mode){
          case 0: r=brightness; break;
          case 1: g=brightness; break;
          case 2: b=brightness; break;
          case 3: r=brightness; g=brightness; break;
          case 4: r=brightness; b=brightness; break;
          case 5: g=brightness; b=brightness; break;
          case 6: r=brightness; g=brightness; b=brightness; break;
        }
      } else {
        // 第8下快速閃顏色呼吸（可套全彩）
        int step = (millis()/200)%7;
        switch(step){
          case 0: r=brightness; g=0; b=0; break;
          case 1: r=0; g=brightness; b=0; break;
          case 2: r=0; g=0; b=brightness; break;
          case 3: r=brightness; g=brightness; b=0; break;
          case 4: r=brightness; g=0; b=brightness; break;
          case 5: r=0; g=brightness; b=brightness; break;
          case 6: r=brightness; g=brightness; b=brightness; break;
        }
      }

      analogWrite(R,r);
      analogWrite(G,g);
      analogWrite(B,b);
    }
  }
}

// ===== 設定單色（短按使用） =====
void setColor(int c){
  switch(c){
    case 0: analogWrite(R,255); analogWrite(G,0); analogWrite(B,0); break;   // 紅
    case 1: analogWrite(R,0); analogWrite(G,255); analogWrite(B,0); break;   // 綠
    case 2: analogWrite(R,0); analogWrite(G,0); analogWrite(B,255); break;   // 藍
    case 3: analogWrite(R,255); analogWrite(G,255); analogWrite(B,0); break; // 黃
    case 4: analogWrite(R,255); analogWrite(G,0); analogWrite(B,255); break; // 紫
    case 5: analogWrite(R,0); analogWrite(G,255); analogWrite(B,255); break; // 青
    case 6: analogWrite(R,0); analogWrite(G,0); analogWrite(B,0); break; // 白
  }
}

// 第8下快速閃顏色
void modeColorFlash(){
  static int flashStep = 0;
  flashStep++;
  if(flashStep>6) flashStep=0;

  switch(flashStep){
    case 0: analogWrite(R,255); analogWrite(G,0); analogWrite(B,0); break;   // 紅
    case 1: analogWrite(R,0); analogWrite(G,255); analogWrite(B,0); break;   // 綠
    case 2: analogWrite(R,0); analogWrite(G,0); analogWrite(B,255); break;   // 藍
    case 3: analogWrite(R,255); analogWrite(G,255); analogWrite(B,0); break; // 黃
    case 4: analogWrite(R,255); analogWrite(G,0); analogWrite(B,255); break; // 紫
    case 5: analogWrite(R,0); analogWrite(G,255); analogWrite(B,255); break; // 青
    case 6: analogWrite(R,0); analogWrite(G,0); analogWrite(B,0); break; // 白
  }
}