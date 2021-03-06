/*
 * DalekDrive.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: Michael
 */

#include <DalekDrive.h>

using namespace frc;

DalekDrive::DalekDrive(int leftMotorChannel, int rightMotorChannel)
{
	m_leftMotor       = new WPI_TalonSRX(leftMotorChannel);
	m_rightMotor      = new WPI_TalonSRX(rightMotorChannel);
	m_leftSlaveMotor  = NULL;
	m_rightSlaveMotor = NULL;
	m_gearShift       = NULL;
	m_drive           = new DifferentialDrive(*m_leftMotor, *m_rightMotor);
	InitDalekDrive();

	m_needFree = true;
}

DalekDrive::DalekDrive(int leftMotorChannel, int leftSlaveMotorChannel,
         int rightMotorChannel, int rightSlaveMotorChannel,
		 int shiftChannel)
{
	m_leftMotor       = new WPI_TalonSRX(leftMotorChannel);
	m_rightMotor      = new WPI_TalonSRX(rightMotorChannel);
	m_leftSlaveMotor  = new WPI_TalonSRX(leftSlaveMotorChannel);
	m_rightSlaveMotor = new WPI_TalonSRX(rightSlaveMotorChannel);
	m_gearShift       = new Solenoid(PCM_ID, shiftChannel);
	m_drive           = new DifferentialDrive(*m_leftMotor, *m_rightMotor);
	InitDalekDrive();

	m_needFree = true;
}

DalekDrive::DalekDrive(WPI_TalonSRX* leftMotor, WPI_TalonSRX* rightMotor)
{
	m_leftMotor       = leftMotor;
	m_rightMotor      = rightMotor;
	m_leftSlaveMotor  = NULL;
	m_rightSlaveMotor = NULL;
	m_gearShift       = NULL;
	m_drive           = new DifferentialDrive(*m_leftMotor, *m_rightMotor);
	InitDalekDrive();

	m_needFree = false;
}

DalekDrive::DalekDrive(WPI_TalonSRX& leftMotor, WPI_TalonSRX& rightMotor)
{
	m_leftMotor       = &leftMotor;
	m_rightMotor      = &rightMotor;
	m_leftSlaveMotor  = NULL;
	m_rightSlaveMotor = NULL;
	m_gearShift       = NULL;
	m_drive           = new DifferentialDrive(*m_leftMotor, *m_rightMotor);
	InitDalekDrive();

	m_needFree = false;
}

DalekDrive::DalekDrive(WPI_TalonSRX* leftMotor, WPI_TalonSRX* leftSlaveMotor,
		WPI_TalonSRX* rightMotor, WPI_TalonSRX* rightSlaveMotor,
		 int shiftChannel)
{
	m_leftMotor       = leftMotor;
	m_leftSlaveMotor  = leftSlaveMotor;
	m_rightMotor      = rightMotor;
	m_rightSlaveMotor = rightSlaveMotor;
	m_gearShift       = new Solenoid(PCM_ID, shiftChannel);
	m_drive           = new DifferentialDrive(*m_leftMotor, *m_rightMotor);
	InitDalekDrive();

	m_needFree = false;
}

DalekDrive::DalekDrive(WPI_TalonSRX& leftMotor, WPI_TalonSRX& leftSlaveMotor,
		WPI_TalonSRX& rightMotor, WPI_TalonSRX& rightSlaveMotor,
		 int shiftChannel)
{
	m_leftMotor       = &leftMotor;
	m_leftSlaveMotor  = &leftSlaveMotor;
	m_rightMotor      = &rightMotor;
	m_rightSlaveMotor = &rightSlaveMotor;
	m_gearShift       = new Solenoid(PCM_ID, shiftChannel);
	m_drive           = new DifferentialDrive(*m_leftMotor, *m_rightMotor);
	InitDalekDrive();

	m_needFree = false;
}

DalekDrive::~DalekDrive()
{
	delete m_drive;
	if(m_gearShift)
		delete m_gearShift;
	if(m_needFree) {
		delete m_leftMotor;
		delete m_rightMotor;
		if(m_leftSlaveMotor)
			delete m_leftSlaveMotor;
		if(m_rightSlaveMotor)
			delete m_rightSlaveMotor;
	}
	m_needFree = false;
	return;
}

