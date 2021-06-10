/**************************************************************
   setupDriveMotors()
 **************************************************************/
void setupDriveMotors(){
  leftDriveMotor.attach(LEFT_DRIVE_MOTOR_PIN,
                             MIN_PWM_SIGNAL_WIDTH, MAX_PWM_SIGNAL_WIDTH);
  rightDriveMotor.attach(RIGHT_DRIVE_MOTOR_PIN,
                              MIN_PWM_SIGNAL_WIDTH, MAX_PWM_SIGNAL_WIDTH);

  // Set the drive motors to their "stopped" position
  stopDriveMotors();

}

/**************************************************************
   handleDriveMotors()
 **************************************************************/
void handleDriveMotors() {
//  int throttle = 0;
//  int strafe = 0;
//  int turn = 0;
//  if (!isAuto) {
//    throttle = -map(radioLinkThrottleValue, RADIOLINK_CONTROLLER_MINIMUM_VALUE, RADIOLINK_CONTROLLER_MAXIMUM_VALUE, -1000, 1000);
//    strafe = map(radioLinkStrafeValue, RADIOLINK_CONTROLLER_MINIMUM_VALUE, RADIOLINK_CONTROLLER_MAXIMUM_VALUE, -1000, 1000);
//    turn = map(radioLinkTurnValue, RADIOLINK_CONTROLLER_MINIMUM_VALUE, RADIOLINK_CONTROLLER_MAXIMUM_VALUE, -1000, 1000);
//  }
//  else {
//    //autoDriving returns throttle, strafe, and turn values; 
//    calculateDriveValues();
//    throttle = getCalculatedThrottleValue();
//    strafe = getCalculatedStrafeValue();
//    turn = getCalculatedTurnValue();
//  }
//  move(throttle, strafe, turn);
//

  //Get X and Y from the Joystick, do whatever scaling and calibrating you need to do based on your hardware.
  int x = -map(radioLinkDriveX, RADIOLINK_CONTROLLER_MINIMUM_VALUE, RADIOLINK_CONTROLLER_MAXIMUM_VALUE, -100, 100);
  int y = map(radioLinkDriveY, RADIOLINK_CONTROLLER_MINIMUM_VALUE, RADIOLINK_CONTROLLER_MAXIMUM_VALUE, -100, 100);

  x = applyDeadband(x);
  y = applyDeadband(y);
  
  //Calculate R+L (Call it V): V =(100-ABS(X)) * (Y/100) + Y
  int v = (100-abs(x)) * (y/100) + y;
  
  //Calculate R-L (Call it W): W= (100-ABS(Y)) * (X/100) + X
  int w = (100-abs(y)) * (x/100) + x;
  
  //Calculate R: R = (V+W) /2
  int right = (v+w) /2;
  
  // Calculate L: L= (V-W)/2
  int left = (v-w)/2;
  
  // Do any scaling on R and L your hardware may require.
  left = map(left, -100, 100, 0, 180);
  right = map(right, -100, 100, 0, 180);

  // Send those values to your Robot.
  leftDriveMotor.write(left);
  rightDriveMotor.write(right);
}

/**************************************************************
   applyDeadband()
 **************************************************************/
int applyDeadband(int value) {
  if (abs(value) <= SCALED_DEADBAND) {
    return 0;
  }
  else {
    return value;
  }
}


/**************************************************************
   stopDriveMotors()
 **************************************************************/
void stopDriveMotors()
{
  leftDriveMotor.write(SERVO_STOPPED);
  rightDriveMotor.write(SERVO_STOPPED);
}
