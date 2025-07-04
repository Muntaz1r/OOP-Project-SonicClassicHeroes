#include "Player.h"

//Initialize static member
int Player::hp = 3;
int Player::numPowerUps = 0;

//Constructor
Player::Player(float px, float py, int h, int w, sf::Texture* texture,
    float vx, float vy, float terminal,
    float ms, bool onGround, bool invicible, bool movingRight,
    float acc_x, float acc_y, float friction, float gravity, bool leader, int boostTime)
    : DynamicEntity(px, py, h, w, texture, vx, vy, terminal), 
    maxSpeed(ms), onGround(onGround), invincible(invicible), movingRight(movingRight)
    , acc_x(acc_x), acc_y(acc_y), friction(friction), gravity(gravity), leader(leader), boostTime(sf::seconds(boostTime)) {
        boosting = false;
        invalid = '\0';
        fallingIntoVoid = false;
        hanging = false;
        knucklesInvinc = false;
        for (int i=0; i<2; ++i) followers[i]=nullptr;

        playerSounds = new SoundManager();
    
        // Index 0: Ring
        // Index 1: Spike
        // Index 2: Spike
        playerSounds->loadSound(0, "Data/sfx/ring.wav");
        playerSounds->loadSound(1, "Data/sfx/spike.wav");
        playerSounds->loadSound(2, "Data/sfx/void.ogg");
}

// Getters
float Player::getMaxSpeed() const { return maxSpeed; }
bool Player::isOnGround() const { return onGround; }
bool Player::isInvincible() const { return invincible; }
bool Player::isMovingRight() const { return movingRight; }
float Player::getAccnX() const { return acc_x; }
float Player::getAccY() const { return acc_y; }
bool Player::getLeader() const {return leader;}
Player* Player::getFollower1() const{return followers[0];}
Player* Player::getFollower2() const{return followers[1];}
bool Player::getBoosting() const{return boosting;}
bool Player::getFallingIntoVoid() const{return fallingIntoVoid;}
bool Player::getHanging() const { return hanging; }
bool Player::getHangingRight() const { return hangingRight; }
sf::Clock Player::getBoostClock() const {return boostClock;}
sf::Time Player::getBoostTime() const {return boostTime;}

//Static Getters
int Player::getNumPowerUps() { return numPowerUps; }
int Player::getHP() { return hp; }


// Setters
void Player::setMaxSpeed(float speed) { maxSpeed = speed; }
void Player::setOnGround(bool value) { onGround = value; }
void Player::setInvincible(bool value) { invincible = value; }
void Player::setMovingRight(bool value) { movingRight = value; }
void Player::setAccX(float value) { acc_x = value; }
void Player::setAccY(float value) { acc_y = value; }
void Player::setLeader(bool value) {leader = value;}
void Player::setFollowers(Player* follower1, Player* follower2){
    followers[0]=follower1;
    followers[1]=follower2;
}
void Player::setGravity(float value){ gravity = value;}
void Player::setFriction(float value){ friction= value;}
void Player::setBoosting(bool value){ boosting = value;}
void Player::setFallingIntoVoid(bool value){fallingIntoVoid = value;}
void Player::setHanging(bool value) { hanging = value; }
void Player::setHangingRight(bool value) { hangingRight = value; }

//Static setters
void Player::setHP(int value) { hp = value; }
void Player::setNumPowerUps(int value){numPowerUps = value;}


//Setting colliding tiles
char* Player::sampleTile(float x, float y, int tileSize, int levelHeight, int levelWidth, char** grid, int* outRow, int* outCol) {
    //Helper function
    int col = static_cast<int>(x / tileSize);
    int row = static_cast<int>(y / tileSize);

    // Handle out-of-bound coordinates
    if (col < 0) {
        col = 0; // Set col to 0 if it is out of bounds (left of the grid)
        if (outCol) *outCol = 0; // Store the closest valid col (0)
    }
    if (col >= levelWidth) {
        col = levelWidth - 1; // Set col to the max valid column
        if (outCol) *outCol = levelWidth - 1; // Store the closest valid col (max column)
    }
    if (row < 0) {
        row = 0; // Set row to 0 if it is out of bounds (top of the grid)
        if (outRow) *outRow = 0; // Store the closest valid row (0)
    }
    if (row >= levelHeight) {
        row = levelHeight - 1; // Set row to the max valid row
        if (outRow) *outRow = levelHeight - 1; // Store the closest valid row (max row)
    }

    if (col < 0 || col >= levelWidth || row < 0 || row >= levelHeight) {
        return &invalid; // assuming 'invalid' is defined somewhere
    }

    if (outRow) *outRow = row;
    if (outCol) *outCol = col;

    return &grid[row][col];
}

