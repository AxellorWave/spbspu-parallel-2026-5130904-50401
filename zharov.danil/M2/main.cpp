#include <exception>
#include <iostream>
#include <string>
#include <unordered_map>
#include "commands.hpp"

int main()
{
  using cmd = void (*)(std::ostream&, const zharov::Args&, zharov::Context&);
  using constCmd = void (*)(std::ostream&, const zharov::Args&, const zharov::Context&);
  std::unordered_map< std::string, cmd > cmds;
  std::unordered_map< std::string, constCmd > constCmds;
  cmds["circle"] = zharov::createCircle;
  constCmds["show"] = zharov::showShape;

  zharov::Context ctx;
  std::string line;
  while (std::getline(std::cin, line))
  {
    const zharov::Args args = zharov::splitLine(line);
    if (args.empty())
    {
      continue;
    }
    const std::string& command = args[0];
    if (cmds.count(command) == 0 && constCmds.count(command) == 0)
    {
      std::cout << "<UNKNOWN COMMAND>\n";
      continue;
    }
    try
    {
      if (cmds.count(command) > 0)
      {
        cmds.at(command)(std::cout, args, ctx);
      }
      else
      {
        constCmds.at(command)(std::cout, args, ctx);
      }
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
