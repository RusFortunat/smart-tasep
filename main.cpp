// 2d TASEP with quenched disorder in parallel hopping rates parameters
// Author: Ruslan Mukhamadiarov
#include <iostream>
#include <vector>
#include <string>
#include "Environment.h"
using namespace std;

// some functions will be here, for plotting & movies for instance


int main() {

	// parameters
	int totalMCS = 10; // Total number of Monte Carlo steps per single run
	//int runsNumber = 1;
	int Lx = 20; // system's length
	int Ly = 10;  // system's width
	double density = 0.5; // particle density
	int N = int(Lx * Ly * density); // number of particles
	double av_speed = 0.5;
	double speed_var = 0.2;
	
	vector<double> RecordRates;  // this will be an awway that will store info of particles hopping distributions
	vector<vector<string>> RecordPositions;  // and this one will repord their positions

	Environment myEnv(Lx, Ly, N, av_speed, speed_var, RecordRates); // initialize the class anew for every run

	double J_par = 0.0; //double J_perp = 0.0;
	for(auto t = 0; t < totalMCS; t++){
		int parallel_hops = 0; int perp_hops = 0;
		myEnv.update(parallel_hops, perp_hops, RecordPositions);
		if(t > 0.9*totalMCS){ // try ingoring the transient regime (I assume here that the system has reached the steady state)
			J_par += 1.0*parallel_hops / (1.0*N);
			//J_perp += 1.0*perp_hops / (1.0*N*runsNumber);
		}
	}
	
	// output data
	cout<< "Hopping Rates " << RecordRates[0] << endl;
	cout << "Positions " << RecordPositions[0][0] << endl;

	cout << "Parallel current in the system " << J_par / (0.1*totalMCS) << endl;
	//cout << "Perpendicular current in the system " << J_perp / (0.1*totalMCS) << endl;

	return 0;
}
