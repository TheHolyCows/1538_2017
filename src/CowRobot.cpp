#include "CowRobot.h"
#include "CowBase.h"

// TODO: A lot of the older functions in this file are clunky. Clean them up.

// TODO: Initializer list
// Constructor for CowRobot
CowRobot::CowRobot()
{	
	m_DSUpdateCount = 0;
		
	m_Controller = NULL;
	// Set up drive motors
	m_LeftDriveA = new CANTalon(11);
	m_LeftDriveB = new CANTalon(12);
	m_LeftDriveC = new CANTalon(13);

	m_RightDriveA = new CANTalon(14);
	m_RightDriveB = new CANTalon(15);
	m_RightDriveC = new CANTalon(16);
	m_Climber = new CANTalon(1);

	m_LeftDriveA->ConfigNeutralMode(CANTalon::NeutralMode::kNeutralMode_Brake);
	m_LeftDriveB->ConfigNeutralMode(CANTalon::NeutralMode::kNeutralMode_Brake);
	m_LeftDriveC->ConfigNeutralMode(CANTalon::NeutralMode::kNeutralMode_Brake);
	m_RightDriveA->ConfigNeutralMode(CANTalon::NeutralMode::kNeutralMode_Brake);
	m_RightDriveB->ConfigNeutralMode(CANTalon::NeutralMode::kNeutralMode_Brake);
	m_RightDriveC->ConfigNeutralMode(CANTalon::NeutralMode::kNeutralMode_Brake);

	m_DriveEncoderRight = new Encoder(MXP_QEI_5_A, MXP_QEI_5_B, false, Encoder::k1X);
	m_DriveEncoderRight->SetDistancePerPulse(0.03054323611111); // 6*pi/360

	m_DriveEncoderLeft = new Encoder(MXP_QEI_4_A, MXP_QEI_4_B, true, Encoder::k1X);
	m_DriveEncoderLeft->SetDistancePerPulse(0.03054323611111); // 6*pi/360

	m_DriveEncoder = m_DriveEncoderRight;


	m_MatchTime = 0;

	m_LEDDisplay = new CowLib::CowAlphaNum(0x70);

	m_Gyro = CowLib::CowGyro::GetInstance();

	m_Shooter = new Shooter(7, 8, 9);
	m_ConveyerLower = new Conveyer(3);
	m_ConveyerUpper = new Conveyer(2);
	m_BallIntakeConveyer = new Conveyer(4);
	m_Turret = new Turret(6);
	m_GearIntake = new GearIntake(5, 10);

	m_LightSolenoid = new Solenoid(0);
	m_Light = new Light(m_LightSolenoid);

	//m_Gyro->Reset();
	m_PowerDistributionPanel = new PowerDistributionPanel();
	m_WebServer = new CowLib::CowLogger();

	m_LeftDriveValue = 0;
	m_RightDriveValue = 0;
	
	m_PreviousGyroError = 0;
	m_PreviousDriveError = 0;

	m_AutoOffsetAngle = 0;
}

void CowRobot::Reset()
{
	ResetEncoders();

	m_PreviousGyroError = 0;
	m_PreviousDriveError = 0;

	m_LeftDriveValue = 0;
	m_RightDriveValue = 0;
	m_MatchTime = 0;

	GetShooter()->SetPIDState(false);
	GetShooter()->SetFeederSpeed(0);
	GetConveyerUpper()->SetSpeed(0);
	GetConveyerLower()->SetSpeed(0);
	GetBallIntakeConveyer()->SetSpeed(0);

}

void CowRobot::SetController(GenericController *controller)
{
	m_Controller = controller;
}

void CowRobot::PrintToDS()
{
	if(m_DSUpdateCount % 10 == 0)
	{
		m_DSUpdateCount = 0;

//		CowLib::PrintToLCD("K:%s\nA:%s\nP:%f\nG:%f\nE:%f\nI:F%f_R%f",
//						   kinectArms,
//						   AutoModes::GetInstance()->GetName(),
//						   m_Winch->GetAverageVoltage(),
//						   m_Gyro->GetAngle(),
//						   m_DriveEncoder->GetDistance(),
//						   m_FrontIR->GetVoltage(), m_RearIR->GetVoltage());
	}
}

