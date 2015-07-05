#include <iostream>
#include <cstdlib>

using namespace std;

struct job{ //Structure for the lists of jobs we'll be using;
	int M1 = -1; //Execution time on any machine belonging to the first centre M1;
	int M2 = -1; //Execution time on any machine belonging to the second centre M2;
	job* next = NULL; //Points to the next job in the list;
};
int generateRandomSchedule(job* job_head){ //This function randomly generates a list of jobs, the length is provided by the user.
	int nbJobs = 0;
	int max_time = 40; //Longest possible time for a job (makespan units, seconds most likely)
	cout << "How many jobs would you like to generate? : " << endl; //Seizes the required amount of jobs 
	cin >> nbJobs;
	job* current_job = job_head;
	for (int j = 0; j < nbJobs; j++){ //Loop creates jobs and assigns random times 
		current_job->M1 = rand() % max_time;
		current_job->M2 = rand() % max_time;
		current_job->next = new job;
		current_job = current_job->next;
	}
	current_job = job_head;
	return nbJobs;
}
int findMinimum(int* table, int size){ //This function returns the position of the minimal element of the table. In the application's context, this corresponds to the machine that finishes it's job first.
	int min = table[0];
	int pos = 0;
	for (int i = 0; i < size; i++){
		if (table[i] < min){
			min = table[0];
			pos = i;
		}
	}
	return pos;
}
int findMaximum(int* table, int size){ //Function returns the minimum element in an array
	int max = table[0];
	int pos = 0;
	for (int i = 0; i < size; i++){
		if (table[i] > max){
			max = table[0];
			pos = i;
		}
	}
	return max;
}
int makeSpan(job* schedule, int nbMachines1, int nbMachines2){ //Function to calculate the makespan of the current schedule, we give it the list of jobs in the scheduled order and two integer values, corresponding to the # of processing cores per machine centre.

	job* current; //Current position in my list of jobs taken as a parameter (schedule);
	int pos_c1, pos_c2;
	//Create integer variables for each machine
	int* centre1 = new int[nbMachines1];
	for (int i = 0; i < nbMachines1; i++){
		centre1[i] = 0; //Initialise every machine to time=0;
	}
	int* centre2 = new int[nbMachines2];
	for (int i = 0; i < nbMachines1; i++){
		centre2[i] = 0; //Initialise every machine to time=0;
	}
	current = schedule;
	//We now have two tables of integers that correspond to the current time of each processor as they execute jobs
	while (current != NULL){
		pos_c1 = findMinimum(centre1, nbMachines1); //Find the position of the processor used in the first centre.
		pos_c2 = findMinimum(centre2, nbMachines2); //Find the position of the processor used in the second centre.
		centre1[pos_c1] = centre1[pos_c1] + current->M1;
		if (centre1[pos_c1] <= centre2[pos_c2]){ //We check the maximum between the time on the first processing centre and the second.
			centre2[pos_c2] = centre2[pos_c2] + current->M2; //When the maximum is on Centre 2, use the time on centre2[pos_c2] for makespan;
		}
		else{
			centre2[pos_c2] = centre2[pos_c1] + current->M2; //When the maximum is on Centre 1, use the time on centre1[pos_c1] for makespan;
		}
		current = current->next;
	}
	return findMaximum(centre2, nbMachines2);
}
int inputJobs(job* job_head){
	int choice = 0;
	cout << "Press 1 to randomly generate a set of jobs" << endl << "Press 2 to manually enter a set of jobs" << endl;  //The user can decide to have a set of jobs generated for them or to enter them manually.
	cin >> choice;
	if (choice == 1){
		return generateRandomSchedule(job_head);
	}
	int nbJobs = 1;
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
		nbJobs++;
	}
	return nbJobs;
}
job* copySchedule(job* job_head){
	//We start by copying the schedule, we do this so that we can return a modified value
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
job* modifySchedule(job* job_head, int nbJobs){ //The modify schedule function uses a random number to swap two elements of the list.
	job* cur = job_head;
	job* neighbour = copySchedule(job_head); //We copy the schedule and allocate it to our neighbour pointer. neighbour being the 
	job* temporary = new job;                        //a slightly modified version of the original

	int pos_1 = rand() % nbJobs; //Both of these integers are determined by a random number generator;
	int pos_2 = rand() % nbJobs;
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
bool acceptDecision(float delta_temp, float temp){
	float random = rand(); //Generates a number between 1 and 10000
	random = random / 100000; //Converts number to decimal
	if (exp(delta_temp / temp) > random){  //If the expression e ^ delta_t / t is bigger than a random number between 0 and 1, accept the configuration.
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
	float temperature = 100;	//The current temperature of the system
	float temp_min = 1;	//The minimum temperature of the system.
	int nbJobs = inputJobs(job_head);	//The number of jobs inputed.


	int new_cost = 0; //The cost of the current schedule.


	int nbMachines1; //Number of machines in the first centre;
	int nbMachines2; //Number of machines in the second centre;
	cout << "Please input the number of machines on the first centre (M1)";
	cin >> nbMachines1;
	cout << "Please input the number of machines on the second centre (M2)";
	cin >> nbMachines2;
	int old_cost = makeSpan(job_head, nbMachines1, nbMachines2);

	while (temperature > temp_min){
		for (int i = 0; i < 1000; i++){
			neighbour = modifySchedule(job_head, nbJobs);
			new_cost = makeSpan(neighbour, nbMachines1, nbMachines2);
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
		cout << "Temperature = : " << temperature << endl;
	}

	cout << "Schedule : " << endl;
	job* output_current = job_head;
	while (output_current->next != NULL){
		cout << "M1 : " << output_current->M1 << " M2 : " << output_current->M2 << endl;
		output_current = output_current->next;
	}
	cout << "Optimal makespan : " << old_cost << endl;
	system("pause");
}