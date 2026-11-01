#include "commands.hpp"
#include <memory>
#include <stdexcept>

namespace
{
  constexpr const char* spaces = " \t\r";

  long long parseNumber(const std::string& word)
  {
    size_t pos = 0;
    const long long value = std::stoll(word, std::addressof(pos));
    if (pos != word.size())
    {
      throw std::invalid_argument("Bad number");
    }
    return value;
  }

  void checkName(const std::string& name)
  {
    const char first = name.front();
    const bool is_letter = (first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z');
    if (!is_letter)
    {
      throw std::invalid_argument("Bad name");
    }
  }

  void printCircle(std::ostream& out, const zharov::Circle& shape)
  {
    out << shape.r << " (" << shape.center.x << " " << shape.center.y << ")\n";
  }
}

zharov::Args zharov::splitLine(const std::string& line)
{
  Args words;
  size_t start = line.find_first_not_of(spaces);
  while (start != std::string::npos)
  {
    const size_t end = line.find_first_of(spaces, start);
    words.push_back(line.substr(start, end - start));
    start = line.find_first_not_of(spaces, end);
  }
  return words;
}

void zharov::createCircle(std::ostream&, const Args& args, Context& ctx)
{
  constexpr size_t circle_args = 5;
  if (args.size() != circle_args)
  {
    throw std::invalid_argument("Bad arguments count");
  }
  const std::string& name = args[1];
  checkName(name);
  if (ctx.circles.count(name) > 0)
  {
    throw std::logic_error("Circle already exists");
  }
  const long long r = parseNumber(args[2]);
  if (r <= 0)
  {
    throw std::invalid_argument("Bad radius");
  }
  const Point center{parseNumber(args[3]), parseNumber(args[4])};
  ctx.circles.emplace(name, Circle{r, center});
}

void zharov::showShape(std::ostream& out, const Args& args, const Context& ctx)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Bad arguments count");
  }
  printCircle(out, ctx.circles.at(args[1]));
}
