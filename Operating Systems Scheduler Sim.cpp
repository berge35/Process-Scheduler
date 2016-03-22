//Operating Systems Scheduler Sim.cpp : Defines the entry point for the console application.
//Created by Christopher Elliott.
//this program takes input processes and executes them based on various algorithms.
#include "process.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

//function to compare two processes based on arrival time
bool compare_arrival(const process & tLHS, const process & tRHS){
	return tLHS.entry_time < tRHS.entry_time;
}

//function to compare two processes based on priority
bool compare_priority(const process & tLHS, const process & tRHS){
	return tLHS.priority < tRHS.priority;
}

//function to execute processes based on sorting 
void output(vector<process> processes, string alg){
	int system_time = 0;
	int total_bursts = 0;
	vector<process> ready;		//processes ready to be executed on cpu
	vector<process> finished;		//processes which have been completed
	for (vector<process>::iterator it = processes.begin(); it != processes.end(); ++it){		//calculate total bursts cpu needs to run
		total_bursts += (*it).cpu_burst;
	}
	while (total_bursts > 0){		//while some processes still have bursts left
		for (vector<process>::iterator it = processes.begin(); it != processes.end(); ++it){
			int id = (*it).id;

			auto foundReady = find_if(ready.begin(), ready.end(), [&id](const process& obj) {return obj.id == id; });	//find matching id in ready queue
			auto foundDone = find_if(finished.begin(), finished.end(), [&id](const process& obj) {return obj.id == id; });	//find matching id in finished vector

			if ((*it).entry_time <= system_time && foundReady == ready.end() && foundDone == finished.end()){		//process has arrived and is not already in queue and has not been completed
				ready.push_back(*it);		//push process to ready queue
			}
		}
		//sort processes according to given attribute
		if (alg == "arrival"){
			sort(ready.begin(), ready.end(), compare_arrival);
		}
		else if (alg == "priority"){
			sort(ready.begin(), ready.end(), compare_priority);
		}
		if (!ready.empty()){
			while (ready.front().cpu_burst > 0){			//while current processes still has execution time left
				cout << "System Time:" << system_time << " | Process:" << ready.front().id << " | Bursts Left:" << ready.front().cpu_burst << "\n";
				system_time++;
				ready.front().cpu_burst--;
				total_bursts--;
			}
			vector<process>::iterator it = processes.begin();
			for (it; it != processes.end(); ++it){		//get the corresponding process from the input vector
				if ((*it).id == ready.front().id){
					break;
				}
			}
			(*it).end_time = system_time;	//time process finished execution
			(*it).turn_around = (*it).end_time - (*it).entry_time;		//turn around time = completion time - arrival time
			(*it).wait_time = (*it).turn_around - (*it).get_start_burst(); // wait time = turnaround time - total executed bursts
			finished.push_back(*it);		//add to finished vector
			ready.erase(ready.begin());		//remove finished process from ready queue
		}
		else{
			cout << "System Time:" << system_time << " | Process:None\n";
			system_time++;
		}
	}
	cout << "Processes Completed at System Time:" << system_time << "\n";
	int total_turn_around = 0;
	int total_wait_time = 0;
	for (vector<process>::iterator it = processes.begin(); it != processes.end(); ++it){		//summing turnaround times and waiting times to be averaged
		total_turn_around += (*it).turn_around;
		total_wait_time += (*it).wait_time;
	}
	float size = (float)processes.size();
	float avg_turn_around = float(total_turn_around) / size;
	float avg_wait_time = float(total_wait_time) / size;
	float throughput = size / (float(system_time));		//throughput = processes completed per unit of time
	cout << "Average Turnaround Time:" << avg_turn_around << " | Average Wait Time:" << avg_wait_time << " | Throughput:" << throughput << "\n\n";
}

//First Come First Served Algorithm
void FCFS(vector<process> processes){
	cout << "\nExecuting First Come First Served Algorithm\n";
	output(processes,"arrival");	//output based on sorted processes
}

//Priority Algorithm
void Priority(vector<process> processes){
	cout << "\nExecuting Priority Algorithm\n";
	output(processes,"priority");	//output based on sorted processes
}

