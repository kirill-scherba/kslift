/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 4 -*- */
/*
 * kslift application
 *
 * main.cpp
 * Copyright (C) Kirill Scherba 2011-2017 <kirill@scherba.ru>
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

  return (EXIT_SUCCESS);
}
