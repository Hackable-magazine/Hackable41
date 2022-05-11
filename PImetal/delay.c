#include <stdint.h>
#include "mmio.h"
#include "delay.h"

// https://github.com/LizardLad/Raspi3-Kernel/tree/master/src
// https://forums.raspberrypi.com/viewtopic.php?t=213964
// https://github.com/LdB-ECM/Raspberry-Pi/tree/master/GLES2_Model
// FIXME LD -> https://github.com/LdB-ECM/Raspberry-Pi/blob/master/GLES2_Model/rpi64.ld


/*
 * Get the BCM System Timer's counter
 * This is done by
 * First do two seperate 32 bit reads of the SYSTEM_TIMER_LOW and SYSTEM_TIMER_HIGH
 * Then if the high bits changed during the LOW read then we have to read it again
 * no register will change in the time it takes to read them again
 * Then join the two together to an unsigned long (64bit integer)
 */
uint64_t micros()
{
	uint32_t high, low;
	high = STC_HIGH;
	low  = STC_LOW;
	if(high != STC_HIGH)
	{
		high = STC_HIGH;
		low  = STC_LOW;
	}
	return ((uint64_t)high << 32) | low;
}

/*
 * Wait a certain number of microseconds using the BCM System Timer
 * To do this we have to read from the get_system_timer() function
 * Then if 0 isn't returned continue to read from the timer until
 * it reaches the sytem timer + the number of microseconds needed to
 * wait
 * This won't work in QEMU becasue QEMU doesn't emulate the timer
 * get_system_timer() will return 0 if it cannot find the system
 * timer so this function needs to check for get_system_timer()
 * returning 0 and if it does then end this function
 */
void wait_usec_system_timer(uint32_t number_of_micros_to_wait)
{
	uint64_t current_time;
	current_time = micros();
	if(current_time)
	{
		while(micros() < number_of_micros_to_wait + current_time);
	}
}

/*
 * Micros is an Arduino compadible function with a resolution of 4Mhz
 * it returns the count of microseconds since the board began running
 * because it uses the BCM System Timer ti isn't compadible with QEMU
 * All this function does is get the system timer then run a modulo
 * division on the counter value with 4 and then check the output
 * and round the value to the closest multiple of four
 */
/*
uint64_t micros()
{
	uint64_t current_time = get_system_timer();
	if(current_time)
	{
		uint64_t current_time_modulo = current_time % 4;
		if(!current_time_modulo)
		{
			return current_time;
		}
		else if(current_time_modulo <= 2)
		{
			return (current_time - current_time_modulo);
		}
		else
		{
			return (current_time + 1);
		}
	}
	return 0;
}
*/