void
DalekDrive::Drive(double outputMagnitude, double curve)
{
	if(m_drive)
		m_drive->CurvatureDrive(outputMagnitude, curve, false);
	return;
}

void
DalekDrive::TankDrive(GenericHID* leftStick, GenericHID* rightStick,
             bool squaredInputs)
{
	if(m_drive)
		m_drive->TankDrive(leftStick->GetY(), rightStick->GetY(), squaredInputs);
}

void
DalekDrive::TankDrive(GenericHID& leftStick, GenericHID& rightStick,
             bool squaredInputs)
{
	if(m_drive)
		m_drive->TankDrive(leftStick.GetY(), rightStick.GetY(), squaredInputs);
}

void
DalekDrive::TankDrive(double leftValue, double rightValue,
             bool squaredInputs)
{
	if(m_drive)
		m_drive->TankDrive(leftValue, rightValue, squaredInputs);
}

void
DalekDrive::ArcadeDrive(GenericHID* stick, bool squaredInputs)
{
	if(m_drive)
		m_drive->ArcadeDrive(stick->GetY(), stick->GetX(), squaredInputs);
}

void
DalekDrive::ArcadeDrive(GenericHID& stick, bool squaredInputs)
{
	if(m_drive)
		m_drive->ArcadeDrive(stick.GetY(), stick.GetX(), squaredInputs);
}

void
DalekDrive::ArcadeDrive(double moveValue, double rotateValue,
               bool squaredInputs)
{
	if(m_drive)
		m_drive->ArcadeDrive(moveValue, rotateValue, squaredInputs);
}

void
DalekDrive::SetLeftRightMotorOutputs(double leftOutput, double rightOutput)
{
	if(m_drive) {
		m_leftMotor->Set(leftOutput);
		m_rightMotor->Set(rightOutput);
	}
}

void
DalekDrive::SetInvertedMotor(MotorType_t motor, bool isInverted)
{
	switch(motor) {
	case LEFT_DRIVEMOTOR:
		if(m_leftMotor) {
			m_leftMotor->SetInverted(isInverted);
			if(m_leftSlaveMotor)
				m_leftSlaveMotor->SetInverted(isInverted);
		}
		break;

	case RIGHT_DRIVEMOTOR:
		if(m_rightMotor) {
			m_rightMotor->SetInverted(isInverted);
			if(m_rightSlaveMotor)
					m_rightSlaveMotor->SetInverted(isInverted);
		}
		break;

	default:
		break;
	}
}

void
DalekDrive::SetMaxOutput(double maxOutput)
{
	if(m_drive)
		m_drive->SetMaxOutput(maxOutput);
}

void
DalekDrive::InitDalekDrive(void)
{
	// Configure the Talon's as needed
	m_leftMotor->SetNeutralMode(NeutralMode::Brake);
	m_rightMotor->SetNeutralMode(NeutralMode::Brake);
	m_leftMotor->ConfigNominalOutputForward(0.0, CANTimeoutMs);
	m_leftMotor->ConfigNominalOutputReverse(-0.0, CANTimeoutMs);
	m_rightMotor->ConfigNominalOutputForward(0.0, CANTimeoutMs);
	m_rightMotor->ConfigNominalOutputReverse(-0.0, CANTimeoutMs);
	m_leftMotor->ConfigPeakOutputForward(1.0, CANTimeoutMs);
	m_leftMotor->ConfigPeakOutputReverse(-1.0, CANTimeoutMs);
	m_rightMotor->ConfigPeakOutputForward(1.0, CANTimeoutMs);
	m_rightMotor->ConfigPeakOutputReverse(-1.0, CANTimeoutMs);
	m_leftMotor->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs); // TBD: how many MS ???
	m_rightMotor->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs);
	m_leftMotor->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, PIDLoopIdx, CANTimeoutMs);
	m_rightMotor->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, PIDLoopIdx, CANTimeoutMs);
	m_leftMotor->SetSensorPhase(true);
	m_rightMotor->SetSensorPhase(false);
	m_leftMotor->SetSelectedSensorPosition(PIDSlotIdx, PIDLoopIdx, CANTimeoutMs);
	m_rightMotor->SetSelectedSensorPosition(PIDSlotIdx, PIDLoopIdx, CANTimeoutMs);
	m_leftMotor->SetInverted(false);
	m_rightMotor->SetInverted(false);
	m_leftMotor->GetSensorCollection().SetQuadraturePosition(0,0);
	m_rightMotor->GetSensorCollection().SetQuadraturePosition(0,0);

	if(m_leftSlaveMotor) {
		m_leftSlaveMotor->Set(ControlMode::Follower, m_leftMotor->GetDeviceID());
		m_leftSlaveMotor->SetInverted(false);
		m_leftSlaveMotor->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs);
	}
	if(m_rightSlaveMotor) {
		m_rightSlaveMotor->Set(ControlMode::Follower, m_rightMotor->GetDeviceID());
		m_rightSlaveMotor->SetInverted(false);
		m_rightSlaveMotor->ConfigOpenloopRamp(RAMP_RATE, CANTimeoutMs);
	}
	if(m_gearShift)
		ShiftGear(HIGH_GEAR);
}

