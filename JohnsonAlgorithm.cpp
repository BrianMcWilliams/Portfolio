/*********Job Shop Scheduling************
*
*	Author:   Brian McWilliams
*
*	Abstract: Implementation of the Johnson algorithm for 2 machine flow-shop where each machine only has one operating centre
*
*	The algorithm logic is the following :
*	1: Find the job with the smallest processing time (on either machine)
*	2: If the smallest processing time is on M1 -> Schedule that job at the beginning
*	   Else -> Schedule that job at the END of the schedule (This case has the processing time being inferior on M2)
*	3: If there are unscheduled jobs go to step 1
*
*	   To reduce the complexity I opted to sort the table of jobs initially, preventing the need to constantly go through the job table to find the minimum, I would simply
*	   order them initially and sequentially add them to my schedule with the johnson algorithm.
*
*
*	Note: I'm taking for granted that the user will be inputting the jobs himself, I didn't think too much about input simply because the main goal here is the correct
*		  Implementation of the lagorithm and not necessarily the fluff around it. I'll be using two tables, one to contain the jobs initially and one to contain the order of the jobs after
*		  applying the algorithm
*
*****************************************/

#include <iostream>
#include <cstdlib>

using namespace std;

struct job{
	int t1; //Time to execute on the first machine
	int t2; //Time to execute on the second machine
};
int inputJobs(job* jobTable,int nbJobs){
	
	int time; //Variable used for input

	
	for (int i = 0; i < nbJobs; i++){ //For every job, input the processing time on M1, M2

		cout << "For job " << i << "please enter t1 : ";  //Ask for t1
		cin >> time;
		jobTable[i].t1 = time;
		cout << endl << "please enter t2 : " << endl; //Ask for t2
		cin >> time;
		jobTable[i].t2 = time;
	}
	cout << "here are the jobs currently entered" << endl;
	for (int i = 0; i < nbJobs; i++){ //For every job, input the processing time on M1, M2
		cout << "Job " << i << "|| t1 : " << jobTable[i].t1 << " t2 : " << jobTable[i].t2 << endl; //Ask for t2
	}
	return nbJobs;
}
int findMinimum(job* selected){ //Returns minimum value for a job (either t1 or t2 value)
	if (selected->t1 > selected->t2){
		return selected->t2;
	}
	else return selected->t1;
}
void quickSort(job* jobTable, int left, int right) { //Quicksort algorithm adapted to work with our job structure
	int i = left, j = right;
	job* leftJob; // Job type pointer for the left side 
	job* rightJob; //Job type pointer for the right side
	int min_piv; //The pivot's minimum
	int min_cur; //The currently pointed job's minimum
	job tmp; //Temporary job to allow us to switch jobs around
	job* pivot = &jobTable[(left + right) / 2]; // Points to the pivot 
	min_piv = findMinimum(pivot);
	while (i <= j) { 
		leftJob = &jobTable[i]; //Initialise leftJob
		min_cur = findMinimum(leftJob);
		while (min_cur < min_piv){ //While left jobs are smaller than the pivot value
			i++;
			leftJob = &jobTable[i]; //identify the job
			min_cur = findMinimum(leftJob);
		}
		rightJob = &jobTable[j];
		min_cur = findMinimum(rightJob);
		while (min_cur > min_piv){ //While right jobs are bigger than the pivot value
			j--;
			rightJob = &jobTable[j];
			min_cur = findMinimum(rightJob);
		}
		if (rightJob->t1 > rightJob->t2){ min_cur = rightJob->t2; } //Set the integer for the job's minimum
		else{ min_cur = leftJob->t1; }
		if (i <= j) { // Swap i and j values
			tmp = jobTable[i]; //Swap both elements
			jobTable[i] = jobTable[j];
			jobTable[j] = tmp;
			i++;
			j--;
		}
	};

	if (left < j) //Call to the left
		quickSort(jobTable, left, j);
	if (i < right) //Call to the right
		quickSort(jobTable, i, right);
}
void jobSort(job* jobTable, int length){ //Takes the table and length and calls quicksort
	quickSort(jobTable,0,length-1);
}

int main(void){

	/******INPUT*******
	* 
	* I'm going to simply have the user input the number of jobs to treat and then input the comput ation time for each of them.
	* Once again, not wasting energy on small details regarding how the jobs will be fed into the system, we're focusing on the algorithm
	*
	*******************/
	int nbJobs;
	cout << "Hello user, please enter the number of jobs to treat" << endl;
	cin >> nbJobs;
	cout << " ----------------  Thank you  ------------------ " << endl << endl;

	job* jobTable = new job[nbJobs]; //Create a table to house the jobs'

	nbJobs = inputJobs(jobTable,nbJobs);
	jobSort(jobTable,nbJobs);
	cout << "here are the jobs currently entered" << endl;
	for (int i = 0; i < nbJobs; i++){ //For every job, input the processing time on M1, M2
		cout << "Job " << i << "|| t1 : " << jobTable[i].t1 << " t2 : " << jobTable[i].t2 << endl; //Ask for t2
	}
	job* jobSchedule = new job[nbJobs]; //Create a table to set the order


	job* M2 = NULL; // Create an iterator that will start at the end of the table, ordering the operations that are shortest on t2;
	job* M1 = NULL; // Create an iterator that will start at the start of the table, ordering the operations that are shortest on t1;

	M1 = &jobSchedule[0]; // Point M1 to the beginning of the table
	M2 = &jobSchedule[nbJobs]; //Point M2 to the end of the table
	M2--;

	job* currentJob = &jobTable[0]; //Create a pointer that starts at the beginning of the job table 
	bool loop = true;
	while (loop) { // While M1 and M2 aren't pointing the same element
		if (currentJob->t1 < currentJob->t2){
			*M1 = *currentJob; M1++; if (M1 > M2){ loop = false; }
		}
		else {
			*M2 = *currentJob; M2--; if (M1 > M2){ loop = false; }
		}
		currentJob++;
	}

	for (int i = 0; i < nbJobs; i++){ //For every job, input the processing time on M1, M2
		cout << "JobSchedule " << i << "|| t1 : " << jobSchedule[i].t1 << " t2 : " << jobSchedule[i].t2 << endl; //Ask for t2
	}
	system("pause");
}