/// Used to handle the recurring logic funtions inside the robot.
/// Please call this once per update cycle.
void CowRobot::handle()
{	
	m_MatchTime = Timer::GetFPGATimestamp() - m_StartTime;

	if(m_Controller == NULL)
	{
		printf("No controller for CowRobot!!\n");
		return;
	}

	//printf("Handling...\n");
	m_Controller->handle(this);

	// Default drive
	float tmpLeftMotor = m_LeftDriveValue;
	float tmpRightMotor = m_RightDriveValue;

	SetLeftMotors(tmpLeftMotor);
	SetRightMotors(tmpRightMotor);

	m_Shooter->Handle();
	m_ConveyerUpper->Handle();
	m_ConveyerLower->Handle();
	m_Turret->Handle();
	m_GearIntake->Handle();
	m_BallIntakeConveyer->Handle();
	m_Light->Handle();

	if(m_DSUpdateCount % 10 == 0)
	{
		//5 is drive
		//4 s1
		//3 s2
		//2 arm
		//1 unused

		//std::cout << "Gyro: " <<  m_Gyro->GetAngle() << std::endl;
		//std::cout << std::dec
		//		  << m_DriveEncoder->Get() << " "
		//	  << m_Gyro->GetAngle() << std::endl;std::cout << "Heading: " << m_Gyro->GetAngle() << " " << m_DriveEncoder->GetDistance() << std::endl;

	}

	//std::cout << "start time: " << m_StartTime << " match time: " << m_MatchTime << std::endl;

	if ((m_MatchTime > 120) && (m_MatchTime < 135))
	{
		int timeLeft = 135 - m_MatchTime;
		m_Light->SetLightStrobeOverride();
		m_Light->SetStrobeRate(timeLeft + 1);
	}
	else if ((m_MatchTime > 105) && (m_MatchTime < 110))
	{
		m_Light->SetStrobeRate(10);
		m_Light->SetLightStrobeOverride();
	}
	else
	{
		m_Light->SetLightOffOverride();
	}

	SmartDashboard::PutNumber("Drive distance", GetDriveDistance());
	SmartDashboard::PutNumber("lEnc", (int)m_DriveEncoderLeft);
	SmartDashboard::PutNumber("rEnc", (int)m_DriveEncoderRight);
	SmartDashboard::PutNumber("enc", (int)m_DriveEncoder);


	m_DSUpdateCount++;
}

double CowRobot::GetDriveDistance()
{
	return m_DriveEncoder->GetDistance();
}

bool CowRobot::DriveDistance(double distance)
{
	double PID_P = CONSTANT("DRIVE_P");
	double PID_D = CONSTANT("DRIVE_D");
	double error = distance - m_DriveEncoder->GetDistance();
	double dError = error - m_PreviousDriveError;
	double output = PID_P*error + PID_D*dError;

	double speed = CONSTANT("PTO_DRIVE_SPEED");
	DriveLeftRight(speed-output, speed+output);


	m_PreviousDriveError = error;

	return (fabs(error) < 4 && CowLib::UnitsPerSecond(fabs(dError)) < 1);
}

bool CowRobot::DriveDistanceWithHeading(double heading, double distance, double speed)
{
	double PID_P = CONSTANT("DRIVE_P");
	double PID_D = CONSTANT("DRIVE_D");
	double error = distance - m_DriveEncoder->GetDistance();
	double dError = error - m_PreviousDriveError;
	double output = PID_P*error + PID_D*dError;
	
	double throttle = CowLib::LimitMix(output, speed);
	throttle *= -1;
	//std::cout << "Drive request speed: " << throttle << std::endl;

	bool headingResult = DriveWithHeading(heading, throttle);
	
	m_PreviousDriveError = error;
	
	return (fabs(error) < 4 && headingResult);
}