int
DalekDrive::GetPosition(MotorType_t motor)
{
	if(motor == LEFT_DRIVEMOTOR)
		return m_leftMotor->GetSensorCollection().GetQuadraturePosition();
	return m_rightMotor->GetSensorCollection().GetQuadraturePosition();
}

void
DalekDrive::ShiftGear(GearType_t speed)
{
	// Assumes that the forward setting of the solenoid == HIGH_GEAR
	if(m_gearShift) {
		double lspeed = m_leftMotor->GetSensorCollection().GetQuadratureVelocity();
		double rspeed = m_rightMotor->GetSensorCollection().GetQuadratureVelocity();

		if((lspeed > RPM_THRESHOLD) && (rspeed > RPM_THRESHOLD))
			return;
		switch(speed) {
		case HIGH_GEAR:
			m_gearShift->Set(false);
			break;
		case LOW_GEAR:
			m_gearShift->Set(true);
			break;
		default:
			break;
		}
	}
}

bool
DalekDrive::printFaults()
{
	Faults f;
	StickyFaults sf;
	bool faultFound = false;

	m_leftMotor->GetFaults(f);
	if(f.HasAnyFault()) {
		faultFound = true;
		frc::SmartDashboard::PutNumber("Left Talon reported faults", f.ToBitfield());
		if(m_leftSlaveMotor) {
			Faults slave;
			m_leftSlaveMotor->GetFaults(slave);
			frc::SmartDashboard::PutNumber("Left slave status",
					slave.ToBitfield());
		}
	}
	m_leftMotor->GetStickyFaults(sf);
	if(sf.HasAnyFault()) {
		faultFound = true;
		frc::SmartDashboard::PutNumber("Left Talon reported sticky faults",
				sf.ToBitfield());
		m_leftMotor->ClearStickyFaults(0);
	}
	m_rightMotor->GetFaults(f);
	if(f.HasAnyFault()) {
		faultFound = true;
		frc::SmartDashboard::PutNumber("Right Talon reported faults", f.ToBitfield());
		if(m_rightSlaveMotor) {
			Faults slave;
			m_rightSlaveMotor->GetFaults(slave);
			frc::SmartDashboard::PutNumber("Right slave status",
				slave.ToBitfield());
		}
	}
	m_rightMotor->GetStickyFaults(sf);
	if(sf.HasAnyFault()) {
		faultFound = true;
		frc::SmartDashboard::PutNumber("Right Talon reported sticky faults",
				sf.ToBitfield());
		m_leftMotor->ClearStickyFaults(0);
	}
	return faultFound;
}

bool
DalekDrive::DriveOk()
{
	// update dashboard of current draw for motors
	frc::SmartDashboard::PutNumber("Left Motor current",
			m_leftMotor->GetOutputCurrent());
	if(m_leftSlaveMotor)
		frc::SmartDashboard::PutNumber("Left Slave Motor current",
			m_leftSlaveMotor->GetOutputCurrent());
	frc::SmartDashboard::PutNumber("Right Motor current",
			m_rightMotor->GetOutputCurrent());
	if(m_rightSlaveMotor)
		frc::SmartDashboard::PutNumber("Right Slave Motor current",
			m_rightSlaveMotor->GetOutputCurrent());

	// check for motor faults
	if (printFaults())
		return false;

	return true;
}
