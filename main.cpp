// 2d TASEP with quenched disorder in parallel hopping rates parameters
// Author: Ruslan Mukhamadiarov
#include <iostream>
#include "Environment.h"
using namespace std;

// some functions will be here, for plotting & movies for instance


int main() {

	// parameters
	int totalMCS = 10000; // Total number of Monte Carlo steps per single run
	int runsNumber = 1;
	int Lx = 100; // system's length
	int Ly = 50;  // system's width
	double density = 0.5; // particle density
	int N = int(Lx * Ly * density); // number of particles
	double av_speed = 0.5;
	double speed_var = 0.2;

	// main loop
	double J_par = 0.0; //double J_perp = 0.0;
	for (int run = 0; run < runsNumber; run++) {

		Environment myEnv(Lx, Ly, N, av_speed, speed_var); // initialize the class anew for every run

		double collect_j_par = 0.0;
		for(auto t = 0; t < totalMCS; t++){
			int parallel_hops = 0; int perp_hops = 0;
			myEnv.update(parallel_hops, perp_hops);
			if(t > 0.9*totalMCS){ // try ingoring the transient regime (I assume here that the system has reached the steady state)
				collect_j_par += 1.0*parallel_hops / (1.0*N*runsNumber);
				//J_perp += 1.0*perp_hops / (1.0*N*runsNumber);
			}
		}
	}

	
	// output data
	cout << "Parallel current in the system " << J_par / (0.1*totalMCS) << endl;
	//cout << "Perpendicular current in the system " << J_perp / (0.1*totalMCS) << endl;

	return 0;
}
