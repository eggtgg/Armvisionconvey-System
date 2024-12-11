#include <ArduinoJson.h>

#include <Servo.h> 

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;

int servo_curent[6] = {20, 100, 70, 65, 70, 70};

const int PUL = 10;
const int s1 = 8;
const int s2 = 9;

bool last_val1=1;
bool last_val2=1;

int run_mode = 3;
int pick_up_mode = 0;
int xung = 150;
int val = 0;

int stopCondition = 0;

void setup() {
  Serial.begin(9600);
  Serial2.begin(115200);

  while (!Serial) continue;
  pinMode(PUL, OUTPUT);

  // thiet lap servo
  myservo1.attach(7); // 30-90 -- 30
  myservo2.attach(2); // 90 vuong goc bang tai, 0 and 180 song song -- 100
  myservo3.attach(3); // 60 vuong goc, cang giam cang duoi ra ---60
  myservo4.attach(4); // 30 vuong goc, cang tang cang duoi ra ---30
  myservo5.attach(5); // 70 vuong goc, cang tang cang duoi ra,  ---70
  myservo6.attach(6); // 70 vuong goc bang tai,  cang tang cang tien gan thung rac ---70
  
  move_arm(servo_curent, 5);
}


void loop(){
  check_arm_and_run_mode();
  step_motor();
}

// kiem tra tin hieu Serial, chi nhan kieu json
void check_arm_and_run_mode() {
    if (Serial2.available()){
        StaticJsonDocument<100> doc;
        DeserializationError err = deserializeJson(doc, Serial2);
        if (err == DeserializationError::Ok) {
          run_mode = doc["rmode7"].as<int>();
          pick_up_mode = doc["pick_mode"].as<int>();
          Serial.print("run_mode");  Serial.print(run_mode);
          Serial.print("pick_up_mode"); Serial.println(pick_up_mode);
        }
        else {
          Serial.print("deserializeJson() returned ");
          Serial.println(err.c_str());
          while (Serial2.available() > 0)
            Serial2.read();
        }
    }
}

// di chuyen canh tay
void move_arm(int servo_target[6], int step) {
  // 2 tham số: servo_target là 1 mảng có độ dài 6 - lưu vị trí của 6 Servo
  // và step là bước nhảy: ví dụ step =5, từ 80- 100. thì Servo di chuyển từ lần lượt từ 80-85-90-95-100

  // tính sai số của đầu vào và vị trí hiện tại của Servo
  int e_1 = servo_target[0] - servo_curent[0];
  int e_2 = servo_target[1] - servo_curent[1];
  int e_3 = servo_target[2] - servo_curent[2];
  int e_4 = servo_target[3] - servo_curent[3];
  int e_5 = servo_target[4] - servo_curent[4];
  int e_6 = servo_target[5] - servo_curent[5];
  // sai số được tổng hợp bằng toán tử hợp
  int e = e_1 || e_2 || e_3 || e_4 || e_5 || e_6;

  // e == 0 thì đây là lúc vị trí servo không đổi không cần xử lý thêm
  if (e==0) {
    myservo1.write(servo_target[0]);
    myservo2.write(servo_target[1]);
    myservo3.write(servo_target[2]);
    myservo4.write(servo_target[3]);
    myservo5.write(servo_target[4]);
    myservo6.write(servo_target[5]);
  }

  // e!=0 lúc này ta cần thay đổi góc của các Servo
  while (e) {

    // gọi hàm move_by_step(), tác dụng hàm này ở bên dưới, phần định nghĩa hàm
    e_1 = move_by_step(e_1, 0, step);
    e_2 = move_by_step(e_2, 1, step);
    e_3 = move_by_step(e_3, 2, step);
    e_4 = move_by_step(e_4, 3, step);
    e_5 = move_by_step(e_5, 4, step);
    e_6 = move_by_step(e_6, 5, step);
    
    // di chuyển Servo
    myservo1.write(servo_curent[0]);
    myservo2.write(servo_curent[1]);
    myservo3.write(servo_curent[2]);
    myservo4.write(servo_curent[3]);
    myservo5.write(servo_curent[4]);
    myservo6.write(servo_curent[5]);

    // tiếp tục tính sai số được tổng hợp bằng toán tử hợp
    e = e_1 || e_2 || e_3 || e_4 || e_5 || e_6;
    // delay giữa mỗi bước
    delay(50);
  }
}

// di chuyển mỗi bước
int move_by_step(int e_n, int index_servo, int step){
  // đầu vào e_n là sai số của servo,
  // index_servo là tên Servo, step tương tự ở hàm trên
  // đầu ra e_n là sai số sau khi thực hiện bước di chuyển này
    int n_servo;
    if (e_n>0) {
      if (e_n>step) {
        n_servo = servo_curent[index_servo] + step;
        servo_curent[index_servo] = n_servo;
        e_n -= step;
      }
      else {
        n_servo = servo_curent[index_servo] + e_n;
        servo_curent[index_servo] = n_servo;
        e_n = 0;
      }
    }
    else {
      if (-e_n>step) {
        n_servo = servo_curent[index_servo] - step;
        servo_curent[index_servo] = n_servo;
        e_n += step;
      }
      else {
        n_servo = servo_curent[index_servo] + e_n;
        servo_curent[index_servo] = n_servo;
        e_n = 0;
      }
    }
    //servo_curent[index_servo] = n_servo;
    return e_n;
}

