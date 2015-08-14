#include <iostream>
#include <cstdlib>

using namespace std;



struct job{ //Structure for the lists of jobs we'll be using;
	int M1 = -1; //Execution time on any machine belonging to the first centre M1;
	int M2 = -1; //Execution time on any machine belonging to the second centre M2;
	job* next = NULL; //Points to the next job in the list;
};
struct schedule{
	job* sched_head_job = NULL; //The head of this schedule
	int makespan = -1; //The fitness of the schedule
	schedule* next = NULL;
};
void checkIntegrity(schedule* population){
	schedule*current = population;
	int current_pos = 0;
	if (!current->sched_head_job){ cout << " ELEMENT " << current_pos << endl; }
	else{ current_pos++; current = current->next; }
}
int generateRandomSchedule(job* job_head){ //This function randomly generates a list of jobs, the length is provided by the user.
	int nb_jobs = 0;
	int max_time = 40; //Longest possible time for a job (makespan units, seconds most likely)
	cout << "How many jobs would you like to generate? : " << endl; //Seizes the required amount of jobs 
	cin >> nb_jobs;
	job* current_job = job_head;
	for (int j = 0; j < nb_jobs; j++){ //Loop creates jobs and assigns random times 
		current_job->M1 = rand() % max_time + 1;
		current_job->M2 = rand() % max_time + 1;
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
schedule* findMinimum(schedule* schedule_head){
	schedule* current = schedule_head;
	schedule* lowest = schedule_head;
	int fitness = current->makespan;
	while (current->next != NULL){
		if (current->makespan < fitness){
			lowest = current;
			fitness = current->makespan;
		}
		current = current->next;
	}
	return lowest;
}
int findMaximum(int* table, int size){ //Function returns the minimum element in an array
	int max = table[0];
	int pos = 0;
	for (int i = 0; i < size; i++){
		if (table[i] > max){
			max = table[i];
		}
	}
	return max;
}
schedule* findMaximum(schedule* schedule_head){
	schedule* current = schedule_head;
	schedule* highest = schedule_head;
	int fitness = current->makespan;
	while (current->next != NULL){
		if (current->makespan > fitness){
			highest = current;
			fitness = current->makespan;
		}
		current = current->next;
	}
	return highest;
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
	//We now have two tables of integers that correspond to the current time of each processor as they execute jobs
	while (current->next->M1 > 0){
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
	return findMaximum(centre2, nb_machines2);
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
	job* mod_head = new job;
	job* mod_current = mod_head;
	job* current = job_head;
	bool run = true;

	while (run){ //I'm using a boolean to moderate the loop because I will be checking if the current pointer is NULL inside the loop itself, I don't want to be checking it twice.
		if (!current){ //If the current pointer for the schedule is NULL, terminate the loop.
			run = false;
		}
		else{ //Else create a job
			mod_current->M1 = current->M1;
			mod_current->M2 = current->M2;
			current = current->next;
			mod_current->next = new job;
			mod_current = mod_current->next;
		}
	}
	return mod_head;
}
void modifySchedule(job* job_head, int nb_jobs){ //The modify schedule function uses a random number to swap two elements of the list.
	job* current = job_head;
	job* temporary = new job;

	int pos_1 = rand() % nb_jobs; //Both of these integers are determined by a random number generator;
	int pos_2 = rand() % nb_jobs;

	int current_pos = 0;
	job* job_pos_1 = current;
	job* job_pos_2 = current;
	bool loop = true;
	while (current_pos < pos_1 && loop){ //Point to both elements we're going to swap;
		current_pos++;
		if (job_pos_1->next == NULL){ loop = false; }
		else{ job_pos_1 = job_pos_1->next; } //We don't need to check to make sure that this pointed could be undefined because the integer pos_1 will always be smaller than the number of jobs in the schedule;
	}
	current_pos = 0;
	loop = true;
	while (current_pos < pos_2 && loop){
		current_pos++;
		if (job_pos_2->next == NULL){ loop = false; }
		else{ job_pos_2 = job_pos_2->next; }// read above ^ 
	}
	temporary->M1 = job_pos_1->M1; //Swap both elements;
	temporary->M2 = job_pos_1->M2;
	job_pos_1->M1 = job_pos_2->M1;
	job_pos_1->M2 = job_pos_2->M2;
	job_pos_2->M1 = temporary->M1;
	job_pos_2->M2 = temporary->M2;


	delete temporary;
	return;
}
void deleteList(job* job_head){
	while (job_head != NULL){ //Loop goes through every element of the list and deletes it.
		job* delete_job = job_head;
		job_head = job_head->next;
		delete delete_job;
	}
}
schedule* generateGenePool(job* job_head, int size_pool, int nb_jobs, int* centre1, int* centre2, int nb_machines1, int nb_machines2){
	schedule* gene_pool = new schedule;
	gene_pool->sched_head_job = new job;
	schedule* current = gene_pool;
	for (int i = 0; i < size_pool; i++){
		job* original = copySchedule(job_head);
		for (int i = 0; i <nb_jobs; i++){
			modifySchedule(original, nb_jobs);
		}
		current->sched_head_job = original;
		current->makespan = makeSpan(original, centre1, centre2, nb_machines1, nb_machines2);
		current->next = new schedule;
		current = current->next;
	}
	return gene_pool;
}
void mutateGene(job* job_head, int nb_jobs){
	for (int i = 0; i < nb_jobs; i++){
		modifySchedule(job_head, nb_jobs);
	}
}
void calculateFitness(schedule* population_head, int* centre1, int* centre2, int nb_machines1, int nb_machines2){
	schedule* current = population_head;
	while (current->next != NULL){
		current->makespan = makeSpan(current->sched_head_job, centre1, centre2, nb_machines1, nb_machines2);
		current = current->next;
	}
	return;
}
void selectGenes(schedule* population, int size_pool, int nb_jobs){ //This function selects the genes that survive and replaces the weakest strands with better performing ones
	int percent = size_pool / 5; //We trim off the bottom 5% of schedules and replace them with our best performing strand;
	schedule* lowest = findMinimum(population);
	schedule* highest = NULL;
	checkIntegrity(population);
	for (int i = 0; i < percent; i++){ //Inside this loop we will kill the lowest performing gene and copy the strongest to it, then we mutate the copies of our strongest gene
		highest = findMaximum(population);

		deleteList(highest->sched_head_job);

		if (lowest->sched_head_job){ highest->sched_head_job = copySchedule(lowest->sched_head_job); }
		else{ system("pause"); }

		mutateGene(highest->sched_head_job, nb_jobs);

	}
	return;
}
void crossGenes(schedule* population, int size_pool, int nb_jobs){

	for (int i = 0; i < (size_pool - size_pool / 11); i++){ //This loop finds the two best performing strands (lowest makespan) and crosses them, for 90% of the population
		schedule* min_first = findMaximum(population);
		min_first->makespan = findMinimum(population)->makespan; //Allocate a fake makespan value to remove this schedule from being the minimum of the list;
		schedule* min_second = findMaximum(population);
		min_second->makespan = min_first->makespan;
		job* current_first = min_first->sched_head_job;
		job* current_second = min_second->sched_head_job;
		job* reciprocal;
		int position = rand() % nb_jobs; //Pick a random job position
		for (int j = 0; j < position; j++){ //Select the job at this position on either strand
			current_first = current_first->next;
			current_second = current_second->next;
		}
		job temporary;
		temporary.M1 = current_first->M1;
		temporary.M2 = current_first->M2;
		current_first->M1 = current_second->M1;
		current_first->M2 = current_second->M2;
		current_second->M1 = temporary.M1;
		current_second->M2 = temporary.M2;
		bool loop = true;
		reciprocal = min_second->sched_head_job;
		while ((reciprocal->M1 != current_second->M1 || reciprocal->M2 != current_second->M2) && loop){ //On the first strand, find the job corresponding to the one selected on the second strand, name reciprocal here		
			if (reciprocal->next->M1 != -1){ reciprocal = reciprocal->next; }
			else{ loop = false; }
		} 
		temporary.next = current_second->next;
		*current_second = *reciprocal;
		current_second->next = temporary.next;
		loop = true;
		temporary.M1 = current_first->M1;
		temporary.M2 = current_first->M2;
		reciprocal = min_first->sched_head_job;
		while (((reciprocal->M1 != temporary.M1) || (reciprocal->M2 != temporary.M2)) && loop){ //On the second strand, find the job coresponding to the one selected on the first strand
			if (reciprocal->next->M1 != -1){ reciprocal = reciprocal->next; }
			else{ loop = false; }
		}
		temporary = *current_second;
		current_second->M1 = reciprocal->M1;
		current_second->M2 = reciprocal->M2;
		reciprocal->M1 = temporary.M1;
		reciprocal->M2 = temporary.M2;
	}
	return;
}

int main(void){

	job* job_head = new job; //The head of our schedule, a list of jobs.
	int nb_jobs = inputJobs(job_head);	//The number of jobs inputed.
	int size_pool = 1000; //100 individus dans la population

	schedule* population = NULL;


	int nb_machines1; //Number of machines in the first centre;
	int nb_machines2; //Number of machines in the second centre;
	cout << "Please input the number of machines on the first centre (M1)";
	cin >> nb_machines1;
	cout << "Please input the number of machines on the second centre (M2)";
	cin >> nb_machines2;

	int* centre1 = new int[nb_machines1];
	int* centre2 = new int[nb_machines2];
	int original_makespan = makeSpan(job_head, centre1, centre2, nb_machines1, nb_machines2);
	job* original_sched = copySchedule(job_head);
	population = generateGenePool(job_head, size_pool, nb_jobs, centre1, centre2, nb_machines1, nb_machines2);
	for (int i = 0; i < 100; i++){
		if (i % 10 == 0){ cout << "Generation" << i << endl; }
		calculateFitness(population, centre1, centre2, nb_machines1, nb_machines2);
		selectGenes(population, size_pool, nb_jobs);
		crossGenes(population, size_pool, nb_jobs);
	}
	calculateFitness(population, centre1, centre2, nb_machines1, nb_machines2);
	job* current_orig = original_sched;
	job* current_final = findMinimum(population)->sched_head_job;
	for (int i = 0; i < nb_jobs; i++){
		cout << " M1 : " << current_orig->M1 << " M2 : " << current_orig->M2 << endl;
		current_orig = current_orig->next;
	}
	cout << " __------___----------------_____________" << endl;
	for (int i = 0; i < nb_jobs; i++){
		cout << " M1 : " << current_final->M1 << " M2 : " << current_final->M2 << endl;
		current_final = current_final->next;
	}
    cout <<"Optimal makespan : " <<findMinimum(population)->makespan << endl;

	system("pause");
}