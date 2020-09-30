# Timer
An easy to use timer for Linux meant to be similar to timers available in
embedded systems.

### Usage

Create as many timers as you like:
```
Timer timer(1000, true, TimerExpiredCallback, NULL);
```

- The first argument is the duration or interval for the timer in milliseconds. 
- The second argument indicates if the timer is periodic or a one-shot.  If
true, the timer will be periodic.  If false, it will be a one-shot. 
- The third argument is a pointer to a callback function that will get called
when the timer expires. 
- The last argument is a pointer.  This will be passed to the callback function.
If it is NULL, a pointer to the timer itself will be passed to the callback
function.  You can also pass a pointer to something that makes sense for your
application.

You can also create a timer and configure it like this:
```
Timer timer;
timer.setInterval(1000);
timer.setPeriodic(true);
timer.setCallback(MyCallback);
```

Then start the timer:
timer.start();

You need to create a callback function that will get called when the timer
expires.  Here is an example prototype:
```
void TimerExpiredCallback(void * pData);
```

### Other Methods
stop - Stops the timer from running and resets the internal count \
pause - Stops the timer from running but does not reset the count \
reset - Resets the count \
isRunning - Returns true if the timer is running \
setInterval - Sets the timer period in milliseconds \
setPeriodic - true=periodic, false=one-shot \
setCallback - Sets the callback function \
setData - Sets the data sent when the callback is called

### Background
Under the hood, an interval timer is used to provide the underlying
timing functionality. Each timer created is a soft timer that gets
exercised in the callback function for the interval timer.  The
period of the interval timer is 1 millisecond.

