/*==============================================================================
 Project: Intro-4-Functions
 Date:    May 16, 2021
 
 This program demonstrates the use of functions, and variable passing between
 the main and function code.
 
 Additional program analysis and programming activities examine function code
 location, function prototypes, and reinforce the concepts of global and local
 variables.
==============================================================================*/

#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "UBMP4.h"           // Include UBMP4 constants and functions

// TODO Set linker ROM ranges to 'default,-0-7FF' under "Memory model" pull-down.
// TODO Set linker code offset to '800' under "Additional options" pull-down.

// Button constant definitions
const char noButton = 0;
const char UP = 1;
const char DOWN = 2;
const char MEIKAI = 3;
const char POGGERS = 4;

const unsigned int D4NOTE = 3405;
const unsigned int D5NOTE = 1703;
const unsigned int A4NOTE = 2273;
const unsigned int AF4NOTE = 2408;


// Program variable definitions
unsigned char LED5Brightness = 125;
unsigned char button;

unsigned char button_pressed(void);
void pwm_LED5(unsigned char pwmValue);
unsigned char getButtonPressed();
void lightLed(unsigned char);
void clearLeds();
void delay_us(unsigned int us);
void playNote(unsigned int period);
unsigned int getTone();

int main(void) {
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure on-board UBMP4 I/O devices
	
    while(1)
	{
        // Read up/down buttons and adjust LED5 brightness
        unsigned int tone = getTone();
        if(tone != 0){
            playNote(tone);
        }
        // Activate bootloader if SW1 is pressed.
        if(SW1 == 0)
        {
            RESET();
        }
    }
}

// Move the function code to here in Program Analysis, step 5.

void delay_us(unsigned int us) {
    while(us > 10000) {
        __delay_us(10000);
        us -= 10000;
    }
    while(us > 1000) {
        __delay_us(1000);
        us -= 1000;
    }
    while(us > 100) {
        __delay_us(100);
        us -= 100;
    }
    while(us > 10) {
        __delay_us(10);
        us -= 10;
    }
}

void playNote(unsigned int period) {
    for(unsigned char cycles = 10; cycles != 0; cycles--) {
        BEEPER = !BEEPER;
        for(unsigned int p = period; p != 0; p--);
    }
}

unsigned int getTone() {
    if(SW2 == 0) {
        return A4NOTE; 
    }
    if(SW3 == 0) {
        return D4NOTE;
    }
    if(SW4 == 0) {
        return D5NOTE;
    }
    if(SW5 == 0) {
        return AF4NOTE;
    }
    return 0;
}

unsigned char getButtonPressed() {
    if(SW2 == 0) {
        return 2;
    }
    if(SW3 == 0) {
        return 3;
    }
    if(SW4 == 0) {
        return 4;
    }
    if(SW5 == 0) {
        return 5;
    }
}

void clearLeds() {
    LATC = 0;
}

void lightLed(unsigned char led) {
    switch(led) {
        case 2:
            LED2 = 1;
            break;
        case 3:
            LED3 = 1;
            break;
        case 4:
            LED4 = 1;
            break;
        case 5:
            LED5 = 1;
            break;
        case 6:
            LED6 = 1;
            break;
    }
}

unsigned char button_pressed(void) {
    if(SW4 == 0)
    {
        return(UP);
    }
    else if(SW5 == 0)
    {
        return(DOWN);
    }
    else if(SW3 == 0) {
        return MEIKAI;
    } else if(SW2 == 0) {
        return POGGERS;
    }
    else
    {
        return(noButton);
    }
}

void pwm_LED5(unsigned char pwmValue) {
    for(unsigned char t = 255; t != 0; t --)
    {
        if(pwmValue == t)
        {
            LED5 = 1;
        }
        __delay_us(20);
    }
    // End the pulse if pwmValue < 255
    if(pwmValue < 255)
    {
        LED5 = 0;
    }
}

