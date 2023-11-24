/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

/*
  Connect your TFT display like this:
  TFT_VCC   to power
  TFT_GND   to ground
  TFT_CS    to ground
  TFT_LED   to power 
  TFT_SCL   PB6   
  TFT_MOSI  PB7   
  TFT_DC    PB8
  TFT_RST   PB9
*/

#include <lvgl.h>
#include "ILI9341_Fast_SDIO.h"
#include "Arduino.h"

static const uint16_t screenWidth  = LCD_WIDTH;
static const uint16_t screenHeight = LCD_HEIGHT;


static lv_disp_draw_buf_t draw_buf ;
static lv_color_t buf[ screenWidth * screenHeight / 10 ];

#define TFT_DC  PB8
#define TFT_RST PB9

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p )
{
  Lcd_Draw_Rect(area->x1,area->y1,area->x2 - area->x1 + 1,area->y2 - area->y1 + 1,(uint16_t*)color_p);
  lv_disp_flush_ready( disp_drv );
}

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */
    
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );
    
    lv_init();

    Lcd_Init(TFT_DC, TFT_RST);          /* TFT init. TFT_CS pin on GND*/
    Lcd_Orientation(3); /* Landscape orientation, flipped */
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight/10 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    Serial.println( "Setup done" );
    
    lv_obj_t * label1 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
    lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
    lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label, align the lines to the center "
                      "and wrap long text automatically.");
    lv_obj_set_width(label1, 150);  /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t * label2 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR);     /*Circular scroll*/
    lv_obj_set_width(label2, 150);
    lv_label_set_text(label2, "It is a circularly scrolling text. ");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, 40);

}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay( 5 );
}
