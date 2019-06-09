/*
 * Twiddle.cpp
 *
 *  Created on: May 23, 2019
 *      Author: nora
 */

#include "Twiddle.h"

#include <iostream>
#include <string>


using std::string;


Twiddle::Twiddle(double kp, double ki, double kd, double delta, int move_count_per_update) 
{
	run = 0;
	index = 0;
	p.push_back(kp);
	p.push_back(ki);
	p.push_back(kd);
	dp.push_back(delta);
	dp.push_back(delta);
	dp.push_back(delta);
	sum_err = 0;
	moveCountPerUpdate = move_count_per_update;
	update_count = 0;
	best_error = 99999999.0;
	isUpdated = true;
	tolerance = delta * 0.02;
}

Twiddle::~Twiddle() 
{
	// TODO Auto-generated destructor stub
}

bool Twiddle::is_done() {
	return dp[0] + dp[1] + dp[2] <= tolerance;
}

bool Twiddle::is_updated()
{
	if(isUpdated == true){
		isUpdated = false;
		return true;
	}
	return false;
}

void Twiddle::update( double error ) 
{

	if(is_done()){
		std::cout << "****************" << std::endl;
		std::cout << "Tuning finished " << ", kp= " << p[0] << ", ki= " << p[1] << ", kd= " << p[2] <<std::endl;
		std::cout << "best err=" << best_error << std::endl;
		return;
	}else{
		sum_err += error*error;
		update_count ++;		
	}
	if(update_count < moveCountPerUpdate){
		return;
	}else{
		sum_err = sum_err/moveCountPerUpdate;
		if(sum_err < 0.01){
			isUpdated = false;
			sum_err = 0;
			update_count = 0;
			return;
		}
		isUpdated = true;
		update_count = 0;
		return; //not update, if want to update, should delete it.
		
		std::cout << std::endl << "****************" << std::endl;
		std::cout << " average_err " << sum_err << std::endl;
	}
	
	if (run == 0) {
		if ( sum_err < best_error ) {
			std::cout << "Error improved from " << best_error << " to " << sum_err << " "; debug_info();
			best_error = sum_err;
			dp[index] *= 1.15;
			// setup for next run
			index = (index+1)%3;
			p[index] += dp[index];
		} else {
			p[index] -= 2 * dp[index];
			run = 1;
		}
	} else {
		if ( sum_err < best_error ) {
			std::cout << "Error improved: "; debug_info();
			best_error = sum_err;
			dp[index] *= 1.15;
		} else {
			p[index] += dp[index];
			dp[index] *= 0.6;
		}
		run = 0;
		index = (index+1)%3;
	}
	sum_err = 0;
}

double Twiddle::get_kp(void)
{
	return p[0];
}
double Twiddle::get_ki(void)
{
	return p[1];
}
double Twiddle::get_kd(void)
{
	return p[2];
}


void Twiddle::debug_info() 
{
	std::cout << "p: " << p[0] << " " << p[1] << " " << p[2];
	std::cout << " dp: " << dp[0] << " " << dp[1] << " " << dp[2] << "\n";
}
