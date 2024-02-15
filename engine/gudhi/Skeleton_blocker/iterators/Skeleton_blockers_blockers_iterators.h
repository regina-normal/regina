/*    This file is part of the Gudhi Library - https://gudhi.inria.fr/ - which is released under MIT.
 *    See file LICENSE or go to https://gudhi.inria.fr/licensing/ for full license details.
 *    Author(s):       David Salinas
 *
 *    Copyright (C) 2014 Inria
 *
 *    Modification(s):
 *      - YYYY/MM Author: Description of the modification
 */

#ifndef SKELETON_BLOCKER_ITERATORS_SKELETON_BLOCKERS_BLOCKERS_ITERATORS_H_
#define SKELETON_BLOCKER_ITERATORS_SKELETON_BLOCKERS_BLOCKERS_ITERATORS_H_

#include <boost/iterator/iterator_facade.hpp>

namespace Gudhi {

namespace skeleton_blocker {

/**
 * @brief Iterator through the blockers of a vertex.
 */
// ReturnType = const Simplex* or Simplex*
// MapIteratorType = BlockerMapConstIterator or BlockerMapIterator

template<typename MapIteratorType, typename ReturnType>
class Blocker_iterator_internal : public boost::iterator_facade<
Blocker_iterator_internal<MapIteratorType, ReturnType>,
ReturnType,
boost::forward_traversal_tag,
ReturnType
> {
 private:
  MapIteratorType current_position;
  MapIteratorType end_of_map;

 public:
  Blocker_iterator_internal() : current_position() { }

  Blocker_iterator_internal(MapIteratorType position, MapIteratorType end_of_map_) :
      current_position(position), end_of_map(end_of_map_) { }

  bool equal(const Blocker_iterator_internal& other) const {
    return current_position == other.current_position;
  }

  void increment() {
    goto_next_blocker();
  }

  ReturnType dereference() const {
    return (current_position->second);
  }

 private:
  /**
   * Let the current pair be (v,sigma) where v is a vertex and sigma is a blocker.
   * If v is not the first vertex of sigma then we already have seen sigma as a blocker
   * and we look for the next one.
   */
  void goto_next_blocker() {
    do {
      ++current_position;
    } while (!(current_position == end_of_map) && !first_time_blocker_is_seen());
  }

  bool first_time_blocker_is_seen() const {
    return current_position->first == current_position->second->first_vertex();
  }
};

/**
 * @brief Iterator through the blockers of a vertex
 */
// ReturnType = const Simplex* or Simplex*
// MapIteratorType = BlockerMapConstIterator or BlockerMapIterator

template<typename MapIteratorType, typename ReturnType>
class Blocker_iterator_around_vertex_internal : public boost::iterator_facade<
Blocker_iterator_around_vertex_internal<MapIteratorType, ReturnType>,
ReturnType,
boost::forward_traversal_tag,
ReturnType
> {
 private:
  MapIteratorType current_position_;

 public:
  Blocker_iterator_around_vertex_internal() : current_position_() { }

  Blocker_iterator_around_vertex_internal(MapIteratorType position) :
      current_position_(position) { }

  Blocker_iterator_around_vertex_internal& operator=(Blocker_iterator_around_vertex_internal other) {
    this->current_position_ = other.current_position_;
    return *this;
  }

  bool equal(const Blocker_iterator_around_vertex_internal& other) const {
    return current_position_ == other.current_position_;
  }

  void increment() {
    current_position_++;
  }

  ReturnType dereference() const {
    return (current_position_->second);
  }

  MapIteratorType current_position() {
    return this->current_position_;
  }
};

}  // namespace skeleton_blocker

namespace skbl = skeleton_blocker;

}  // namespace Gudhi

#endif  // SKELETON_BLOCKER_ITERATORS_SKELETON_BLOCKERS_BLOCKERS_ITERATORS_H_
