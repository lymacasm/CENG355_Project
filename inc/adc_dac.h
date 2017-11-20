/*
 * adc_dac.h
 *
 *  Created on: Oct 23, 2017
 *      Author: lymacasm
 */

#ifndef ADC_DAC_H_
#define ADC_DAC_H_


extern void adc_dac_init();
extern int get_resistance_ohms(int voltage);
extern uint32_t get_adc();


#endif /* ADC_DAC_H_ */
