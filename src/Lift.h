/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Lift.h
 * Author: Kirill Scherba <kirill@scherba.ru>
 *
 * Created on December 28, 2017, 11:07 PM
 */

#pragma once

#include <iostream>
#include <cstdlib>
#include <vector>
#include <thread>
#include <chrono>
#include <set>

class Lift {

private:

  // Lift parameters
  int floors_number = 5; // Number of floors (5-20)
  int floor_height = 3; // One floor height in meters
  int lift_speed = 1; // Speed of lift in meters/sec
  int opening_doors_time = 3; // Opening and closing time in sec

  // Lift state
  float position = 0.00;
  bool moves = false;
  int direction = NONE;
  bool doors_opened = false;

  // Buttons queue
  std::set<int> iQueue;
  std::set<int> oQueue;

  // Lift running flag
  bool running = true;

  // Constants
  const int BEFORE_OPEN_DOORS_TIME = 1000; // Timeout before open and after close doors
  const int SLEEP_TIME = 1000; // Lift thread sleep timeout
  enum direction { NONE, UP, DOWN }; // Lift direction


public:

  //  Lift(int number_of_floors, int height_of_one_floor = 3,
  //       int speed_of_the_elevator = 1,  int time_opening_doors = 3) :
  //       floors_number(number_of_floors),
  //       floor_height(height_of_one_floor),
  //       elevator_speed(speed_of_the_elevator),
  //       opening_doors_time(time_opening_doors) {
  //
  //  }

  /**
   * Lift Class constructor
   * @param arguments Vector with lift parameters
   */
  Lift(const std::vector<std::string> &arguments) {

    // Parse vector
    for (size_t i = 0; i < arguments.size(); i++) {
      switch (i) {
        case 0: floors_number = std::atoi(arguments[i].c_str());
          break;
        case 1: floor_height = std::atoi(arguments[i].c_str());
          break;
        case 2: lift_speed = std::atoi(arguments[i].c_str());
          break;
        case 3: opening_doors_time = std::atoi(arguments[i].c_str());
          break;
      }
    }

    showArguments();
  }

  /**
   * Copy constructor (don't used)
   * @param orig
   */
  Lift(const Lift& orig) {
  }

  /**
   * Lift class destructor
   */
  virtual ~Lift() {
  }

public:

  /**
   * Start lift thread and get console commands
   */
  void start() {
    std::thread t([this]() { lift(); }); // Lift thread
    liftButtons(); // User Input (lift buttons)
    t.join(); // Wait thread finish
  }

private:

  /**
   * Lift thread function
   */
  void lift() {

    const int sleep_time = SLEEP_TIME; // time to sleep in milliseconds
    std::cout << "\rLift started...\n\n";
    prompt();

//    // Test: lift going up
//    position = 0.00;
//    moves = true;
//    direction = UP;
//
//    // Test: lift going down
//    position = 15.00;
//    moves = true;
//    direction = DOWN;

    getFloor();

    // Lift life cycle
    while(running) {

      auto start = std::chrono::high_resolution_clock::now();
      delay (sleep_time);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> elapsed = end-start;
      //std::cout << "\x1b[A" << "\r[debug]Waited " << elapsed.count() << " ms\n";
      //std::cout << "\r[debug]Waited " << elapsed.count() << " ms\n";
      //prompt();

      // Check buttons queues
      if (iQueue.size()) {

      }

      if (oQueue.size()) {

      }

      // when the lift moves
      if(moves) {
        // Check lift position in meters
        float way = lift_speed * (elapsed.count() / 1000.0);
        position += direction == UP ? way : -way;
        //std::cout << "\rPosition in meters: " << position << "\n";

        // Check floor
        if (!((int)position % floor_height)) {
          int floor = getFloor(); // Calculate and show floor

          // Stop moving
          if(direction == UP and floor == floors_number) stopped(floor);
          if(direction == DOWN and floor == 1) stopped(floor);
        }
      }
    }
    std::cout << "\nLift stopped\n\n";
  }

  /**
   * Calculate current floor
   */
  int getFloor(bool show_message = true) {
     int floor = ((int)position / floor_height) + 1;
      if(show_message) std::cout << "\rFloor: " << floor << "\n";
     return floor;
  };
  
  /**
   * When lift stopped
   *
   * @param floor
   */
  void stopped(int floor) {
    moves = false;
    std::cout << "Lift is stopped at " << floor << " floor\n";
    doorsOpen();

    // Remove this floor buttons from queues
    auto it = iQueue.find(floor);
    if(it != iQueue.end()) iQueue.erase(it);
    
    it = oQueue.find(floor);
    if(it != oQueue.end()) oQueue.erase(it);

    // Get new job
    whatToDo(floor);
  }

