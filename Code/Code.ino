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
int wall0 = 0;
int wall1 = 320;
int wall2 = 240;

//Pot data
int potPinR = 10; //Pin to read pot
float divider = float((wall1 - paddleSize)) / 1023; //Amount to divide pot information by
int potInfo; //Value for pot

//Point-keeping data
int lose = 0; //A state to see if the enemy has scored a point
int win = 0; //A state to see if the player has scored a point
uint8_t pPoints = 0; //Number of points the player has
uint8_t oP = -1;
uint8_t ePoints = 0; //Number of points the enemy has
int oE = -1;
int delayTime = 2000; //Number of milliseconds to delay after a point has been scored
int playerPointPlaceX = wall1 / 4;
int pointPlaceY = wall2 / 4;
int enemyPointPlaceX = (wall1 / 4) * 3;
int pointSize = 2;

//Ball data
int ballSpeed = 1; //#of pixels the ball moves per second
int bounceAngle = 270; //Temporary. Angle ball bounces off of stuff
int ballRadius = 6; //Radius of ball in pixels
int oBallX = (wall1 / 2); //Base pixel X value for the ball
int oBallY = (wall2 / 2); //Base pixel Y value for the ball
int ballX = (wall1 / 2); //Pixel X value for the ball
int ballY = (wall2 / 2); // Pixel Y value for the ball
int ballXCenter = ballX + ballRadius;
int ballYCenter = ballY + ballRadius;
float ballInterX;
float ballInterY;
float ballXSpeed = 5; //Milliseconds it takes for the ball to move one x pixel
float ballYSpeed = 10; //Milliseconds it takes for the ball to move one y pixel
int oldBallX = ballX;
int oldBallY = ballY;

//Enemy paddle data
float interEnemyX;
float enemySpeed = 4; //Number of milliseconds it takes to move one pixel for enemy paddle.
float enemySpeedA = enemySpeed;
int enemyLocationX = (wall1 / 2) - int((paddleSize / 2)); //Pixel X value for the enemy paddle
int enemyLocationY =  wall0 + (paddleBuffer + paddleSizeO); //Pixel Y value for the enemy paddle. Constant.
int enemyTarget = ballX;
int oldEnemyX = 1;

//Player paddle data
int paddlePlaceX = int(round(analogRead(potPinR) * divider)); //Pixel X value of your paddle as read by potentiometer. Between 0 and (320-paddleSize)
int paddlePlaceY = wall2 - paddleBuffer; //Pixel Y valued of your paddle. Constant.
int oldPaddleX = paddlePlaceX;
int oldPaddleY = paddlePlaceY;
int directionthingy = 0;

int wall0B = wall0 + (ballRadius);
int wall1B = wall1 - (ballRadius);
int wall2B = wall2 - (ballRadius);

void cleanUpCleanUp(){ //Everybody do your share!
  tft.drawRect(ballX - 1, ballY - 1, (ballRadius*2)+2, (ballRadius*2)+2, ILI9341_BLACK);
}

void beepBoop(){ //I'm a calculator! Yaaay!
  ballInterX = ballInterX + ballXSpeed;
  ballInterY = ballInterY + ballYSpeed;

  oldBallX = ballX;
  oldBallY = ballY;
  
  ballX = int(ballInterX);
  ballY = int(ballInterY);
  ballXCenter = ballX + ballRadius;
  ballYCenter = ballY + ballRadius;
}

void paddleBounce(){
  if(ballX + ballRadius >= paddlePlaceX + int(round((paddleSize / 4) * 3))){
    ballXSpeed = ballXSpeed + int(round(ballXSpeed / 4));
  }
  if(ballX + ballRadius <= paddlePlaceX + int(round(paddleSize / 4))){
    ballXSpeed = ballXSpeed - int(round(ballXSpeed / 4));
  }
}

void boingyBoingyBoingy(){ //All hail Animaniacs
  if(ballXCenter >= wall1B || ballXCenter <= wall0B){
    ballXSpeed = -ballXSpeed;
  }
  if(ballYCenter >= wall2B || ballYCenter <= wall0B){
    ballYSpeed = -ballYSpeed;
  }
  if(((ballYCenter + ballRadius) >= paddlePlaceY && (ballYCenter - ballRadius) <= (paddlePlaceY + paddleSizeO) && (ballXCenter + ballRadius) >= paddlePlaceX && (ballXCenter - ballRadius) <= (paddlePlaceX + paddleSize)) || ((ballYCenter - ballRadius) <= paddlePlaceY && (ballYCenter + ballRadius) >= (paddlePlaceY + paddleSizeO) && (ballXCenter - ballRadius) <= paddlePlaceX && (ballXCenter + ballRadius) >= (paddlePlaceX + paddleSize))){
    if((ballYCenter + (ballRadius-1)) >= paddlePlaceY && (ballYCenter - (ballRadius-1)) <= (paddlePlaceY + paddleSizeO)){
      ballXSpeed = -ballXSpeed;
      delay(20);
    }
    else{
     paddleBounce();
     ballYSpeed = -ballYSpeed;
     delay(20);
    }
  }
  if(((ballYCenter + ballRadius) >= enemyLocationY && (ballYCenter - ballRadius) <= (enemyLocationY + paddleSizeO) && (ballXCenter + ballRadius) >= enemyLocationX && (ballXCenter - ballRadius) <= (enemyLocationX + paddleSize))/* || ((ballYCenter - ballRadius) <= enemyLocationY && (ballYCenter + ballRadius) >= (enemyLocationY + paddleSizeO) && (ballXCenter - ballRadius) <= enemyLocationX && (ballXCenter + ballRadius) >= (enemyLocationX + paddleSize))*/){
    if((ballYCenter + (ballRadius-1)) >= enemyLocationY && (ballYCenter - (ballRadius-1)) <= (enemyLocationY + paddleSizeO)){
      ballXSpeed = -ballXSpeed;
    }
    else{
     ballYSpeed = -ballYSpeed;
    }
  }
}

