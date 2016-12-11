#pragma once

#include "../surface/Surface.h"
#include "Robot.h"

#include <bits/unordered_set.h>

using namespace std;

namespace explorer {

  template<typename P, typename M, typename H>
  class AStarRobot : public Robot<P, M> {
    public:
      AStarRobot<P, M, H>(const Surface<P, M>&);
      vector<P> findPath(P, P) override;

    private:
      vector<P> search(P, P) override;
      void clearData();
      vector<P> restorePath(P, P);
      vector<P>& reversePath(vector<P>&);
      virtual size_t heuristic(P) const;
      P getMin();

      const Surface<P, M>& surface_;
      unordered_set<P, H> openSet_;
      unordered_set<P, H> closedSet_;
      unordered_map<P, size_t, H> gScore_;
      unordered_map<P, size_t, H> fScore_;
      unordered_map<P, P, H> parents_;
      P finish_;
  };

  template<typename P, typename M, typename H>
  AStarRobot<P, M, H>::
  AStarRobot(const Surface<P, M> &surface) :
          surface_(surface)
  {}

  template<typename P, typename M, typename H>
  std::vector<P>
  AStarRobot<P, M, H>::
  findPath(P start, P finish)
  {
    this->clearData();
    finish_ = finish;

    std::vector<P> path = this->search(start, finish);

    if (path.size() > 0) {
      if (!surface_.checkPath(path, start, finish)) {
//        path.clear();
        cout << "The path found is invalid" << endl;
      }
    } else {
      cout << "No path found" << endl;
    }

    return path;
  }

  template<typename P, typename M, typename H>
  std::vector<P>
  AStarRobot<P, M, H>::
  search(P start, P finish)
  {
    if (surface_.isWalkable(start)) {
      openSet_.emplace(start);
      gScore_[start] = fScore_[start] = 0;
    }

    while(!openSet_.empty()) {
      P current = this->getMin();

      if (current == finish) {
        return this->restorePath(start, finish);
      }

      closedSet_.emplace(current);

      for (auto& point : surface_.lookup(current)) {

        if (!surface_.isWalkable(point) || closedSet_.count(point) != 0) {
          continue;
        }

        size_t tempScore = gScore_[current] + surface_.distance(current, point);

        if (openSet_.count(point) == 0 || tempScore < gScore_[point]) {
          parents_[point] = current;
          gScore_[point] = tempScore;
          fScore_[point] = gScore_[point] + this->heuristic(point);
          if (openSet_.count(point) == 0)
            openSet_.emplace(point);
        }
      }
    }

    return vector<P>{};
  }

  template<typename P, typename M, typename H>
  vector<P>
  AStarRobot<P, M, H>::
  restorePath(P start, P finish)
  {
    vector<P> path{};
    P current = finish;

    path.push_back(current);

    while (current != start) {
      if (parents_.count(current)) {
        current = parents_[current];
        path.push_back(current);
      } else {
        cout << "Error: no parent for current point" << endl;
        break;
      }
    }

    return this->reversePath(path);
  }

  template<typename P, typename M, typename H>
  vector<P>&
  AStarRobot<P, M, H>::
  reversePath(vector<P>& path)
  {
    size_t last = path.size() - 1;
    for (size_t i = 0; i <= last / 2; i++) {
      P tmp = path[i];
      path[i] = path[last - i];
      path[last - i] = tmp;
    }

    return path;
  }

  template<typename P, typename M, typename H>
  P
  AStarRobot<P, M, H>::
  getMin()
  {
    size_t minScore = 0;
    P minPoint;

    for (const auto& point: openSet_) {
      if (minScore == 0 || fScore_[point] < minScore) {
        minScore = fScore_[point];
        minPoint = point;
      }
    }

    openSet_.erase(minPoint);

    return minPoint;
  }

  template<typename P, typename M, typename H>
  void
  AStarRobot<P, M, H>::
  clearData()
  {
    openSet_.clear();
    closedSet_.clear();
    gScore_.clear();
    fScore_.clear();
    parents_.clear();
  }

  template<typename P, typename M, typename H>
  size_t
  AStarRobot<P, M, H>::
  heuristic(P point) const
  {
    return surface_.distance(point, finish_);
  }

} // end namespace explorer