//Implements the process class
#include "process.h"


//constructor to initialize process object
process::process()
{
	priority = 0;
	entry_time = 0;
	cpu_burst = 0;
	turn_around = 0;
	wait_time = 0;
	id = 0;
	end_time = 0;
	start_burst = 0;
}

//destructor
process::~process(){
}

//returns starting cpu bursts
int process::get_start_burst(){
	return start_burst;
}

//sets starting cpu bursts
void process::set_start_burst(int num){
	start_burst = num;
	cpu_burst = num;
}
