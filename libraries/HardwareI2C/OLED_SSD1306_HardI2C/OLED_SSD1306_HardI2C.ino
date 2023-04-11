// Copyright 2021 IOsetting <iosetting(at)outlook.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
/******************************************************************************
 *
 * \brief       Demo code of SSD1306 OLED
 * \remarks     test-board: HLK-W806-KIT-V1.0
 *
 *              In ssd1306.h define SSD1306_MODE_I2C
 *
 *              I2C Mode:
 *                PA1   -> SCL
 *                PA4   -> SDA
 *                GND   -> GND
 *                3.3V  -> VCC
 ******************************************************************************/

#include "Arduino.h"
#include "ssd1306.h"
#include "HardwareI2C.h"

HardwareI2C Wire;

int y1 = 0, y2 = 0;
uint8_t d1 = 0, d2 = 0;
char buf[10];

void setup()
{
    Wire.begin();
    Wire.setClock(1000000); // 1Мгц - максимальная частота шины
    SSD1306_Init();
    SSD1306_DrawLine(0, 0, 127, 0, 1);
    SSD1306_DrawLine(0, 0, 0, 63, 1);
    SSD1306_DrawLine(127, 0, 127, 63, 1);
    SSD1306_DrawLine(0, 63, 127, 63, 1);
    SSD1306_GotoXY(5, 5);
    SSD1306_Puts((char*)"OLED:11x18", &Font_11x18, 1);
    SSD1306_GotoXY(10, 52);
    SSD1306_Puts((char*)"Font size: 7x10", &Font_7x10, 1);
    SSD1306_UpdateScreen(); // display
    delay(2000);

    SSD1306_Fill(0);

    SSD1306_GotoXY(5, 5);
    SSD1306_Puts((char*)"OLED", &Font_16x26, 1);
    SSD1306_GotoXY(10, 52);
    SSD1306_Puts((char*)"W806 SDK Demo", &Font_6x12, 1);
    SSD1306_UpdateScreen();
    delay(2000);

    SSD1306_ToggleInvert(); // Invert display
    SSD1306_UpdateScreen();
    delay(2000);

    SSD1306_Fill(0);

    SSD1306_GotoXY(5, 5);
    SSD1306_Puts((char*)"OLED", &Font_16x32, 1);
    SSD1306_GotoXY(10, 45);
    SSD1306_Puts((char*)"W806 SDK Demo", &Font_8x16, 1);
    SSD1306_UpdateScreen();
    delay(3000);

    SSD1306_ToggleInvert(); // Invert display
    SSD1306_UpdateScreen();
    delay(2000);

    SSD1306_Fill(0);
    y1 = 64, y2 = 0;
    while(y1 > 0) {
	SSD1306_DrawLine(0, y1, 127, y2, 1);
	SSD1306_UpdateScreen();
	y1 -= 2;
	y2 += 2;
    }
    delay(1000);

    SSD1306_Fill(0);
    y1 = 127, y2 = 0;
    while(y1 > 0) {
	SSD1306_DrawLine(y1, 0, y2, 63, 1);
	SSD1306_UpdateScreen();
	y1 -= 2;
	y2 += 2;
    }
    delay(1000);

    SSD1306_Fill(1);
    SSD1306_UpdateScreen();
    SSD1306_DrawCircle(64, 32, 25, 0);
    SSD1306_UpdateScreen();
    SSD1306_DrawCircle(128, 32, 25, 0);
    SSD1306_UpdateScreen();
    SSD1306_DrawCircle(0, 32, 25, 0);
    SSD1306_UpdateScreen();
    SSD1306_DrawCircle(32, 32, 25, 0);
    SSD1306_UpdateScreen();
    SSD1306_DrawCircle(96, 32, 25, 0);
    SSD1306_UpdateScreen();
    delay(1000);

    SSD1306_Fill(0);
    SSD1306_UpdateScreen();
    int32_t i = -100;

    while(i <= 100) {
	memset(&buf[0], 0, sizeof(buf));
	sprintf(buf, "%d", i);
	SSD1306_GotoXY(50, 27);
	SSD1306_Puts(buf, &Font_7x10, 1);
	SSD1306_DrawLine(64, 10, (i + 100) * 128 / 200, (i + 100) * 64 / 200, 1);
	SSD1306_UpdateScreen();
	SSD1306_Fill(0);
	i++;
    }
    SSD1306_GotoXY(10, 45);
    sprintf(buf, "END");
    SSD1306_Puts(buf, &Font_7x10, 1);
    SSD1306_UpdateScreen();
    SSD1306_Fill(0);
}

void loop()
{
}
