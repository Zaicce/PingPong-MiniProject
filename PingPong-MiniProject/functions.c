#include <pic32mx.h>
#include <stdint.h>
#include "header.h"
#include <stdlib.h>


int period = 10000;
int flag = 0;
int speed = 3000000;
int timeoutcount = 0;
char player1[] = "Player 1:   ";
char player2[] = "Player 2:   ";
char scoreCap[] = "Score Cap:  ";
char ai[] = "AI      :   ";
char p1s = '0';
char p2s = '0';
char sc = '5';
char ais = '0';
int dif;
char speedOn[] = "Speed Up:  On";
char speedOff[] = "Speed Up:  Off";
int speedUp = 1;

void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--){

	}
}

void screen (void){

    display_string(0, player1);
    display_string(1, player2);
    display_string(2, scoreCap);
}
void init(void){

    TRISECLR = 0xff;

    TRISDSET = 0xE0;
    TRISFSET = 0x2;

    PR2 = period;       //Set the timer period to the variable period
    T2CONSET = 0x70;    //Prescale 256
    T2CONSET = 0x8000;  //Timer enable, bit 15 is set to 1
    TMR2 = 0;           //Sets timer to 0

   // gameSelection();
   // display_update();

}
int getbtns(void){
    int btn1 = (PORTF >> 1) & 0x1;          //Button 1
    int restOfButtons = (PORTD >> 4) & 0xe; //Button 2-4
    int tot = btn1 | restOfButtons;         //Merge these into 1 value and return it
    return tot;
}
void gameSelection (void)
{
    int button;
    display_string(0, "Game Selection");
    display_string(1, "1) 1 Player Mode");
    display_string(2, "2) 2 Player Mode");
    display_string(3, "3) Quick Play");
    display_update();
    delay(2000000);
    p1s = '0';
    p2s = '0';
    ais = '0';
    speedUp = 1;
    PORTE = 0x00;
    while (1)
    {
        button = getbtns();
        if (button == 1)
        {
            difficulty();
        }
        else if (button == 2)
        {
            gameSettings(0);
        }
        else if (button == 4)
        {
            quickPlay();
        }

    }
}
void difficulty(void)
{
    int button;
    int aiEnable = 1;
    display_string(0, "Difficulty");
    display_string(1, "1) Easy");
    display_string(2, "2) Medium");
    display_string(3, "3) Hard");
    display_update();
    delay(2000000);
    while (1)
    {
        button = getbtns();
        if (button == 0x1)
        {
            dif = 75; //easy
            gameSettings(aiEnable);
        }
        if (button == 0x2)
        {
            dif = 85; //medium
            gameSettings(aiEnable);
        }
        if (button == 0x4)
        {
            dif = 90; //hard
            gameSettings(aiEnable);
        }
        if (button == 0x8)
        {
            gameSelection();
        }
    }
}

void gameSettings(int aiEnable)
{
    display_string(2, "2) Speed Up: On");
    gameSettingScreen();
    display_update();
    delay(2000000);
    while (1)
    {
        int buttonstatus = getbtns();
        if (buttonstatus == 0x8)
        {
            gameSelection();
        }
        if (buttonstatus == 0x1)
            {       //Select score cap with button 1, game will end when either players gets this many points
                delay(2000000);
                sc++;
                if(sc > '9')
                {               //Highest possible score is 9, will cycle back to 1 if set to higher than 9
                    sc = '1';
                }
                delay(200000);
                //scoreCap[11]=sc;
            }
        if (buttonstatus == 2 && speedUp == 1)
        {
            display_string(2, "2) Speed Up: Off");
            speedUp = 0;
            delay(2000000);
        }
        else if (buttonstatus == 2 && speedUp == 0)
        {
            display_string(2, "2) Speed Up: On");
            speedUp = 1;
            delay(2000000);
        }
        scoreCap[11] = sc;
        gameSettingScreen();
        display_update();
        delay(200000);

        if (buttonstatus == 0x4 && aiEnable == 0)
        {
            display_string(0, "Press button");
            display_string(1, "2 for Right Side");
            display_string(2, "4 for Left Side");
            display_string(3, "  ");
            display_update();
            while (1)
            {
                buttonstatus = getbtns();
                if(buttonstatus == 0x8 || buttonstatus == 0x2)
                {     //If either button 4 or 2 is pressed the game will begin
                    if(buttonstatus == 0x8)
                    {
                        PORTE = 0x80;
                        flag = 0;
                    }
                    else if(buttonstatus == 0x2)
                    {
                        PORTE = 0x01;
                        flag = 1;
                    }
                    game();
                }
            }
        }
        else if(buttonstatus == 0x4 && aiEnable == 1){
            display_string(0, "Press button");
            display_string(1, "4 to start a");
            display_string(2, "game vs AI ");
            display_string(3, "  ");
            display_update();
            while (1)
            {
                buttonstatus = getbtns();
                //If button 4 is pressed the game will begin
                if(buttonstatus == 0x8)
                {
                    PORTE = 0x80;
                    flag = 0;
                    aiGame();
                }

            }
        }
    }
}
void gameSettingScreen(void)
{
    char score[] = "1) Score Cap:  ";
    score[14] = sc;
    display_string(0, "Game Setting");
    display_string(1, score);
    display_string(3, "3) Done");
}

