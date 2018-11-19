# simple-Scheduler-for-Arduino
a short of TaskScheduler,a simpler task scheduler and tested on ESP8266


Thanks：
  https://github.com/arkhipenko/TaskScheduler
  
This is a more simpler task scheduler then TaskScheduler, and the most codes is copied frome TaskScheduler's codes.

I have tested on ESP8266.

It's designed for long Interval (>100 ms) tasks to schedule. Best not to used for a precise time controled programme.

How to use: 

（可以看看 Task和Scheduler类的属性成员，所有成员都是public的 可以在函数中自由修改，以实现调度的动态控制，没有使用设置函数）

  1. Create callback funs and tasks: 创建回调函数和任务
  
      void t1Callback();
      
      void t2Callback();
      
      ...
      Task t1(2000, &t1Callback);
      
      Task t2(3000, &t2Callback);
      ...
      
  2. Create a scheduler：创建调度器
  
      Scheduler runner;
      
  3. Add the tasks to scheduler in setup() ：
  
  把任务加入调度器，同时调度器会执行任务的iOnStart函数，如果有的话。removeTask时会执行该任务的iOnStop函数
  
      runner.addTask(t1,T_FOREVER);      //the T_FOREVER is invocations，T_FOREVER means always running。
      
      runner.addTask(t1,100)；           //this run 100 times。
      
      //执行完100次以后调度器执行removeTask.可以先判断Task.iRunCounter的值是否为零，然后增加它或再addTask
      
  4. Executting the scheduler's execute() fun:
  
        for example in loop {}，
        
        loop {
        
        runner.execute();
        
        /*you can add a delay。timeNextIteration是下次最先执行回调函数的 任务的最小时间，可以delay一下
              
        if (runner.timeNextIteration<3) runner.timeNextIteration = 2;
        
        delay(runner.timeNextIteration-2);
        
        /*
        
        }
        