/* Program Analysis
 * 
 * 1.   Which function in this program will run first? How do you know?
 * 
 * OSC_config() runs first because it's right at the start
 * 
 * 2.   What is the purpose of the 'unsigned char' variable type declaration in
 *      the button_pressed() function? Is it used by this function to receive
 *      a variable from, or return a variable to the main code?
 * 
 * That means the function returns an unsigned char variable
 * 
 * 3.   How does the function call statement 'button = button_pressed();' in the
 *      main code support your answer in 2, above?
 * 
 * It puts the value of button_pressed in the button variable
 * 
 * 4.   What is the purpose of the 'unsigned char' variable type declaration in
 *      the pwm_LED5() function? Where does the value of the variable come from?
 *      Where does this value get stored in the function?
 * 
 * The function needs an unsigned char to know how to do the pwm. This is required so it loops a different amount each time.
 * It is stored in the function and deleted when it leaves the function.
 * 
 * 5.   C language compilers typically read through the entire program in a
 *      single pass, converting C code into machine code. The two functions,
 *      button_pressed() and pwm_LED5(), are located above the main() function
 *      so that their memory locations and variable requirements will be
 *      determined before the rest of the program compiles. When the compiler
 *      interprets the function call statements in the main code, it already
 *      knows where those functions are be in memory and which memory locations
 *      are used by their variables.
 * 
 *      Try moving the button_pressed() and pwm_LED5() functions to below the
 *      closing brace of the main() function, and build the code. What happens?
 * 
 *      The compiler should have reported an error since it did not understand
 *      what the function call was referring to since it had not seen the named
 *      function before the function call. We can eliminate this error by adding
 *      a function prototype above the main code. The function prototype is like
 *      the function declaration (the first line of the function), and lets the
 *      compiler know both the function's name and the type of variables that
 *      the function will use. A C language 'linker' program will take care of
 *      sorting out the names and memory locations of all of the functions and
 *      variables in a program following the compilation step.
 * 
 *      Let's try this out. Leave the functions in their new location, below
 *      the main() function, and add the two function prototypes (shown below)
 *      above main(), in the location where the functions were originally
 *      located:

unsigned char button_pressed(void);

void pwm_LED5(unsigned char);

 *      What is the difference between the function prototype for pwm_LED5()
 *      and the actual pwm_LED5() function declaration statement later in the
 *      code?
 * 
 * The prototype has no code, it just defines what the function will look like.
 * 
 * 6.   Building the program with the added function prototypes should now work
 *      without generating errors, just as it did in the original program.
 * 
 *      In C, functions may be located above the code that calls them, below
 *      their function calls if a function prototype is supplied, or even in 
 *      completely separate files known as function libraries.
 * 
 *      Function libraries are typically joined to your project's main '.c' file
 *      using an 'include' statement to a '.h' header file containing both
 *      variable definitions and the function prototypes for an associated C
 *      file which contains the actual function code.
 * 
 *      The '#include "UBMP4.h"' statement near the top of this program is an
 *      example of a statement that adds external functions to our program. The
 *      'UBMP4.h' file contains function prototypes for functions located in
 *      the 'UBMP4.c' file, along with various symbolic constants used by both
 *      our program and the code in the UBMP4.c functions.
 * 
 *      Open the UBMP4.c file to find the OSC_config() and UBMP4_config()
 *      functions called from the main() function in this program. Are any
 *      values passed between this code and the two setup functions? How do
 *      you know?
 * 
 * No values are passed to them or out of them because they say void.
 * 
 * 7.   The 'button' variable is a global variable because it was assigned
 *      at the beginning of the program, outside of any functions. Global
 *      variables are available to all functions. How does the 'button' variable
 *      get assigned a value? In which function does this occur?
 * 
 * Button gets assigned a value in main.
 * 
 * 8.   Which variable does the value of LED5Brightness get transferred to in
 *      the pwm_LED5() function? Is this variable global, or local to the LED
 *      function? Could the pwm_LED5 function use the LED5Brightness variable
 *      directly, instead of transferring its value to another variable?
 * 
 * It gets transferred into pwm_LED5. It could use it since it's a global variable, but that would make the function less versatile.
 * 
 * Programming Activities
 * 
 * 1.   It might be useful to have a button that instantly turns LED D5 fully
 *      on or off instead of waiting for it to brighten and dim while the UP
 *      and DOWN buttons are held to change the PWM duty cycle. But, PWM dimming
 *      dimming capability is still a useful feature that should be retained.
 * 
 *      Modify the button_pressed() and main() functions to use SW3 as an
 *      instant on button, and SW2 as an instant off button. Pressing either of
 *      these buttons will over-write the current LED5Brightness value with
 *      either 255 or 0, while still allowing SW4 and SW5 to adjust the
 *      brightness in smaller increments when pressed.
 *
 * I have done it
 * 
 * 2.   Create a function that will return a number from 1-4 corresponding to
 *      which of the SW2 to SW5 switches is pressed, or return 0 if no switches
 *      are pressed. Then, create a function that will accept a number from 1 to
 *      4 that lights the corresponding LED beside each button.
 * 
 * Cool
 * 
 * 3.   Create a sound function that receives a parameter representing a tone's
 *      period. Modify your button function, above, to return a variable that
 *      will be passed to the sound function to make four different tones.
 * 
 * Very cool
 * 
 * 4.   A function that converts an 8-bit binary value into its decimal
 *      equivalent would be useful for helping us to debug our programs. Create
 *      a function that converts an 8-bit binary number into three decimal
 *      character variables representing the hundreds, tens and ones digits
 *      of the binary number passed to it. For example, passing the function
 *      a value of 142 will result in the hundreds variable containing the
 *      value 1, the tens variable containing 4, and the ones variable 2. How
 *      could you test this function to verify that it works? Try it!
 * 
 * Did it but I like my music code more
 */
