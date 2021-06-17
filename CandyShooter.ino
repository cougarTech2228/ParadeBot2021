static const int TURRET_SPEED = 20;
static const int ELEVATOR_SPEED = 20;

void setupCandyShooter(){
    pinMode(CANDY_SHOOTER_RELAY_PIN, OUTPUT);
}

void processCandyInput(){
    if(robotMode != ROBOT_MODE_CANDY){
        Serial.println("Not in candy mode");
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

    // TODO: add limit switches
    if(radioLinkControlX <= 500){
        Serial.println("turret turn left");
        analogWrite(TURRET_MOTOR_PIN, 128 + TURRET_SPEED);
    }
    else if(radioLinkControlX >= 1500){
        Serial.println("turret turn right");
        analogWrite(TURRET_MOTOR_PIN, 128 - TURRET_SPEED);
    }

    if(radioLinkButton == BUTTON_PRESSED){
        Serial.println("shooting");
        digitalWrite(CANDY_SHOOTER_RELAY_PIN, 1);
    }
    else{
        digitalWrite(CANDY_SHOOTER_RELAY_PIN, 0);
    }
}