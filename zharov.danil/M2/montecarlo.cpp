#include "montecarlo.hpp"
#include <algorithm>
#include <future>
#include <random>

size_t zharov::countInside(const std::vector< Circle >& shapes, Frame frame, size_t tries, size_t seed)
{
  std::default_random_engine eng(seed);
  std::uniform_real_distribution< double > dist_x(frame.left_lower.x, frame.right_upper.x);
  std::uniform_real_distribution< double > dist_y(frame.left_lower.y, frame.right_upper.y);
  size_t inside = 0;
  for (size_t i = 0; i < tries; ++i)
  {
    const double x = dist_x(eng);
    const double y = dist_y(eng);
    bool covered = false;
    for (auto j = shapes.cbegin(); j != shapes.cend() && !covered; ++j)
    {
      covered = isInside(x, y, *j);
    }
    inside += covered;
  }
  return inside;
}

double zharov::calcArea(const std::vector< Circle >& shapes, size_t threads, size_t tries, size_t seed)
{
  constexpr size_t max_threads = 1000;
  const size_t count = std::min(threads, max_threads);
  const Frame frame = getFrame(shapes);
  const size_t tries_on_thread = tries / count;
  const size_t rest = tries % count;

  std::vector< std::future< size_t > > results;
  results.reserve(count);
  for (size_t i = 0; i < count; ++i)
  {
    const size_t cur_tries = tries_on_thread + (i < rest);
    results.push_back(std::async(std::launch::async, countInside, shapes, frame, cur_tries, seed + i));
  }

  size_t inside = 0;
  for (size_t i = 0; i < count; ++i)
  {
    inside += results[i].get();
  }

  const double width = frame.right_upper.x - frame.left_lower.x;
  const double height = frame.right_upper.y - frame.left_lower.y;
  return width * height * inside / tries;
}