void quickPlay(void){
    int buttonstatus;
    sc = '5';
    display_string(0, "Press button");
    display_string(1, "2 for Right Side");
    display_string(2, "4 for Left Side");
    display_string(3, "  ");
    display_update();

    while(1){
        buttonstatus = getbtns();
        if(buttonstatus == 0x8 || buttonstatus == 0x2)
        {     //If either button 4 or 2 is pressed the game will begin
            if(buttonstatus == 0x8)
            {
                PORTE = 0x80;
                flag = 0;
            }
            else if(buttonstatus == 0x2)
            {
                PORTE = 0x01;
                flag = 1;
            }
            game();
        }
    }

}


void game(void){
    delay(200000);
    int start = 0;
    while(1){
        int buttonstatus = getbtns();   //Fetch the button status

        if(buttonstatus == 0x2 & PORTE == 0x01| buttonstatus == 0x8 & PORTE == 0x80){
            start = 1;
        }

        if(buttonstatus == 0x1){        //Go back to player menu if button 1 is pressed
            gameSelection();

        }

        //Checks if the clock period is over (time-out)
        if(IFS(0) & 0x100){
            timeoutcount++;     //Increment timercounter
            IFSCLR(0) = 0x100;  //Reset flag by setting bit 8 back to 0
        }
        if(timeoutcount == 10){

            if(PORTE==0x01 & buttonstatus == 0x2){  //If player 2 manages to return the ball the direction is changed
                flag = 1;                           //Changes direction to left
                if (speedUp == 1)
                {
                    PR2 = PR2*0.9;
                }
            }
            else if(buttonstatus == 0x2 & PORTE != 0x01 & flag == 0 & start == 1 | PORTE == 0x00 & flag == 0){   //If player 2 hits too early or not at all
                p1s++;              //player 1 will get another points
                PORTE = 0x80;       //If player 1 scores, then player 1 will start with the ball
                start = 0;          //Pauses game
                PR2 = period;       //Resets the period when a player scores
                delay(400000);
            }


            if(PORTE == 0x80 & buttonstatus == 0x8){    //If player 1 manages to return the ball the direction is changed
                flag = 0;                               //Changes direction to right
                if (speedUp == 1)
                {
                    PR2 = PR2*0.9;
                }
            }
            else if(buttonstatus == 0x8 & PORTE != 0x80 & flag == 1 & start == 1 | PORTE == 0x00 & flag == 1){   //If player 1 hits too early or not at all
                p2s++;              //player 2 will get another point
                PORTE = 0x01;       //Player 2 will start with the ball
                start = 0;          //Pauses game
                PR2 = period;       //Period is reset when a player scores
                delay(400000);
            }

            if(flag == 0 & start == 1){              //If flag (direction) is 0 the the LEDS will shift right
                PORTE = (PORTE >> 1);   //Shifting the bits in porte will shift which LED is lit
            }

            if(flag == 1 & start == 1){              ////If flag (direction) is 0 the the LEDS will shift right
                PORTE = (PORTE << 1);
            }


            player1[11] = p1s;          //Updates player 1's score and puts it in char array player1
            player2[11] = p2s;          //same as above, but for player 2
            scoreCap[11] = sc;          //Will remove in final version

            screen();                   //Updates the information which is sent to the display
            display_update();           //Updates the display
            timeoutcount = 0;           //Resets the timer

            if ( p1s == sc | p2s == sc )
            {
                scoreScreen();
            }
        }
    }
}
void scoreScreen ()
{
    display_string (0, "Congratulations ");
    if (p1s<p2s)
    {
        display_string(1, "PLAYER 2 ");
    }

    else
    {
        display_string(1, "PLAYER 1 ");
    }
    display_update();
    delay(100000000);
    gameSelection();

}
void aiGame ()
{
    delay(200000);
    int ra;
    int start = 0;
    int buttonstatus;
    int temp;

    while(1){
        buttonstatus = getbtns();   //Fetch the button status
        temp = TMR2 * 11 + PR2;
        ra = temp%100;      //Generates a pseudo random number between 0-99

        if(buttonstatus == 0x8 & PORTE == 0x80){    //The player starts the game
            start = 1;
            flag = 0;
        }

        if(buttonstatus == 0x1){        //Go back to game selection if button 1 is pressed
            gameSelection();

        }

        //Checks if the clock period is over (time-out)
        if(IFS(0) & 0x100){
            timeoutcount++;     //Increment timeoutcount
            IFSCLR(0) = 0x100;  //Reset flag by setting bit 8 back to 0
        }
        if(timeoutcount == 10){

            if(PORTE==0x01 & ra <= dif){  //If the AI manages to return the ball the direction is changed
                flag = 1;                           //Changes direction to left
                if (speedUp == 1)       //Checks if speed up is enabled
                {
                    PR2 = PR2*0.9;      //Will only speed up the game if speedUp is 1
                }
            }
            else if(PORTE==0x01 & flag == 0 & start == 1 & ra > dif){   //If the AI fails to return the ball
                p1s++;              //player 1 will get another point
                PORTE = 0x80;       //If player 1 scores, then player 1 will start with the ball
                start = 0;          //Pauses game
                PR2 = period;       //Resets the period when a player scores
                delay(400000);
            }


            if(PORTE == 0x80 & buttonstatus == 0x8){    //If player 1 manages to return the ball the direction is changed
                flag = 0;                               //Changes direction to right
                if (speedUp == 1)
                {
                    PR2 = PR2*0.9;
                }
            }
            else if(buttonstatus == 0x8 & PORTE != 0x80 & flag == 1 & start == 1 | PORTE == 0x00 & flag == 1){   //If player 1 hits too early or not at all
                ais++;              //player 2 will get another point
                PORTE = 0x01;       //Player 2 will start with the ball
                delay(20000000); //start = 0;          //Pauses game
                PR2 = period;       //Period is reset when a player scores
                delay(400000);
            }

            if(flag == 0 & start == 1){              //If flag (direction) is 0 the LEDS will shift right
                PORTE = (PORTE >> 1);   //Shifting the bits in porte will shift which LED is lit
            }

            if(flag == 1 & start == 1){              ////If flag (direction) is 0 the the LEDS will shift right
                PORTE = (PORTE << 1);
            }

            player1[11] = p1s;           //Updates player 1's score and puts it in char array player1
            ai[11] = ais;                //same as above, but for the AI


            aiScreen();                 //Updates the information which is sent to the display
            display_update();           //Updates the display
            timeoutcount = 0;           //Resets the timer

            if ( p1s == sc | ais == sc ){
                aiScoreScreen();
            }

        }
    }
}

//This functions shows the winner of the game, player 1 or AI
void aiScreen (void)
{

    display_string(0, player1);
    display_string(1, ai);
    display_string(2, scoreCap);
}
void aiScoreScreen ()
{
    if (p1s<ais)
    {
        display_string (0, "Too Bad :(");
        display_string (1, "AI Wins ");
    }

    else
    {
        display_string (0, "Congratulations ");
        display_string (1, "PLAYER 1 ");
    }
    display_update();
    delay(100000000);
    gameSelection();

}
