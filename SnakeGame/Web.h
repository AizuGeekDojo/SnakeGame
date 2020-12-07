#include <WiFi.h>
String str = "<!DOCTYPE html><html id='swip'><head><meta charset='utf-8' name='viewport' content='width=device-width, initial-scale=1' /><style>:root{--Width:min(30vw,25vh);}.Div {position: relative;width: var(--Width);}.Div:before {content: '';display: block;padding-top: 100%;}.btn {position: absolute;top: 0;left: 0;width: 100%;height: 100%;font-size: calc(var(--Width) / 2);}.float {float: left;}.Left{margin-right:calc(var(--Width) / 2);}.Right{margin-left:calc(var(--Width) / 2);}.teki{width:calc(var(--Width) * 3);margin: 0px auto;}.clear {clear: both;}</style></head><body>操作方法<br><input type='radio' name='sb' checked='checked' onchange='Show(true);'>ボタン</input><input type='radio' name='sb' onchange='Show(false);'>スワイプ</input><br><div id='b'><center><div class='Div'><button class='btn' id='up'>↑</button></div><br><div class='teki'><div class='Div float Left'><button class='btn' id='left'>←</button></div><div class='Div float Right'><button class='btn' id='right'>→</button></div><div class='clear'></div></div><br><div class='Div'><button class='btn' id='down'>↓</button></div></center></div><script type='text/javascript'>let flg = true;let btns = ['up', 'right', 'left', 'down'];let btn = new Array(4);function Show(f) {flg = f;if (f == true) {document.getElementById('b').hidden = false;} else {document.getElementById('b').hidden = true;}}function Send(dir) {var xhr = new XMLHttpRequest();xhr.open('GET', '?' + dir);xhr.timeout = 1000;xhr.setRequestHeader('Cache-Control', 'no-cache');xhr.setRequestHeader('If-Modified-Since', 'Thu, 01 Jun 1970 00:00:00 GMT');xhr.responseType = 'document';xhr.ontimeout = function (e) { xhr.abort(); }xhr.send();}for (let i = 0; i < 4; i++) {btn[i] = document.getElementById(btns[i]);btn[i].onclick = function () { Send(btns[i]) };}let t = document.getElementById('swip');let sx, sy, mx, my;t.addEventListener('touchstart', function (e) {e.preventDefault();sx = e.touches[0].pageX;sy = e.touches[0].pageY;mx = e.touches[0].pageX;my = e.touches[0].pageY;});t.addEventListener('touchmove', function (e) {e.preventDefault();mx = e.changedTouches[0].pageX;my = e.changedTouches[0].pageY;});t.addEventListener('touchend', function (e) {let RLUD = [sy - my, mx - sx, sx - mx, my - sy, 100];let n = RLUD.indexOf(Math.max(...RLUD));if (!flg && n < 4) { Send(btns[n]); }});</script></body></html>";
String header;

const IPAddress ip(192, 168, 0, 3);
const IPAddress subnet(255, 255, 255, 0);
WiFiServer server(80);

class _Web {
  private:
    char *ssid="ESP32";
  public:
    _Web(char* s) {
      ssid = s;
    }
    void init() {
      WiFi.softAP(ssid);
      delay(100);
      WiFi.softAPConfig(ip, ip, subnet);
      server.begin();
    }
    int _update() {
      int dir = EMPTY;
      WiFiClient client = server.available();
      if (client) {
        String currentLine = "";
        while (client.connected()) {
          if (client.available()) {
            char c = client.read();
            header += c;
            if (c == '\n') {
              if (currentLine.length() == 0) {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();
                client.println(str);
                if (header.indexOf("GET /?up") >= 0) {
                  dir = UP;
                }
                if (header.indexOf("GET /?right") >= 0) {
                  dir = RIGHT;
                }
                if (header.indexOf("GET /?left") >= 0) {
                  dir = LEFT;
                }
                if (header.indexOf("GET /?down") >= 0) {
                  dir = DOWN;
                }
                client.println();
                // Break out of the while loop
                break;
              } else {
                currentLine = "";
              }
            } else if (c != '\r') {
              currentLine += c;
            }
          }
        }
        header = "";
        client.stop();
      }
      return dir;
    }
};
