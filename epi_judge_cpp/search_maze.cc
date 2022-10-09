#include <istream>
#include <string>
#include <vector>

#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::vector;
enum class Color { kWhite, kBlack };
struct Coordinate {
  bool operator==(const Coordinate &that) const {
    return x == that.x && y == that.y;
  }

  int x, y;
};

bool doSearchMaze(const Coordinate &currentPixel, const Coordinate &e,
                  std::vector<std::vector<Color>> &maze,
                  std::vector<Coordinate> &path) {
  const auto xMax = maze.size() - 1;
  const auto yMax = maze.at(0).size() - 1;
  const auto pixelOutOfBound = (currentPixel.x > xMax) ||
                               (currentPixel.x < 0) ||
                               (currentPixel.y > yMax) || (currentPixel.y < 0);

  if (pixelOutOfBound)
    return false;

  const auto pixelColor = maze.at(currentPixel.x).at(currentPixel.y);
  if (pixelColor == Color::kBlack)
    return false;

  path.emplace_back(currentPixel);

  // Mark as visited.
  maze.at(currentPixel.x).at(currentPixel.y) = Color::kBlack;

  if (currentPixel == e)
    return true;

  const auto adjacentPixels =
      std::vector<Coordinate>{{currentPixel.x, currentPixel.y + 1},
                              {currentPixel.x, currentPixel.y - 1},
                              {currentPixel.x + 1, currentPixel.y},
                              {currentPixel.x - 1, currentPixel.y}};
  for (const auto &nextPixel : adjacentPixels) {
    if (doSearchMaze(nextPixel, e, maze, path))
      return true;
  }

  path.pop_back();
  return false;
}

std::vector<Coordinate> SearchMaze(std::vector<std::vector<Color>> maze,
                                   const Coordinate &s, const Coordinate &e) {
  std::vector<Coordinate> path{};
  doSearchMaze(s, e, maze, path);
  return path;
}

namespace test_framework {
template <> struct SerializationTrait<Color> : SerializationTrait<int> {
  using serialization_type = Color;

  static serialization_type Parse(const json &json_object) {
    return static_cast<serialization_type>(
        SerializationTrait<int>::Parse(json_object));
  }
};
} // namespace test_framework

namespace test_framework {
template <>
struct SerializationTrait<Coordinate> : UserSerTrait<Coordinate, int, int> {
  static std::vector<std::string> GetMetricNames(const std::string &arg_name) {
    return {};
  }

  static std::vector<int> GetMetrics(const Coordinate &x) { return {}; }
};
} // namespace test_framework

bool PathElementIsFeasible(const vector<vector<Color>> &maze,
                           const Coordinate &prev, const Coordinate &cur) {
  if (!(0 <= cur.x && cur.x < maze.size() && 0 <= cur.y &&
        cur.y < maze[cur.x].size() && maze[cur.x][cur.y] == Color::kWhite)) {
    return false;
  }
  return cur == Coordinate{prev.x + 1, prev.y} ||
         cur == Coordinate{prev.x - 1, prev.y} ||
         cur == Coordinate{prev.x, prev.y + 1} ||
         cur == Coordinate{prev.x, prev.y - 1};
}

bool SearchMazeWrapper(TimedExecutor &executor,
                       const vector<vector<Color>> &maze, const Coordinate &s,
                       const Coordinate &e) {
  vector<vector<Color>> copy = maze;

  auto path = executor.Run([&] { return SearchMaze(copy, s, e); });

  if (path.empty()) {
    return s == e;
  }

  if (!(path.front() == s) || !(path.back() == e)) {
    throw TestFailure("Path doesn't lay between start and end points");
  }

  for (size_t i = 1; i < path.size(); i++) {
    if (!PathElementIsFeasible(maze, path[i - 1], path[i])) {
      throw TestFailure("Path contains invalid segments");
    }
  }

  return true;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "maze", "s", "e"};
  return GenericTestMain(args, "search_maze.cc", "search_maze.tsv",
                         &SearchMazeWrapper, DefaultComparator{}, param_names);
}
