#ifndef __COMMAND_MANAGER__
#define __COMMAND_MANAGER__

#include "command_interface.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class CommandManager {
public:
  void registerCommand(const std::string &name,
                       std::unique_ptr<CommandInterface> command);
  int executeCommand(const std::string &name,
                     const std::vector<std::string> &args);
  bool hasCommand(const std::string &name) const;

private:
  std::map<std::string, std::unique_ptr<CommandInterface>> commands_;
};

#endif // !__COMMAND_MANAGER__