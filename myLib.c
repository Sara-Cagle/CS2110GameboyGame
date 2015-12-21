//Sara Cagle myLib.c

#include "images/cirno.h"
#include "myLib.h"

void setPixel(int x, int y, u16 color){
	videoBuffer[OFFSET(x, y, 240)] = color;
}

void drawRect(int xRow, int yCol, int width, int height, u16 color){

	for(int x=0; x<=width; x++){
		for(int y=0; y<=height; y++){
			setPixel(xRow+x, yCol+y, color); //debug
		}
	}
}

void drawHollowRect(int x, int y, int width, int height, u16 color){
	for(int y=0; y<height; y++){ //vertical lines
		setPixel(0, y, color);
		setPixel(width, y, color);
	}
	for(int x=0; x<width; x++){ //horizontal lines
		setPixel(x, 0, color);
		setPixel(x, height, color);
	}
}

void waitForVblank(){
	while(SCANLINECOUNTER >160);
	while(SCANLINECOUNTER < 160);
}

void drawImage(int x, int y, const unsigned short imageArray[], int width, int height){
	for(int i=0; i<=height; i++){
		for(int j=0; j<=width; j++){
			setPixel(x+i, y+j, imageArray[OFFSET(i, j, width)]);
		}
	}		
}

void movePlayer(int oldX, int oldY){
	drawRect(oldX, oldY, 16, 16, BLACK);
}

void drawPlayer(int x, int y){
	drawImage(x, y, cirno, 16, 16);
}

int collision(int bulletX, int bulletY, int playerX, int playerY){
	if(bulletY>=playerY && bulletY<playerY+16 && bulletX+16>playerX && bulletX+16<playerX+16){
	//if(bulletX+15>=playerX && bulletX < playerX+16 && bulletY > playerY && bulletY+15 < playerY+16){
	//I wrote these collision functions all by myself!
	//I realize they are lacking in a lot of things
	//for instance, hitting a vertical bullet on the side will do nothing
		return 1;
	}
	return 0;
}

int horzCollision(int bulletX, int bulletY, int playerX, int playerY){
	if(bulletY+15>playerY && bulletY<playerY+16 && bulletX < playerX+16 && bulletX+16>playerX){
	//hitting a horizontal bullet on the side will kill you, but on the top will  not
		return 1;
	}
	return 0;
}

int getPoint(int bulletX, int playerX){
	if(bulletX==playerX){
		return 1;
	}
	return 0;
}

void writeDirections(){
	drawString(60, 20, ">Don't get hit", WHITE);
	drawString(70, 20, ">One life", WHITE);
	drawString(80, 20, ">Points from avoiding falling ice", WHITE);
}
