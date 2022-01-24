#include <Arduino.h>
#include <SD.h>
#include <HTTPClient.h>
#include <M5StickCPlus.h>
#include <vector>
#include <random>

const std::vector<std::vector<int>> window {
  {230},
  {120}
};

unsigned short int size_player {20};
short int x {120};
short int y {65};

unsigned short int move_stat {0};

unsigned short int size_point {10};
short int x_point {20};
short int y_point {20};

unsigned short int point {0};

void border() {
  M5.Lcd.drawLine(0, 0, 240, 0, WHITE);
  M5.lcd.drawLine(0, 0, 0, 130, WHITE);
  M5.lcd.drawLine(0, 130, 240, 130, WHITE);
  M5.lcd.drawLine(239, 0, 239, 130, WHITE);
}

void random_number() {
  std::random_device random;
  std::mt19937 gen(random());
  std::uniform_int_distribution<> random_number_1(0, window.at(0).at(0));
  std::uniform_int_distribution<> random_number_2(0, window.at(1).at(0));
  x_point = random_number_1(gen);
  y_point = random_number_2(gen);
}

void collision() {
  if (x < 0) {
    x = 2;
  }
  if (x > window.at(0).at(0)) {
    x = 218;
  }
  if (y < 0) {
    y = 5;
  }
  if (y > window.at(1).at(0)) {
    y = 115;
  }

  if (x < x_point + size_point &&
        x + size_player > x_point &&
        y < y_point + size_point &&
        size_player + y > y_point) {
        point++;
        random_number();
        M5.Beep.setBeep(60, 500);
        M5.Lcd.printf("points: %d", point);
      }
}

void spawn_points() {
  for (unsigned short int i {0}; i < 1; i++) {
    M5.Lcd.fillRect(x_point, y_point, 5, 5, RED);
  }
}

void movement() {
  if(M5.BtnA.isReleased()) {
    move_stat += 1;
    M5.Lcd.print(move_stat);
  }
}

void player() {
  collision();
  M5.Lcd.fillRect(x, y, size_player, size_player, GREEN);
  switch (move_stat) {
    case 0:
      x += 15;
      break;
    case 1:
      y += 15;
      break;
    case 2:
      x -= 15;
      break;
    case 3:
      y -= 15;  
      break;
    default:
      move_stat = 0;
      break;
  }
}

void winning() {
  if (point >= 10){
    for (unsigned short int i {0}; i < 99; i++) {
      M5.Lcd.print("you win!");
    }
  }
}

void update() {
  M5.Lcd.fillScreen(BLACK);
  border();
}

void setup() {
  Serial.begin(9600);
  M5.Beep.begin();
  M5.begin();
  M5.lcd.setRotation(1);
}

void game_loop() {
  player();
  delay(2000);
  update();
}

void loop() {
  winning();
  spawn_points();
  M5.update();
  M5.Beep.update();
  game_loop();
  movement();
}