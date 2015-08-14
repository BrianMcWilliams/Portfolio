#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

struct job{ //Structure for the lists of jobs we'll be using;
	int M1 = -1; //Execution time on any machine belonging to the first centre M1;
	int M2 = -1; //Execution time on any machine belonging to the second centre M2;
	job* next = NULL; //Points to the next job in the list;
};
int generateRandomSchedule(job* job_head){ //This function randomly generates a list of jobs, the length is provided by the user.
	int nb_jobs = 0;
	int max_time = 40; //Longest possible time for a job (makespan units, seconds most likely)
	cout << "How many jobs would you like to generate? : " << endl; //Seizes the required amount of jobs 
	cin >> nb_jobs;
	job* current_job = job_head;
	for (int j = 0; j < nb_jobs; j++){ //Loop creates jobs and assigns random times 
		current_job->M1 = rand() % max_time;
		current_job->M2 = rand() % max_time;
		current_job->next = new job;
		current_job = current_job->next;
	}
	current_job = job_head;
	return nb_jobs;
}
int findMinimum(int* table, int size){ //This function returns the position of the minimal element of the table. In the application's context, this corresponds to the machine that finishes it's job first.
	int min = table[0];
	int pos = 0;
	for (int i = 0; i < size; i++){
		if (table[i] < min){
			min = table[i];
			pos = i;
		}
	}
	return pos;
}
int findMaximum(int* table, int size){ //Function returns the position of the maximum element in an array
	int max = table[0];
	int pos = 0;
	for (int i = 0; i < size; i++){
		if (table[i] > max){
			max = table[i];
			pos = i;
		}
	}
	return pos;
}
int makeSpan(job* schedule, int* centre1, int* centre2, int nb_machines1, int nb_machines2){ //Function to calculate the makespan of the current schedule, we give it the list of jobs in the scheduled order and two integer values, corresponding to the # of processing cores per machine centre.

	job* current; //Current position in my list of jobs taken as a parameter (schedule);
	int pos_c1, pos_c2;
	//Create integer variables for each machine
	for (int i = 0; i < nb_machines1; i++){
		centre1[i] = 0; //Initialise every machine to time=0;
	}
	for (int i = 0; i < nb_machines2; i++){
		centre2[i] = 0; //Initialise every machine to time=0;
	}
	current = schedule;
	//We now have two arrays of integers that correspond to the current time of each processor as they execute jobs
	while (current->next != NULL){
		pos_c1 = findMinimum(centre1, nb_machines1); //Find the position of the processor used in the first centre.
		pos_c2 = findMinimum(centre2, nb_machines2); //Find the position of the processor used in the second centre.
		centre1[pos_c1] = centre1[pos_c1] + current->M1;
		if (centre1[pos_c1] <= centre2[pos_c2]){ //We check the maximum between the time on the first processing centre and the second.
			centre2[pos_c2] = centre2[pos_c2] + current->M2; //When the maximum is on Centre 2, use the time on centre2[pos_c2] for makespan;
		}
		else{
			centre2[pos_c2] = centre1[pos_c1] + current->M2; //When the maximum is on Centre 1, use the time on centre1[pos_c1] for makespan;
		}
		current = current->next;
	}
	int ret_value = centre2[findMaximum(centre2, nb_machines2)];
	return ret_value;
}
int inputJobs(job* job_head){
	int choice = 0;
	cout << "Press 1 to randomly generate a set of jobs" << endl << "Press 2 to manually enter a set of jobs" << endl;  //The user can decide to have a set of jobs generated for them or to enter them manually.
	cin >> choice;
	if (choice == 1){
		return generateRandomSchedule(job_head);
	}
	int nb_jobs = 1;
	int time; //Variable used for input
	job* current = job_head;
	current->next = NULL;
	bool terminate = false;
	while (!terminate){
		cout << "Please enter the execution time on machine 1 : " << endl;
		cin >> time; //Input integer value for this job's M1 execution time and allocate it to the M1 variable
		current->M1 = time;
		cout << "Please enter the execution time on machine 2 : " << endl;
		cin >> time; //Input integer value for this job's M2 execution time and allocate it to the M2 variable
		current->M2 = time;
		cout << "Press 1 to enter another job, press 2 to quit";
		cin >> time;
		if (time != 1){
			terminate = true; //If the key entered was not 1 for continue, terminate loop.
		}
		current->next = new job; //Create new job.
		current = current->next; //Shift forward current pointer
		current->next = NULL; //Set next pointer as being NULL;
		nb_jobs++;
	}
	return nb_jobs;
}
job* copySchedule(job* job_head){
	//We start by copying the schedule, we do this so that we can return 
	job* mod_head = new job;
	job* mod_current = mod_head;
	job* current = job_head;
	bool run = true;
	while (run){ //I'm using a boolean to moderate the loop because I will be checking if the current pointer is NULL inside the loop itself, I don't want to be checking it twice.
		mod_current->M1 = current->M1;
		mod_current->M2 = current->M2;
		current = current->next;
		if (current == NULL){ //If the current pointer for the schedule is NULL, terminate the loop.
			run = false;
		}
		else{
			mod_current->next = new job;
			mod_current = mod_current->next;
		}
	}
	job* cur = mod_head;
	return mod_head;
}
job* modifySchedule(job* job_head, int nb_jobs){ //The modify schedule function uses a random number to swap two elements of the list.
	job* neighbour = copySchedule(job_head); //We copy the schedule and allocate it to our neighbour pointer. neighbour being the 
	job* temporary = new job;                        //a slightly modified version of the original
	int pos_1 = rand() % nb_jobs; //Both of these integers are determined by a random number generator;
	int pos_2 = rand() % nb_jobs;
	int current_pos = 0;
	job* job_pos_1 = neighbour;
	job* job_pos_2 = neighbour;
	while (current_pos < pos_1){ //Point to both elements we're going to swap;
		current_pos++;
		job_pos_1 = job_pos_1->next; //We don't need to check to make sure that this pointed could be undefined because the integer pos_1 will always be smaller than the number of jobs in the schedule;
	}
	current_pos = 0;
	while (current_pos < pos_2){
		current_pos++;
		job_pos_2 = job_pos_2->next;// read above ^ 
	}
	temporary->M1 = job_pos_1->M1; //Swap both elements;
	temporary->M2 = job_pos_1->M2;
	job_pos_1->M1 = job_pos_2->M1;
	job_pos_1->M2 = job_pos_2->M2;
	job_pos_2->M1 = temporary->M1;
	job_pos_2->M2 = temporary->M2;

	return neighbour;
}
job* modifySchedule(job* job_head, int nb_jobs, bool wide_search){ //The modify schedule function uses a random number to swap two elements of the list.
	job* cur = job_head;
	job* neighbour = copySchedule(job_head); //We copy the schedule and allocate it to our neighbour pointer. neighbour being the 
	job* temporary = new job;                        //a slightly modified version of the original
	int nb_iterations = 50 * nb_jobs;
	for (int i = 0; i < nb_iterations; i++){
		int pos_1 = rand() % nb_jobs; //Both of these integers are determined by a random number generator;
		int pos_2 = rand() % nb_jobs;
		int current_pos = 0;
		job* job_pos_1 = neighbour;
		job* job_pos_2 = neighbour;
		while (current_pos < pos_1){ //Point to both elements we're going to swap;
			current_pos++;
			job_pos_1 = job_pos_1->next; //We don't need to check to make sure that this pointed could be undefined because the integer pos_1 will always be smaller than the number of jobs in the schedule;
		}
		current_pos = 0;
		while (current_pos < pos_2){
			current_pos++;
			job_pos_2 = job_pos_2->next;// read above ^ 
		}
		temporary->M1 = job_pos_1->M1; //Swap both elements;
		temporary->M2 = job_pos_1->M2;
		job_pos_1->M1 = job_pos_2->M1;
		job_pos_1->M2 = job_pos_2->M2;
		job_pos_2->M1 = temporary->M1;
		job_pos_2->M2 = temporary->M2;
	}
	return neighbour;
}
bool acceptDecision(float delta_temp, float temp){
	float random = rand(); //Generates a number between 1 and 10000
	random = 1 - (random / 100000); //Converts number to decimal
	if (exp(delta_temp / temp)> random){  //If the expression e ^ delta_t / t is bigger than a random number between 0 and 1, accept the configuration.

		return true;
	}
	else { //else, reject the configuration, return false.

		return false;
	}
}
void deleteList(job* job_head){
	while (job_head != NULL){ //Loop goes through every element of the list and deletes it.
		job* delete_job = job_head;
		job_head = job_head->next;
		delete delete_job;
	}
}
int main(void){

	job* job_head = new job; //The head of our schedule, a list of jobs.
	job* neighbour = NULL;
	float temp_min = 0.0005;	//The minimum temperature of the system.
	int nb_jobs = inputJobs(job_head);	//The number of jobs inputed.
	job* best_head = copySchedule(job_head);
	job* original = copySchedule(job_head);
	int new_cost = 0; //The cost of the current schedule.
	float optimisation = 0;

	int nb_machines1; //Number of machines in the first centre;
	int nb_machines2; //Number of machines in the second centre;
	cout << "Please input the number of machines on the first centre (M1)";
	cin >> nb_machines1;
	cout << "Please input the number of machines on the second centre (M2)";
	cin >> nb_machines2;
	int* centre1 = new int[nb_machines1];
	int* centre2 = new int[nb_machines2];
	int old_cost = makeSpan(job_head, centre1, centre2, nb_machines1, nb_machines2);
	cout << " original : " << old_cost;
	int lowest = old_cost;
	int original_cost = old_cost;

		float temperature = 100;

		while (temperature > temp_min){
			for (int i = 0; i < 10000; i++){
				neighbour = modifySchedule(job_head, nb_jobs);
				new_cost = makeSpan(neighbour, centre1, centre2, nb_machines1, nb_machines2);
				if (new_cost < lowest){ lowest = new_cost; deleteList(best_head); best_head = copySchedule(neighbour); }
				if (acceptDecision(old_cost - new_cost, temperature)){
					deleteList(job_head);
					job_head = neighbour;
					old_cost = new_cost;
				}
				else{
					deleteList(neighbour);
				}
				i++;
			}
			temperature = temperature * 0.85;
			cout << "new temperature : " << temperature << endl;
		}

	job* current_final = best_head;
	for (int i = 0; i < nb_jobs; i++){
		cout << " M1 : " << current_final->M1 << " M2 : " << current_final->M2 << endl;
		current_final = current_final->next;
	}

	optimisation = original_cost - old_cost;
	cout << " Optimal makespan : " << old_cost << endl;
	system("pause");
}