  /**
   * Doors open and then close after doors opening timeout
   */
  void doorsOpen() {
    delay (BEFORE_OPEN_DOORS_TIME);
    std::cout << "Doors is opened...\n";
    doors_opened = true;
    delay (opening_doors_time*1000);
    doors_opened = false;
    std::cout << "Doors is closed...\n";
    delay (BEFORE_OPEN_DOORS_TIME);
  }

  /**
   * What done after stop or on the floor
   *
   * @param Current floor
   */
  void whatToDo(int floor) {
    
    std::cout << "What to do at " << floor << " floor? :-)\n";

    // When moves on the floor
    if(moves) {
      // ...
    }

    // After doors is closed (or when lift was stopped at any floor and now button is pressed)
    else {
      // Check internal buttons first
      if(iQueue.size()) {
        // Find floor upper than this and continue our way if so
        if(direction == UP || direction == NONE) {
          // If we have internal buttons upper than this floor
          auto it = iQueue.upper_bound(floor);
          if(it != iQueue.end()) {
            direction = UP;
            moves = true;
            std::cout << "Going Up...\n";
          }
          // If we have internal buttons lower than this floor
          else { //if(iQueue.size()) {
            direction = DOWN;
            moves = true;
            std::cout << "Going Down...\n";
          }
        }
        // Find floor lower than this and continue our way if so
        else if(direction == DOWN) {
          // If we have internal buttons upper than this floor
          auto it = iQueue.lower_bound(floor);
          if(it != iQueue.begin()) {
            direction = DOWN;
            moves = true;
            std::cout << "Going Down...\n";
          }
          // If we have internal buttons upper than this floor
          else { //if(iQueue.size()) {
            direction = UP;
            moves = true;
            std::cout << "Going Up...\n";
          }
        }
      }
      // Check outside buttons
      else if(oQueue.size()) {
        // \TODO Check outside buttons
      }

    }
  }

  /**
   * Wait floor buttons input
   */
  void liftButtons() {

    /**
     * Input buttons help message
     */
    auto help = []() {
      std::cout << "\nHelp:\n\nPress lift buttons.\n" <<
        "Button should begin from letter: 'i' - internal(in lift) or 'o' - outside(in floor).\n" <<
        "F.e.: i1 - button 1 pressed inside lift, o2 - button pressed at second floor.\n" <<
        "Press 'q' to exit this application\n" <<
        "\n";
    };

    help(); // Show help

    // Process users input (buttons)
    while (running) {

      // Show prompt and get input
      prompt();
      std::string button;
      std::cin >> button;

      // If help request
      if (button == "?" || button == "help") help();

      // If quit request
      else if (button == "q" || button == "quit") running = false;

      // If wrong input
      else if (button[0] != 'i' && button[0] != 'o') {
        std::cout << "Wrong button. See Help: \n";
        help();
      }

      // Check buttons
      else {
        char type = button[0];
        int value = std::atoi(button.substr(1).c_str());

        // Wrong input value
        if (value <= 0) {
          std::cout << "Wrong button pressed. See Help: \n";
          help();
        }

        // Too hight floor
        else if (value > floors_number) {
          std::cout << "Too hight! :-) We have only " << floors_number << " floors\n";
        }

        // Valid button pressed
        else {
          if (type == 'i') {
            std::cout << "Button " << value << " pressed inside the lift...\n";
            iQueue.insert(value); // Add button to the Internal Buttons Set
            if(!moves) whatToDo(getFloor(false));
          }
          else {
            std::cout << "Button pressed at the " << value << " floor...\n";
            oQueue.insert(value); // Add button to the Outside Buttons Set
            if(!moves) whatToDo(getFloor(false));
          }
        }
      }
    }
  }

  /**
   * Show prompt
   */
  void prompt() {
    std::cout << "lift > " << std::flush;;
  };

  /**
   * Show Lift class parameters
   *
   * @return
   */
  void showArguments() {
    std::cout << "\n" <<
      "Lift parameters: " << "\n" <<
      "\n" <<
      "  Number of floors: " << floors_number << "\n" <<
      "  Height of one floor (m): " << floor_height << "\n" <<
      "  Speed of the elevator (m/sec): " << lift_speed << "\n" <<
      "  Time of opening doors (sec): " << opening_doors_time << "\n"
      << "\n";
  }

  /**
   * Delay (just delay...)
   * @param sleep_time Time of delay in milliseconds
   */
  void delay (int sleep_time) {
    std::this_thread::sleep_for((std::chrono::milliseconds) sleep_time);
  }
};
