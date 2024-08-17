# ECO Solar Boost MPPT-3000 PRO ESPHome External Component

Repozitář obsahuje [ESPHome](https://esphome.io/) komponentu pro AZO Digital ECO Solar Boost MPPT-3000 PRO invertor.

ESP32 naslouchá na I2C sběrnici a dekóduje obsaho zobrazený na displeji. Otestováno s verzí invertoru 5.49.

## Použití
ESP32 module se připojí na LCD modul invertoru.
 - GND na GND
 - SDA pin na nakonfigurovaný GPIO pin, default 21
 - SCL pin na nakonfigurovaný GPIO pin, default 22
 - datové piny musí být připojeny přes 1K rezistor s diodou nebo digitální optočlen (např. 2 kanálový VO0631T, 2x 1 kanálový 6N137-L)
 - ESP32 nelze napájet z 5V invertoru, neposkytuje dostatečný proud
 - zapnutý invertor lze detekovat optočlenem připojeným na 5V napájení LCD, přidat do konfigurace `binary_sensor gpio`

![Schematic](./img/Schematic_MPPT3k.png)

![Diagram](./img/MPPT3000-LCD-wire.jpg)

Soubor `mppt3000.yaml` obsahuje vzorovou konfiguraci ESPHome.
 - komponenta není kompatibilní s konfigurací `i2c` sběrnice
 - výchozí `update_interval` je 30 sekund, jelikož invertor zobrazuje veškerá data zhruba každých 20 sekund
 - je doporučeno nastavit timeout filter na neinkrementalní veličiny
 - odposlech se občas může zaseknout, ESP32 lze vzdáleně nebo automatizovaně restartovat nakonfigurovanou komponentou restart switch
 - v konfiguraci lze využít odkaz na Github nebo zkopírovat adresář `components` do úložiště ESPHome a nastavit `- source: components`

## Mapování pinů LCD desky PCF8574A na HD44780
LCD driver je ovládán ve 4-bitovém režimu přes I2C expandér. Invertor očekává PCF8574 na adrese 0x27 nebo PCF8574A na adrese 0x3F.
 - P0 - RS
 - P1 - R/W
 - P2 - E
 - P3 - nezapojeno, vždy 1
 - P4 - DB4
 - P5 - DB5
 - P6 - DB6
 - P7 - DB7
