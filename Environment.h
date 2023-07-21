// 2d TASEP with quenched disorder in parallel hopping rates parameters
// Author: Ruslan Mukhamadiarov
#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Environment{
	public:
		Environment(int Lx, int Ly, int N, double av_speed, double speed_var, vector<double> &RecordRates); // constructor
		~Environment(); // destructor

		void update(int &par_hops, int &perp_hops, vector<vector<string>> &record_positions);

	private:
		int _Lx;
		int _Ly;
		int _N;
		int** Lattice;
		double** Particles;
};