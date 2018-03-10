
/*
 * 名称：护眼学习灯
 * 作者：周米笑
 * 邮件：mixiao.zhou@qq.com
 * Github: https://github.com/mixiao07/babaohe
 * 版本：2017-04-03 1.0.0
 * 备注：
 *    材料：
 *    Arduino Nano
 *    喇叭
 *    继电器
 *    RGB 三色 LED
 *    光敏电阻
 *    人体传感器
 *    超声波测距传感器
 *    触摸板
 *    其他：面包板、杜邦线、热熔胶、插座电线、USB充电头等
 *    
 *    开发工具：
 *    Arduino 1.8.1
 */

unsigned long htime = 0;
bool kaideng = false;    //控制灯开关
bool yourenma = false;    //人体传感器 判断有没有人
bool jinglema = false;    //超声波测距 判断近不近
bool taianle =  false;    //光敏电阻 判断黑不黑
bool chifanqie = false;    //判断有没有进入番茄学习法
unsigned long fanqietime =0;

void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);  //LED灯  
  digitalWrite(13, LOW);
  
  pinMode(2,OUTPUT);   //继电器开关
  digitalWrite(2, LOW);
  
  pinMode(3,INPUT);    //人体红外传感器
 
  pinMode(4,OUTPUT);   //超声波测距 trig
  pinMode(5,INPUT);    //超声波测距echo
  digitalWrite(4, LOW);
  
  pinMode(6,INPUT);    //触控板
  
  pinMode(8,OUTPUT);   //红灯
  pinMode(9,OUTPUT);   //绿灯
  pinMode(10,OUTPUT);  //蓝灯
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  
  pinMode(11,OUTPUT);  // 喇叭
                 
  //Serial.begin(9600);
  //Serial.println("Hello MiXiao Gooooo");
  delay(1000);
  digitalWrite(10, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  int renti;
  int guanx;
  int chumo;
  float juli;
  unsigned long ctime1;

  //判断有没有人
  ctime1 = millis();
  renti = digitalRead(3);
  if (renti) {
    htime = ctime1;
    yourenma = true;   //发现有人了
  }
  else {
    if ((unsigned long)(ctime1 - htime) > (unsigned long)(120000)) {
      yourenma = false;   //发现没有人了
    }    
  }
  //判断够不够亮
  guanx = analogRead(A1);
  if(guanx>140){
    taianle = true;    
  }
  else{
    taianle = false;    
  }

 // 测试距离 小于40cm就报警
 juli = 200;
 if (yourenma) {
   digitalWrite(4, HIGH);
   delayMicroseconds(20);
   digitalWrite(4, LOW);
   juli = pulseIn(5, HIGH) / 58.82;
   if( juli < 40){
    jinglema  = true;  
   }
   else{
    jinglema = false; 
   }
 }

  if((juli < 60) && yourenma && taianle){
   kaideng = true;   
   digitalWrite(2,HIGH);   //开灯
  }
  else if(!yourenma){
    kaideng = false;
    digitalWrite(2,LOW);  //关灯
  } 

 if(yourenma && jinglema){
  tone(11,2000,100);
  delay(100);
 }

  chumo=digitalRead(6);
  if(chumo){
    unsigned long kaishitime = millis();

    // 等到松手不触摸
    do {
      delay(5);
      chumo=digitalRead(6);   

      ctime1 = millis();
      if((ctime1 - kaishitime) > 2000){
        break;   
      }
    } while(chumo);

    ctime1 = millis();

    if((ctime1 - kaishitime) > 2000){
      if(!chifanqie){
        chifanqie = true;
        fanqietime=ctime1;  
        digitalWrite(8,HIGH);
        digitalWrite(9,LOW);
        digitalWrite(10,LOW);
        tone(11,3000,100);
        delay(200);
        tone(11,3000,100);
      }      
    }      
  }

  //番茄时间 25 分钟
  if(chifanqie){
    ctime1 = millis();
    if(( ctime1 - fanqietime) > 1500000) {
      chifanqie = false;
      digitalWrite(8,LOW);
      digitalWrite(9,HIGH);
      digitalWrite(10,LOW);
      tone(11,3000,100);
      delay(200);
      tone(11,3000,100);
      delay(200);
      tone(11,3000,300);
    }
  }

/*
 Serial.println(guanx);
 Serial.print("den = ");
 Serial.println(kaideng);
 Serial.print(juli);
 Serial.println(" cm");
 */

 delay(200);
}

