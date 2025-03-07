#include <fstream>
#include <filesystem>
#include <iostream>
#include <format>

int main(int argc, char *argv[])
{
  std::filesystem::path rootDir = argv[1];

  std::filesystem::remove_all(rootDir / "include");

  std::string projectName = "ngyn";

  std::filesystem::path includePath = rootDir / "include";

  std::filesystem::path srcPath = rootDir / "src";

  std::filesystem::create_directories(includePath / projectName);

  std::fstream entryHeader{includePath / projectName / std::format("{}.hpp", projectName), entryHeader.out};

  entryHeader << "#pragma once\n\n";

  for(auto const &dirEntry : std::filesystem::directory_iterator(srcPath / projectName))
  {
    std::string dirname = std::filesystem::path(dirEntry).filename().string();

    std::filesystem::path dirPath = includePath / projectName / dirname;
    std::filesystem::create_directories(dirPath);

    std::fstream dirHeader{dirPath / std::format("{}.hpp", dirname), dirHeader.out};

    dirHeader << "#pragma once\n\n";

    std::string dirIncludes = std::format("#include \"{}/{}.hpp\"\n", dirname, dirname);

    entryHeader << dirIncludes;

    for(auto const &subDirEntry : std::filesystem::directory_iterator(dirEntry))
    {
      std::string filename = std::filesystem::path(subDirEntry).filename().string();
      std::string includes = std::format("#include \"../../src/ngyn/{}/{}\"\n", dirname, filename);

      if(!filename.ends_with(".hpp"))
      {
        continue;
      }

      std::fstream subDirFile{dirPath / filename, subDirFile.out};

      subDirFile << "#pragma once\n\n";
      subDirFile << includes;

      dirHeader << includes;
    }
  }
}