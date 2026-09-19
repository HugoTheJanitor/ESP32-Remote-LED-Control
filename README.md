# ESP32 Remote LED Control

A simple IoT project that allows four physical LEDs connected to an ESP32 to be controlled remotely through a web interface.

The ESP32 connects to Wi-Fi, periodically requests the current LED states from a Flask server over HTTPS, and updates the GPIO outputs accordingly.

## Project photo

![Project photo](Photos/PhotoOfProject.jpg)

## Hardware

- ESP32-WROOM-32E development board
- 4 LEDs: red, green, blue and yellow
- 4 × 330 Ω resistors
- Breadboard
- Jumper wires
- USB cable or power bank

## GPIO connections

| LED | ESP32 GPIO |
| --- | ---: |
| Red | 18 |
| Green | 19 |
| Blue | 21 |
| Yellow | 23 |

Each LED is connected through a 330 Ω resistor. The cathodes are connected to GND.

## ESP32-WROOM-32E pinout

![ESP32-WROOM-32E pinout](Photos/ESP32%20WROOM%2032E%20PinOut.png)

Official ESP32-WROOM-32E datasheet and pin descriptions:

https://documentation.espressif.com/esp32-wroom-32e_esp32-wroom-32ue_datasheet_en.html

The datasheet contains the module pin layout, GPIO descriptions, peripheral functions, electrical characteristics and other hardware information.

## How it works

```text
Phone / PC
    |
    | HTTPS
    v
Public ngrok URL
    |
    v
Flask server on PC
    ^
    | HTTPS GET /status
    |
ESP32 connected to Wi-Fi
    |
    v
GPIO 18 / 19 / 21 / 23
    |
    v
4 LEDs
```

The Flask server stores the current state of the four LEDs.

For example:

```text
1,0,1,0
```

means:

- Red: ON
- Green: OFF
- Blue: ON
- Yellow: OFF

The ESP32 requests `/status` approximately once per second, parses the response, and sets the corresponding GPIO pins HIGH or LOW.

## ESP32 firmware

The firmware uses:

- `WiFi.h`
- `HTTPClient.h`
- `WiFiClientSecure.h`

Before uploading the sketch, set the Wi-Fi credentials for the network that the ESP32 should connect to:

```cpp
const char* SSID = "YOUR_WIFI_NAME";
const char* PASSWORD = "YOUR_WIFI_PASSWORD";
```

Do not commit real Wi-Fi credentials to a public repository.

The ESP32 communicates with the remote server using the public HTTPS endpoint:

```cpp
const char* SERVER_URL = "https://YOUR-NGROK-DOMAIN/status";
```

## Server

The web server is implemented with Flask.

Typical routes are:

```text
/                 Web interface
/toggle/<color>   Toggle one LED
/status           Get all LED states
/status/<color>   Get one LED state
```

The development version can be started with:

```powershell
python app.py
```

Then expose port 5000 through ngrok:

```powershell
ngrok http 5000
```

The public ngrok URL can then be opened from another network or from mobile data.

## Remote operation

The ESP32 and the device controlling it do not need to be on the same Wi-Fi network.

For example:

```text
ESP32 -> Mentor's Wi-Fi -> Internet -> Flask server
Phone -> Mobile Internet -> Internet -> Flask server
```

As long as both sides have Internet access and the server is running, the LEDs can be controlled remotely.

## Arduino IDE setup

Install the ESP32 board package by Espressif Systems and select:

```text
ESP32 Dev Module
```

Then select the correct COM port and upload the sketch.

After the firmware has been uploaded, Arduino IDE does not need to remain open. The ESP32 stores the firmware in flash memory and starts it automatically whenever power is applied.

## Notes

This project is intended as a learning prototype for Wi-Fi networking, HTTP/HTTPS communication, GPIO control and basic IoT architecture.

For a production deployment, certificate validation, authentication, a persistent server deployment and stronger secret management should be added.

## License

This project is licensed under the MIT License.
