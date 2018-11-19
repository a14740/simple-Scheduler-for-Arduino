/*
 * sScheduler.h

 */
#include <Arduino.h>

#ifndef SSCHEDULER_H_
#define SSCHEDULER_H_

#define T_FOREVER 0xFFFFFFFF

typedef void (*TaskCallback)();
typedef void (*TaskOnEnable)();
typedef void (*TaskOnDisable)();

class Scheduler;
class Task {
//  friend class Scheduler;
  public:

     Task(unsigned long aInterval, TaskCallback aCallback,
    		 TaskOnEnable aOnStart=NULL, TaskOnDisable aOnStop=NULL) {
    	 iInterval=aInterval;
    	 iCallback=aCallback;
		 iOnStart=aOnStart;
		 iOnStop=aOnStop;
		 iPreviousMillis=0;
    	 iRunCounter=T_FOREVER;
		 iScheduler=NULL;
		 iPrev=NULL;
		 iNext=NULL;
     }
//     ~Task();

//     void setInterval(unsigned long aInterval) {	iInterval = aInterval;	};
//     void setIterations(long aIterations) {	iRunCounter=aIterations;	};
//     void setCallback(TaskCallback aCallback) {	iCallback=aCallback;	};
//     void setOnEnable(TaskOnEnable aCallback) {iOnDisable=aOnDisable;};

//     void setOnDisable(TaskOnDisable aCallback) ;
//     void yield(TaskCallback aCallback);
//     void yieldOnce(TaskCallback aCallback);

     TaskCallback              iCallback;
     TaskOnEnable				iOnStart;
     TaskOnDisable				iOnStop;

    volatile bool 			  isEnabled;
    volatile unsigned long    iInterval;             // execution interval in milliseconds (or microseconds). 0 - immediate
    volatile unsigned long    iDelay;                // actual delay until next execution (usually equal iInterval)
    volatile unsigned long    iPreviousMillis;       // previous invocation time (millis).  Next invocation = iPreviousMillis + iInterval.  Delayed tasks will "catch up"
    volatile unsigned long    iRunCounter;           // current left number of iteration (starting with 1). Resets on enable.

    Task                     *iPrev, *iNext;         // pointers to the previous and next tasks in the chain
    Scheduler                *iScheduler;            // pointer to the current scheduler

};


// ------------------ Scheduler -----------------------

class Scheduler {
//  friend class Task;
  public:
     Scheduler() {
    	 timeNextIteration =10;
    	 iFirst = NULL;
    	 iLast = NULL;
    	 iCurrent = NULL;
    	 taskNum=0;
     }
//	~Scheduler();
//     void init();
     void addTask(Task& aTask, unsigned long aIterations=T_FOREVER) {

    	 // Avoid adding task twice to the same scheduler
    	     if (aTask.iScheduler == this)
    	         return;
    	     aTask.iScheduler = this;
			 taskNum ++;
			 aTask.iRunCounter=aIterations;

    	 // First task situation:
    	     if (iFirst == NULL) {
    	         iFirst = &aTask;
    	         aTask.iPrev = NULL;
    	     }
    	     else {
    	 // This task gets linked back to the previous last one
    	         aTask.iPrev = iLast;
    	         iLast->iNext = &aTask;
    	     }
    	 // "Previous" last task gets linked to this one - as this one becomes the last one
    	     aTask.iNext = NULL;
    	     iLast = &aTask;
    	     if (aTask.iOnStart) aTask.iOnStart();
    	 };

     void removeTask(Task& aTask) {
    		aTask.iScheduler = NULL;
    		taskNum--;
    		if (aTask.iOnStop) aTask.iOnStop();

    	    if (aTask.iPrev == NULL) {
    	        if (aTask.iNext == NULL) {
    	            iFirst = NULL;
    	            iLast = NULL;
    	            return;
    	        }
    	        else {
    	            aTask.iNext->iPrev = NULL;
    	            iFirst = aTask.iNext;
    	            aTask.iNext = NULL;
    	            return;
    	        }
    	    }

    	    if (aTask.iNext == NULL) {
    	        aTask.iPrev->iNext = NULL;
    	        iLast = aTask.iPrev;
    	        aTask.iPrev = NULL;
    	        return;
    	    }

    	    aTask.iPrev->iNext = aTask.iNext;
    	    aTask.iNext->iPrev = aTask.iPrev;
    	    aTask.iPrev = NULL;
    	    aTask.iNext = NULL;
    	};
//     void disableTask(bool aRecursive = true);
//     void enableTask(bool aRecursive = true);
//     Task& currentTask() { return *iCurrent; };
     bool execute();                              // Returns true if none of the tasks' callback methods was invoked (true = idle run)

    Task       			*iFirst, *iLast, *iCurrent;        // pointers to first, last and current tasks in the chain
    unsigned long		timeNextIteration;				// number of ms until next iteration of the chain
    uint8				taskNum;
};



bool Scheduler::execute() {
    bool     idleRun = true;
      // millis, interval;
	Task *nextTask;  // support for deleting the task in the onDisable method
    iCurrent = iFirst;
	timeNextIteration = T_FOREVER;
	unsigned long toExeTime ,m ;
    while (iCurrent) {
		nextTask = iCurrent->iNext;
        do {
    // Disable task on last iteration:
                if (iCurrent->iRunCounter == 0)  {	removeTask(*iCurrent); break;	}
        		m =millis();
                toExeTime = m -iCurrent->iPreviousMillis;
                if ( toExeTime < iCurrent->iInterval ) {
                	toExeTime=iCurrent->iInterval - toExeTime;
                	if (toExeTime < timeNextIteration) timeNextIteration = toExeTime;
                	break;	}
                
                iCurrent->iPreviousMillis = m ;
                if (iCurrent->iInterval < timeNextIteration) timeNextIteration = iCurrent->iInterval;
                iCurrent->iCallback();

                if (iCurrent->iRunCounter != T_FOREVER) iCurrent->iRunCounter--;  // do not decrement (-1) being a signal of never-ending task
                idleRun = false;

        } while (0);    //guaranteed single run - allows use of "break" to exit
        iCurrent = nextTask;
        yield();
    }


    return (idleRun);
}




















#endif /* SLEEPSCHEDULER_H_ */
