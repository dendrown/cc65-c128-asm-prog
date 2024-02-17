/* ------------------------------------------------------------------------- */
/* Commodore 128 Assembly Language                                           */
/* C-128 Screen Memory                                                       */
/* Listing 11.1 BALLBOUNCE.BAS -> ballbounce.c                               */
/*                                                                           */
/* This program "uses the Commodore 128's 40-column text mode rather than    */
/* its high-resolution mode, but it creates a colorful display with some     */
/* entertaining animation—and it also illustrates some important principles  */
/* about Commodore 128 graphics" (Andrews 1986).                             */
/* ------------------------------------------------------------------------- */
#include <c128.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;

#define CH_BALL     81
#define CH_SPACE    96
#define CH_RULE     99
#define CH_UPPER    142

#define BALL_TL     320                 // Top-left of ball area
#define BALL_BL     640                 // Bottom-left
#define BALL_BR     679                 // Bottom-right

#define FLOOR_L     680
#define FLOOR_R     719

#define VICSCN  ((uchar *) 0x0400)      // VIC 40-col text screen
#define VICCOL  ((uchar *) 0xd800)      // VIC 40-col text screen

uchar frameCount = 1;
unsigned position = 640;

/* ------------------------------------------------------------------------- */
void screenInit()
{
    unsigned i;

    clrscr();
    putchar(CH_UPPER);

    for (i = BALL_TL; i <= BALL_BR; ++i)
        VICCOL[i] = COLOR_RED;

    for (i = FLOOR_L; i <= FLOOR_R; ++i)
    {
        VICCOL[i] = COLOR_YELLOW;
        VICSCN[i] = CH_RULE;
    }

    bgcolor(COLOR_BLACK);
    bordercolor(COLOR_BLUE);
    textcolor(COLOR_WHITE);
    cputsxy(6, 2, "follow the bouncing ball...");
}

/* ------------------------------------------------------------------------- */
void delay(unsigned cnt)
{
    while (cnt--)
        ;
}

/* ------------------------------------------------------------------------- */
void moveBall(bool isUp)
{
    uchar i;

    for (i = 1; i <= 8; ++i)
    {
        // Display the ball
        VICSCN[position] = CH_BALL;
        delay(1000);
        VICSCN[position] = CH_SPACE;

        // After moving 40 chars to the right, we reset
        if (++frameCount > 40)
        {
            position = isUp ? BALL_TL       // upper left
                            : BALL_BL;      // lower left
            frameCount = 1;
            break;
        }
        else
            position += (isUp ? -39 : 41);  // up|down & right one
    }
}

/* ------------------------------------------------------------------------- */
int main()
{
    screenInit();

    while (!kbhit())
    {
        moveBall(true);     // what goes up
        moveBall(false);    // must come down
    }

    return EXIT_SUCCESS;
}
