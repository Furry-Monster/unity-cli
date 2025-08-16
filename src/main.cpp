#include "clean_command.h"
#include "command_manager.h"
#include "structure_command.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

void printUsage(const std::string &programName) {
  std::cout << "Unity CLI Tool - Unity项目管理工具\n\n";
  std::cout << "用法: " << programName << " <command> [options]\n\n";
  std::cout << "可用命令:\n";
  std::cout << "  clean     删除Unity项目中的不必要文件\n";
  std::cout << "  structure 根据YAML模板创建Assets目录结构\n";
  std::cout << "  help      显示此帮助信息\n\n";
  std::cout << "示例:\n";
  std::cout << "  " << programName << " clean /path/to/unity/project\n";
  std::cout << "  " << programName
            << " structure /path/to/unity/project template.yaml\n";
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printUsage(argv[0]);
    return 1;
  }

  CommandManager manager;

  manager.registerCommand("clean", std::make_unique<CleanCommand>());
  manager.registerCommand("structure", std::make_unique<StructureCommand>());

  std::string command = argv[1];

  if (command == "help" || command == "-h" || command == "--help") {
    printUsage(argv[0]);
    return 0;
  }

  std::vector<std::string> args;
  for (int i = 2; i < argc; ++i) {
    args.push_back(argv[i]);
  }

  try {
    return manager.executeCommand(command, args);
  } catch (const std::exception &e) {
    std::cerr << "错误: " << e.what() << std::endl;
    return 1;
  }
}
