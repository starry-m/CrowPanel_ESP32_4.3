#include <lvgl.h>
#include <SPI.h>
#include <Arduino_GFX_Library.h>
#include "ui.h"
#include <ShiftRegister74HC595.h>
#include <stdlib.h>
// #include "cifar/cifar.h"
// #include "mbnet128/mbnet128.h"
#include "mnist/mnist.h"
// #include "vww96/vww96.h"
//4.3
#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12
#define SD_CS 10

#define I2S_DOUT      20
#define I2S_BCLK      35
#define I2S_LRC       19
#define BUTTON_PIN    38


/******************************************************************************/

#define TFT_BL 2
#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin


#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *lcd = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

//UI

int led;
SPIClass& spi = SPI;



Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
  GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
  40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
  45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
  5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
  8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */
);
// option 1:
// ILI6485 LCD 480x272
Arduino_RPi_DPI_RGBPanel *lcd = new Arduino_RPi_DPI_RGBPanel(
  bus,
  480 /* width */, 0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 43 /* hsync_back_porch */,
  272 /* height */, 0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 12 /* vsync_back_porch */,
  1 /* pclk_active_neg */, 7000000 /* prefer_speed */, true /* auto_flush */);


#endif /* !defined(DISPLAY_DEV_KIT) */


/*******************************************************************************
   Please config the touch panel in touch.h
 ******************************************************************************/
#include "touch.h"

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf[480 * 272 / 8];
static lv_disp_drv_t disp_drv;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  lcd->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  lcd->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (touch_has_signal())
  {
    if (touch_touched())
    {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
      // Serial.print( "Data x :" );
      // Serial.println( touch_last_x );

      // Serial.print( "Data y :" );
      // Serial.println( touch_last_y );
    }
    else if (touch_released())
    {
      data->state = LV_INDEV_STATE_REL;
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}

ShiftRegister74HC595<2> sr(17, 18, 37);
uint8_t NEW_NUMBERS[10][8]={
  0x70, 0x88, 0x98, 0xa8, 0xc8, 0x88, 0x70, 0x00,  // 0
  0x20, 0x60, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00,  // 1
  0x70, 0x88, 0x08, 0x70, 0x80, 0x80, 0xf8, 0x00,  // 2
  0xf8, 0x08, 0x10, 0x30, 0x08, 0x88, 0x70, 0x00,  // 3
  0x10, 0x30, 0x50, 0x90, 0xf8, 0x10, 0x10, 0x00,  // 4
  0xf8, 0x80, 0xf0, 0x08, 0x08, 0x88, 0x70, 0x00,  // 5
  0x38, 0x40, 0x80, 0xf0, 0x88, 0x88, 0x70, 0x00,  // 6
  0xf8, 0x08, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00,  // 7
  0x70, 0x88, 0x88, 0x70, 0x88, 0x88, 0x70, 0x00,  // 8
  0x70, 0x88, 0x88, 0x78, 0x08, 0x10, 0xe0, 0x00,  // 9
};
// uint8_t NUMBERS[10][8]={
//   {0x3C, 0x42, 0x81, 0x81, 0x81, 0x42, 0x3C, 0x00},//0
//   {0x04, 0x06, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00},//1
//   {0x3E, 0x41, 0x01, 0x3E, 0x40, 0x40, 0x7F, 0x00},//2
//   {0x3E, 0x41, 0x01, 0x1E, 0x01, 0x41, 0x3E, 0x00},//3
//   {0x10, 0x18, 0x24, 0x44, 0x7F, 0x04, 0x04, 0x00},//4
//   {0x7F, 0x40, 0x7E, 0x01, 0x01, 0x41, 0x3E, 0x00},//5
//   {0x3E, 0x40, 0x7E, 0x41, 0x41, 0x41, 0x3E, 0x00},//6
//   {0x7F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00},//7
//   {0x3E, 0x41, 0x41, 0x3E, 0x41, 0x41, 0x3E, 0x00},//8
//   {0x3E, 0x41, 0x41, 0x3F, 0x01, 0x01, 0x3E, 0x00} //9

// };
unsigned char reverse_bits_fast(unsigned char b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4; // 交换高4位与低4位
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2; // 交换每2位
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1; // 交换每1位
  return b;
}
//行 8-15 列 0-7
void decode_nums_to_pannel(uint8_t num)
{
  uint8_t *num_ptr = NEW_NUMBERS[num];
  static uint8_t current_rows = 0;
  uint8_t buf[2];
  buf[0] = *(num_ptr+current_rows);
  buf[1]=1 << current_rows;
  // for (int i = 0; i < 8; i++)
  // {
  //   sr.setNoUpdate(i, *(num_ptr+current_rows) && 1<<i);
  // }
  sr.setAll(buf);
  // sr.setNoUpdate(current_rows+8,1);
  // sr.updateRegisters();
  current_rows = (current_rows + 1) % 8;

}


#ifdef __cplusplus
    extern "C" {
#endif
TaskHandle_t LED_Task_Handle = NULL;
#ifdef __cplusplus
    }
#endif
extern uint8_t volatile display_data;
static void LEDPanel_Task(void* parameter)
{	
  for(uint8_t i = 0; i < 10; i++)
  {
    for(uint8_t j = 0; j < 8; j++)
    {
      NEW_NUMBERS[i][j] = reverse_bits_fast(NEW_NUMBERS[i][j]>>1);
    }
  }
    while (1)
    {
        // sr.setAllHigh();
        // vTaskDelay(pdMS_TO_TICKS(1000));   /* 延时500个tick */
        // // printf("LED_Task Running,LED1_ON\r\n");
        
        // sr.setAllLow();    
        decode_nums_to_pannel(display_data);
        vTaskDelay(pdMS_TO_TICKS(3));   /* 延时500个tick */		 		
        // printf("LED_Task Running,LED1_OFF\r\n");
    }
}
static TaskHandle_t lvgl_Task_Handle = NULL;
static void lvgl_Task(void* parameter)
{	
    while (1)
    {
      lv_timer_handler();
      // lv_tick_inc(5);
      delay(50);
    }
}
void setup()
{
  Serial.begin( 115200 ); /*初始化串口*/

  

  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  pinMode(0, OUTPUT);//TOUCH-CS


  //lvgl初始化
  lv_init();

  // Init Display
  lcd->begin();
  lcd->setTextSize(2);
  lcd->fillScreen(BLACK);

  //触摸初始化
  touch_init();
  //setTouch(calData);
  screenWidth = lcd->width();
  screenHeight = lcd->height();
  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * screenHeight / 8);

  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  delay(300);
#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif
  ui_init();//开机UI界面
  

  lv_timer_handler();
  // sr.setNoUpdate(0,1);
  // sr.setNoUpdate(8,1);
  // sr.updateRegisters();
  xTaskCreate(LEDPanel_Task,"LED_Task",4096,NULL,5,&LED_Task_Handle);
  // xTaskCreate(lvgl_Task,"LVGL_Task",2048,NULL,5,&lvgl_Task_Handle);
  
  
}
int sr_tick=0;
void loop()
{
  
  lv_timer_handler();
  if(led == 1)
  digitalWrite(38, HIGH);
  if(led == 0)
  digitalWrite(38, LOW);
  // lv_tick_inc(5);
  delay(50);
  // if(sr_tick %20 == 0)
  // {
  //   sr.setAllLow();
  // }
  // else if (sr_tick %35 == 0)
  // {
  //   /* code */
  //   sr.setAllHigh();
  // }
  // sr_tick++;
  
 
}

  