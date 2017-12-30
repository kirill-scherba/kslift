/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 4 -*- */
/*
 * kslift application
 *
 * LiftButton.h
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

/*
 * File:   LiftButton.h
 * Author: Kirill Scherba <kirill@scherba.ru>
 *
 * Created on December 29, 2017, 7:31 PM
 */

#ifndef LIFTBUTTON_H
#define LIFTBUTTON_H

#include <set>
#include <mutex>

class LiftButton {

private:

  std::set<int> queue;
  std::mutex queue_mutex;

public:

  LiftButton() {};

  LiftButton(const LiftButton& orig) {};

  virtual ~LiftButton() {};

public:

  /**
   * Insert value to queue
   *
   * @param floor Floor number
   */
  inline void insert(int floor) {
    std::lock_guard<std::mutex> lock(queue_mutex); // lock tread
    queue.insert(floor);
  }

  /**
   * Return buttons queue size
   *
   * @return Buttons queue size
   */
  inline size_t size() {
    return queue.size();
  }

  /**
   * If button upper than input floor is present in the queue 
   * 
   * @param floor Floor number
   * 
   * @return True if floor upper than selected in input parameter is present
   */
  bool hasUpper(int floor) {
    std::lock_guard<std::mutex> lock(queue_mutex); // lock thread
    bool retval = false;
    auto it = queue.upper_bound(floor);
    if(it != queue.end()) retval = true;
    return retval;
  }

  /**
   * If button lower than input floor is present in the queue
   * 
   * @param floor Floor number
   * 
   * @return True if floor lower than selected in input parameter is present
   */
  bool hasLower(int floor) {
    std::lock_guard<std::mutex> lock(queue_mutex); // lock tread
    bool retval = false;
    auto it = queue.lower_bound(floor);
    if(it != queue.begin()) retval = true;
    return retval;
  }

  /**
   * Remove floor from queue
   *
   * @param floor Floor number
   * 
   * @return True if floor was present in Queue and removed
   */
  bool erase(int floor) {
    std::lock_guard<std::mutex> lock(queue_mutex); // lock tread
    bool retval = false;
    if(find(floor, false)) {
      queue.erase(floor);
      retval = true;
    }
    return retval;
  }

  /**
   * Find floor in queue
   * 
   * @param floor Floor number
   * 
   * @return True if floor present in Queue 
   */
  bool find(int floor, bool lock = true) {
    if(lock) std::lock_guard<std::mutex> lock(queue_mutex); // lock tread
    bool retval = false;
    auto it = queue.find(floor);
    if(it != queue.end()) retval = true;
    return retval;
  }

  /**
   * Print buttons queue rows
   */
  inline void printList() {
    for(auto &i : queue) std::cout << i << "\n";
  }

private:

};

#endif /* LIFTBUTTON_H */
