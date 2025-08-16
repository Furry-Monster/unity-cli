#ifndef __STRUCTURE_COMMAND__
#define __STRUCTURE_COMMAND__

#include "command_interface.h"
#include <filesystem>
#include <string>
#include <vector>

class StructureCommand : public CommandInterface {
public:
  int execute(const std::vector<std::string> &args) override;
  std::string getDescription() const override;

private:
  struct DirectoryNode {
    std::string name;
    std::vector<DirectoryNode> children;
    bool isFile = false;
  };

  bool isUnityProject(const std::filesystem::path &path) const;
  DirectoryNode parseYamlTemplate(const std::filesystem::path &yamlPath);
  void createDirectoryStructure(const std::filesystem::path &basePath,
                                const DirectoryNode &node);
  void parseYamlNode(const std::string &content, size_t &pos,
                     DirectoryNode &parent, int currentIndent = 0);
  int getIndentLevel(const std::string &line) const;
  std::string trim(const std::string &str) const;
};

#endif // !__STRUCTURE_COMMAND__