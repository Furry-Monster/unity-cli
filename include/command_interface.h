#ifndef __COMMAND_INTERFACE__
#define __COMMAND_INTERFACE__

#include <string>
#include <vector>

class CommandInterface {
public:
  virtual ~CommandInterface() = default;
  virtual int execute(const std::vector<std::string> &args) = 0;
  virtual std::string getDescription() const = 0;
};

#endif // !__COMMAND_INTERFACE__