#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>
#include <list>
#include <time.h>
#include "Timer.h"

using namespace std;

void MyCallback1(void * pData);
void MyCallback2(void * pData);
void MyCallback3(void * pData);

Timer timer1(1000, true, MyCallback1, NULL);
Timer timer2(2000, true, MyCallback2, NULL);

void MyCallback1(void * pData)
{
   static uint32_t ticks = 0;
   ticks++;

   cout << "Callback 1, " << ticks <<  endl;

}

void MyCallback2(void * pData)
{
   static int count = 0;
   Timer* pTimer = (Timer*)pData;
   cout << "Callback 2, data = " << pTimer << endl;
   count++;
   if (count >= 3)
   {
      cout << "Stopping timer 2" << endl;
      pTimer->stop();
   }
}

void MyCallback3(void * pData)
{
   static int count = 0;
   Timer* pTimer = (Timer*)pData;
   cout << "Callback 3, data = " << pTimer << endl;
   count++;
   if (count >= 5)
   {
      cout << "Killing timer 3" << endl;
      delete pTimer;
   }
}

void MyCallback4(void * pData)
{
   cout << "Timer 4 has expired." << endl;
}

int main ()
{
   timer1.start();
   timer2.start();

   // Creating a timer with new
   Timer *timer3 = new Timer(3000, true, MyCallback3, NULL);
   timer3->start();
   cout << "Timer 3 is " << timer3 << endl;

   // Create a one-shot timer using the alternate method
   Timer timer4;
   timer4.setInterval(5000);
   timer4.setPeriodic(false);
   timer4.setCallback(MyCallback4);;
   timer4.start();

   /* Do busy work. */
   while (1);
}

