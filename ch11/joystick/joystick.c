/* ------------------------------------------------------------------------- */
/* Commodore 128 Assembly Language                                           */
/* Controlling Animation with Hand Controllers                               */
/* Listing 11.2 JOYSTICK.BAS -> joystick.c                                   */
/*                                                                           */
/* This program "uses the C-128's 40-column text mode, and contains some     */
/* very important principles that are applicable to graphics programs        */
/* written in assembly language" (Andrews 1986).                             */
/* ------------------------------------------------------------------------- */
#include <c128.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;

#define CH_SPACE    32
#define CH_BALL     81
#define CH_UPPER    142

#define VICSCN  ((uchar *) 0x0400)      // VIC 40-col text screen
#define VICCOL  ((uchar *) 0xd800)      // VIC 40-col text screen
#define D1PRA   ((uchar *) 0xdc00)      // VIC 40-col text screen

#define XY(x,y) (40*(y)+(x))            // base VICSCN/COL offset

// Start in the centre of the screen
int x = 40/2;
int y = 25/2;

/* ------------------------------------------------------------------------- */
void screenInit()
{
    unsigned i;

    clrscr();
    putchar(CH_UPPER);

    bordercolor(COLOR_PURPLE);
    bgcolor(COLOR_BLACK);

    // We want yellow characters all across the screen
    for (i = 0; i < 1000; ++i)
        VICCOL[i] = COLOR_YELLOW;
}

/* ------------------------------------------------------------------------- */
void delay(unsigned cnt)
{
    while (cnt--)
        ;
}

/* ------------------------------------------------------------------------- */
void moveUp()
{
    if (--y < 0)
        y = 24;
}

void moveDown()
{
    if (++y > 24)
        y = 0;
}

void moveLeft()
{
    if (--x < 0)
        x = 39;
}

void moveRight()
{
    if (++x > 39)
        x = 0;
}

/* ------------------------------------------------------------------------- */
void readJoystick()
{
    uchar joy, btn;

    // Print dot at screen position (x,y)
    VICSCN[XY(x,y)] = CH_BALL;

    joy = *D1PRA;                       // joystick value
    btn = 0x10 & joy;                   // trigger button

    // Clear spot when trigger is NOT pressed
    delay(500);
    if (0x10 == btn)
        VICSCN[XY(x,y)] = CH_SPACE;

    // Convert switch values to an number between 0 and 10
    switch(0x0f - (0x0f & joy))
    {
        case  1: moveUp();                      break;
        case  2: moveDown();                    break;
        case  4: moveLeft();                    break;
        case  5: moveLeft();    moveUp();       break;
        case  6: moveLeft();    moveDown();     break;
        case  8: moveRight();                   break;
        case  9: moveRight();   moveUp();       break;
        case 10: moveRight();   moveDown();     break;
    }
}

/* ------------------------------------------------------------------------- */
int main()
{
    screenInit();

    do
    {
        readJoystick();
#if 0
        gotoxy(33,0);
        cprintf("[%02d,%02d]", x, y);
#endif
    } while (!kbhit() || 'q' != cgetc());

    return EXIT_SUCCESS;
}
