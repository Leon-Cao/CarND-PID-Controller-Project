#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"
#include "Twiddle.h"

// for convenience
using nlohmann::json;
using std::string;

// DEFINATION
#ifndef MOVE_COUNT_PER_TWIDDLE_UPDATE
#define MOVE_COUNT_PER_TWIDDLE_UPDATE 	300
#endif

#ifndef PARAMETER_DELTA
#define PARAMETER_DELTA 	0.01
#endif

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main() {
  uWS::Hub h;

  PID steer_pid, throttle_pid;
  /**
   * TODO: Initialize the pid variable.
   */
  double target_throttle = 0.4;
  double target_steer = 0;

  //steer_twiddle(0.143, 0.00, 2.97, 0.01, 300); throttle=0.3, ki =0.02 at running time.
  Twiddle steer_twiddle(0.143, 0.00, 2.97, PARAMETER_DELTA, MOVE_COUNT_PER_TWIDDLE_UPDATE);
  Twiddle throttle_twiddle(0.0, 0.0, 0, PARAMETER_DELTA, MOVE_COUNT_PER_TWIDDLE_UPDATE);
 
  h.onMessage([&steer_pid, &throttle_pid, &target_throttle, &target_steer, &steer_twiddle, &throttle_twiddle]
  					(uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());
          double steer_value, throttle_value;
		  double kp, ki, kd;
          /**
           * TODO: Calculate steering value here, remember the steering value is
           *   [-1, 1].
           * NOTE: Feel free to play around with the throttle and speed.
           *   Maybe use another PID controller to control the speed!
           */
           
		  steer_twiddle.update(cte);
		  if(steer_twiddle.is_updated() == true){
			  kp = steer_twiddle.get_kp();
			  //skip ki update due to it causes issue when start from 0
  			  if(speed > 20)
				  ki = 0.02; //steer_twiddle.get_ki();
			  else			  	
				  ki = 0;
			  kd = steer_twiddle.get_kd();
			  std::cout << "****Update sterr_twiddle! " << " kp= " << kp
			  	<< ", ki= " << ki << ", kd= " <<kd << std::endl;
			  steer_pid.Init(kp, ki, kd);
      	  }
          steer_pid.UpdateError(cte);
		  
		  throttle_twiddle.update(cte);  
		  if(throttle_twiddle.is_updated() == true){
				kp = throttle_twiddle.get_kp();
				ki = throttle_twiddle.get_ki();
				kd = throttle_twiddle.get_kd();
				std::cout << "****Update throttle_twiddle!" << " kp= " << kp << ", ki=" << ki 
				  << ", kd=" <<kd << std::endl;
				throttle_pid.Init(kp, ki, kd);
	  	  }
		  throttle_pid.UpdateError(abs(cte));
		  
		  
          steer_value = steer_pid.TotalError();
		  throttle_value = target_throttle - throttle_pid.TotalError();
		  
          // DEBUG
          //std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle_value;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          //std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}