//Round Robin Algorithm
void RR(vector<process> processes){
	int quantum = 0;
	cout << "\nExecuting Round Robin Algorithm\n";
	cout << "Enter Time Quantum: ";
	cin >> quantum;
	int system_time = 0;
	int total_bursts = 0;
	vector<process> ready;		//processes ready to be executed on cpu
	vector<process> finished;		//processes which have been completed
	for (vector<process>::iterator it = processes.begin(); it != processes.end(); ++it){		//calculate total bursts cpu needs to run
		total_bursts += (*it).cpu_burst;
	}
	while (total_bursts > 0){		//while some processes still have bursts left
		for (vector<process>::iterator it = processes.begin(); it != processes.end(); ++it){
			int id = (*it).id;

			auto foundReady = find_if(ready.begin(), ready.end(), [&id](const process& obj) {return obj.id == id; });	//find matching id in ready queue
			auto foundDone = find_if(finished.begin(), finished.end(), [&id](const process& obj) {return obj.id == id; });	//find matching id in finished vector

			if ((*it).entry_time <= system_time && foundReady == ready.end() && foundDone == finished.end()){		//process has arrived and is not already in queue and has not been completed
				ready.push_back(*it);		//push process to ready queue
			}
		}
		if (!ready.empty()){
			int qnt = 0;
			while (ready.front().cpu_burst > 0 && qnt < quantum){			//while current processes still has execution time left
				cout << "System Time:" << system_time << " | Process:" << ready.front().id << " | Bursts Left:" << ready.front().cpu_burst << "\n";
				system_time++;
				ready.front().cpu_burst--;
				total_bursts--;
				qnt++;
			}
			if (ready.front().cpu_burst == 0){	//if process completed
				vector<process>::iterator it = processes.begin();
				for (it; it != processes.end(); ++it){		//get the corresponding process from the input vector
					if ((*it).id == ready.front().id){
						break;
					}
				}
				(*it).end_time = system_time;	//time process finished execution
				(*it).turn_around = (*it).end_time - (*it).entry_time;		//turn around time = completion time - arrival time
				(*it).wait_time = (*it).turn_around - (*it).get_start_burst(); // wait time = turnaround time - total executed bursts
				finished.push_back(*it);		//add to finished vector
				ready.erase(ready.begin());		//remove finished process from ready queue
			}
			else{		//process still not finished, move to back of ready queue
				for (vector<process>::iterator it = processes.begin(); it != processes.end(); ++it){		//check for new processes before rearranging 
					int id = (*it).id;

					auto foundReady = find_if(ready.begin(), ready.end(), [&id](const process& obj) {return obj.id == id; });	//find matching id in ready queue
					auto foundDone = find_if(finished.begin(), finished.end(), [&id](const process& obj) {return obj.id == id; });	//find matching id in finished vector

					if ((*it).entry_time <= system_time && foundReady == ready.end() && foundDone == finished.end()){		//process has arrived and is not already in queue and has not been completed
						ready.push_back(*it);		//push process to ready queue
					}
				}
				auto it2 = ready.begin();
				rotate(it2, it2 + 1, ready.end());		//move first process to back, move up next process
			}
		}
		else{
			cout << "System Time:" << system_time << " | Process:None\n";
			system_time++;
		}
	}
	cout << "Processes Completed at System Time:" << system_time << "\n";
	int total_turn_around = 0;
	int total_wait_time = 0;
	for (vector<process>::iterator it = processes.begin(); it != processes.end(); ++it){		//summing turnaround times and waiting times to be averaged
		total_turn_around += (*it).turn_around;
		total_wait_time += (*it).wait_time;
	}
	float size = (float)processes.size();
	float avg_turn_around = float(total_turn_around) / size;
	float avg_wait_time = float(total_wait_time) / size;
	float throughput = size / (float(system_time));		//throughput = processes completed per unit of time
	cout << "Average Turnaround Time:" << avg_turn_around << " | Average Wait Time:" << avg_wait_time << " | Throughput:" << throughput << "\n\n";
}

//This function reads given inputs from input.txt
vector<process> readInput(){
	cout << "Input File Address: ";
	string file;	//given file name
	cin >> file;
	vector<process> processes;		//output vector of input processes
	int id;
	int arrive;
	int priority;
	int burst;
	ifstream inputFile;
	inputFile.open(file);
	if (inputFile.is_open()){		//if file opened correctly
		while (inputFile >> id >> arrive >> priority >> burst){		//read lines of input text, split into variables
			process new_process;	//assign variables to new process object
			new_process.id = id;
			new_process.entry_time = arrive;
			new_process.priority = priority;
			new_process.set_start_burst(burst);
			processes.push_back(new_process);		//add new process to vector
		}
		inputFile.close();
		return processes;
	}
	else{
		cout << "File Not Found or Contains no Data!\n";
		return processes;
	}

}
//Main function, includes a simple menu for input and algorithm selection
int main(){
	vector<process> processes;
	bool exit = false;
	while (!exit){
		int option = 0;
		cout << "Process Scheduler Simulator\n"
			<< "Please Enter an Option:\n"
			<< "1. Input\n"
			<< "2. First Come First Served Algorithm\n"
			<< "3. Priority Algorithm\n"
			<< "4. Round Robin Algorithm\n"
			<< "5. Exit\n";
		cin >> option;
		switch (option){
		default:
			break;
		case 1:
			while (processes.empty()){		//while no new input processes, ask for input
				processes = readInput();
			}
			break;
		case 2:
			if (!processes.empty()){
				FCFS(processes);
			}
			else{
				cout << "No Processes Input!\n";
			}
			break;
		case 3:
			if (!processes.empty()){
				Priority(processes);
			}
			else{
				cout << "No Processes Input!\n";
			}
			break;
		case 4:
			if (!processes.empty()){
				RR(processes);
			}
			else{
				cout << "No Processes Input!\n";
			}
			break;
		case 5:
			exit = true;
			break;
		}
	}
	return 0;
}