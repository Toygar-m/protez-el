#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* ssid = "tsur_atolye";
const char* password = "atolye_2024ESD";

Servo myServo;
const int servoPin = 13;

WebServer server(80);

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>El Protez Kontrol Paneli</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: Arial, sans-serif;
            user-select: none;
        }
        body {
            background: linear-gradient(150deg, #4c24ff 15%, #230d81 50%);
            width: 100%;
            height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            flex-direction: column;
            text-align: center;
        }
        .img-container {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            margin-bottom: 40px;
        }
        .img-container img {
            width: 100px;
            filter: drop-shadow(0 12px 55px rgba(0, 0, 0, 0.7));
            border-radius: 10px;
            margin-bottom: 20px;
        }
        .img-container h1 {
            color: white;
            font-size: 24px;
            font-weight: bold;
            text-shadow: 0 0 20px rgba(0, 0, 0, 0.7);
        }
        .container {
            display: flex;
            flex-wrap: wrap;
            gap: 20px;
            justify-content: center;
            margin-left: 20px;
            margin-right: 20px;
        }
        button {
            border: none;
            border-radius: 15px;
            box-shadow: 0 0 20px rgba(0, 0, 0, 0.3);
            background-color: #4c24ff;
            color: white;
            width: 150px;
            height: 80px;
            display: flex;
            justify-content: center;
            align-items: center;
            font-size: 18px;
            font-weight: bold;
            text-transform: uppercase;
            transition: all 0.3s ease-in-out;
        }
        button:hover {
            background-color: #230d81;
            transform: scale(1.1);
            box-shadow: 0 0 40px rgba(0, 0, 0, 0.5);
        }
    </style>
</head>
<body>
    <div class="img-container">
        <img src="data:image/png;base64,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" alt="Top Kapı El Protez" />
        <h1>Top Kapı El Protez Uygulaması</h1>
    </div>
    <div class="container">
        <button onclick="moveServo(0)">Haraket 1</button>
        <button onclick="moveServo(10)">Haraket 2</button>
        <button onclick="moveServo(20)">Haraket 3</button>
        <button onclick="moveServo(45)">Haraket 4</button>
        <button onclick="moveServo(60)">Haraket 5</button>
        <button onclick="moveServo(80)">Haraket 6</button>
        <button onclick="moveServo(90)">Haraket 7</button>
        <button onclick="moveServo(135)">Haraket 8</button>
        <button onclick="moveServo(150)">Haraket 9</button>
        <button onclick="moveServo(180)">Haraket 10</button>
    </div>
    <script>
      function moveServo(angle) {
          fetch(`/servo?angle=${angle}`)
          .then(response => response.text())
          .then(data => {
              console.log(data);
          })
          .catch(error => console.error("Hata:", error));
      }
    </script>

</body>
</html>
)rawliteral";

void moveServo(int angle) {
  myServo.write(angle);
  Serial.print("Servo açısı: ");
  Serial.println(angle);
}

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleServo() {
  if (server.hasArg("angle")) {
    String angleParam = server.arg("angle");
    int angle = angleParam.toInt();
    moveServo(angle);
    server.send(200, "text/plain", "Servo " + angleParam + " dereceye hareket etti.");
  } else {
    server.send(400, "text/plain", "Hata: Açı parametresi eksik!");
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Wi-Fi'ye bağlanıyor...");
  }
  Serial.println("Wi-Fi'ye bağlanıldı!");
  Serial.print("IP Adresi: ");
  Serial.println(WiFi.localIP());

  myServo.attach(servoPin);

  server.on("/", handleRoot);
  server.on("/servo", handleServo);

  server.begin();
}

void loop() {
  server.handleClient();
}