#include "ch.h" // ChibiOS
#include "hal.h" // ChibiOS HAL
#include "mc_interface.h" // Motor control functions
#include "hw.h" // Pin mapping on this hardware
#include "timeout.h" // To reset the timeout
 
// Example thread
static THD_FUNCTION(example_thread, arg);
static THD_WORKING_AREA(example_thread_wa, 2048); // 2kb stack for this thread
 
void app_example_init(void) {
	// Set the UART TX pin as an input with pulldown
	palSetPadMode(HW_UART_TX_PORT, HW_UART_TX_PIN, PAL_MODE_INPUT_PULLDOWN);
	palSetPadMode(HW_UART_RX_PORT, HW_UART_RX_PIN, PAL_MODE_OUTPUT_PUSHPULL); // added
 
	// Start the example thread
	chThdCreateStatic(example_thread_wa, sizeof(example_thread_wa),
		NORMALPRIO, example_thread, NULL);
}
 
static THD_FUNCTION(example_thread, arg) {
	(void)arg;
 
	chRegSetThreadName("APP_EXAMPLE");

	//bool servoTest = false; // added
 
	for(;;) {
/*
		// Read the pot value and scale it to a number between 0 and 1 (see hw_46.h)
		float pot = (float)ADC_Value[ADC_IND_EXT];
		pot /= 4095.0;
 
		if (palReadPad(HW_UART_TX_PORT, HW_UART_TX_PIN)) {
			// If the button is pressed, run the motor with speed control
			// proportional to the POT position with a speed between 0 ERPM
			// and 10000 ERPM
			mc_interface_set_pid_speed(pot * 10000.0);
		} else {
			// If the button is not pressed, release the motor
			mc_interface_release_motor();
		}
*/
		//servoTest = !servoTest; // added
/*
		if (servoTest) // added
		{
			palSetPad(HW_UART_RX_PORT, HW_UART_RX_PIN);
		}
		else
		{
			palClearPad(HW_UART_RX_PORT, HW_UART_RX_PIN);
		} // added
*/
 
		// Run this loop at .5Hz
// PPM pulse period
		for (int i = 0; i < 250; ++i)
		{
			palClearPad(HW_UART_RX_PORT, HW_UART_RX_PIN);
			chThdSleepMilliseconds(18);
			palSetPad(HW_UART_RX_PORT, HW_UART_RX_PIN);
			chThdSleepMilliseconds(2);
		}
		for (int i = 0; i < 250; ++i)
		{
			palClearPad(HW_UART_RX_PORT, HW_UART_RX_PIN);
			chThdSleepMilliseconds(18.5);
			palSetPad(HW_UART_RX_PORT, HW_UART_RX_PIN);
			chThdSleepMilliseconds(1.5);
		}
		for (int i = 0; i < 250; ++i)
		{
			palClearPad(HW_UART_RX_PORT, HW_UART_RX_PIN);
			chThdSleepMilliseconds(19);
			palSetPad(HW_UART_RX_PORT, HW_UART_RX_PIN);
			chThdSleepMilliseconds(1);
		}

		
 
		// Reset the timeout
		timeout_reset();
	}
}
