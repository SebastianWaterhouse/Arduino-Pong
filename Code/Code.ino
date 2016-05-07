//Calling in libraries
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

//Default magic stuff? Not really sure what this does.
#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

//Universal object data (not really universal just stuff that fit in more than one category)
int paddleSize = 12; //# of pixels acrosss the paddles are
int paddleSizeO = 5; //# of pixels thick the paddles are
int paddleBuffer = 32; //# of pixels wide the buffer between edge of screen and paddle

int thing = 0;

int wall0 = 0;
int wall1 = 240;
int wall2 = 320;

//Ball data
int ballSpeed = 1; //#of pixels the ball moves per second
int bounceAngle = 270; //Temporary. Angle ball bounces off of stuff
int ballRadius = 6; //Radius of ball in pixels
int ballX = 120; //Pixel X value for the ball
int ballY = 160; // Pixel Y value for the ball
int ballXCenter = ballX + ballRadius;
int ballYCenter = ballY + ballRadius;
float ballInterX;
float ballInterY;
float ballXSpeed = 10; //Milliseconds it takes for the ball to move one x pixel
float ballYSpeed = 5; //Milliseconds it takes for the ball to move one y pixel

//Enemy paddle data
int enemySpeed = 5; //Speed in pixels enemy paddle moves per second
int enemyLocationX = 108; //Pixel X value for the enemy paddle
int enemyLocationY =  wall0 + (paddleBuffer - paddleSizeO); //Pixel Y value for the enemy paddle. Constant.

//Player paddle data
int paddlePlaceX; // **NOT IMPLEMENTED** Pixel X value of your paddle as read by potentiometer. Between 0 and (320-paddleSize)
int paddlePlaceY = wall2 - paddleBuffer; //Pixel Y valued of your paddle. Constant.

int wall0B = wall0 + (ballRadius);
int wall1B = wall1 - (ballRadius);
int wall2B = wall2 - (ballRadius);

void cleanUpCleanUp(){ //Everybody do your share!
  tft.drawRect(ballX - 1, ballY - 1, (ballRadius*2)+2, (ballRadius*2)+2, ILI9341_BLACK);
  tft.drawRect(ballX - 2, ballY - 2, (ballRadius*2)+3, (ballRadius*2)+3, ILI9341_BLACK);
}

void beepBoop(){ //I'm a calculator! Yaaay!
  ballInterX = ballInterX + ballXSpeed;
  ballInterY = ballInterY + ballYSpeed;

  ballX = int(ballInterX);
  ballY = int(ballInterY);
  ballXCenter = ballX + ballRadius;
  ballYCenter = ballY + ballRadius;
}

void boingyBoingyBoingy(){ //All hail Animaniacs
  if(ballXCenter >= wall1B || ballXCenter <= wall0B){
    ballXSpeed = -ballXSpeed;
  }
  if(ballYCenter >= wall2B || ballYCenter <= wall0B){
    ballYSpeed = -ballYSpeed;
  }
}

void doTheBall() {
  boingyBoingyBoingy();
  beepBoop();
  cleanUpCleanUp();
  tft.fillRect(ballX, ballY, ballRadius*2, ballRadius*2, ILI9341_WHITE);
}

void playerPaddleCleanUp() {
  tft.drawRect(enemyLocationX - 1, paddlePlaceY - 1, paddleSize + 2, paddleSizeO + 2, ILI9341_BLACK);
}

void goGoGadgetPlayerPaddle(){
  tft.fillRect(enemyLocationX, paddlePlaceY, paddleSize, paddleSizeO, ILI9341_WHITE);
  playerPaddleCleanUp();
}

void setup() {
  ballXSpeed = (1000/ballXSpeed) / 1000;
  ballYSpeed = (1000/ballYSpeed) / 1000;
  ballInterX = ballX;
  ballInterY = ballY;
  Serial.begin(9600);
  Serial.println("Pong started!"); 
 
  tft.begin();

  //Diagnostics
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 

  Serial.println(F("Done!"));
  
  tft.fillScreen(ILI9341_BLACK);
}

void loop() {
  goGoGadgetPlayerPaddle();
  doTheBall();
  delay(1);
  thing = thing+1;
  if(thing == 100){
    thing = 0;
  }
}
