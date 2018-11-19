# simple-Scheduler-for-Arduino
a short of TaskScheduler,a simpler task scheduler and tested on ESP8266


Thanks：
  https://github.com/arkhipenko/TaskScheduler
  
This is a more simpler task scheduler then TaskScheduler, and the most codes is copied frome TaskScheduler's codes.
I have tested on ESP8266.
It's designed for long Interval (>100 ms) tasks to schedule. Best not to used for a precise time controled programme.

How to use:
  1. Create callback funs and tasks:
  
      void t1Callback();
      
      void t2Callback();
      
      ...
      Task t1(2000, &t1Callback);
      
      Task t2(3000, &t2Callback);
      ...
      
  2. Create a scheduler：
  
      Scheduler runner;
      
  3. Add the tasks to scheduler：
  
      runner.addTask(t1,T_FOREVER); //the T_FOREVER is invocations，T_FOREVER means always running。
      
      runner.addTask(t1,100)；      //this run 100 times。
      
  4. Executting the scheduler's execute() fun:
  
        for example in loop {}，
        
        loop {
        
        runner.execute();
        
        if (runner.timeNextIteration<3) runner.timeNextIteration = 2;
        
        delay(runner.timeNextIteration-2);
        
        }
        
