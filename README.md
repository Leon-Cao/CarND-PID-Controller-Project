# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

## Reflection
### P,I,D component of the PID algorithm
 * After did some tuning, I got the below understanding on P,I,D of PID algorithm
   * P, it directly affects CTE and its effect is the middle one among P,I,D. So, it could set to a less value like < 0.5. otherwise, otherwise it correct the error too much and bring a large overshoot. 
   * I, its effect for error correction is very big. Espically on the start phase (I means accelerating speed from 0 to target value). On running phase it can be set to 0.02, but in start phase, 0.02 may cause car run out of road. So, I set "I" to 0 if speed less than 20MPH, and set it to 0.02 when speed larger than 20MPH.
   * D, its effect for error correction is smallest among those 3 parameters. So, it can be set a 'big' value comparing with other parameters. So, Initial it as 3.
 
### How to chose the final hyperparameters
  * I use twiddle to tune the PID coefficients. Successed on tuning PID, but not so success on tuning throttle. 
  * Tuning PID coefficients. 
    * First I set throttle to a fix value. I started at throttle equaled to 0.2， after I got a better PID value. I tried throttle equaled to 0.3 and 0.4. Finally, I used 0.4.
    * I use twiddle algorithm to tune PID coefficients. First I use p = \[0,0,0\] and dp = \[1,1,1\], but I found it did not work. So, I manually tuned p =\[0.3, 0, 3\]. And then let twiddle to tune a better value. I set dp = \[0.01, 0.01, 0.01\] and executed twiddle once when got 300 CTE value. 
    * Exist twiddle logic. When average CTE less than 0.01, or, when sum of dp less than 0.0002. After running a long time, both threshold were not touched. Finally, I manuel choiced a set of parameters for PID.
  * Throttle tunning. It is not successful, but let me describe what I tried.
    * Target throttle, I set target throttle to 0.4 which means the target speed larger than 40MPH. And the throttle = target_throttle - throttle_correction. 
    * CTE, CTE should use absolution value. CTE describe the error rate of the middle of road. The error deviation for left and right are same for throttle. If the CTE is big, then should break and let car can correct the error in time and keep car in road.
    * Initial value of throttle_p. I did not find good initial PID value for throttle. Due to tune steer's PID take me a lot of time. I have no time to tune PID of throttle. Sorry for that.
    
### What can be improved
  * For Twiddle
    * Now, I think execute twiddle when got 300 CTE value is not good enough, due to 300 CTE point means the road is short, I should set it to a bigger one which can include straight road and curvve road. Then the tuned PID parameter will include two type of road information.
    * Should initial dp_p, dp_i and dp_d to different value due to the effect of kp, ki and kd are different. Something like dp = \[0.1, 0.002, 0.5\].
    * Run out of road. When I try to tune PID parameters, the car always run out of road and stop at some place. It stop the twiddle algorithm. So, I should limited the error correction and keep car in road and let twiddle algorithm can keep working to get a better value set.
    

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

Fellow students have put together a guide to Windows set-up for the project [here](https://s3-us-west-1.amazonaws.com/udacity-selfdrivingcar/files/Kidnapped_Vehicle_Windows_Setup.pdf) if the environment you have set up for the Sensor Fusion projects does not work for this project. There's also an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3).

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).

