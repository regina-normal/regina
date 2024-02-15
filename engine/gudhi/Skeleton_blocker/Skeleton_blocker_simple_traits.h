/*    This file is part of the Gudhi Library - https://gudhi.inria.fr/ - which is released under MIT.
 *    See file LICENSE or go to https://gudhi.inria.fr/licensing/ for full license details.
 *    Author(s):       David Salinas
 *
 *    Copyright (C) 2014 Inria
 *
 *    Modification(s):
 *      - YYYY/MM Author: Description of the modification
 */

#ifndef SKELETON_BLOCKER_SKELETON_BLOCKER_SIMPLE_TRAITS_H_
#define SKELETON_BLOCKER_SKELETON_BLOCKER_SIMPLE_TRAITS_H_

#include <gudhi/Skeleton_blocker/Skeleton_blocker_simplex.h>

#include <string>
#include <sstream>

namespace Gudhi {

namespace skeleton_blocker {

/**
 * @extends SkeletonBlockerDS
 * @ingroup skbl
 * @brief Simple traits that is a model of SkeletonBlockerDS and
 * can be passed as a template argument to Skeleton_blocker_complex
 */
struct Skeleton_blocker_simple_traits {
  /**
   * @brief Global and local handle similar to <a href="https://www.boost.org/doc/libs/release/libs/graph/doc/subgraph.html">boost subgraphs</a>.
   * Vertices are stored in a vector.
   * For the root simplicial complex, the local and global descriptors are the same.
   * For a subcomplex L and one of its vertices 'v', the local descriptor of 'v' is its position in
   * the vertex vector of the subcomplex L whereas its global descriptor is the position of 'v'
   * in the vertex vector of the root simplicial complex.
   */
  struct Root_vertex_handle {
    typedef int boost_vertex_handle;

    explicit Root_vertex_handle(boost_vertex_handle val = -1)
        : vertex(val) { }
    boost_vertex_handle vertex;

    bool operator!=(const Root_vertex_handle& other) const {
      return !(this->vertex == other.vertex);
    }

    bool operator==(const Root_vertex_handle& other) const {
      return this->vertex == other.vertex;
    }

    bool operator<(const Root_vertex_handle& other) const {
      return this->vertex < other.vertex;
    }

    friend std::ostream& operator<<(std::ostream& o,
                                    const Root_vertex_handle & v) {
      o << v.vertex;
      return o;
    }
  };

  struct Vertex_handle {
    typedef int boost_vertex_handle;

    explicit Vertex_handle(boost_vertex_handle val = -1)
        : vertex(val) { }

    operator int() const {
      return static_cast<int> (vertex);
    }

    boost_vertex_handle vertex;

    bool operator==(const Vertex_handle& other) const {
      return this->vertex == other.vertex;
    }

    bool operator!=(const Vertex_handle& other) const {
      return this->vertex != other.vertex;
    }

    bool operator<(const Vertex_handle& other) const {
      return this->vertex < other.vertex;
    }

    friend std::ostream& operator<<(std::ostream& o, const Vertex_handle & v) {
      o << v.vertex;
      return o;
    }
  };

  class Graph_vertex {
    bool is_active_;
    Root_vertex_handle id_;

   public:
    virtual ~Graph_vertex() { }

    void activate() {
      is_active_ = true;
    }

    void deactivate() {
      is_active_ = false;
    }

    bool is_active() const {
      return is_active_;
    }

    void set_id(Root_vertex_handle i) {
      id_ = i;
    }

    Root_vertex_handle get_id() const {
      return id_;
    }

    virtual std::string to_string() const {
      std::ostringstream res;
      res << id_;
      return res.str();
    }

    friend std::ostream& operator<<(std::ostream& o, const Graph_vertex & v) {
      o << v.to_string();
      return o;
    }
  };

  class Graph_edge {
    Root_vertex_handle a_;
    Root_vertex_handle b_;
    int index_;

   public:
    Graph_edge()
        : a_(-1),
        b_(-1),
        index_(-1) { }

    int& index() {
      return index_;
    }

    int index() const {
      return index_;
    }

    void setId(Root_vertex_handle a, Root_vertex_handle b) {
      a_ = a;
      b_ = b;
    }

    Root_vertex_handle first() const {
      return a_;
    }

    Root_vertex_handle second() const {
      return b_;
    }

    friend std::ostream& operator<<(std::ostream& o, const Graph_edge & v) {
      o << "(" << v.a_ << "," << v.b_ << " - id = " << v.index();
      return o;
    }
  };
};

}  // namespace skeleton_blocker

namespace skbl = skeleton_blocker;

}  // namespace Gudhi

#endif  // SKELETON_BLOCKER_SKELETON_BLOCKER_SIMPLE_TRAITS_H_
