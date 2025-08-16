#include "structure_command.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

int StructureCommand::execute(const std::vector<std::string> &args) {
  if (args.size() < 2) {
    std::cerr << "错误: 请提供Unity项目路径和YAML模板路径\n";
    std::cerr << "用法: unity-cli structure <project_path> <template.yaml>\n";
    return 1;
  }

  fs::path projectPath = args[0];
  fs::path templatePath = args[1];

  if (!fs::exists(projectPath)) {
    std::cerr << "错误: 项目路径不存在: " << projectPath << std::endl;
    return 1;
  }

  if (!isUnityProject(projectPath)) {
    std::cerr << "错误: 指定路径不是Unity项目: " << projectPath << std::endl;
    return 1;
  }

  if (!fs::exists(templatePath)) {
    std::cerr << "错误: YAML模板文件不存在: " << templatePath << std::endl;
    return 1;
  }

  std::cout << "开始根据模板创建目录结构..." << std::endl;

  try {
    DirectoryNode structure = parseYamlTemplate(templatePath);
    fs::path assetsPath = projectPath / "Assets";
    createDirectoryStructure(assetsPath, structure);
    std::cout << "目录结构创建完成!" << std::endl;
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "创建目录结构时发生错误: " << e.what() << std::endl;
    return 1;
  }
}

std::string StructureCommand::getDescription() const {
  return "根据YAML模板在Assets目录下创建文件夹结构";
}

bool StructureCommand::isUnityProject(const fs::path &path) const {
  return fs::exists(path / "Assets") && fs::exists(path / "ProjectSettings");
}

StructureCommand::DirectoryNode
StructureCommand::parseYamlTemplate(const fs::path &yamlPath) {
  std::ifstream file(yamlPath);
  if (!file.is_open()) {
    throw std::runtime_error("无法打开YAML文件: " + yamlPath.string());
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content = buffer.str();

  DirectoryNode root;
  root.name = "Assets";

  size_t pos = 0;
  parseYamlNode(content, pos, root);

  return root;
}

void StructureCommand::parseYamlNode(const std::string &content, size_t &pos,
                                     DirectoryNode &parent, int currentIndent) {
  std::istringstream stream(content.substr(pos));
  std::string line;

  while (std::getline(stream, line)) {
    pos += line.length() + 1;

    if (line.empty() || line[0] == '#') {
      continue;
    }

    int indent = getIndentLevel(line);
    std::string trimmedLine = trim(line);

    if (trimmedLine.empty()) {
      continue;
    }

    if (indent < currentIndent) {
      pos -= line.length() + 1;
      break;
    }

    if (indent == currentIndent) {
      DirectoryNode node;

      if (trimmedLine.back() == '/') {
        node.name = trimmedLine.substr(0, trimmedLine.length() - 1);
        node.isFile = false;
      } else if (trimmedLine.find('.') != std::string::npos) {
        node.name = trimmedLine;
        node.isFile = true;
      } else {
        node.name = trimmedLine;
        node.isFile = false;
      }

      size_t nextPos = pos;
      parseYamlNode(content, nextPos, node, currentIndent + 2);
      pos = nextPos;

      parent.children.push_back(std::move(node));
    }
  }
}

void StructureCommand::createDirectoryStructure(const fs::path &basePath,
                                                const DirectoryNode &node) {
  if (node.name != "Assets") {
    fs::path currentPath = basePath / node.name;

    if (node.isFile) {
      if (!fs::exists(currentPath)) {
        std::ofstream file(currentPath);
        std::cout << "创建文件: " << currentPath << std::endl;
      }
    } else {
      if (!fs::exists(currentPath)) {
        fs::create_directories(currentPath);
        std::cout << "创建目录: " << currentPath << std::endl;
      }

      for (const auto &child : node.children) {
        createDirectoryStructure(currentPath, child);
      }
    }
  } else {
    for (const auto &child : node.children) {
      createDirectoryStructure(basePath, child);
    }
  }
}

int StructureCommand::getIndentLevel(const std::string &line) const {
  int indent = 0;
  for (char c : line) {
    if (c == ' ') {
      indent++;
    } else if (c == '\t') {
      indent += 4;
    } else {
      break;
    }
  }
  return indent;
}

std::string StructureCommand::trim(const std::string &str) const {
  size_t start = str.find_first_not_of(" \t\r\n");
  if (start == std::string::npos) {
    return "";
  }
  size_t end = str.find_last_not_of(" \t\r\n");
  return str.substr(start, end - start + 1);
}