#include "clean_command.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int CleanCommand::execute(const std::vector<std::string> &args) {
  if (args.empty()) {
    std::cerr << "错误: 请提供Unity项目路径\n";
    std::cerr << "用法: unity-cli clean <project_path>\n";
    return 1;
  }

  fs::path projectPath = args[0];

  if (!fs::exists(projectPath)) {
    std::cerr << "错误: 项目路径不存在: " << projectPath << std::endl;
    return 1;
  }

  if (!isUnityProject(projectPath)) {
    std::cerr << "错误: 指定路径不是Unity项目: " << projectPath << std::endl;
    return 1;
  }

  std::cout << "开始清理Unity项目: " << projectPath << std::endl;

  try {
    cleanUnityProject(projectPath);
    std::cout << "清理完成!" << std::endl;
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "清理过程中发生错误: " << e.what() << std::endl;
    return 1;
  }
}

std::string CleanCommand::getDescription() const {
  return "删除Unity项目中的不必要文件和目录";
}

bool CleanCommand::isUnityProject(const fs::path &path) const {
  return fs::exists(path / "Assets") && fs::exists(path / "ProjectSettings");
}

void CleanCommand::cleanUnityProject(const fs::path &projectPath) {
  auto cleanableDirs = getCleanableDirectories();
  auto cleanableFiles = getCleanableFiles();

  for (const auto &dirName : cleanableDirs) {
    fs::path dirPath = projectPath / dirName;
    if (fs::exists(dirPath)) {
      std::cout << "删除目录: " << dirPath << std::endl;
      removeDirectory(dirPath);
    }
  }

  for (const auto &fileName : cleanableFiles) {
    fs::path filePath = projectPath / fileName;
    if (fs::exists(filePath)) {
      std::cout << "删除文件: " << filePath << std::endl;
      removeFile(filePath);
    }
  }

  for (const auto &entry : fs::directory_iterator(projectPath)) {
    if (entry.is_regular_file()) {
      std::string filename = entry.path().filename().string();
      if (filename.ends_with(".tmp") || filename.ends_with(".log") ||
          filename.starts_with("Temp")) {
        std::cout << "删除临时文件: " << entry.path() << std::endl;
        removeFile(entry.path());
      }
    }
  }
}

void CleanCommand::removeDirectory(const fs::path &path) {
  std::error_code ec;
  fs::remove_all(path, ec);
  if (ec) {
    std::cerr << "警告: 无法删除目录 " << path << ": " << ec.message()
              << std::endl;
  }
}

void CleanCommand::removeFile(const fs::path &path) {
  std::error_code ec;
  fs::remove(path, ec);
  if (ec) {
    std::cerr << "警告: 无法删除文件 " << path << ": " << ec.message()
              << std::endl;
  }
}

std::vector<std::string> CleanCommand::getCleanableDirectories() const {
  return {"Library", "Temp",    "obj",   "Logs",
          ".vs",     ".vscode", "Build", "Builds"};
}

std::vector<std::string> CleanCommand::getCleanableFiles() const {
  return {"*.csproj",  "*.sln", "*.userprefs", "*.pidb",
          "*.booproj", "*.svd", "*.user",      "*.unityproj",
          "*.suo",     "*.tmp", "*.log"};
}
