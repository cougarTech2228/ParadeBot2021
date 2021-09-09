static int NON_CHILD_MAX_SPEED = 60;
static int CHILD_MAX_SPEED = 30
;

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
  float throttle = -map(radioLinkDriveY, 200, 1800, -100, 100);
  float turn = -map(radioLinkDriveX, 200, 1800, -100, 100);

  int tempRight = ((100 - abs(turn)) * (throttle / (float)100)) + throttle;
  int tempLeft = ((100 - abs(throttle)) * (turn / (float)100)) + turn;
  
  int maxMotorSpeed;
  int minMotorSpeed;
  if(childModeEnabled){
    maxMotorSpeed = 90 + CHILD_MAX_SPEED;
    minMotorSpeed = 90 - CHILD_MAX_SPEED;
  }
  else{
    maxMotorSpeed = 90 + NON_CHILD_MAX_SPEED;
    minMotorSpeed = 90 - NON_CHILD_MAX_SPEED;
  }

  int right = (tempRight + tempLeft) / 2;
  int left = (tempRight - tempLeft) / 2;

  right = map(right, -100, 100, maxMotorSpeed, minMotorSpeed);
  left = map(left, -100, 100, minMotorSpeed, maxMotorSpeed);

  rightDriveMotor.write(right);
  leftDriveMotor.write(left);

  //Serial.print(right);
  //Serial.print(", ");
  //Serial.println(left);
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
