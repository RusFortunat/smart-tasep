// 2d TASEP with quenched disorder in parallel hopping rates parameters
// Author: Ruslan Mukhamadiarov
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include "Environment.h"
using namespace std;

// RNG
random_device rd{};
mt19937 RNG{ rd() };
uniform_int_distribution<int> Dir{ 0, 3 }; // Choose hop direction
uniform_real_distribution<double> Rand{ 0.0, 1.0 }; // just a dice from 0 to 1

// constructor
Environment::Environment(int Lx, int Ly, int N, double av_speed, double speed_var, vector<double> &RecordRates){
	_Lx = Lx;
	_Ly = Ly;
	_N = N;
	normal_distribution<float> speed(av_speed, speed_var);

	Lattice = new int*[Lx]; // in principle, this array is not necessary, but computational purposes it could be useful
	for (auto i = 0; i < Lx; i++) {
		Lattice[i] = new int[Ly]; 
		for(auto j = 0; j < Ly; j++){
			Lattice[i][j] = 0;
		}
	}

	Particles = new double*[N]; // will store here the particles positions and their corresponding hopping rates
	for (auto i = 0; i < N; i++){
		Particles[i] = new double[2]; // [0] -- particles position; [1] -- particle hopping rate;
		Particles[i][0] = 0;
		double pick_hopping_rate = speed(RNG); // distribute particles hopping rates forward
		if(pick_hopping_rate < 0) pick_hopping_rate = 0;
		Particles[i][1] = pick_hopping_rate;
		RecordRates.push_back(pick_hopping_rate);
	}

	// randomly distribute particles over the lattice
	for (auto n = 0; n < N; n++) {
		int X = int((Lx-1)*Rand(RNG));
		int Y = int((Ly-1)*Rand(RNG));
		if (Lattice[X][Y] == 0) {
			Lattice[X][Y] = 1;
			int coord = X*Ly + Y; // decode 2d coordinate as a number
			Particles[n][0] = coord;
		}
		else {
			n--;
		}
	}
}

// deconstructor
Environment::~Environment(){
	// sanity check
	int counted_n = 0;
	for(auto i = 0; i < _Lx; i++){
		for(auto j = 0; j < _Ly; j++){
			if(Lattice[i][j] == 1) counted_n++;
		}
	}
	cout << "number of particles in the system: begin " << _N << "; end " << counted_n << endl;

	// and now delete all this shit 
	for(auto i = 0; i < _Lx; i++){
		delete Lattice[i];
	}
	for(auto n = 0; n < _N; n++){
		delete Particles[n];
	}
	delete[] Lattice;
	delete[] Particles;
}

void Environment::update(int &par_hops, int &perp_hops, vector<vector<string>> &RecordPositions){	
	// record positions
	vector<string> row;
	for(auto n = 0; n < _N; n++){
		string element = to_string(Particles[n][0]);
		row.push_back(element);
		//row.push_back(',');
	}
	RecordPositions.push_back(row);
	// update them
	for (int moveAttempt = 0; moveAttempt < _N; moveAttempt++) {
		int pick_particle = int((_N-1)*Rand(RNG)); // Picks the random particle
		int X = int(Particles[pick_particle][0]) / _Ly; // decode particle's position 
		int Y = int(Particles[pick_particle][0]) - X*_Ly; 
		// Simple implementation of Periodic boundary conditions
		int xPrev = X == 0 ? _Lx - 1 : X - 1;
		int xNext = X == _Lx -1 ? 0 : X + 1;
		int yPrev = Y == 0 ? _Ly - 1 : Y - 1;
		int yNext = Y == _Ly - 1 ? 0 : Y + 1;
		// 1) pick direction, 2) check if the next lattice site occupied, 3) attempt to hop
		int dice = Dir(RNG); 
		if (dice == 0 || dice == 1) { // hop forward
			if(Lattice[X][yNext] == 0){ // wouldn't care normally, but here particles are not identical
				double another_dice = Rand(RNG);
				if(another_dice < Particles[pick_particle][1]){
					int temp = Lattice[X][Y];
					Lattice[X][Y] = Lattice[X][yNext];
					Lattice[X][yNext] = temp;
					Particles[pick_particle][0] = X*_Ly + yNext;
					par_hops++;
				}
			}
		}
		if (dice == 2) {// hop up
			if(Lattice[xPrev][Y] == 0){
				int temp = Lattice[X][Y];
				Lattice[X][Y] = Lattice[xPrev][Y];
				Lattice[xPrev][Y] = temp;
				Particles[pick_particle][0] = xPrev*_Ly + Y;
				perp_hops++;
			}
		}
		if (dice == 3) {// hop down	
			if(Lattice[xNext][Y] == 0){
				int temp = Lattice[X][Y];
				Lattice[X][Y] = Lattice[xNext][Y];
				Lattice[xNext][Y] = temp;
				Particles[pick_particle][0] = xNext*_Ly + Y;
				perp_hops--;
			}
		}
	}
}

