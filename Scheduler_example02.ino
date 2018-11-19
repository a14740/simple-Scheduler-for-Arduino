/*
 * This is a copy of https://github.com/arkhipenko/TaskScheduler/blob/master/examples/Scheduler_example02/Scheduler_example02.ino ,
 * and modified.

*/

#include "sScheduler.h"

Scheduler runner;
// Callback methods prototypes
void t1Callback();
void t2Callback();
void t3Callback(); 
void t3OnStart();
void t3OnStop();
// Tasks
Task t4();
Task t1(2000, &t1Callback);  //adding task to the chain on creation

Task t2(3000, &t2Callback);  //adding task to the chain on creation

Task t3(5000, &t3Callback,&t3OnStart,t3OnStop);

// Test
// Initially only tasks 1 and 2 are enabled
// Task1 runs every 2 seconds 10 times and then stops
// Task2 runs every 3 seconds indefinitely
// Task1 enables Task3 at its first run
// Task3 run every 5 seconds
// loop() runs every 1 second (a default scheduler delay, if no shorter tasks' interval is detected)
// Task1 disables Task3 on its last iteration and changed Task2 to run every 1/2 seconds
// Because Task2 interval is shorter than Scheduler default tick, loop() executes ecery 1/2 seconds now
// At the end Task2 is the only task running every 1/2 seconds
//
// NOTE that t1 and t2 are affected by the delay() function in the setup() method and are scheduled immediately twice to "catch up" with millis().





void t1Callback() {
    Serial.print("t1: ");
    Serial.println(millis());
    
    if (t1.iRunCounter == 10) {
      runner.addTask(t3,T_FOREVER);
//      t3.enable();
      Serial.println("t1: enabled t3 and added to the chain");
    }
    
    if (t1.iRunCounter == 1) {
//      t3.disable();
      runner.removeTask(t3);
      t2.iInterval=500;
      Serial.println("t1: disable t3 and delete it from the chain. t2 interval set to 500");
    }
}

void t2Callback() {
    Serial.print("t2: ");
    Serial.println(millis());
  
}

void t3Callback() {
    Serial.print("t3: ");
    Serial.println(millis());
  
}


void t3OnStart(){
	Serial.print("t3: ");
    Serial.println("I'm start!");
}
void t3OnStop(){
	Serial.print("t3: ");
    Serial.println("I will stop!");
}



void setup () {
  Serial.begin(115200);
  delay(5000);
  Serial.println("Scheduler TEST");
  
  runner.addTask(t1,10);
  runner.addTask(t2,T_FOREVER);
//  runner.startNow();  // set point-in-time for scheduling start
}


void loop () {
  
  runner.execute();
  if (runner.timeNextIteration<3) runner.timeNextIteration = 2;
  delay(runner.timeNextIteration-2);
//  Serial.println("Loop ticks at: ");
//  Serial.println(millis());
}
