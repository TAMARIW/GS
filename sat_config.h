// Tamariw configuration file
// 2024-06-12

#ifndef _SAT_CONFIG_H_
#define _SAT_CONFIG_H_

// Docking parameters
#define DOCK_LATCH_CURRENT_mA 1000
#define DOCK_UNLATCH_CURRENT_mA -1000
#define DOCK_CAPTURE_CURRENT_mA 2000
#define DOCK_CONTROL_VELOCITY_SP 0.0
#define DOCK_CONTROL_DISTANCE_SP_MM 20

// Parameters of the electromagnet
#define COIL_PARAM_u0 1.25663706127e-6 // Vacuum permeability [N/A^2]
#define COIL_PARAM_N 100               // Number of turns
#define COIL_PARAM_A 0.1               // Cross section area of coil [m^2]

// Docking controller gains
#define DOCK_CONTROLLER_GAIN_KP 0.0001
#define DOCK_CONTROLLER_GAIN_KI 0.0
#define DOCK_CONTROLLER_GAIN_KD 0.0
#define DOCK_CONTROLLER_GAIN_KF 1.0

// Battery charge and voltage monitor
#define PIN_CHARGE_ENABLE GPIO_038   // PC6
#define BATT_VOLT_ADC_PIN ADC_CH_014 // PC4
#define BATT_VOLT_ADC_IDX ADC_IDX1   // ADC3_CH14
#define BATT_VOLT_ADC_RES 12         // PC4

// Timeouts
#define TOF_MUX_TIMEOUT_MILLIS 2
#define TOF_RANGE_TIMEOUT_MILLIS 2

// Buffer size
#define MAX_BUFFER_SIZE_TCMD 25
#define MAX_BUFFER_SIZE_TELEM 200

// ToF Kalman Filter parameters
#define KF1D_Q_POS 0.1f
#define KF1D_Q_VEL 0.1f
#define KF1D_R 1.0f
#define KF1D_MAX_TOF_ERROR 4

// ToF sensor
#define TOF_MAX_LENGTH_MM 400
#define TOF_MIN_LENGTH_MM 0

// #define TOF_I2C_HAL_IDX I2C_IDX3
// #define TOF_I2C_HAL_GPIO_SCL GPIO_008 // PA8
// #define TOF_I2C_HAL_GPIO_SDA GPIO_041 // PC9
#define TOF_I2C_HAL_IDX I2C_IDX1
#define TOF_I2C_HAL_GPIO_SCL GPIO_022 // PB6
#define TOF_I2C_HAL_GPIO_SDA GPIO_023 // PB7
#define TOF_I2C_HAL_FREQUENCY_HZ 400000
#define TOF_I2C_ADDRESS 0x29
#define TOF_I2C_MUX_ADDRESS 0x70
#define TOF_CALIBRATE_TARGET_MM 100
#define TOF_CALIBRATE_SAMPLES 20
#define TOF_A_PIN_XSHUT GPIO_077 // PE13
#define TOF_B_PIN_XSHUT GPIO_078 // PE14
#define TOF_TBOOT_MILLIS 2

// ToF dimensions
#define TOF_DIMENSION_WIDTH_MM 44.95
#define TOF_DIMENSION_LENGTH_MM 101.5

// Moving average window size
#define EM_MAVG_WINDOW 5

// Electromagnet enable pin
#define EM_ENABLE_PIN GPIO_056
#define EM_SAFETY_THRESHOLD 50
#define EM_SAFETY_INTERMEDIATE 20

// PWMs for electromagnets
#define EM_PWM_FREQUENCY 20000
#define EM_PWM_INCREMENTS 1000
#define EM0_PWM_IDX PWM_IDX06
#define EM1_PWM_IDX PWM_IDX07
#define EM2_PWM_IDX PWM_IDX05
#define EM3_PWM_IDX PWM_IDX04

// Direction pins for electromagnets
#define EM0_PIN_IN1 GPIO_076
#define EM0_PIN_IN2 GPIO_075
#define EM1_PIN_IN1 GPIO_073
#define EM1_PIN_IN2 GPIO_074
#define EM2_PIN_IN1 GPIO_061
#define EM2_PIN_IN2 GPIO_060
#define EM3_PIN_IN1 GPIO_062
#define EM3_PIN_IN2 GPIO_063

// ADC channels for electromagnets current sense
#define EM_ADC_IDX ADC_IDX1
#define EM0_ADC_CH ADC_CH_012
#define EM1_ADC_CH ADC_CH_013
#define EM2_ADC_CH ADC_CH_011
#define EM3_ADC_CH ADC_CH_010

// Periods of threads
#define THREAD_PERIOD_TCMD_MILLIS 100
#define THREAD_PERIOD_COIL_MILLIS 6
#define THREAD_PERIOD_DOCK_MILLIS 20
#define THREAD_PERIOD_RANGE_MILLIS 15
#define THREAD_PERIOD_TELEM_MILLIS 50

// Thread priorities
#define THREAD_PRIO_TCMD 80
#define THREAD_PRIO_COIL 90
#define THREAD_PRIO_DOCK 50
#define THREAD_PRIO_RANGE 100
#define THREAD_PRIO_TELEM 0

// Thread start times
#define THREAD_START_TCMD_MILLIS 100
#define THREAD_START_COIL_MILLIS 200
#define THREAD_START_DOCK_MILLIS 300
#define THREAD_START_RANGE_MILLIS 400
#define THREAD_START_TELEM_MILLIS 500

// Current control params
#define PID_COIL_KP 0.065
#define PID_COIL_KI 0.3
#define PID_COIL_UMAX 90
#define PID_COIL_UMIN 0

// LEDs (see: led.cpp)
#define LED_FAR_COUNTS 6
#define LED_NEAR_COUNTS 6

#endif // sat_config.h