void Player::setCollidingTiles(int tileSize, int levelHeight, int levelWidth, char** grid) {
    
    float left   = pos_x;
    float right  = pos_x + width;
    float top    = pos_y;
    float bottom = pos_y + height;

    // Buffer helps avoid jitter or premature collision detection
    float bufferY = tileSize / 10.0f;
    float bufferX = tileSize / 10.0f;

    int row, col;

    // BELOW
    collidingTiles.belowLeft  = sampleTile(left + bufferX, bottom + bufferY, tileSize, levelHeight, levelWidth, grid, &row, &col);
    collidingTiles.yBelow = row * tileSize;
    collidingTiles.belowRight = sampleTile(right - bufferX, bottom + bufferY, tileSize, levelHeight, levelWidth, grid);

    // ABOVE
    collidingTiles.aboveLeft  = sampleTile(left + bufferX, top - bufferY, tileSize, levelHeight, levelWidth, grid, &row, &col);
    collidingTiles.yAbove = row * tileSize;
    collidingTiles.aboveRight = sampleTile(right - bufferX, top - bufferY, tileSize, levelHeight, levelWidth, grid);

    // LEFT
    collidingTiles.leftTop    = sampleTile(left - bufferX, top + bufferY, tileSize, levelHeight, levelWidth, grid, &row, &col);
    collidingTiles.xLeft = col * tileSize;
    collidingTiles.leftBottom = sampleTile(left - bufferX, bottom - bufferY, tileSize, levelHeight, levelWidth, grid);

    // RIGHT
    collidingTiles.rightTop   = sampleTile(right + bufferX, top + bufferY, tileSize, levelHeight, levelWidth, grid, &row, &col);
    collidingTiles.xRight = col * tileSize;
    collidingTiles.rightBottom = sampleTile(right + bufferX, bottom - bufferY, tileSize, levelHeight, levelWidth, grid);
    // If this object has followers, update their collision tiles too
    if (leader) {
        for (int i = 0; i < 2; ++i)
            followers[i]->setCollidingTiles(tileSize, levelHeight, levelWidth, grid);
    }
}

// Movement
void Player::jump() {
    if(onGround){
        velocity_y -= acc_y;
    }
    onGround = false;
    if(leader){
        for(int i=0; i<2; ++i) followers[i]->jump();
    }
}

void Player::moveLeft() {
    if(onGround){
        velocity_x -= acc_x;
        if (velocity_x < -maxSpeed) velocity_x = -maxSpeed;
        movingRight = false;
    }
    if(leader){
        for(int i=0; i<2; ++i){
            if(!(pos_x - followers[i]->getPosX() > 100)){
                followers[i]->moveLeft();
            }
        }
    }
}

void Player::moveRight() {
    if(onGround){
        velocity_x += acc_x;
        if (velocity_x > maxSpeed) velocity_x = maxSpeed;
        movingRight = true;
    }
    if(leader){
        for(int i=0; i<2; ++i){
            if(!(pos_x - followers[i]->getPosX() < -100)){
                followers[i]->moveRight();
            }
        }
    }
}

void Player::flyDown(){} // Tails specific ftn

//Damage mechanics
void Player::takeDamage() {
    if (!invincible && hp > 0 && !knucklesInvinc && leader) {
        --hp;
        invincible = true;
        invincibilityClock.restart();
    }
}


