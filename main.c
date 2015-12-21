//Sara Cagle main.c

#include <stdlib.h>
#include <stdio.h>
#include "images/bullet.h" 
#include "images/cirno.h" 
#include "images/splashScreen.h"
#include "images/horzBullet.h"
#include "images/loseScreen.h"
#include "images/directionScreen.h"
#include "images/creditScreen.h"
#include "images/winScreen.h"
#include "myLib.h"
#define WINNINGSCORE 20
#define NUMBULLET 2




u16 *videoBuffer = (u16 *)0x6000000;

enum {SPLASH, DIRECTIONS, GAME, WIN, LOSE, CREDITS};

int splashPage();
void directionPage();
int playGame(int);
void winGame();
int loseGame();
void creditPage();

int main(void){
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	int seed = 0;
	int state = SPLASH;
	while(1){
		
		switch(state){

		case SPLASH:
			seed = splashPage();
			state = DIRECTIONS;
			break;
		case DIRECTIONS:
			directionPage();
			state = GAME;
			break;
		case GAME:
			state = playGame(seed);
			break;
		case WIN:
			winGame();
			state = SPLASH;
			break;
		case LOSE:
			state = loseGame();
			break;
		case CREDITS:
			creditPage();
			state = SPLASH;
		default:
			break;
		}
	}
}

int splashPage(){
	int seed=0;
	u16 *pointer = videoBuffer;
	for(int i=0; i<38400; i++){ //paints screen black when you restart
		*pointer++ = BLACK;
	}
	drawImage(0,0,splashPageImage,240,144); //draw splash screen
	while(!KEY_DOWN_NOW(BUTTON_START)){
		seed++;
	}
	while(KEY_DOWN_NOW(BUTTON_START));
	return seed;
}	

void directionPage(){
	drawImage(0,0,directionScreen,240,160); //draw direction page
	writeDirections();
	drawHollowRect(100,100,100,100,RED);
	while(1){
		if(KEY_DOWN_NOW(BUTTON_START)){
			return;
		}
	}
}


