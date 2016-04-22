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
int bouncePercent = round(tan(bounceAngle));
int ballRadius = 5; //Radius of ball in pixels
int ballX = 120; //Pixel X value for the ball
int ballY = 160; // Pixel Y value for the ball
float ballDirection = 1; //Direction ball travels, 0 being just positive Y and 90 being just positive X
int ballPercent = round(tan(ballDirection));

//Enemy paddle data
int enemySpeed = 5; //Speed in pixels enemy paddle moves per second
int enemyLocationX = 108; //Pixel X value for the enemy paddle
int enemyLocationY = 32; //Pixel Y value for the enemy paddle

int wall1 = 320 - (ballRadius);
int wall2 = 0 + (ballRadius);
int wall3 = 240 - (ballRadius);
int wall4 = 0 + (ballRadius);

void doTheBall() {
  tft.fillRect(ballX-ballRadius, ballY-ballRadius, ballRadius*2, ballRadius*2, ILI9341_BLACK);
  tft.fillCircle(ballX, ballY, ballRadius, ILI9341_WHITE);
}

void setup() {
  Serial.begin(250000);
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
  delay(15);
  if(ba
  ballY = ballY + ballDirection;
  ballX = ballX + ballDirection;
  if(ballY <= wall2 || ballY >= wall1 || ballX <= wall4 || ballX >= wall3){
    ballDirection = -ballDirection;
  }
}