//Updating everything relevant 
void Player::update(float deltaTime, int &score, int volume) {
    playerSounds->setVolume(volume);
    float previousX = pos_x;
    float previousY = pos_y;
    // Updating position and velocities
    DynamicEntity::update(deltaTime, score, volume);
    if(onGround){
        if(velocity_x>0){ // friction
            velocity_x -= friction;
            if (velocity_x<0){
                velocity_x = 0;
            }
        }else if(velocity_x<0){
            velocity_x += friction;
            if (velocity_x>0){
                velocity_x = 0;
            }
        }
        
    }
    if(abs(velocity_y) > 0){// going  up or down
        velocity_y += gravity; 
    }
    // Updating invinciillity
    if (invincible && invincibilityClock.getElapsedTime() >= invincibilityDuration && !knucklesInvinc) {
        invincible = false;
    }
    // if (pos_y >=  635) {
    //     pos_y = 635; // Clamp to ground
    //     velocity_y = 0;
    //     onGround = true;
    // }
    
    
    // Animation logic
    if (hanging) {
        if (getHangingRight()) {
            if (sprite.getTexture() != &hangingRightTexture) {
                sprite.setTexture(hangingRightTexture);
            }
            hangingRightAnimation.update(deltaTime);
        }
        else {
            if (sprite.getTexture() != &hangingLeftTexture) {
                sprite.setTexture(hangingLeftTexture);
            }
            hangingLeftAnimation.update(deltaTime);
        }
    }
    else if (!onGround) { // Jumping
        if (isMovingRight()) {
            if (sprite.getTexture() != &jumpRightTexture) {
                sprite.setTexture(jumpRightTexture);
            }
            jumpRightAnimation.update(deltaTime);
        }
        else {
            if (sprite.getTexture() != &jumpLeftTexture) {
                sprite.setTexture(jumpLeftTexture);
            }
            jumpLeftAnimation.update(deltaTime);
        }
    }
    else if (boosting && (velocity_x != 0 || velocity_y != 0)) {
        if (isMovingRight()) {
            // Switch to running texture if not already
            if (sprite.getTexture() != &boostRightTexture) {
                sprite.setTexture(boostRightTexture);
            }
            boostRightAnimation.update(deltaTime);
        }
        else {
            if (sprite.getTexture() != &boostLeftTexture) {
                sprite.setTexture(boostLeftTexture);
            }
            boostLeftAnimation.update(deltaTime);
        }
    }
    else if (velocity_x != 0) { // check if not stationary
        if (isMovingRight()) {
            // Switch to running texture if not already
            if (sprite.getTexture() != &walkRightTexture) {
                sprite.setTexture(walkRightTexture);
            }
            walkRightAnimation.update(deltaTime);
        }
        else {
            if (sprite.getTexture() != &walkLeftTexture) {
                sprite.setTexture(walkLeftTexture);
            }
            walkLeftAnimation.update(deltaTime);
        }
    }
    else { // for idle situations
        if (isMovingRight()) {
            // Switch to idle texture if not already once
            if (sprite.getTexture() != &idleRightTexture) {
                sprite.setTexture(idleRightTexture);
                sprite.setTextureRect(sf::IntRect(0, 0, 40, 40));
            }
        }
        else {
            if (sprite.getTexture() != &idleLeftTexture) {
                sprite.setTexture(idleLeftTexture);
                sprite.setTextureRect(sf::IntRect(0, 0, 40, 40));
            }
        }
        
        // Reset to avoid overlap
        walkRightAnimation.reset();
        walkLeftAnimation.reset();
        jumpRightAnimation.reset();
        jumpLeftAnimation.reset();
    }
    
    collisionHandle(previousX, previousY, score);
    
    if(leader){
        if(fallingIntoVoid){
            fallingIntoVoid = false;
            velocity_x = 0;
            pos_x = 20;
            this->takeDamage();
            pos_y = 50;
            velocity_y = terminal_velocity;
        }
        for(int i=0; i<2; ++i) {
            followers[i]->update(deltaTime, score, volume);
            if(abs(pos_x - followers[i]->getPosX()) > 960){
                followers[i]->setPosY(0);
                followers[i]->setVelocityY(followers[i]->getTerminalVelocity());
                followers[i]->setPosX(pos_x + 20*i);
            }else if(pos_x - followers[i]->getPosX() > 100){
                followers[i]->setVelocityX(followers[i]->getMaxSpeed());
            }else if(pos_x - followers[i]->getPosX() < -100){
                followers[i]->setVelocityX(-followers[i]->getMaxSpeed());
            }
        }
    }else if(fallingIntoVoid){
       fallingIntoVoid = false;
       pos_x = 1000000; // initiates teleport to leader
    }
}


