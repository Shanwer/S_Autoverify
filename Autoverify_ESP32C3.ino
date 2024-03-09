#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>

#define USE_SERIAL Serial

//ESP32认证部分源代码帮助来自Duya25446 https://github.com/duya25446
String GetIp()
{
    String ip;
    HTTPClient http;
    USE_SERIAL.print("[HTTP] GET IP begin...\n");
    http.begin("http://10.1.99.100/drcom/chkstatus?callback=dr1002&jsVersion=4.X&v=4299&lang=zh");
    int httpCode = http.GET();
    if(httpCode > 0)
    {
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK)
      {
        String resp = http.getString();
        int i = resp.indexOf("\"v46ip\":\"")+9;
        int n = resp.indexOf("\",\"myv6ip\"");//计算
        ip = resp.substring(i,n);
        USE_SERIAL.println(ip);
        //复制到
      }
    }
    else
    {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());    
    }
    http.end();
    return ip;
}

void login(String ip,String user_account,String isp,String password)
{
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  String auth = user_account + "@" + isp;
  String url = "http://10.1.99.100:801/eportal/portal/login?callback=dr1003&login_method=1&user_account=,0," + auth + "&user_password=" + password + "&wlan_user_ip=" + ip + "&wlan_user_ipv6=&wlan_user_mac=000000000000&wlan_ac_ip=&wlan_ac_name=&jsVersion=4.1.3&terminal_type=1&lang=zh-cn&v=10114&lang=zh";
  USE_SERIAL.println(url);
  HTTPClient http;
  USE_SERIAL.print("[HTTP] Login begin...\n");
  http.begin(url);
  int httpCode = http.GET();
  if(httpCode > 0)
    {
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK)
      {
        String resp = http.getString();
        USE_SERIAL.println(resp);
        //复制到
      }
    }
    else
    {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());    
    }
    http.end();
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
}

void setup()
{

  USE_SERIAL.begin(115200);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);//状态指示用LED灯
  //连接WiFi：12号以2秒的频率进行闪烁
  //认证，12,13号会保持常亮，直到操作结束
  for(uint8_t t = 4; t > 0; t--) 
  {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.begin("233", "shanwer");//这个是开始wifi连接
  while (WiFi.status() != WL_CONNECTED) {//这个是判断是否连接上
    digitalWrite(12, HIGH);
    delay(1000);
    Serial.print("Connecting to WiFi...");
    digitalWrite(12, LOW);
    delay(1000);
  }
  login(GetIp(),"114514","cmcc","1919810");
}

void loop() 
{
  delay(600000);//等待10分钟，误差在毫秒级别，这是给？个隔壁的宿舍用的，就不费脑筋上IDF了
  login(GetIp(),"114514","cmcc","1919810");
}