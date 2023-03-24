/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Bryan Newbold.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 *  @brief Wirish timer class.
 */

#ifndef _HARDWARETIMER_H_
#define _HARDWARETIMER_H_

# include <Arduino.h>
//#include <wm_tim.h>
/** Timer mode. */
//typedef timer_mode TimerMode;
//#define TIMER_COUNT 6
extern TIM_HandleTypeDef* timer_devices[TIMER_COUNT];
extern timer_irq_callback timer_callback[TIMER_COUNT]; 

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_Callback(TIM_HandleTypeDef *htim);

/**
 * @brief Interface to one of the 16-bit timer peripherals.
 */
class HardwareTimer
{
private:
    TIM_HandleTypeDef *dev;
    uint8_t instance = 255;
public:
    /**
     * @brief Construct a new HardwareTimer instance.
     * @param timerNum number of the timer to control.
     */
    HardwareTimer(uint8_t timerNum) 
    {
      assert_param(IS_TIM_INSTANCE(timerNum));

      this->dev = timer_devices[timerNum];
      instance = timerNum;
      this->dev->Instance = this->instance;

    }
	
    uint8_t configure(uint32_t period, uint32_t units, uint32_t flags, timer_irq_callback callback = NULL)
       { 
        this->dev->Instance = this->instance;
        this->dev->Init.Unit = units;
        this->dev->Init.Period = period;
        timer_callback[this->instance] = (timer_irq_callback) callback;
        this->dev->Init.AutoReload = flags;
        if (HAL_TIM_Base_Init(this->dev) != HAL_OK)  return 1;
        return 0;
       }


    /**
     * @brief Stop the counter, without affecting its configuration.
     *
     * @see HardwareTimer::resume()
     */
    void stop(void) { 
        if (timer_callback[this->instance] != NULL) HAL_TIM_Base_Stop_IT(this->dev); 
        else HAL_TIM_Base_Stop(this->dev); }
    
    //void pause(void)  { stop(); }  
    /**
     * @brief Resume a paused timer, without affecting its configuration.
     *
     * The timer will resume counting and firing interrupts as
     * appropriate
     *
     * @see HardwareTimer::pause()
     */
    void start(void) { 
        if (timer_callback[this->instance] != NULL) HAL_TIM_Base_Start_IT(this->dev); 
        else HAL_TIM_Base_Start(this->dev); }

    //void resume(void)  { start(); }    

    /**
     * @brief Get the timer's units.
     * @return TIM_UNIT_US or TIM_UNIT_MS
     */
    uint32_t getTimerUnits(void) {
        return ((TIM->CR & TIM_CR_TIM_UNIT(this->dev->Instance - TIM0)) == 
                 (uint32_t)TIM_CR_TIM_UNIT(this->dev->Instance - TIM0));
    }

     /**
     * @brief Get the timer's reload mode.
     * @return TIM_AUTORELOAD_PRELOAD_ENABLE or ...DISABLE
     */
    uint32_t getTimerReloadFlag(void) {
        return ((TIM->CR & TIM_CR_TIM_MODE(this->dev->Instance - TIM0)) == 
                 (uint32_t)TIM_CR_TIM_MODE(this->dev->Instance - TIM0));
    }

    /**
     * @brief Get the timer overflow value.
     * @see HardwareTimer::setOverflow()
     */
    uint32_t getPeriod() 
     { return (*(uint32_t *)(&(TIM->TIM0_PRD) + instance)); }

    /**
     * @brief Set the timer overflow (or "reload") value.
     *
     * The new value won't take effect until the next time the counter
     * overflows.  You can force the counter to reset using
     * HardwareTimer::refresh().
     *
     * @param val The new overflow value to set
     * @see HardwareTimer::refresh()
     */
    void setPeriod(uint32_t val) 
     { WRITE_REG(*(uint32_t *)(&(TIM->TIM0_PRD) + instance), val);
       this->dev->Init.Period = val;
     }

    /**
     * @brief Get the current timer count.
     *
     * @return The timer's current count value
     */
    uint32_t getCount(void) 
     { return (*(uint32_t *)(&(TIM->TIM0_CNT) + instance)); }

    /**
     * @brief Set the current timer count.
     *
     * @param val The new count value to set.  If this value exceeds
     *            the timer's overflow value, it is truncated to the
     *            overflow value.
     */
    //  Not available for W8xx mcu
    //void setCount(uint16 val) { timer_set_count(this->dev, min(val, this->getOverflow())); }

    /**
     * @brief Get the compare value for the given channel.
     * @see HardwareTimer::setCompare()
     */
    //uint32_t getCompare(void) { return getPeriod(); }

    /**
     * @brief Set the compare value for the given channel.
     *
     * @param channel the channel whose compare to set, from 1 to 4.
     * @param compare The compare value to set.  If greater than this
     *                timer's overflow value, it will be truncated to
     *                the overflow value.
     *
     * @see timer_mode
     * @see HardwareTimer::setMode()
     * @see HardwareTimer::attachInterrupt()
     */
    //void setCompare(uint32_t val) { setPeriod(val);}

    /**
     * @brief Attach an interrupt handler to the given channel.
     *
     * This interrupt handler will be called when the timer's counter
     * reaches the given channel compare value.
     *
     * @param channel the channel to attach the ISR to, from 0 to 4.
     *   Channel 0 is for overflow interrupt (update interrupt).
     * @param handler The ISR to attach to the given channel.
     * @see voidFuncPtr
     */
    void attachInterrupt( timer_irq_callback callback) {
        this->stop();
        timer_callback[this->instance] = (timer_irq_callback) callback;
    }

    /**
     * @brief Remove the interrupt handler attached to the given
     *        channel, if any.
     *
     * The handler will no longer be called by this timer.
     *
     * @param channel the channel whose interrupt to detach, from 0 to 4.
     *   Channel 0 is for overflow interrupt (update interrupt).
     * @see HardwareTimer::attachInterrupt()
     */
    void detachInterrupt() {
        this->stop();
        timer_callback[this->instance] = (timer_irq_callback) NULL;
    }

    /**
     * @brief Reset the counter, and update the prescaler and overflow
     *        values.
     *
     * This will reset the counter to 0 in upcounting mode (the
     * default).  It will also update the timer's prescaler and
     * overflow, if you have set them up to be changed using
     * HardwareTimer::setPrescaleFactor() or
     * HardwareTimer::setOverflow().
     *
     * @see HardwareTimer::setPrescaleFactor()
     * @see HardwareTimer::setOverflow()
     */
    //void refresh(void) { timer_generate_update(this->dev); }

	

    /**
     * @brief Enable/disable DMA request for the input channel.
     */
    //void enableDMA(timer_channel channel) { timer_dma_enable_req(this->dev, channel); }

    //void disableDMA(timer_channel channel) { timer_dma_disable_req(this->dev, channel); }


    /**
     * @brief Get a pointer to the underlying libmaple timer_dev for
     *        this HardwareTimer instance.
     */
    TIM_HandleTypeDef* c_dev(void) { return this->dev; }


};


/**
 * @brief Deprecated.
 *
 * Pre-instantiated timer instances.
 */

/*
extern HardwareTimer Timer0;
extern HardwareTimer Timer1;
extern HardwareTimer Timer2;
extern HardwareTimer Timer3;
extern HardwareTimer Timer4;
extern HardwareTimer Timer5;
*/
#endif
