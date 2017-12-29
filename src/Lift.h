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

#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <thread>
#include <chrono>

#include "LiftButton.h"

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
  bool paused = false;

  // Buttons queue
  LiftButton iQueue;
  LiftButton oQueue;

  // Lift running flag
  bool running = true;
  bool started = false;

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
    std::thread t([this]() { lift(); }); // Start lift thread
    while(!started) delay(100); // Wait lift thread started
    showArguments(); // Show lift parameters
    liftButtons(); // User Input (lift buttons)
    t.join(); // Wait thread finish
  }

private:

  /**
   * Lift thread function
   */
  void lift() {

    const int sleep_time = SLEEP_TIME; // default time to sleep in milliseconds
    std::cout << "\rLift started...\n\n";
    started = true;

    // Lift life cycle
    while(running) {

      auto start = std::chrono::high_resolution_clock::now();
      delay (sleep_time);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> elapsed = end-start;
      if(paused) continue;

      // when the lift moves
      if(moves) {
        // Check lift position in meters
        float way = lift_speed * (elapsed.count() / 1000.0);
        position += direction == UP ? way : -way;

        // Check floor
        if (!((int)position % floor_height)) {
          int floor = getFloor(); // Calculate and show floor
          whatToDo(floor);
          // Stop moving at end and first floor
          // \TODO This happened at the beginning of development. May be this 
          //       code does not need more 
          //if(moves) {
          //  if(direction == UP and floor == floors_number) stopped(floor);
          //  if(direction == DOWN and floor == 1) stopped(floor);
          //}
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
     if(show_message) { 
       std::cout << "\rFloor: " << floor << "\n";
       prompt();
     }
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
    iQueue.erase(floor);
    oQueue.erase(floor);

    // Get new job
    whatToDo(floor);
  }

  /**
   * Doors open and then close after doors opening timeout
   */
  void doorsOpen() {
    delay (BEFORE_OPEN_DOORS_TIME);
    std::cout << "\rDoors opening...\n";
    doors_opened = true;
    delay (opening_doors_time*1000);
    doors_opened = false;
    std::cout << "Doors closing...\n";
    delay (BEFORE_OPEN_DOORS_TIME);
  }

  /**
   * What done after stop or on the floor
   *
   * @param Current floor
   */
  void whatToDo(int floor) {
    
    // When moves on the floor
    if(moves) {
      // Stop if floor in Queue
      if(iQueue.find(floor)) {
        iQueue.erase(floor);
        doorsOpen();
        moves = false;
        prompt();
        whatToDo(floor);
      }
    }

    // After doors is closed (or when lift was stopped at any floor and now button is pressed)
    else {
      // Check internal buttons first
      if(iQueue.size()) {
        // Open doors if this floor button pressed
        if(iQueue.find(floor)) {
          iQueue.erase(floor);
          doorsOpen();
        }
        // Find floor upper than this and continue our way if so
        else if(direction == UP || direction == NONE) {
          // If we have internal buttons upper than this floor
          if(iQueue.hasUpper(floor)) {
            direction = UP;
            std::cout << "\rGoing Up...\n";
          }
          // If we have internal buttons lower than this floor
          else { //if(iQueue.size()) {
            direction = DOWN;
            std::cout << "\rGoing Down...\n";
          }
          moves = true;
        }
        // Find floor lower than this and continue our way if so
        else if(direction == DOWN) {
          // If we have internal buttons upper than this floor
          if(iQueue.hasLower(floor)) {
            direction = DOWN;
            std::cout << "Going Down...\n";
          }
          // If we have internal buttons upper than this floor
          else { //if(iQueue.size()) {
            direction = UP;
            std::cout << "Going Up...\n";
          }
          moves = true;
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
      std::cout << "\nHelp:\n\n" <<
        "Type a command and press Enter button.\n" <<
        "\n" <<
        "Commands:\n" << 
        "\n" <<
        "'i'number or 'o'number - lift buttons:\n" <<
        "\n" <<
        "  button should begin with letter: 'i' - internal(in lift) or 'o' - outside(in floor)\n" <<
        "  f.e.: i1 - button 1 pressed inside lift, o2 - button pressed at second floor,\n" <<
        "  it is possible to enter comma delimited array of buttons: i2,i4,o4,o5\n" <<
        "  or enter buttons in real time\n" <<
        "  or enter 'p' command to pause lift and enter buttons offline\n" <<
        "\n" <<
        "'f' - show floor\n" <<
        "'p' - pause the lift and press buttons\n" <<
        "'i' - show list of internal buttons pressed\n" <<
        "'o' - show list of outside buttons pressed\n" <<
        "'?' - show this help\n" << 
        "'q' - exit this application\n" <<
        "\n";
    };

    help(); // Show help

    // Process users input (buttons)
    while (running) {

      // Show prompt and get input
      prompt();
      std::string input;
      std::cin >> input;
      
      std::string str;
      std::stringstream stream(input);
      while( std::getline(stream, str, ',') ) {
              
        std::cout << str << "\n";

        // If help request
        if (str == "?" || str == "help") help();

        // If pause pressed
        else if (str == "p") { 
          if(!paused) std::cout << "paused... press 'p' to continue\n";
          paused = !paused;
        }  

        // If list internal buttons request
        else if (str == "f") {
          std::cout << "Lift is on " << getFloor(false) << " floor\n";
        }  

        // If list internal buttons request
        else if (str == "i") {
          std::cout << "List of internal buttons pressed:\n";
          iQueue.printList();
        }  

        // If list outside buttons request
        else if (str == "o") {
          std::cout << "List of outside buttons pressed:\n";
          oQueue.printList();
        }  

        // If quit request
        else if (str == "q" || str == "quit") running = false;

        // If wrong input
        else if (str[0] != 'i' && str[0] != 'o') {
          std::cout << "Wrong button. See Help: \n";
          help();
        }

        // Check buttons
        else {
          char type = str[0];
          int value = std::atoi(str.substr(1).c_str());

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
    std::cout << 
      "Lift parameters: " << "\n" <<
      "\n" <<
      "  Number of floors: " << floors_number << "\n" <<
      "  Height of one floor (m): " << floor_height << "\n" <<
      "  Speed of the elevator (m/sec): " << lift_speed << "\n" <<
      "  Time of opening doors (sec): " << opening_doors_time << "\n";
  }

  /**
   * Delay (just delay...)
   * @param sleep_time Time of delay in milliseconds
   */
  void delay (int sleep_time) {
    std::this_thread::sleep_for((std::chrono::milliseconds) sleep_time);
  }
};
