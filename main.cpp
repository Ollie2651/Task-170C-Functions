#include "main.h"
#include "mbed.h"
#include "uop_msb.h"

// LED Bar Display
BusOut dataBits(LED_D0_PIN, LED_D1_PIN, LED_D2_PIN, LED_D3_PIN, LED_D4_PIN, LED_D5_PIN, LED_D6_PIN, LED_D7_PIN);
DigitalOut LED_BAR_OE(LED_BAR_OE_PIN,1);
DigitalOut LED_RED_LE(LED_RED_LE_PIN);
DigitalOut LED_GRN_LE(LED_GRN_LE_PIN);
DigitalOut LED_BLUE_LE(LED_BLUE_LE_PIN);
AnalogIn pot(AN_POT_PIN);
DigitalIn Button(USER_BUTTON);

void setLatch(uint8_t dat, char col);
void led_init(uint8_t dat, bool enabled);
int getDelayMS();
int getPatternRed();
int getPatternGreen();
int RedLightsPatterns[] = {0b00000011, 0b00001100, 0b00110000, 0b11000000, 0b00110000, 0b00001100, 0b00000011, 0b11111111};
int GreenLightsPatterns[] = {0b11000000, 0b00110000, 0b00001100, 0b00000011, 0b00001100, 0b00110000, 0b1100000, 0b11111111};
int BlueLightsPatterns[] = {0b00011000, 0b00100100, 0b01000010, 0b10000001, 0b01000010, 0b00100100, 0b00011000, 0b11111111};
int RedPattern;
int GreenPattern;
int BluePattern;

int main()
{
    printf("Functions demo\n");
    
    while(Button == 0);
    
    for(int i = 0; i < 8; i++) {
        printf("Red: %d\n", RedLightsPatterns[i]);
    }

    led_init(0x00, true);

    while(true){
    for (int i = 0; i < 8; i++) {
        RedPattern = RedLightsPatterns[i];
        setLatch(RedPattern,'r');
    

        //Update the green
        GreenPattern = GreenLightsPatterns[i];
        setLatch(GreenPattern,'g');

        //Update the blue
        BluePattern = BlueLightsPatterns[i];
        setLatch(BluePattern, 'b');


        //Get delay value
        int delay_ms = getDelayMS();

        //Enable all the LED BAR Latch Outputs
        LED_BAR_OE = 0;

        //Wait
        wait_us(delay_ms*250);

        //Disable the latch outputs
        LED_BAR_OE = 1;
        unsigned int potVal = pot.read_u16();
        printf("potentiometer value: %d\n", potVal);

        //Wait
        wait_us(delay_ms*250);
    }
    int i = 0;
    }
}

void setLatch(uint8_t dat, char col)
{
    //For safety
    LED_RED_LE = 0;
    LED_GRN_LE = 0;
    LED_BLUE_LE = 0;

    //Now follow the sequence
    wait_us(1);
    dataBits = dat;    //Set the 8-bit data pattern
    wait_us(1);

    //Depending on the chosen colour
    switch (col)
    {
        case 'r':
            LED_RED_LE  = 1;    //Copy dataBits to red latch outputs
            wait_us(1);         
            LED_RED_LE  = 0;    //Ignore inputs (dataBits)
            break;
        case 'g':
            LED_GRN_LE  = 1;    //Copy dataBits to red latch outputs
            wait_us(1);         
            LED_GRN_LE  = 0;    //Ignore inputs (dataBits)
            break;
        case 'b':
            LED_BLUE_LE  = 1;    //Copy dataBits to red latch outputs
            wait_us(1);         
            LED_BLUE_LE  = 0;    //Ignore inputs (dataBits)
            break;
    }
    wait_us(1); 
}

void led_init(uint8_t dat, bool enabled)
{
    //Disable the LED outputs
    LED_BAR_OE = 1;

    //Set data to 0
    dataBits = dat;

    //Give a little time
    wait_us(1);

    //Enable all latches
    LED_RED_LE  = 1;
    LED_GRN_LE  = 1;
    LED_BLUE_LE  = 1;

    //Give a little time
    wait_us(1);

    //Enable all latches
    LED_RED_LE  = 0;
    LED_GRN_LE  = 0;
    LED_BLUE_LE  = 0;

    if (enabled) {
        LED_BAR_OE = 0;
    }
}

int getDelayMS() 
{
    float p = pot; // 0 ... 1
    int delay = 100 + 900*p;    //100...1000ms
    return delay;
}

/*int getPatternRed() 
{
    for(int i = 0; i < 7; i++) {
        int RedPattern = RedLightsPatterns[i];
        wait_us(1000000);
        return RedPattern;
    }
}
*/


