#include "command_manager.h"
#include <stdexcept>

void CommandManager::registerCommand(
    const std::string &name, std::unique_ptr<CommandInterface> command) {
  commands_[name] = std::move(command);
}

int CommandManager::executeCommand(const std::string &name,
                                   const std::vector<std::string> &args) {
  auto it = commands_.find(name);
  if (it == commands_.end()) {
    throw std::runtime_error("未知命令: " + name);
  }

  return it->second->execute(args);
}

bool CommandManager::hasCommand(const std::string &name) const {
  return commands_.find(name) != commands_.end();
}
