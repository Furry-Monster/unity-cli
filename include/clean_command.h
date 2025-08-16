#ifndef __CLEAN_COMMAND__
#define __CLEAN_COMMAND__

#include "command_interface.h"
#include <filesystem>
#include <string>
#include <vector>

class CleanCommand : public CommandInterface {
public:
  int execute(const std::vector<std::string> &args) override;
  std::string getDescription() const override;

private:
  bool isUnityProject(const std::filesystem::path &path) const;
  void cleanUnityProject(const std::filesystem::path &projectPath);
  void removeDirectory(const std::filesystem::path &path);
  void removeFile(const std::filesystem::path &path);

  std::vector<std::string> getCleanableDirectories() const;
  std::vector<std::string> getCleanableFiles() const;
};

#endif // !__CLEAN_COMMAND__