/*
 * Timer.h
 *
 * A timer for Linux.
 *
 * Usage:
 * 
 * Create as many timers as like:
 * Timer timer1(1000, true, TimerExpiredCallback, NULL);
 *
 * The first argument is the duration or interval for the timer in milliseconds.
 * The second argument indicates if the timer is periodic or a one-shot.  If
 * true, the timer will be periodic.  If false, it will be a one-shot.
 * The third argument is a pointer to a callback function that will get called
 * when the timer expires.
 * The last argument is a pointer.  This will be passed to the callback function.
 * If it is NULL, a pointer to the timer itself will be passed to the callback
 * function.  You can also pass a pointer to something that makes sense for your
 * application.
 *
 * Then start the timer:
 * timer1.start();
 *
 * You need to create a callback function that will get called when the timer
 * expires.  Here is an example prototype:
 * void TimerExpiredCallback(void * pData);
 *
 * Ohter Methods:
 * stop - Stops the timer from running and resets the internal count
 * pause - Stops the timer from running but does not reset te count
 * reset - Resets the count
 * isRunning - Returns true if the timer is running
 * setInterval - Sets the timer period in milliseconds
 * setPeriodic - true = periodic, false=one-shot
 * setCallback - Sets the callback function
 * setData - Sets the data sent when the callback is called
 *
 * Author: Rob Bultman
 * License: MIT
 * Date: September 30, 2020
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <list>

typedef void (*TimerExpired)(void *);

class Timer
{
   public:
      Timer();
      Timer(int32_t timeout_ms, bool periodic, TimerExpired TimerCallback, void *data);
      ~Timer() {pTimers.remove(this);}
      void start() {running = true;}
      void stop();
      void reset() {current = timeout;}
      void pause() {running = false;}
      bool isRunning() {return running;}
      void setInterval(int32_t timeout_ms);
      void setPeriodic(bool isPeriodic) {periodic = isPeriodic;}
      void setCallback(TimerExpired TimerCallback) {callback = TimerCallback;}
      void setData(void *);

   private:
      int32_t timeout;
      bool periodic;
      bool running;
      TimerExpired callback;
      void * pData;
      int32_t current;
      static std::list<Timer*>pTimers;
      static bool initialized;
      static void timer_handler(int);
      void createTimer(int32_t timeout_ms, bool periodic, TimerExpired TimerCallback, void *data);
};

#endif // TIMER_H

