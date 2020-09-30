#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <list>
#include "Timer.h"

using namespace std;

std::list<Timer*>Timer::pTimers;
bool Timer::initialized = false;

void Timer::createTimer(int32_t timeout_ms, bool _periodic, TimerExpired TimerCallback, void *data)
{
   static struct sigaction sa;
   static struct itimerval timer;

   timeout = timeout_ms;
   periodic = _periodic;
   callback = TimerCallback;
   setData(data);
   current = timeout_ms;

   if (initialized)
   {
      pTimers.push_back(this);
      return;
   }

   pTimers.push_back(this);

   running = false;

   /* Install timer_handler as the signal handler for SIGVTALRM. */
   memset (&sa, 0, sizeof (sa));
   sa.sa_handler = &timer_handler;
   sigaction (SIGALRM, &sa, NULL);

   /* Configure the timer to expire after 250 msec... */
   timer.it_value.tv_sec = 0;
   timer.it_value.tv_usec = 1000;
   /* ... and every 250 msec after that. */
   timer.it_interval.tv_sec = 0;
   timer.it_interval.tv_usec = 1000;

   initialized = true;

   setitimer (ITIMER_REAL, &timer, NULL);
}

Timer::Timer()
{
   createTimer(1000, false, NULL, NULL);
}

Timer::Timer(int32_t timeout_ms, bool _periodic, TimerExpired TimerCallback, void *data)
{
   createTimer(timeout_ms, _periodic, TimerCallback, data);
}

void Timer::stop()
{
   running = false;
   current = timeout;
}

void Timer::setData(void *data)
{
   if (data == NULL)
   {
      data = this;
   }
   pData = data;
}

void Timer::setInterval(int32_t timeout_ms)
{
   timeout = timeout_ms;
   current = timeout;
}

void Timer::timer_handler (int signum)
{
   std::list<Timer*>::iterator it;
   std::list<Timer*>callbackList;

   // Note: We need 3 loops because calling the callback
   //       might cause the timer to delete itself, which
   //       would cause the iterator to fail.  So, we
   //       collect callbacks to call, then call them, then
   //       maintain the timers.

   // Find callbacks to call
   for(it = Timer::pTimers.begin(); it != Timer::pTimers.end(); it++)
   {
      Timer* pTimer = *it;
      if (pTimer->running)
      {
         pTimer->current--;
         if(pTimer->current == 0)
         {
            if (pTimer->callback)
            {
               callbackList.push_back(pTimer);
            }
         }
      }
   }
   // Call the callbacks
   for(it = callbackList.begin(); it != callbackList.end(); it++)
   {
      Timer* pTimer = *it;
      pTimer->callback(pTimer->pData);
   }
   // maintain the timers
   for(it = Timer::pTimers.begin(); it != Timer::pTimers.end(); it++)
   {
      Timer* pTimer = *it;
      if (pTimer->running)
      {
         // The loop above may have cause current to be zero, so check it
         if(pTimer->current == 0)
         {
            if (pTimer->periodic)
            {
               pTimer->current = pTimer->timeout;
            }
            else
            {
               pTimer->running = false;
            }
         }
      }
   }
}

