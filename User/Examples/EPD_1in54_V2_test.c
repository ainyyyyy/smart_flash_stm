/*****************************************************************************
* | File      	:		EPD_1in54_V2_test.c
* | Author      :   Waveshare team
* | Function    :   1.54inch e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-11
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
#include "EPD_1in54_V2.h"
#include "string.h"


int EPD_test(char* buff,char* buff2,char* buff3,char* buff4,char* buff5)
{
    printf("EPD_1in54_V2_test Demo\r\n");
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_1IN54_V2_Init();
    EPD_1IN54_V2_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_1IN54_V2_WIDTH % 8 == 0)? (EPD_1IN54_V2_WIDTH / 8 ): (EPD_1IN54_V2_WIDTH / 8 + 1)) * EPD_1IN54_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_1IN54_V2_WIDTH, EPD_1IN54_V2_HEIGHT, 270, WHITE);

/*#if 1   //show image for array    
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_1in54);

    EPD_1IN54_V2_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif
*/
#if 1   // Drawing on the image
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
		
    // 2.Drawing on the image
    Paint_DrawString_EN(5, 0, "Overwrites:", &Font20, WHITE, BLACK);
    Paint_DrawString_EN(5, 20, buff5, &Font20, WHITE, BLACK);
    Paint_DrawString_EN(5, 40, "Reliability,%:", &Font20, WHITE, BLACK);
		Paint_DrawString_EN(5, 60, buff, &Font20, WHITE, BLACK);
		Paint_DrawString_EN(5, 80, "Free Space:", &Font20, WHITE, BLACK);
		Paint_DrawString_EN(5, 100, buff2, &Font20, WHITE, BLACK);
		Paint_DrawString_EN(5, 120, "Used Space:", &Font20, WHITE, BLACK);
		Paint_DrawString_EN(5, 140, buff3, &Font20, WHITE, BLACK);
		Paint_DrawString_EN(5, 160, "Total Space:", &Font20, WHITE, BLACK);
		Paint_DrawString_EN(5,180, buff4, &Font20, WHITE, BLACK);


    EPD_1IN54_V2_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif



    printf("Clear...\r\n");
    EPD_1IN54_V2_Init();
    //EPD_1IN54_V2_Clear();

    printf("Goto Sleep...\r\n");
    EPD_1IN54_V2_Sleep();
    free(BlackImage);
    BlackImage = NULL;

    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

