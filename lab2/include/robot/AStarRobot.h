#pragma once

#include "../surface/Surface.h"
#include "Robot.h"

#include <unordered_map>
#include <unordered_set>
#include <algorithm>

namespace explorer {

  using std::unordered_map;
  using std::unordered_set;
  using std::vector;

  template<typename PointType, typename MeasureType, typename Hash>
  class AStarRobot : public Robot<PointType, MeasureType> {
    public:
      using surface_type = typename Robot<PointType, MeasureType>::surface_type;
      using point_vector = typename Robot<PointType, MeasureType>::point_vector;

      using Robot<PointType, MeasureType>::surface_;

      AStarRobot<PointType, MeasureType, Hash>(const surface_type& surface) :
              Robot<PointType, MeasureType>(surface) {};

    private:
      point_vector search(const PointType&, const PointType&) final override;
      point_vector restorePath(const PointType&, const PointType&);
      point_vector& reversePath(point_vector&);
      PointType popNextCell();
      void clearData();

      unordered_set<PointType, Hash> openSet_;
      unordered_set<PointType, Hash> closedSet_;
      unordered_map<PointType, size_t, Hash> gScore_;
      unordered_map<PointType, size_t, Hash> fScore_;
      unordered_map<PointType, PointType, Hash> parents_;
  };


  template<typename PointType, typename MeasureType, typename Hash>
  typename AStarRobot<PointType, MeasureType, Hash>::point_vector
  AStarRobot<PointType, MeasureType, Hash>::
  search(const PointType& start, const PointType& finish)
  {
    this->clearData();

    if (surface_.isWalkable(start)) {
      openSet_.insert(start);
      gScore_[start] = fScore_[start] = 0;
    }

    while(!openSet_.empty()) {
      PointType current = this->popNextCell();
      if (current == finish) return this->restorePath(start, finish);
      closedSet_.insert(current);

      for (auto& point : surface_.lookup(current)) {
        if (!surface_.isWalkable(point) || closedSet_.count(point) != 0) continue;
        size_t tempScore = gScore_[current] + surface_.distance(current, point);

        if (openSet_.count(point) == 0 || tempScore < gScore_[point]) {
          parents_[point] = current;
          gScore_[point] = tempScore;
          fScore_[point] = gScore_[point] + surface_.distance(point, finish);
          if (openSet_.count(point) == 0) openSet_.insert(point);
        }
      }
    }

    return point_vector{};
  }

  template<typename PointType, typename MeasureType, typename Hash>
  typename AStarRobot<PointType, MeasureType, Hash>::point_vector
  AStarRobot<PointType, MeasureType, Hash>::
  restorePath(const PointType& start, const PointType& finish)
  {
    point_vector path{};
    PointType current = finish;

    try {
      path.push_back(current);
      while (current != start) {
        current = parents_.at(current);
        path.push_back(current);
      }
    } catch(const out_of_range& e) {
      throw logic_error("No parent for current point");
    }

    return this->reversePath(path);
  }

  template<typename PointType, typename MeasureType, typename Hash>
  typename AStarRobot<PointType, MeasureType, Hash>::point_vector&
  AStarRobot<PointType, MeasureType, Hash>::
  reversePath(point_vector& path)
  {
    reverse(path.begin(), path.end());
    return path;
  }

  template<typename PointType, typename MeasureType, typename Hash>
  PointType
  AStarRobot<PointType, MeasureType, Hash>::
  popNextCell() // finds min Fscore in open set and pops its key
  {
    size_t minScore = 0;
    PointType minPoint;

    for (const auto& point: openSet_) {
      if (minScore == 0 || fScore_[point] < minScore) {
        minScore = fScore_[point];
        minPoint = point;
      }
    }

    openSet_.erase(minPoint);

    return minPoint;
  }

  template<typename PointType, typename MeasureType, typename Hash>
  void
  AStarRobot<PointType, MeasureType, Hash>::
  clearData()
  {
    openSet_.clear();
    closedSet_.clear();
    gScore_.clear();
    fScore_.clear();
    parents_.clear();
  }

} // end namespace explorer