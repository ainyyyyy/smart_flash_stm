/*****************************************************************************
* | File      	:   EPD_2in13_V3_test.c
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper V3 test demo
* | Info        :
*----------------
* |	This version:   V1.1
* | Date        :   2021-10-30
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_Test.h"
#include "EPD_2in13_V3.h"

int EPD_test(char* data, char* data2, char* data3, char* data4, char* data5)
{
    Debug("EPD_2in13_V3_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    Debug("e-Paper Init and Clear...\r\n");
		EPD_2in13_V3_Init();
    EPD_2in13_V3_Clear();

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2in13_V3_WIDTH % 8 == 0)? (EPD_2in13_V3_WIDTH / 8 ): (EPD_2in13_V3_WIDTH / 8 + 1)) * EPD_2in13_V3_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        Debug("Failed to apply for black memory...\r\n");
        return -1;
    }
    Debug("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_2in13_V3_WIDTH, EPD_2in13_V3_HEIGHT, 90, WHITE);
		Paint_Clear(WHITE);

/*#if 1   //show image for array    
    Debug("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_2in13);

    EPD_2in13_V3_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif*/

#if 1  // Drawing on the image
	Paint_NewImage(BlackImage, EPD_2in13_V3_WIDTH, EPD_2in13_V3_HEIGHT, 90, WHITE);  	
    Debug("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    //Paint_Clear(WHITE);
		
		Paint_DrawString_EN(140, 0, data5, &Font16, WHITE, BLACK);
		Paint_DrawString_EN(10, 0, "Overwrites, num:", &Font12, WHITE, BLACK);
		
    Paint_DrawString_EN(140, 26, data, &Font16, WHITE, BLACK);
		Paint_DrawString_EN(10, 26, "Reliability, %:", &Font12, WHITE, BLACK);
		
		Paint_DrawString_EN(140, 52, data2, &Font16, WHITE, BLACK);
		Paint_DrawString_EN(10, 52, "Free Space, byte:", &Font12, WHITE, BLACK);
		
		Paint_DrawString_EN(140, 79, data3, &Font16, WHITE, BLACK);
		Paint_DrawString_EN(10, 79, "Used Space, byte:", &Font12, WHITE, BLACK);
		
		Paint_DrawString_EN(140, 104, data4, &Font16, WHITE, BLACK);
		Paint_DrawString_EN(10, 104, "Total Space, byte:", &Font12, WHITE, BLACK);
		
		
		
    EPD_2in13_V3_Display_Base(BlackImage);
    DEV_Delay_ms(300);
		free(BlackImage);
    BlackImage = NULL;
#endif

/*#if 1   //Partial refresh, example shows time
	Paint_NewImage(BlackImage, EPD_2in13_V3_WIDTH, EPD_2in13_V3_HEIGHT, 90, WHITE);  
    Debug("Partial refresh\r\n");
    Paint_SelectImage(BlackImage);
	
    PAINT_TIME sPaint_time;
    sPaint_time.Hour = 12;
    sPaint_time.Min = 34;
    sPaint_time.Sec = 56;
    UBYTE num = 10;
    for (;;) {
        sPaint_time.Sec = sPaint_time.Sec + 1;
        if (sPaint_time.Sec == 60) {
            sPaint_time.Min = sPaint_time.Min + 1;
            sPaint_time.Sec = 0;
            if (sPaint_time.Min == 60) {
                sPaint_time.Hour =  sPaint_time.Hour + 1;
                sPaint_time.Min = 0;
                if (sPaint_time.Hour == 24) {
                    sPaint_time.Hour = 0;
                    sPaint_time.Min = 0;
                    sPaint_time.Sec = 0;
                }
            }
        }
        Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
        Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);

        num = num - 1;
        if(num == 0) {
            break;
        }
		EPD_2in13_V3_Display_Partial(BlackImage);
        DEV_Delay_ms(500);//Analog clock 1s
    }
#endif*/

	
	
	
	/*Debug("Clear...\r\n");
	EPD_2in13_V3_Init();
    EPD_2in13_V3_Clear();
	
    Debug("Goto Sleep...\r\n");
    EPD_2in13_V3_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    Debug("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();*/
    return 0;
}

