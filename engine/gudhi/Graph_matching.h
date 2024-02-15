/*    This file is part of the Gudhi Library - https://gudhi.inria.fr/ - which is released under MIT.
 *    See file LICENSE or go to https://gudhi.inria.fr/licensing/ for full license details.
 *    Author:       Francois Godi
 *
 *    Copyright (C) 2015 Inria
 *
 *    Modification(s):
 *      - YYYY/MM Author: Description of the modification
 */

#ifndef GRAPH_MATCHING_H_
#define GRAPH_MATCHING_H_

#include <gudhi/Neighbors_finder.h>

#include <vector>
#include <unordered_set>
#include <algorithm>

namespace Gudhi {

namespace persistence_diagram {

/** \internal \brief Structure representing a graph matching. The graph is a Persistence_diagrams_graph.
 *
 * \ingroup bottleneck_distance
 */
class Graph_matching {
 public:
  /** \internal \brief Constructor constructing an empty matching. */
  explicit Graph_matching(Persistence_graph &g);
  /** \internal \brief Is the matching perfect ? */
  bool perfect() const;
  /** \internal \brief Augments the matching with a maximal set of edge-disjoint shortest augmenting paths. */
  bool multi_augment();
  /** \internal \brief Sets the maximum length of the edges allowed to be added in the matching, 0 initially. */
  void set_r(double r);

 private:
  Persistence_graph* gp;
  double r;
  /** \internal \brief Given a point from V, provides its matched point in U, null_point_index() if there isn't. */
  std::vector<int> v_to_u;
  /** \internal \brief All the unmatched points in U. */
  std::unordered_set<int> unmatched_in_u;

  /** \internal \brief Provides a Layered_neighbors_finder dividing the graph in layers. Basically a BFS. */
  Layered_neighbors_finder layering() const;
  /** \internal \brief Augments the matching with a simple path no longer than max_depth. Basically a DFS. */
  bool augment(Layered_neighbors_finder & layered_nf, int u_start_index, int max_depth);
  /** \internal \brief Update the matching with the simple augmenting path given as parameter. */
  void update(std::vector<int> & path);
};

inline Graph_matching::Graph_matching(Persistence_graph& g)
    : gp(&g), r(0.), v_to_u(g.size(), null_point_index()), unmatched_in_u(g.size()) {
  for (int u_point_index = 0; u_point_index < g.size(); ++u_point_index)
    unmatched_in_u.insert(u_point_index);
}

inline bool Graph_matching::perfect() const {
  return unmatched_in_u.empty();
}

inline bool Graph_matching::multi_augment() {
  if (perfect())
    return false;
  Layered_neighbors_finder layered_nf(layering());
  int max_depth = layered_nf.vlayers_number()*2 - 1;
  double rn = sqrt(gp->size());
  // verification of a necessary criterion in order to shortcut if possible
  if (max_depth < 0 || (unmatched_in_u.size() > rn && max_depth >= rn))
    return false;
  bool successful = false;
  std::vector<int> tries(unmatched_in_u.cbegin(), unmatched_in_u.cend());
  for (auto it = tries.cbegin(); it != tries.cend(); it++)
    // 'augment' has side-effects which have to be always executed, don't change order
    successful = augment(layered_nf, *it, max_depth) || successful;
  return successful;
}

inline void Graph_matching::set_r(double r) {
  this->r = r;
}

inline bool Graph_matching::augment(Layered_neighbors_finder & layered_nf, int u_start_index, int max_depth) {
  // V vertices have at most one successor, thus when we backtrack from U we can directly pop_back 2 vertices.
  std::vector<int> path;
  path.emplace_back(u_start_index);
  do {
    if (static_cast<int> (path.size()) > max_depth) {
      path.pop_back();
      path.pop_back();
    }
    if (path.empty())
      return false;
    path.emplace_back(layered_nf.pull_near(path.back(), static_cast<int> (path.size()) / 2));
    while (path.back() == null_point_index()) {
      path.pop_back();
      path.pop_back();
      if (path.empty())
        return false;
      path.pop_back();
      path.emplace_back(layered_nf.pull_near(path.back(), path.size() / 2));
    }
    path.emplace_back(v_to_u.at(path.back()));
  } while (path.back() != null_point_index());
  // if v_to_u.at(path.back()) has no successor, path.back() is an exposed vertex
  path.pop_back();
  update(path);
  return true;
}

inline Layered_neighbors_finder Graph_matching::layering() const {
  std::vector<int> u_vertices(unmatched_in_u.cbegin(), unmatched_in_u.cend());
  std::vector<int> v_vertices;
  Neighbors_finder nf(*gp, r);
  for (int v_point_index = 0; v_point_index < gp->size(); ++v_point_index)
    nf.add(v_point_index);
  Layered_neighbors_finder layered_nf(*gp, r);
  for (int layer = 0; !u_vertices.empty(); layer++) {
    // one layer is one step in the BFS
    for (auto it1 = u_vertices.cbegin(); it1 != u_vertices.cend(); ++it1) {
      std::vector<int> u_succ(nf.pull_all_near(*it1));
      for (auto it2 = u_succ.begin(); it2 != u_succ.end(); ++it2) {
        layered_nf.add(*it2, layer);
        v_vertices.emplace_back(*it2);
      }
    }
    // When the above for finishes, we have progress of one half-step (from U to V) in the BFS
    u_vertices.clear();
    bool end = false;
    for (auto it = v_vertices.cbegin(); it != v_vertices.cend(); it++)
      if (v_to_u.at(*it) == null_point_index())
        // we stop when a nearest exposed V vertex (from U exposed vertices) has been found
        end = true;
      else
        u_vertices.emplace_back(v_to_u.at(*it));
    // When the above for finishes, we have progress of one half-step (from V to U) in the BFS
    if (end)
      return layered_nf;
    v_vertices.clear();
  }
  return layered_nf;
}

inline void Graph_matching::update(std::vector<int>& path) {
  // Must return 1.
  unmatched_in_u.erase(path.front());
  for (auto it = path.cbegin(); it != path.cend(); ++it) {
    // Be careful, the iterator is incremented twice each time
    int tmp = *it;
    v_to_u[*(++it)] = tmp;
  }
}


}  // namespace persistence_diagram

}  // namespace Gudhi

#endif  // GRAPH_MATCHING_H_
