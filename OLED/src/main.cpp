#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
*/

#define u8g_logo_width 106
#define u8g_logo_height 64
static unsigned char u8g_logo_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x06, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x3E, 0x00, 0x03, 0x60, 0x1C, 0xE3, 0x45, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xCD, 0x03, 0x02, 0x62, 0x1C, 0x17, 0x44, 0x47, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xA6, 0x00, 0x04, 0x62, 0x84, 0x14, 
  0x47, 0x53, 0x00, 0x00, 0x00, 0x02, 0x00, 0x80, 0xFE, 0x00, 0x64, 0xE1, 
  0x5D, 0xE8, 0x7B, 0x8F, 0x05, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x3F, 0x00, 
  0x24, 0xC1, 0xF1, 0x9C, 0xC0, 0x31, 0x05, 0x00, 0x00, 0x25, 0x00, 0x00, 
  0xB8, 0x00, 0x22, 0x41, 0x0C, 0x84, 0x5B, 0x34, 0x04, 0x00, 0x00, 0xCD, 
  0x00, 0x00, 0x87, 0x80, 0x41, 0xC1, 0x84, 0x8C, 0x5E, 0xCC, 0x03, 0x00, 
  0x00, 0x1D, 0x01, 0xC0, 0x43, 0x30, 0x80, 0x41, 0x8C, 0x85, 0x48, 0x06, 
  0x02, 0x00, 0x00, 0x3D, 0xC6, 0xF1, 0x41, 0x07, 0x00, 0xEF, 0xF3, 0xC7, 
  0xE0, 0xF3, 0x01, 0x00, 0x00, 0x3D, 0x28, 0xD1, 0x41, 0x00, 0x00, 0x00, 
  0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7D, 0x30, 0xCA, 0x23, 0x00, 
  0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7D, 0xE0, 0xCC, 
  0x20, 0x00, 0xF0, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5, 
  0xE0, 0x56, 0x10, 0x00, 0x0F, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x87, 0xC0, 0xA1, 0xE7, 0x60, 0x00, 0x7F, 0x00, 0x00, 0xF0, 0x17, 
  0x00, 0x00, 0x00, 0x0B, 0x00, 0x21, 0x20, 0x19, 0xE0, 0x3F, 0x00, 0xE0, 
  0x7F, 0x3F, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x10, 0x70, 0x07, 0xFC, 0xBF, 
  0xDF, 0x01, 0x02, 0xE0, 0x00, 0x00, 0x00, 0x4E, 0x00, 0x00, 0x00, 0x00, 
  0x4A, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x2E, 0x00, 0x00, 
  0x00, 0x00, 0xA0, 0x00, 0x00, 0x70, 0x00, 0x00, 0x04, 0x00, 0x00, 0x04, 
  0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0xFF, 0x3F, 0xF1, 0x0F, 0x08, 0x00, 
  0x00, 0x04, 0x0E, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x90, 0xFC, 0x3F, 
  0x08, 0x00, 0x00, 0x9C, 0x3D, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x50, 
  0xFE, 0x5F, 0x14, 0x00, 0xE0, 0x67, 0x3C, 0x00, 0x60, 0x00, 0x14, 0x00, 
  0x00, 0x48, 0xFF, 0xBF, 0x14, 0x00, 0xE0, 0x43, 0x7F, 0x00, 0xFE, 0x01, 
  0x02, 0x00, 0x03, 0x48, 0xFF, 0x7F, 0x14, 0x00, 0x40, 0x00, 0x7E, 0x80, 
  0xF9, 0x03, 0x01, 0x00, 0xFB, 0x4F, 0xFF, 0xFF, 0x14, 0x00, 0xE0, 0x00, 
  0x30, 0x00, 0x64, 0x1F, 0x01, 0x80, 0x03, 0x48, 0xF9, 0xFF, 0x12, 0x00, 
  0xC0, 0x00, 0x00, 0x00, 0x20, 0x03, 0x01, 0x80, 0x03, 0xC8, 0xF7, 0x7F, 
  0x0A, 0x00, 0xC0, 0x01, 0x00, 0x03, 0x00, 0x00, 0x01, 0x80, 0x03, 0x88, 
  0xFF, 0x3F, 0x09, 0x00, 0x80, 0x03, 0x00, 0x03, 0x00, 0x00, 0x01, 0x83, 
  0x7F, 0x08, 0x0F, 0x8F, 0x04, 0x00, 0x00, 0x07, 0x10, 0x00, 0x00, 0x00, 
  0x21, 0x06, 0xFE, 0x1F, 0xF8, 0x3F, 0x06, 0x00, 0x00, 0x0C, 0x00, 0x3E, 
  0x00, 0x00, 0x67, 0x03, 0xE3, 0x1F, 0x00, 0x00, 0x03, 0x00, 0x00, 0x30, 
  0x00, 0x00, 0x00, 0xC0, 0xEF, 0x20, 0x01, 0x1E, 0x00, 0x80, 0x00, 0x00, 
  0x00, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0xA7, 0x01, 0x10, 0x00, 0x40, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0xB8, 0xFF, 0xBF, 0x01, 0x10, 
  0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0xF2, 0x01, 0x70, 0x86, 0xFF, 0xFF, 
  0x03, 0x08, 0x07, 0x04, 0x00, 0x00, 0x00, 0x20, 0x82, 0xFC, 0x01, 0xDC, 
  0xFF, 0xFF, 0x1F, 0x88, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x67, 0x01, 
  0xC0, 0x0F, 0xFF, 0xFF, 0xFF, 0xC4, 0x1F, 0x02, 0x00, 0x00, 0x00, 0xC0, 
  0xFF, 0x81, 0xBF, 0x8F, 0x7F, 0x1C, 0xFF, 0xCF, 0x0F, 0x01, 0x00, 0x00, 
  0x00, 0xC0, 0x7F, 0xFC, 0x7F, 0x8F, 0x3F, 0x18, 0xC2, 0x0F, 0x83, 0x00, 
  0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xBF, 0xBE, 0xFF, 0x0F, 0x01, 0x1F, 
  0xC0, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0xFF, 0x3F, 0xFC, 0xFF, 0x87, 
  0x01, 0xFC, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0x1F, 0xFD, 
  0xFF, 0xC3, 0x00, 0xE0, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 
  0x5F, 0xFF, 0xFF, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 
  0xFF, 0xFF, 0xDF, 0xFF, 0xC9, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x0F, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x10, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x10, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0xFE, 
  0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 
  0xFF, 0x7E, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 
  0xFF, 0xFF, 0x7F, 0x3D, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x80, 0xFF, 0xFF, 0xFF, 0x3A, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x0F, 0x40, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0x1D, 0x3E, 0x20, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x7F, 0x0C, 0xDC, 
  0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x3F, 
  0x04, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0x0F, 0x00, 0x40, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.clearBuffer();				
 /* u8g2.setFont(u8g2_font_ncenB08_tr);	
  u8g2.drawStr(0,10,"Hello World!");	*/
  u8g2.drawXBM( 0, 0, u8g_logo_width, u8g_logo_height, u8g_logo_bits);	
  u8g2.sendBuffer();	
  

  delay(1000);  
}
