//This class represents a process recieved by the scheduler
#pragma once
class process
{
public:
	process();		//Constructor
	~process();	//Destructor
	int id;			//process id
	int priority;		//process priority
	int cpu_burst;		//Cpu bursts needed to complete first process
	int entry_time;		//Time process entered scheduler
	int turn_around;		//Turnaround time for this process
	int wait_time;		//wait time for this process
	int end_time;		//time the process finished
	int get_start_burst();		//returns starting cpu bursts
	void set_start_burst(int);		//sets starting cpu bursts
private:
	int start_burst;	//number of bursts, given at process reception
};