int playGame(int seed){
	u16 *pointer = videoBuffer;
	for(int i=0; i<38400; i++){ //paints screen black to start
		*pointer++ = BLACK;
	}	

	srand(seed);
	BULLET bullets[NUMBULLET]; //initializing the bullets
	BULLET oldBullets[NUMBULLET]; //initializing old bullets
	BULLET *current; //pointer to current bullet
	char buffer[41]; //you can only print 41 char on the screen
	int score=0;
	int delta[] = {1, 2, 3};
	int numDelta = sizeof(delta)/sizeof(delta[0]);
	for(int i=0; i<NUMBULLET; i++){
		bullets[i].y =  rand()%240;
		bullets[i].x = 0;
		bullets[i].xDelta = delta[rand()%numDelta];
		oldBullets[i] = bullets[i];
	}
	HORZBULLET horzBullets[NUMBULLET];
	HORZBULLET oldHorzBullets[NUMBULLET]; //initializing old bullets
	HORZBULLET *hcurrent;
	for(int i=0; i<NUMBULLET; i++){
		horzBullets[i].y = 0;
		horzBullets[i].x = rand()%130;
		horzBullets[i].yDelta = delta[rand()%numDelta];
		oldHorzBullets[i] = horzBullets[i];
	}


	//player initialization
	PLAYER you;
	you.x = 129;
	you.y= 120-11;
	drawPlayer(you.x, you.y);

	sprintf(buffer, "SCORE: %d", score);//draw the score
	drawRect(150, 0, 10, 240, NICEBLUE);
	drawString(153, 100, buffer, WHITE);

	


	while(1){
		
		waitForVblank();
		//player controls
		if(KEY_DOWN_NOW(BUTTON_UP)){
			if(you.x-1!=0){ //don't go off top of screen
				movePlayer(you.x, you.y);
				drawPlayer(you.x--, you.y);
			}
		}
		if(KEY_DOWN_NOW(BUTTON_DOWN)){
			if(you.x+15+1!=150){ //don't go off bottom of screen
				movePlayer(you.x, you.y);
				drawPlayer(you.x++, you.y);
			}
		}
		if(KEY_DOWN_NOW(BUTTON_LEFT)){
			if(you.y-1!=0){ //don't go off left side of screen
				movePlayer(you.x, you.y);
				drawPlayer(you.x, you.y--);
			}
		}
		if(KEY_DOWN_NOW(BUTTON_RIGHT)){
			if(you.y+15+1!=240){ //don't go off right side of screen
				movePlayer(you.x, you.y);
				drawPlayer(you.x, you.y++);
			}
		}
		
		//turn this code on to avoid clipping the score
		//but it slows down significantly
		/*sprintf(buffer, "SCORE: %d", score);//draw the score
		drawRect(150, 0, 10, 240, NICEBLUE);
		drawString(153, 100, buffer, WHITE);*/


		//vertical bullets--------------------------------------------------------
		for(int i=0; i<NUMBULLET;i++){ //updating bullet locations
			current = bullets + i; //current points to ith object in array
			current->x = current->x + current->xDelta;
			if(collision(current->x, current->y, you.x, you.y)){
				//you've hit a bullet, you lose!
				return LOSE;
			}
			if(getPoint(current->x, you.x)){ //check to see if you've passed a bullet
				score++;
				sprintf(buffer, "SCORE: %d", score);//drawing the score
				drawRect(150, 0, 10, 240, NICEBLUE);
				drawString(153, 100, buffer, WHITE);
				if(score>=WINNINGSCORE){
					return WIN;
				}
			}
			if(current->x>=134){ //if current bullet hits the bottom
				current->x=0; //recycled back to top
				current->y =  rand()%240;
			}
		}

		for(int i=0; i<NUMBULLET; i++){ //drawing black over old bullets
			drawRect(oldBullets[i].x, oldBullets[i].y, 16, 16, BLACK);
		}
		for(int i=0; i<NUMBULLET; i++){ //drawing new bullets
			current = bullets+i;	
			drawImage(current->x, current->y, bullet, 16, 16);
			oldBullets[i] = bullets[i];
		}

		//horz bullets --------------------------------------------------------------

		for(int i=0; i<NUMBULLET;i++){ //updating bullet locations
			hcurrent = horzBullets + i; //current points to ith object in array
			hcurrent->y = hcurrent->y + hcurrent->yDelta;
			if(horzCollision(hcurrent->x, hcurrent->y, you.x, you.y)){
				//you've hit a bullet, you lose!
				movePlayer(you.x, you.y);
				return LOSE;
			}
			if(hcurrent->y>=224){ //if current bullet hits the bottom
				hcurrent->y=0;
				hcurrent->x = rand()%130;
			}
		}

		for(int i=0; i<NUMBULLET; i++){ //drawing black over old bullets
			drawRect(oldHorzBullets[i].x, oldHorzBullets[i].y, 16, 16, BLACK);
		}
		for(int i=0; i<NUMBULLET; i++){ //drawing new bullets
			hcurrent = horzBullets+i;	
			drawImage(hcurrent->x, hcurrent->y, horzBullet, 16, 16);
			oldHorzBullets[i] = horzBullets[i];
		}
		//-----------------------------------------------------------


		if(KEY_DOWN_NOW(BUTTON_SELECT)){ //if you press any button, you lose
			return LOSE;
		}
	}	
}

void winGame(){
	drawImage(0,0,winScreen,240,160);
	while(1){
		if(KEY_DOWN_NOW(BUTTON_START)){
			return;
		}
	}
}

int loseGame(){
	drawImage(0,0,loseScreen,240,160); //draw lose page
	//int contCount = 10;
	//char buffer[41];

	while(1){
	//	while(contCount>0){
			/*int i=0;
			int j=0;
			while(i<100000000){
				j=i;
				i++;
			}
			//drawRect(150,90, 40,50, BLACK);
			sprintf(buffer, "%d",contCount);
			contCount--;
			drawString(150, 100, buffer, WHITE);*/


		if(KEY_DOWN_NOW(BUTTON_A)|KEY_DOWN_NOW(BUTTON_START)){
			return SPLASH;
		}
		if(KEY_DOWN_NOW(BUTTON_B)){
			return CREDITS;
		}
	}
}

void creditPage(){
	drawImage(0,0,creditScreen,240,160);
	while(1){
		if(KEY_DOWN_NOW(BUTTON_START)){
			return;
		}
	}

}


