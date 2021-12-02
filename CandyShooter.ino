static const int TURRET_SPEED = 60;
static const int ELEVATOR_SPEED = 20;
static const int ENCODER_MIN = 280;
static const int ENCODER_MAX = 930;

void setupCandyShooter(){
    pinMode(CANDY_SHOOTER_RELAY_PIN, OUTPUT);
    turretMotor.attach(TURRET_MOTOR_PIN,
                             MIN_PWM_SIGNAL_WIDTH, MAX_PWM_SIGNAL_WIDTH);
}

void processCandyInput(){
    if(robotMode != ROBOT_MODE_CANDY){
        return;
    }

    Serial.println("In candy mode");

    if(radioLinkControlY <= 500){
        Serial.println("Raising elevator");
        analogWrite(CANDY_LOADER_MOTOR_PIN, 128 + ELEVATOR_SPEED);
    }
    else if(radioLinkControlY >= 1500){
        Serial.println("Lowering elevator");\
        analogWrite(CANDY_LOADER_MOTOR_PIN, 128 - ELEVATOR_SPEED);
    }

    int turretAngle = analogRead(TURRET_ENCODER_PIN);
    Serial.println(turretAngle);
    
    // TODO: add limit switches
    if(radioLinkControlX <= 500 && turretAngle > ENCODER_MIN){
        Serial.println("turret turn left");
        turretMotor.write(90 + TURRET_SPEED);
    }
    else if(radioLinkControlX >= 1500 && turretAngle < ENCODER_MAX){
        Serial.println("turret turn right");
        turretMotor.write(90 - TURRET_SPEED);
    }
    else{
        Serial.println("turret not moving");
        turretMotor.write(90);  
    }

    if(radioLinkButton == BUTTON_PRESSED){
        Serial.println("shooting");
        digitalWrite(CANDY_SHOOTER_RELAY_PIN, 1);
    }
    else{
        digitalWrite(CANDY_SHOOTER_RELAY_PIN, 0);
    }
}
