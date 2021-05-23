/*Created 04-28-2021 - Julian Hüsing Auditor
AuditorSystems_Stepper.h
//BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

-------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_STEPPER_h
#define _AUDITORSYSTEMS_STEPPER_h

#include <AuditorSystems.h>
#include <ESP_FlexyStepper.h>

namespace AuditorSystems
{
	
//--------------------------
	template<
		typename T_ACCELERATION_STEPS_PER_S,
		typename T_DECELERATION_STEPS_PER_S,	
		typename T_DISTANCE_IN_STEPS,	
		typename T_Enabled,	
		typename T_ESTOP_HIGH_LOW,
		typename T_MOTOR_DIRECTION_PIN,	
		typename T_MOTOR_EN_PIN,	
		typename T_MOTOR_STEP_PIN,	
		typename T_SPEED,	
		typename T_StepperOutputPinsDistanceToTarget,	
		typename T_StepperOutputPinsmotionComplete,	
		typename T_StepperOutputPinsPositionInSteps


	> class Stepper :
		public T_ACCELERATION_STEPS_PER_S,	
		public T_DECELERATION_STEPS_PER_S,	
		public T_DISTANCE_IN_STEPS,	
		public T_Enabled,
		public T_ESTOP_HIGH_LOW,
		public T_MOTOR_DIRECTION_PIN,	
		public T_MOTOR_EN_PIN,	
		public T_MOTOR_STEP_PIN,	
		public T_SPEED,	
		public T_StepperOutputPinsDistanceToTarget,	
		public T_StepperOutputPinsmotionComplete,	
		public T_StepperOutputPinsPositionInSteps	

	{


	
		
	public:
		_V_PROP_( ACCELERATION_STEPS_PER_S )
	public:
		_V_PROP_( DECELERATION_STEPS_PER_S )
	public:
		_V_PROP_( DISTANCE_IN_STEPS )
	public:
		_V_PROP_( Enabled )
	public:
		_V_PROP_(ESTOP_HIGH_LOW)
	public:
		_V_PROP_( MOTOR_DIRECTION_PIN )
	public:
		_V_PROP_( MOTOR_EN_PIN )
	public:
		_V_PROP_( MOTOR_STEP_PIN )
	public:
		_V_PROP_( SPEED )
	public:
		_V_PIN_( StepperOutputPinsDistanceToTarget )
	public:
		_V_PIN_( StepperOutputPinsmotionComplete )
	public:
		_V_PIN_( StepperOutputPinsPositionInSteps )
	
	public:

		bool emergencySwitchTriggered = 0;
		//int distance_in_steps=DISTANCE_IN_STEPS();
		ESP_FlexyStepper stepper;

	protected:
		void stepper_init()
		{
			  stepper.connectToPins(MOTOR_STEP_PIN(), MOTOR_DIRECTION_PIN());
			  // set the speed and acceleration rates for the stepper motor
			  stepper.setSpeedInStepsPerSecond(SPEED());
			  stepper.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_STEPS_PER_S());
			  stepper.setDecelerationInStepsPerSecondPerSecond(DECELERATION_STEPS_PER_S());
			  
			  // Not start the stepper instance as a service in the "background" as a separate task
			  // and the OS of the ESP will take care of invoking the processMovement() task regularily so you can do whatever you want in the loop function
			  stepper.startAsService();
		}
	public:
		void STEP_o_Receive(void *_Data)
		{
			stepper.setTargetPositionRelativeInSteps(DISTANCE_IN_STEPS());
			//stepper.setTargetPositionRelativeInMillimeters(DISTANCE_IN_STEPS());
			
		}
	
	public:
		void ESTOP_o_Receive(void *_Data)
		{
			emergencySwitchTriggered=*( bool *)_Data;
			switch(ESTOP_HIGH_LOW())
			{
				case false: 
					emergencySwitchTriggered=emergencySwitchTriggered;
					break;
				case true:
					emergencySwitchTriggered=!emergencySwitchTriggered;
					break;
				default:
				break;
			}
				
		}
		void ACCELERATION_STEPS_PER_S_o_Receive(void *_Data)
		{
			stepper.setAccelerationInStepsPerSecondPerSecond(*( int *)_Data);
			
		}
		void DECELERATION_STEPS_PER_S_o_Receive(void *_Data)
		{
			stepper.setAccelerationInStepsPerSecondPerSecond(*( int *)_Data);
			
		}		
		void SPEED_o_Receive(void *_Data)
		{
			stepper.setSpeedInStepsPerSecond((*( int *)_Data));
			
		}
		
	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) return;
			
		T_StepperOutputPinsmotionComplete::SetPinValue(stepper.motionComplete());
		T_StepperOutputPinsPositionInSteps::SetPinValue(stepper.getCurrentPositionInSteps()); // Send output		
		T_StepperOutputPinsDistanceToTarget::SetPinValue(stepper.getDistanceToTargetSigned());
		if(emergencySwitchTriggered)
			{
				//clear the target position (if it the stepper is moving at all) and stop moving
				stepper.emergencyStop();
				//clear the flag that has been set in the ISR 
				emergencySwitchTriggered = 0;
			}
		}
	public:
		inline void SystemInit()
		{
			stepper_init();
			
				
		}
			
	};
//---------------------------
}
#endif