/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 4 -*- */
/*
 * kslift application
 *
 * main.cpp
 * Copyright (C) Kirill Scherba 2017 <kirill@scherba.ru>
 *
 * kslift is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * kslift is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * File:   main.cpp
 * Author: Kirill Scherba <kirill@scherba.ru>
 *
 * kslift c++ application.
 * 
 * Application API:
 * 
 *  CMD_USER: ...
 *      @input ...
 *      @output ...
 * 
 * Created on December 20, 2017, 9:52 AM
 */

#include <iostream>
#include <vector>

#include "Lift.h"

#define APP_VERSION "0.0.1"

/**
 * Main kslift application function
 *
 * @param argc Number of parameters
 * @param argv Parameters array
 *
 * @return EXIT_SUCCESS
 */
int main(int argc, char** argv) {

  std::cout << "Lift test task by Kirill Scherba c++ application ver " APP_VERSION  "\n";
  
  const char *usage = "Usage: kslift "
                      "<number_of_floors(5-20)> "
                      "[height_of_one_floor(m) = 3] "
                      "[lift_speed(m/sec) = 3] "
                      "[time_of_opening_doors(sec) = 3]";
  
  // Check application parameters
  if (argc < 2) {
    std::cout << "\nInvalid number of application arguments.\n\n";
    std::cout << usage << "\n\n";
  }
  else if (std::atoi(argv[1]) < 5 || std::atoi(argv[1]) > 20) {
    std::cout << "\nWrong number of floors. This value should be from 5 to 20." << "\n\n";
    std::cout << usage << "\n";
  }
  // Create lift object
  else {
    std::vector<std::string> arguments(argv + 1, argv + argc);  
    auto lift = Lift(arguments);
    lift.start();
  }
  
  return (EXIT_SUCCESS);
}
