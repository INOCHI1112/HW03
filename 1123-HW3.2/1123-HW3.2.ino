const int ledPin = 3;      // LED 接在 D3
const int buttonPin = 2;   // 按鈕接在 D2

int ledState = HIGH;       // LED 初始狀態
int mode = 0;              // LED 模式：0=恆亮, 1=慢閃, 2=中閃, 3=快閃

int buttonState;           // 當前按鈕狀態
int lastButtonState = LOW; // 上一次按鈕狀態

unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;    // 去抖動時間 50ms

unsigned long previousMillis = 0;    // 記錄上次閃爍時間

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // 按鈕用 INPUT_PULLUP
  digitalWrite(ledPin, ledState);    // 初始 LED 狀態
}

void loop() {
  int reading = digitalRead(buttonPin);

  // 去抖動判斷
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // 按鈕狀態改變
    if (reading == LOW && buttonState == HIGH) { // 按下按鈕時
      mode++;
      if (mode > 3) mode = 0; // 循環回 0
    }
    buttonState = reading;
  }

  lastButtonState = reading;

  // 控制 LED
  unsigned long currentMillis = millis();
  switch(mode) {
    case 0: // 恆亮
      digitalWrite(ledPin, HIGH);
      break;
    case 1: // 慢閃爍
      if (currentMillis - previousMillis >= 1000) { // 1秒切換一次
        previousMillis = currentMillis;
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
      }
      break;
    case 2: // 中速閃爍
      if (currentMillis - previousMillis >= 500) { // 0.5秒切換一次
        previousMillis = currentMillis;
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
      }
      break;
    case 3: // 快閃爍
      if (currentMillis - previousMillis >= 200) { // 0.2秒切換一次
        previousMillis = currentMillis;
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
      }
      break;
  }
}