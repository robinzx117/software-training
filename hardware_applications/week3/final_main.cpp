#include <iostream>
#include <STSL/RJRobot.h>
#include <vector>
#include <array>
#include <map>
#include <algorithm>

using namespace std;

int main() {

    RJRobot robot;

    /*
     * Create a map of actions the robot knows
     */

    map<string, pair<int,int>> known_moves;

    known_moves["forward"] = {100,100};
    known_moves["backward"] = {-100,-100};
    known_moves["left"] = {100,-100};
    known_moves["right"] = {-100,100};

    /*
     * Drive in a square using these known actions
     */

    array<string,8> square_path;

    for(auto i = 0; i < square_path.size(); i++) {
        if( (i % 2) == 0 ) {
            square_path[i] = "forward";
        } else {
            square_path[i] = "left";
        }
    }

    for(const auto command : square_path) {
        auto speeds = known_moves[command];
        robot.SetMotor(MotorPort::A, speeds.first);
        robot.SetMotor(MotorPort::B, speeds.second);
        robot.Wait(250ms);
    }

    /*
     * Count the number of black squares on a strip of paper
     */

    vector<int> measurements;

    auto black_threshold = 10;

    auto grey_threshold = 40;

    // Measure light values until we see a black square
    while(measurements.empty() || measurements.back() > black_threshold) {
        // Measure square color
        measurements.push_back(robot.LightValue());

        // Move to next square
        auto action = known_moves["forward"];
        robot.SetMotor(MotorPort::A, action.first);
        robot.SetMotor(MotorPort::B, action.second);
        robot.Wait(250ms);
    }

    // Remove the black square from the list
    measurements.erase(measurements.end()-1);

    auto is_grey = bind2nd(std::less<int>{}, grey_threshold);

    auto number_of_grey_squares = count_if(measurements.begin(), measurements.end(), is_grey);

    cout << number_of_grey_squares << " grey squares detected." << endl;

    return 0;
}
