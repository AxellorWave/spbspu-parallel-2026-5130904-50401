#include <iostream>
#include <vector>

namespace zharov
{
  struct Circle
  {
    size_t r;
    double x, y;
  };
}

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    std::cerr << "not enough args\n";
    return 1;
  }
  if (argc > 4)
  {
    std::cerr << "too many args\n";
    return 1;
  }

  long long threads = 0, tries = 0, seed = 0;
  threads = std::atoll(argv[1]);
  tries = std::atoll(argv[2]);
  if (argc == 4)
  {
    seed = std::atoll(argv[3]);
  }

  if (threads <= 0 || tries <= 0 || seed < 0)
  {
    std::cerr << "args must be positive\n";
    return 1;
  }

  size_t skip = 0;
  zharov::Circle shape{0, 0, 0};
  std::vector< zharov::Circle > shapes;
  while (std::cin >> shape.r >> skip >> shape.x >> shape.y)
  {
    shapes.push_back(shape);
  }
  if (!std::cin.eof())
  {
    std::cerr << "bad input\n";
    return 1;
  }

  for (auto i = shapes.cbegin(); i != shapes.cend(); ++i)
  {
    std::cout << i->r << " " << i->x << " " << i->y << "\n";
  }
}
