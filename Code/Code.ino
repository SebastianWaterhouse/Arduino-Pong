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


//Player paddle data
int paddlePlace; //Pixel X value of your paddle as read by potentiometer. Between 0 and (320-paddleSize)

//Ball data
int ballSpeed = 1; //#of pixels the ball moves per second
int bounceAngle = 270; //Temporary. Angle ball bounces off of stuff
int ballRadius = 6; //Radius of ball in pixels
int ballX = 120; //Pixel X value for the ball
int ballY = 160; // Pixel Y value for the ball
float ballInterX;
float ballInterY;
float ballXSpeed = 50; //Milliseconds it takes for the ball to move one x pixel
float ballYSpeed = 70; //Milliseconds it takes for the ball to move one y pixel

//Enemy paddle data
int enemySpeed = 5; //Speed in pixels enemy paddle moves per second
int enemyLocationX = 108; //Pixel X value for the enemy paddle
int enemyLocationY = 32; //Pixel Y value for the enemy paddle

int wall0 = 0 + (ballRadius*2);
int wall1 = 320 - (ballRadius*2);
int wall2 = 240 - (ballRadius*2);

void cleanUpCleanUp(){ //Everybody do your share!
  tft.drawRect(ballX, ballY, ((ballRadius*2)+1)/**(-(int(ballXSpeed)/abs(int(ballXSpeed))))*/, ((ballRadius*2)+1)/**(-(int(ballYSpeed)/abs(int(ballYSpeed))))*/, ILI9341_BLACK);
}

void beepBoop(){ //I'm a calculator! Yaaay!
  ballInterX = ballInterX + ballXSpeed;
  ballInterY = ballInterY + ballYSpeed;

  ballX = int(ballInterX);
  ballY = int(ballInterY);
}

void boingyBoingy(){ //All hail Animaniacs
  if(ballX >= wall2 || ballX <= wall0){
    ballXSpeed = -ballXSpeed;
  }
  if(ballY >= wall1 || ballY <= wall0){
    ballYSpeed = -ballYSpeed;
  }
}

void doTheBall() {
  boingyBoingy();
  beepBoop();
  tft.fillRect(ballX, ballY, ballRadius*2, ballRadius*2, ILI9341_WHITE);
  cleanUpCleanUp();
}

/*void playerPaddleCleanUp() {
  tft.
}

void goGoGadgetPlayerPaddle(){
  
}*/

void setup() {
  ballXSpeed = (1000/ballXSpeed)/1000;
  ballYSpeed = (1000/ballYSpeed)/1000;
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
  doTheBall();
  delay(1);
}
