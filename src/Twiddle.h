/*
 * Twiddle.h
 *
 *  Created on: May 23, 2019
 *      Author: nora
 */

#ifndef SRC_TWIDDLE_H_
#define SRC_TWIDDLE_H_

#include <vector>

using namespace std;

class Twiddle {
public:
	Twiddle(double kp, double ki, double kd, double delta, int move_count_per_update);
	virtual ~Twiddle();

	bool is_done();

	void update( double error );

	double get_kp(void);
	double get_ki(void);
	double get_kd(void);
	bool is_updated();

	vector<double> get_p();

	void debug_info();

private:
	double tolerance;
	vector<double> p;
	vector<double> dp;
	double sum_dp;
	double sum_err;
	int update_count;
	int moveCountPerUpdate;
	int index;
	int run;
	double best_error;
	bool isUpdated;
};

#endif /* SRC_TWIDDLE_H_ */
