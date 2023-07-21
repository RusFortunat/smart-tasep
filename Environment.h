// 2d TASEP with quenched disorder in parallel hopping rates parameters
// Author: Ruslan Mukhamadiarov
#pragma once
#include <iostream>
using namespace std;

class Environment{
	public:
		Environment(int Lx, int Ly, int N, double av_speed, double speed_var); 	// constructor
		~Environment(); // destructor

		void update(int &par_hops, int &perp_hops);

	private:
		int _Lx;
		int _Ly;
		int _N;
		int** Lattice;
		double** Particles;
};