/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   LiftButton.h
 * Author: Kirill Scherba <kirill@scherba.ru>
 *
 * Created on December 29, 2017, 7:31 PM
 */

#pragma once

#include <set>

class LiftButton {

private:

  std::set<int> queue;

public:

  LiftButton() {};

  LiftButton(const LiftButton& orig) {};

  virtual ~LiftButton() {};

public:

  /**
   * Insert value to queue
   *
   * @param value
   * @return
   */
  inline void insert(int value) {
    queue.insert(value);
  }

  /**
   * Return queue size
   *
   * @return
   */
  inline size_t size() {
    return queue.size();
  }

  /**
   * If button upper than input floor is present in the queue 
   * 
   * @param floor
   * @return 
   */
  bool hasUpper(int floor) {
    bool retval = false;
    auto it = queue.upper_bound(floor);
    if(it != queue.end()) retval = true;
    return retval;
  }

  /**
   * If button lower than input floor is present in the queue
   * 
   * @param floor
   * @return 
   */
  bool hasLower(int floor) {
    bool retval = false;
    auto it = queue.lower_bound(floor);
    if(it != queue.begin()) retval = true;
    return retval;
  }

  /**
   * Erase floor from queue
   *
   * @param floor
   * @return
   */
  bool erase(int floor) {
    bool retval = false;
    if(find(floor)) {
      queue.erase(floor);
      retval = true;
    }
    return retval;
  }

  /**
   * Find floor in queue
   * 
   * @param floor
   * @return 
   */
  bool find(int floor) {
    bool retval = false;
    auto it = queue.find(floor);
    if(it != queue.end()) retval = true;
    return retval;
  }

  /**
   * Print queue rows
   */
  void printList() {
    for(auto &i : queue) std::cout << i << "\n";
  }

private:

};
