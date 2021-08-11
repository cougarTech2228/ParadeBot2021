#include <Servo.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>
//#include <LiquidCrystal_I2C.h>
//
//LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

static const int FAILSAFE_LED_PIN = 2;

static const int ENABLE_BOT_BUTTON_PIN = 27;
static const int CHILD_MODE_BUTTON_PIN = 26;
static const int ENABLE_BOT_LED_PIN = 29;
static const int CHILD_MODE_LED_PIN = 28;

static const int RIGHT_DRIVE_MOTOR_PIN = 13;
static const int LEFT_DRIVE_MOTOR_PIN = 14;

static const int BALL_SHOOTER_LEFT_PIN = 7;
static const int BALL_SHOOTER_RIGHT_PIN = 8;
static const int TURRET_MOTOR_PIN = 6;
static const int CANDY_LOADER_MOTOR_PIN = 5;

static const int BALL_CYLINDER_RELAY_PIN = 22;
static const int CANDY_SHOOTER_RELAY_PIN = 23;

static const int TURRET_LIMIT_LEFT = 24;
static const int TURRET_LIMIT_RIGHT = 25;

static const int MIN_PWM_SIGNAL_WIDTH = 1000;
static const int MAX_PWM_SIGNAL_WIDTH = 2000;

static const int SERVO_FULL_REVERSE = 160;
static const int SERVO_STOPPED = 90;
static const int SERVO_FULL_FORWARD = 20;
static const int SERVO_DEADBAND = 8;
static const int SERVO_SAFETY_MARGIN = 20;

static const float CHILD_MODE_THROTTLE_FACTOR = .2;
static const float CHILD_MODE_TURN_FACTOR = .2;
static const float CHILD_MODE_STRAFE_FACTOR = .3;

static const int TURN_CORRECTION = 10;
static const int TURN_COMPENSATION = 7;

/* Radio link channels */
static const int RIGHT_STICK_X = 1;
static const int LEFT_STICK_Y = 2;
static const int RIGHT_STICK_Y = 3;
static const int LEFT_STICK_X = 4;
static const int RIGHT_TOGGLE = 5;
static const int BUTTON = 6;
static const int LEFT_TOGGLE = 7;
static const int DIAL = 8;

static const int ROBOT_MODE_BALL = 1;
static const int ROBOT_MODE_CANDY = 2;
static const int ROBOT_MODE_UNKNOWN = 0;

int robotMode = ROBOT_MODE_UNKNOWN;

static const int RADIOLINK_CONTROLLER_MINIMUM_VALUE = 200;
static const int RADIOLINK_CONTROLLER_NEUTRAL_VALUE = 1000;
static const int RADIOLINK_CONTROLLER_MAXIMUM_VALUE = 1800;

static const int TOGGLE_BOTTOM = 200;
static const int TOGGLE_MIDDLE = 1000;
static const int TOGGLE_TOP = 1800;

static const int BUTTON_PRESSED = 1800;
static const int BUTTON_NOT_PRESSED = 200;

static const int BUTTON_DEBOUNCE_TIME_MS = 300;

static const int SCALED_DEADBAND = 10; //5% deadband because scaled values are -100 to 100

// More information on the SBus: https://github.com/uzh-rpg/rpg_quadrotor_control/wiki/SBUS-Protocol
static int channels[18];

int radioLinkDriveX = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;
int radioLinkDriveY = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;
int radioLinkControlX = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;
int radioLinkControlY = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;
int radioLinkToggleRight = -1;
int radioLinkToggleLeft = -1;
int radioLinkButton = -1;

Servo rightDriveMotor, leftDriveMotor;
Servo ballShooterMotor;
bool failSafeEnabled = false;
bool botEnabled = false;
bool childModeEnabled = true;
bool receivedOneSBusPacketSinceReset = false;
bool isAuto = false;

static byte sBusBuffer[25];
static int sBusPacketsLost = 0;

// These variables will hold the adjusted forward and reverse speeds
// based on the current Child Mode setting. We've also found that full
// forward and reverse was too fast to control the robot safely so
// we are going to decrease the max range here. The Child Mode setting
// will always less than this initial value.
int adjustedServoFullForward = SERVO_FULL_FORWARD + SERVO_SAFETY_MARGIN;
int adjustedServoFullReverse = SERVO_FULL_REVERSE - SERVO_SAFETY_MARGIN;

//Timed loop variables
unsigned long startTime = millis();
int countIterations = 0;






/**************************************************************
   setup()
 **************************************************************/
void setup() {
  wdt_disable();

  setupDriveMotors();
  setupBallShooter();
  setupCandyShooter();
  //The SBUS is a non standard baud rate of 100 kbs
  Serial1.begin(100000, SERIAL_8E2);
  // put your setup code here, to run once:
  Serial1.flush();

  // Setup debug/monitor serial port
  Serial.begin(115200);
  stopDriveMotors();
//  setupLEDs();

  //sets up interrupts for the child mode and enable buttons
  setupButtonInterrupts();
  stopDriveMotors();
  
  stopDriveMotors();

  wdt_enable(WDTO_1S);
  botEnabled = false;

  Serial.println("Set up");
}

/**************************************************************
   loop()
 **************************************************************/
void loop() {

  timedLoop();
  /*
  if (failSafeEnabled) {
    stopDriveMotors();
    botEnabled = false;
  }
  else if (botEnabled) {
    // put your main code here, to run repeatedly:
    
      //handle drive motors is called from inside processControllerData();
      processControllerData();
    
    wdt_reset();
  }
  else {
    wdt_reset();
  }
  */
  wdt_reset();
  processControllerData();
  
  digitalWrite(CHILD_MODE_LED_PIN, childModeEnabled);
  digitalWrite(ENABLE_BOT_LED_PIN, botEnabled);
}

/**************************************************************
   timedLoop()
 **************************************************************/
void timedLoop() {
  unsigned long currTime = millis();
  if (currTime >= startTime + 10 * countIterations) {
    countIterations++;

    //this shouldn't be called but if it is it will reset the loop
    if (countIterations > 100) {
      countIterations = 0;
      startTime = millis();
    }

    //1 second
    if (countIterations % 100 == 0) {
      countIterations = 0;
      startTime = millis();
    }

    //500 milliseconds
    if (countIterations % 50 == 0) {
    }

    //250 milliseconds
    if (countIterations % 25 == 0) {
//      updateLEDs();
    }

    //100 milliseconds
    if (countIterations % 10 == 0) {
    }
  }
}