bool CowRobot::TurnToHeading(double heading)
{
	double PID_P = CONSTANT("TURN_P");
	double PID_D = CONSTANT("TURN_D");
	double error = heading - m_Gyro->GetAngle();
	double dError = error - m_PreviousGyroError;
	double output = PID_P*error + PID_D*dError;

	//speed *= -speed;

	DriveLeftRight(-output, output);

	m_PreviousGyroError = error;

	return (fabs(error) < 1 && CowLib::UnitsPerSecond(fabs(dError)) < 0.5);
}

bool CowRobot::DriveWithHeading(double heading, double speed)
{
	double PID_P = CONSTANT("TURN_P");
	double PID_D = CONSTANT("TURN_D");
	double error = heading - m_Gyro->GetAngle();
	double dError = error - m_PreviousGyroError;
	double output = PID_P*error + PID_D*dError;

	//speed *= -speed;
				
	DriveLeftRight(speed-output, speed+output);
	
	m_PreviousGyroError = error;
	
	return (fabs(error) < 1 && CowLib::UnitsPerSecond(fabs(dError)) < 0.5);
}

bool CowRobot::DriveWithHeading(double heading, double speed, double maxSpeed)
{
	double PID_P = CONSTANT("TURN_P");
	double PID_D = CONSTANT("TURN_D");
	double error = heading - m_Gyro->GetAngle();
	double dError = error - m_PreviousGyroError;
	double output = PID_P*error + PID_D*dError;
	output = CowLib::LimitMix(output, maxSpeed);

	DriveLeftRight(speed-output, speed+output);

	m_PreviousGyroError = error;

	return (fabs(error) < 1 && CowLib::UnitsPerSecond(fabs(dError)) < 0.5);
}


// Allows skid steer robot to be driven using tank drive style inputs
// @param leftDriveValue
// @param rightDriveValue
void CowRobot::DriveLeftRight(float leftDriveValue, float rightDriveValue)
{
	m_LeftDriveValue = leftDriveValue;
	m_RightDriveValue = rightDriveValue;
}

void CowRobot::DriveSpeedTurn(float speed, float turn, bool quickTurn)
{
	// Linear degredation of steeering based off of velocity
	//velocity *= 0.003;
	float temp_vel = speed;
	float sensitivity = 0;

	if (temp_vel < 0)
		temp_vel = -temp_vel;

	//printf("Velocity: %f, stick: %f\r\n", velocity, temp_vel);
	
	if(speed < 0.10 && speed > -0.10)
		speed = 0;
	if (((turn < 0.10) && (turn > -0.10)) || ((speed == 0) && !quickTurn))
		turn = 0;

	if(quickTurn)
	{
		if(speed == 0.0)
		{
			sensitivity = 1;
		}
		else
		{
			sensitivity = 0.75;
		}

	}
	else
	{
		sensitivity = 0.22;
	}

	turn *= sensitivity;
	turn = -turn;
	

	float left_power = CowLib::LimitMix(speed + turn);
	float right_power = CowLib::LimitMix(speed - turn);

	DriveLeftRight(left_power, right_power);
}

// Allows robot to spin in place
// @param turnRate
void CowRobot::QuickTurn(float turnRate)
{
	// when provided with + turn, quick turn right

	float left = -1 * turnRate;
	float right = turnRate;

	DriveLeftRight(left, right);
}

// sets the left side motors
void CowRobot::SetLeftMotors(float val)
{
	if (val > 1.0)
		val = 1.0;
	if (val < -1.0)
		val = -1.0;

	m_LeftDriveA->Set(val);
	m_LeftDriveB->Set(val);
	m_LeftDriveC->Set(val);

}

// sets the left side motors
void CowRobot::SetRightMotors(float val)
{
	if (val > 1.0)
		val = 1.0;
	if (val < -1.0)
		val = -1.0;

	m_RightDriveA->Set(-val);
	m_RightDriveB->Set(-val);
	m_RightDriveC->Set(-val);
}

void CowRobot::StartTime()
{
	m_StartTime = Timer::GetFPGATimestamp();
}

