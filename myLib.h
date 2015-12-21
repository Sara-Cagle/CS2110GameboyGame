//Sara Cagle myLib.h

#include "text.h"

typedef unsigned short u16;
extern u16 *videoBuffer;

#define REG_DISPCNT *(u16 *)0x4000000
#define SCANLINECOUNTER *(volatile u16 *)0x4000006
#define MODE3 3
#define BG2_ENABLE (1<<10)


#define OFFSET(r, c, numcols) ((r)*(numcols) + (c))

#define COLOR(r,g,b) ((b)<<10 | (g)<<5 | (r))
#define RED COLOR(31,0,0)
#define BLUE COLOR(0,0,31)
#define GREEN COLOR(0,31,0)
#define CYAN COLOR(0, 31, 31)
#define MAGENTA COLOR(31, 0, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK 0
#define WHITE COLOR(31,31,31)
#define NICEBLUE COLOR(10,25, 31)

// *** Input =========================================================

// Buttons

#define BUTTON_A	(1<<0)
#define BUTTON_B	(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT	(1<<5)
#define BUTTON_UP	(1<<6)
#define BUTTON_DOWN	(1<<7)
#define BUTTON_R	(1<<8)
#define BUTTON_L	(1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
#define BUTTONS *(volatile unsigned int *)0x4000130

typedef struct{
	int x;
	int y;
	int xDelta;
} BULLET;

typedef struct{
	int x;
	int y;
} PLAYER;

typedef struct{
	int x;
	int y;
	int xDelta;
	int yDelta;
} HORZBULLET;


// Prototypes
void setPixel(int x, int y, u16 color);
void drawRect(int xRow, int yCol, int width, int height, u16 color);
void drawHollowRect(int x, int y, int width, int height, u16 color);
void waitForVblank();
void drawImage(int x, int y, const unsigned short imageArray[], int width, int height);
void movePlayer(int oldX, int oldY); 
void drawPlayer();
int horzCollision(int bulletX, int bulletY, int playerX, int playerY);
int collision(int bulletX, int bulletY, int playerX, int playerY);
int getPoint(int bulletX, int playerX);
void writeDirections();


