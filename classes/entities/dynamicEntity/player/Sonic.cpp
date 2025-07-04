#include "Sonic.h"

Sonic::Sonic(float px, float py, int h, int w, sf::Texture* texture,
    float vx, float vy, float terminal,
    float ms, bool onGround, bool invincible, bool moving_right,
    float acc_x, float acc_y, float friction, float gravity,
    int boostTime, bool leader)
  : Player(px, py, h, w, texture, vx, vy, terminal, ms, onGround, invincible, 
      moving_right, acc_x, acc_y, friction, gravity, leader, boostTime)
{
  originalAcc = acc_x;
  originalMaxSpeed = ms;

  if (!idleRightTexture.loadFromFile("Data/0right_still.png")) {
      cout << "Failed to load 0right_still.png\n";
  }
  if (!walkRightTexture.loadFromFile("Data/0right.png")) {
      cout << "Failed to load 0right.png\n";
  }

  if (!idleLeftTexture.loadFromFile("Data/0left_still.png")) {
      cout << "Failed to load 0left_still.png" << endl; 
  }

  if (!walkLeftTexture.loadFromFile("Data/0left.png")) {
      cout << "Failed to load 0left.png" << endl;
  }
      
  if (!jumpRightTexture.loadFromFile("Data/0upR.png")) {
      cout << "Failed to load 0upR.png" << endl;
  }

  if (!jumpLeftTexture.loadFromFile("Data/0upL.png")) {
      cout << "Failed to load 0upL.png" << endl;
  }

  if (!boostRightTexture.loadFromFile("Data/boostR.png")) {
      cout << "Failed to load boostR.png" << endl;
  }

  if (!boostLeftTexture.loadFromFile("Data/boostL.png")) {
      cout << "Failed to load boostL.png" << endl;
  }

  if (!hangingRightTexture.loadFromFile("Data/hangR.png")) {
      cout << "Failed to load hangR.png" << endl;
  }

  if (!hangingLeftTexture.loadFromFile("Data/hangL.png")) {
      cout << "Failed to load hangL.png" << endl;
  }
  
  sprite.setTexture(idleRightTexture); // start with idle right texture
  sprite.setTextureRect(IntRect(0, 0, 40, 40));
  sprite.setScale(2.0f, 2.0f);
  
  // Setup animation for walking
  walkRightAnimation.initialize(&sprite, &walkRightTexture, 40, 40, 12, 0.07f);
  walkLeftAnimation.initialize(&sprite, &walkLeftTexture, 40, 40, 12, 0.07f);
  
  // Setup animation for jumping
  jumpRightAnimation.initialize(&sprite, &jumpLeftTexture, 40, 40, 8, 0.1f);
  jumpLeftAnimation.initialize(&sprite, &jumpLeftTexture, 40, 40, 8, 0.1f);

  // Setup animation for boosting
  boostRightAnimation.initialize(&sprite, &boostRightTexture, 40, 40, 8, 0.025f);
  boostLeftAnimation.initialize(&sprite, &boostLeftTexture, 40, 40, 8, 0.025f);

  // Setup animation for hanging
  hangingRightAnimation.initialize(&sprite, &hangingRightTexture, 40, 40, 8, 0.1f);
  hangingLeftAnimation.initialize(&sprite, &hangingLeftTexture, 40, 40, 8, 0.1f);
}

//Getters
float Sonic::getBoostedAcc() const { return boostedAcc; }
float Sonic::getBoostedMaxSpeed() const { return boostedMaxSpeed; }

void Sonic::specialAbility() {
    if (!boosting) {
        cout << "Sonic boost activated!\n";
        if(numPowerUps > 0){
            this->powerUp();
        }
        acc_x = boostedAcc;
        maxSpeed = boostedMaxSpeed;
        if(movingRight){
            moveRight();
        }else{
            moveLeft();
        }
        boostClock.restart();
        boosting = true;
    }
}

void Sonic::update(float deltaTime, int &score, int volume) {
    Player::update(deltaTime, score, volume);

    // Stop boosting if boost time is done
    if (boosting && boostClock.getElapsedTime() >= boostTime) {
        cout << "Sonic boost expired.\n";
        acc_x = originalAcc;
        maxSpeed = originalMaxSpeed;
        boosting = false;
        if(boostTime > sf::seconds(7)){ //If powerup just used up
            boostTime = sf::seconds(7);
        }
    }
}
void Sonic::powerUp() {
    cout<<"Sonic powered up\n";
    boostTime +=sf::seconds(4);
    numPowerUps--;
}

Sonic::~Sonic(){}