void doTheBall() {
  if(oldBallX != ballX || oldBallY != ballY){
   tft.fillRect(ballX, ballY, ballRadius*2, ballRadius*2, ILI9341_WHITE);
  }
  cleanUpCleanUp();
  boingyBoingyBoingy(); 
  beepBoop();
}

void playerPaddleCleanUp() {
  tft.drawRect(paddlePlaceX - 1, paddlePlaceY - 1, paddleSize + 2, paddleSizeO + 2, ILI9341_BLACK);
}

void goGoGadgetPlayerPaddle(){
  tft.fillRect(paddlePlaceX, paddlePlaceY, paddleSize, paddleSizeO, ILI9341_WHITE);
  playerPaddleCleanUp();
}

void enemyPaddleCleanUp(){
  tft.drawRect(enemyLocationX - 1, enemyLocationY -1, paddleSize + 2, paddleSizeO + 2, ILI9341_BLACK);
}

void enemyPaddleCalculator(){
  enemyTarget = ballX;
  if(enemyTarget < enemyLocationX){
   enemySpeedA = -enemySpeed;
  }
  if(enemyTarget > enemyLocationX){
    enemySpeedA = enemySpeed;
  }
  if(enemyTarget == enemyLocationX){
    enemySpeedA = 0;
  }
  interEnemyX = interEnemyX + enemySpeedA;
  oldEnemyX = enemyLocationX;
  enemyLocationX = int(interEnemyX);
}

void enemyPaddleDraw(){
  tft.fillRect(enemyLocationX, enemyLocationY, paddleSize, paddleSizeO, ILI9341_WHITE); 
  enemyPaddleCleanUp();
  enemyPaddleCalculator();
}

void playerCleanUp(){
  tft.drawRect(paddlePlaceX - 1, paddlePlaceY - 1, paddleSize + 2, paddleSizeO + 2, ILI9341_BLACK);
}

void playerCalculator(){
  potInfo = int(round(analogRead(potPinR) * divider));
  if(potInfo - paddlePlaceX != 0){
    if(potInfo - paddlePlaceX < 0){
      directionthingy = -1;
    }
    if(potInfo - paddlePlaceX > 0){
      directionthingy = 1;
    }
  }
  if(potInfo - paddlePlaceX == 0){
    directionthingy = 0;
  }
  paddlePlaceX = paddlePlaceX + directionthingy;
}

void playerPaddleDraw(){
  tft.fillRect(paddlePlaceX, paddlePlaceY, paddleSize, paddleSizeO, ILI9341_WHITE);
  playerCleanUp();
  playerCalculator();
}

void winF(){
  if(ballY + (ballRadius * 2) >= wall2){
    win = 1;
    oP = pPoints;
    pPoints = pPoints + 1;
  }
}

void loseF(){
  if(ballY - (ballRadius * 2) <= wall0){
    lose = 1;
    oE = ePoints;
    ePoints = ePoints + 1;
  }
}

void pointDraw(){
  if(oP != pPoints){
    tft.drawChar(playerPointPlaceX, pointPlaceY, pPoints + 48, ILI9341_GREEN, ILI9341_BLACK, pointSize);
  }
  if(oE != ePoints){
    tft.drawChar(enemyPointPlaceX, pointPlaceY, ePoints + 48, ILI9341_RED, ILI9341_BLACK, pointSize);
  }
}

void winLoseF(){
  winF();
  loseF();
  if(win == 1 || lose ==1){
    cleanUpCleanUp();
    ballX = oBallX;
    ballY = oBallY;
    ballInterX = float(oBallX);
    ballInterY = float(oBallY);
    win = 0;
    lose = 0;
    delay(delayTime);
  }
  pointDraw();
}

void setup() {
  interEnemyX = enemyLocationX;
  enemySpeed = (1000/enemySpeed) / 1000;
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

  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
}

void loop() {
  doTheBall();
  goGoGadgetPlayerPaddle();
  enemyPaddleDraw();
  playerPaddleDraw();
  winLoseF();
  delay(1);
}