// gán giá trị của các servo vào mảng
void assign_arm_values(int arm[6], int a, int b, int c, int d, int e, int f) {
    arm[0] = a;
    arm[1] = b;
    arm[2] = c;
    arm[3] = d;
    arm[4] = e;
    arm[5] = f;
}

// hành động cánh tay gắp vật
void pick_up(int pick_up_mode) {
  // truyền vào biến pick_up_mode
  // tương ứng với các chế độ gắp cánh tay
    int arm_home[6] = {20, 100, 70, 65, 70, 70};
    int arm_lower[6];
    int arm_pick[6];
    int arm_up[6];
    int arm_left[6];
    int arm_drop[6];

    // chuyển giữa các chế độ gắp khác nhau
    switch (pick_up_mode) {
        case 0:
            assign_arm_values(arm_lower, 20, 100, 70, 65, 20, 70);
            assign_arm_values(arm_pick, 80, 100, 70, 65, 20, 70);
            assign_arm_values(arm_up, 80, 100, 70, 65, 70, 70);
            assign_arm_values(arm_left, 80, 100, 70, 65, 70, 160);
            assign_arm_values(arm_drop, 20, 100, 70, 65, 50, 160);
            break;
        case 1:
            assign_arm_values(arm_lower, 20, 10, 70, 65, 20, 70);
            assign_arm_values(arm_pick, 80, 10, 70, 65, 20, 70);
            assign_arm_values(arm_up, 80, 10, 70, 65, 70, 70);
            assign_arm_values(arm_left, 80, 10, 70, 65, 70, 160);
            assign_arm_values(arm_drop, 20, 10, 70, 65, 50, 160);
            break;
        case 2:
            assign_arm_values(arm_lower, 20, 100, 70, 50, 30, 70);
            assign_arm_values(arm_pick, 80, 100, 70, 50, 30, 70);
            assign_arm_values(arm_up, 80, 100, 70, 65, 70, 70);
            assign_arm_values(arm_left, 80, 100, 70, 65, 70, 160);
            assign_arm_values(arm_drop, 20, 100, 70, 65, 50, 160);
    }
    // thực hiện
    move_arm(arm_lower, 5);
    move_arm(arm_pick, 5);
    move_arm(arm_up, 5);
    move_arm(arm_left, 5);
    move_arm(arm_drop, 5);
    move_arm(arm_home, 5);
}

// quay băng tải
void step_motor() {
  // kiểm tra run_mode
  check_arm_and_run_mode();

  switch_run_mode(run_mode); // hàm này sẽ làm thay đổi biến val, dựa vào runmode truyền vào
  //Serial.println(val);
  while (val) {
    //Serial.println("hello !");
    gen_pulse(xung);
    check_arm_and_run_mode();
    switch_run_mode(run_mode);
  }

  if (run_mode == 1){
    pick_up(pick_up_mode);
  }
}

// chuyển chế độ quay băng tải
void switch_run_mode(int run_mode) {
  //đầu vào runmode
  bool val1 = digitalRead(s1);
  bool val2 = digitalRead(s2);

  if (val1 != last_val1){
      Serial.print("val1: "); Serial.println(val1);
      // String value_sens = "{\"sensor\":\"val1\"" + ",\"time\":" + val1 + "}";
      // Serial2.println(value_sens);
      
      String value_sens = "";
      value_sens += "{\"sensor\":1";
      value_sens += ",\"value\":" ;
      value_sens += val1;
      value_sens += "}";
      Serial.println(value_sens);
      Serial2.print(value_sens);


  }
  if (val2 != last_val2){
      Serial.print("val2: "); Serial.println(val2);

      String value_sens = "";
      value_sens += "{\"sensor\":2";
      value_sens += ",\"value\":" ;
      value_sens += val2;
      value_sens += "}";
      
      Serial.println(value_sens);
      Serial2.print(value_sens);
  }

  last_val1 = val1;
  last_val2 = val2;
  // chuyển 4 chế độ quay băng tải bằng cách kiểm soát biến val
  // xem hàm step_motor() bên trên
  switch (run_mode) {
      case 0: // dừng khi nhận vật thể ở cả 2 cảm biến
        val = val1 && val2;
        break;
      case 1: // dừng khi có vật thể ở cảm biến 1
        val = val1;
        break;
      case 2: // dừng khi có vật thể ở cảm biến 2
        val = val2;
        break;
      case 3: // chạy không dừng
        val = true;
        break;
      case 4: // dừng bất chấp
        val = false;
        break;
    }  
}

// phát xung
void gen_pulse(int xung){
  digitalWrite(PUL, HIGH);
  delayMicroseconds(xung);
  digitalWrite(PUL, LOW);
  delayMicroseconds(xung);
}