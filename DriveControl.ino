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
  int throttle = map(radioLinkDriveY, 200, 1800, -100, 100);
  int turn = -map(radioLinkDriveX, 200, 1800, -100, 100);

  int tempRight = (100 - abs(turn)) * (throttle / 100) + throttle;
  int tempLeft = (100 - abs(throttle)) * (turn / 100) + turn;

  int right = (tempRight + tempLeft) / 2;
  int left = (tempRight - tempLeft) / 2;

  right = map(right, -100, 100, 180, 0);
  left = map(left, -100, 100, 0, 180);

  rightDriveMotor.write(right);
  leftDriveMotor.write(left);

  Serial.print(right);
  Serial.print(", ");
  Serial.println(left);
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
