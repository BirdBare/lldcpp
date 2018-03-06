//
//
//
//
//



#include "timer_lld.h"

	struct TimerObject TIMER1_OBJECT = {
		.rcc.reg_offset = 0x44,
		.rcc.bit_offset = 0,
		.rcc.peripheral_bus = APB2,
		.max_counts = 16,
		.timer = TIM1};

	struct TimerObject TIMER2_OBJECT = {
		.rcc.reg_offset = 0x40,
		.rcc.bit_offset = 0,
		.rcc.peripheral_bus = APB1,
		.max_counts = 32,
		.timer = TIM2};

	struct TimerObject TIMER3_OBJECT = {
		.rcc.reg_offset = 0x40,
		.rcc.bit_offset = 1,
		.rcc.peripheral_bus = APB1,
		.max_counts = 16,
		.timer = TIM3};

	struct TimerObject TIMER4_OBJECT = {
		.rcc.reg_offset = 0x40,
		.rcc.bit_offset = 2,
		.rcc.peripheral_bus = APB1,
		.max_counts = 16,
		.timer = TIM4};

	struct TimerObject TIMER5_OBJECT = {
		.rcc.reg_offset = 0x40,
		.rcc.bit_offset = 3,
		.rcc.peripheral_bus = APB1,
		.max_counts = 32,
		.timer = TIM5};

	struct TimerObject TIMER6_OBJECT = {
		.rcc.reg_offset = 0x40,
		.rcc.bit_offset = 4,
		.rcc.peripheral_bus = APB1,
		.max_counts = 16,
		.timer = TIM6};

	struct TimerObject TIMER7_OBJECT = {
		.rcc.reg_offset = 0x40,
		.rcc.bit_offset = 5,
		.rcc.peripheral_bus = APB1,
		.max_counts = 16,
		.timer = TIM7};

	struct TimerObject TIMER8_OBJECT = {
		.rcc.reg_offset = 0x44,
		.rcc.bit_offset = 1,
		.rcc.peripheral_bus = APB2,
		.max_counts = 16,
		.timer = TIM8};

	struct TimerObject TIMER9_OBJECT = {
		.rcc.reg_offset = 0x44,
		.rcc.bit_offset = 16,
		.rcc.peripheral_bus = APB2,
		.max_counts = 16,
		.timer = TIM9};

	struct TimerObject TIMER10_OBJECT = {
		.rcc.reg_offset = 0x44,
		.rcc.bit_offset = 17,
		.rcc.peripheral_bus = APB2,
		.max_counts = 16,
		.timer = TIM10};

	struct TimerObject TIMER11_OBJECT = {
		.rcc.reg_offset = 0x44,
		.rcc.bit_offset = 18,
		.rcc.peripheral_bus = APB2,
		.max_counts = 16,
		.timer = TIM11};

	struct TimerObject TIMER12_OBJECT = {
		.rcc.reg_offset = 0x40,
		.rcc.bit_offset = 6,
		.rcc.peripheral_bus = APB1,
		.max_counts = 16,
		.timer = TIM12};

	struct TimerObject TIMER13_OBJECT = {
		.rcc.reg_offset = 0x40,
		.rcc.bit_offset = 7,
		.rcc.peripheral_bus = APB1,
		.max_counts = 16,
		.timer = TIM13};

	struct TimerObject TIMER14_OBJECT = {
		.rcc.reg_offset = 0x40,
		.rcc.bit_offset = 8,
		.rcc.peripheral_bus = APB1,
		.max_counts = 16,
		.timer = TIM14};


#include "timer_timer_lld.c"





static inline void GENERAL_TIMER_HANDLER(struct TimerObject *timer_object)
{
	timer_object->timer->SR &= ~TIM_SR_UIF;

	if(timer_object->timer_config->callback != 0)
	{
		timer_object->timer_config->callback(timer_object->timer_config->args);
	}
	//call callback function if set. always called at end of interrupt if set.
}

void TIM6_DAC_IRQHandler(void)
{
	GENERAL_TIMER_HANDLER(&TIMER6_OBJECT);
}