//Tile Checkers
bool Player::collidesBelow(char target) {
    return *collidingTiles.belowLeft  == target ||
           *collidingTiles.belowRight == target;
}

bool Player::collidesAbove(char target) {
    return *collidingTiles.aboveLeft  == target ||
           *collidingTiles.aboveRight == target;
}

bool Player::collidesLeft(char target) {
    return *collidingTiles.leftTop    == target ||
           *collidingTiles.leftBottom == target;
}

bool Player::collidesRight(char target) {
    return *collidingTiles.rightTop    == target ||
           *collidingTiles.rightBottom == target;
}

//Tile setters
void Player::setBelow(char target) {
    *collidingTiles.belowLeft  = target;
    *collidingTiles.belowRight = target;
}

void Player::setAbove(char target) {
    *collidingTiles.aboveLeft  = target;
    *collidingTiles.aboveRight = target;
}

void Player::setLeft(char target) {
    *collidingTiles.leftTop    = target;
    *collidingTiles.leftBottom = target;
}

void Player::setRight(char target) {
    *collidingTiles.rightTop    = target;
    *collidingTiles.rightBottom = target;
}

void Player::collisionHandle(float previousX, float previousY, int &score){

    if(collidesBelow('p') && !(collidesBelow('\0') || collidesBelow('w') 
    || collidesBelow('q') || collidesBelow('b') || collidesBelow('x'))){
        if(leader){
            playerSounds->play(2);
            cout<<"Player fell into the void\n";
            velocity_y = terminal_velocity;
            fallingIntoVoid = true;
            for (int i =0; i<2; ++i){
                followers[i]->setVelocityY(terminal_velocity);
                followers[i]->setFallingIntoVoid(true);
            }
        }
    }
    //Below ther is : out-of-bounds, wall or platform
    if (!fallingIntoVoid && (pos_y >= previousY && (collidesBelow('\0') || collidesBelow('w') 
    || collidesBelow('q') || collidesBelow('b') || collidesBelow('x')))) {
        // if (pos_y - previousY > 64/5.0f) // when hit ground really hard
        //     pos_y = previousY - velocity_y;
        // else   
        //     pos_y = previousY;
        // onGround = true;
        // velocity_y = 0;
        pos_y = collidingTiles.yBelow - height - 2;
        velocity_y = 0;
        onGround = true;
    }

    if ((pos_x> previousX && (collidesRight('\0') || collidesRight('w') 
    || collidesRight('x') || collidesRight('b')) && movingRight)) {
        pos_x = collidingTiles.xRight - width;  // Prevent movement if collided
        velocity_x = velocity_x/1.5f;
    }

    // Left collision check (only when moving left)
    if (pos_x < previousX && (collidesLeft('\0') || collidesLeft('w') || 
    collidesLeft('x') || collidesLeft('b')) && !movingRight) {
        pos_x = collidingTiles.xLeft + 64;  // Prevent movement if collided
        velocity_x = velocity_x/1.5f;
    }
    if (pos_x < previousX && pos_x < 0){
        pos_x = previousX;
        velocity_x = velocity_x/1.5f;
    }
    
    if (pos_y <= previousY && (collidesAbove('\0') || collidesAbove('w') || 
    collidesAbove('x') || collidesAbove('b') || pos_y < 0 )){
        pos_y = collidingTiles.yAbove + 64;//tile size
        velocity_y = -velocity_y*0.2;
    }
    
    //Handling falling off
    if (!(collidesBelow('\0')||collidesBelow('q') || collidesBelow('w')
    || collidesBelow('b') || collidesBelow('x'))) {
        if (onGround) {
            velocity_y += gravity;
            onGround = false;
        }
    }
    
    // For some reason leader has above definded while followers don't idk y
    if(((*collidingTiles.belowLeft == 'p' && movingRight) 
    || (*collidingTiles.belowRight == 'p' && !movingRight)) && !fallingIntoVoid){ 
        fallingIntoVoid = true;
    }

    
    //Ring collection
    if (collidesBelow('R')) {
        playerSounds->play(0);
        // Check both tiles (belowLeft and belowRight) and set to 's' only if they are 'R'
        if (*collidingTiles.belowLeft == 'R') {
            *collidingTiles.belowLeft = 's';  // Set only belowLeft to 's' if it's 'R'
            score += 100;
        }
        if (*collidingTiles.belowRight == 'R') {
            score += 100;
            *collidingTiles.belowRight = 's';  // Set only belowRight to 's' if it's 'R'
        }
    }
    
    if (collidesAbove('R')) {
        playerSounds->play(0);
        // Check both tiles (aboveLeft and aboveRight) and set to 's' only if they are 'R'
        if (*collidingTiles.aboveLeft == 'R') {
            score += 100;
            *collidingTiles.aboveLeft = 's';  // Set only aboveLeft to 's' if it's 'R'
        }
        if (*collidingTiles.aboveRight == 'R') {
            score += 100;
            *collidingTiles.aboveRight = 's';  // Set only aboveRight to 's' if it's 'R'
        }
    }

    if (collidesRight('R')) {
        playerSounds->play(0);
        // Collect right coins if they are 'R'
        if (*collidingTiles.rightTop == 'R') {
            score += 100;
            *collidingTiles.rightTop = 's';  // Set to 's' if it's a coin
        }
        if (*collidingTiles.rightBottom == 'R') {
            score += 100;
            *collidingTiles.rightBottom = 's';  // Set to 's' if it's a coin
        }
    }
    
    if (collidesLeft('R')) {
        playerSounds->play(0);
        // Collect left coins if they are 'R'
        if (*collidingTiles.leftTop == 'R') {
            score += 100;
            *collidingTiles.leftTop = 's';  // Set to 's' if it's a coin
        }
        if (*collidingTiles.leftBottom == 'R') {
            score += 100;
            *collidingTiles.leftBottom = 's';  // Set to 's' if it's a coin
        }
    }

    //Extra Life collection
    if (collidesAbove('l') || collidesBelow('l') 
    || collidesRight('l') || collidesLeft('l') && leader){
        ++hp;
        //Left
        if (*collidingTiles.leftTop == 'l') {
            *collidingTiles.leftTop = 's';  // Set to 's' if it's collected
        }
        if (*collidingTiles.leftBottom == 'l') {
            *collidingTiles.leftBottom = 's';  // Set to 's' if it's collected
        }
        //Right
        if (*collidingTiles.rightTop == 'l') {
            *collidingTiles.rightTop = 's';  // Set to 's' if it's collected
        }
        if (*collidingTiles.rightBottom == 'l') {
            *collidingTiles.rightBottom = 's';  // Set to 's' if it's collected
        }
        //Above
        if (*collidingTiles.aboveLeft == 'l') {
            *collidingTiles.aboveLeft = 's';  // Set only aboveLeft to 's' if it's 'collected
        }
        if (*collidingTiles.aboveRight == 'l') {
            *collidingTiles.aboveRight = 's';  // Set only aboveRight to 's' if it's collected
        }
        //Below
        if (*collidingTiles.belowLeft == 'l') {
            *collidingTiles.belowLeft = 's';  // Set only aboveLeft to 's' if it's 'collected
        }
        if (*collidingTiles.belowRight == 'l') {
            *collidingTiles.belowRight = 's';  // Set only aboveRight to 's' if it's collected
        }
    }
    
    //Power up collection
    if (collidesAbove('u') || collidesBelow('u') 
    || collidesRight('u') || collidesLeft('u') && leader){
        cout<<"Power up\n";
        ++numPowerUps;
        //Left
        if (*collidingTiles.leftTop == 'u') {
            *collidingTiles.leftTop = 's';  // Set to 's' if it's collected
        }
        if (*collidingTiles.leftBottom == 'u') {
            *collidingTiles.leftBottom = 's';  // Set to 's' if it's collected
        }
        //Right
        if (*collidingTiles.rightTop == 'u') {
            *collidingTiles.rightTop = 's';  // Set to 's' if it's collected
        }
        if (*collidingTiles.rightBottom == 'u') {
            *collidingTiles.rightBottom = 's';  // Set to 's' if it's collected
        }
        //Above
        if (*collidingTiles.aboveLeft == 'u') {
            *collidingTiles.aboveLeft = 's';  // Set only aboveLeft to 's' if it's 'collected
        }
        if (*collidingTiles.aboveRight == 'u') {
            *collidingTiles.aboveRight = 's';  // Set only aboveRight to 's' if it's collected
        }
        //Below
        if (*collidingTiles.belowLeft == 'u') {
            *collidingTiles.belowLeft = 's';  // Set only aboveLeft to 's' if it's 'collected
        }
        if (*collidingTiles.belowRight == 'u') {
            *collidingTiles.belowRight = 's';  // Set only aboveRight to 's' if it's collected
        }
    }
    
    
    //Crystal Collection
    if (collidesBelow('C') || collidesAbove('C') || 
        collidesLeft('C') || collidesRight('C')) {
        cout << "Collect crystal\n";
    }

    if (collidesBelow('C')) {
        playerSounds->play(0);
        // Check both tiles (belowLeft and belowRight) and set to 's' only if they are 'C'
        if (*collidingTiles.belowLeft == 'C') {
            *collidingTiles.belowLeft = 's';  // Set only belowLeft to 's' if it's 'C'
            score += 200;
        }
        if (*collidingTiles.belowRight == 'C') {
            score += 200;
            *collidingTiles.belowRight = 's';  // Set only belowRight to 's' if it's 'C'
        }
    }
    
    if (collidesAbove('C')) {
        playerSounds->play(0);
        // Check both tiles (aboveLeft and aboveRight) and set to 's' only if they are 'C'
        if (*collidingTiles.aboveLeft == 'C') {
            score += 200;
            *collidingTiles.aboveLeft = 's';  // Set only aboveLeft to 's' if it's 'C'
        }
        if (*collidingTiles.aboveRight == 'C') {
            score += 200;
            *collidingTiles.aboveRight = 's';  // Set only aboveRight to 's' if it's 'C'
        }
    }

    if (collidesRight('C')) {
        playerSounds->play(0);
        // Collect right crystals if they are 'C'
        if (*collidingTiles.rightTop == 'C') {
            score += 200;
            *collidingTiles.rightTop = 's';  // Set to 's' if it's a crystal
        }
        if (*collidingTiles.rightBottom == 'C') {
            score += 200;
            *collidingTiles.rightBottom = 's';  // Set to 's' if it's a crystal
        }
    }
    
    if (collidesLeft('C')) {
        playerSounds->play(0);
        // Collect left coins if they are 'C'
        if (*collidingTiles.leftTop == 'C') {
            score += 200;
            *collidingTiles.leftTop = 's';  // Set to 's' if it's a crystal
        }
        if (*collidingTiles.leftBottom == 'C') {
            score += 200;
            *collidingTiles.leftBottom = 's';  // Set to 's' if it's a crystal
        }
    }
    
    if (pos_y >= previousY && collidesBelow('x')) {
        playerSounds->play(1);
        takeDamage();
    }
}

void Player::render(sf::RenderWindow& window, float cameraOffsetX) {
    float time = invincibilityClock.getElapsedTime().asSeconds();
    static const float flickerRate = 0.000001f;

    int cycle = static_cast<int>(time / flickerRate);
    bool shouldRender = true;

    if (invincible && !boosting) {
        shouldRender = (cycle % 2 == 0);
    }

    if (shouldRender) {
        if (leader) {
            for (int i = 0; i < 2; ++i) {
                followers[i]->render(window, cameraOffsetX);
            }
        }
        Entity::render(window, cameraOffsetX);
    }
}

Player::~Player(){
    delete playerSounds;
};