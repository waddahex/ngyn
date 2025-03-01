#pragma once

#include <string>
#include <fstream>
#include <filesystem>

#include "logger.hpp"
#include "strings.hpp"

namespace ngyn::files
{
  struct FilesOptions
  {
    bool append     = false; // Writes append the new content to the end of the file
    bool recursive  = false; // Create folders/file from path if it doesn't exist
  };

  enum FilesResult
  {
    Success,
    FileDoesNotExist,
    InvalidParentDirectory,
    InvalidCharacter
  };

  // Read the contents of a file
  std::string read(const std::filesystem::path &path);

  /**
   * Writes data to a file
   * 
   * @param path Relative or absolute path and/or filename
   * @param data Content that will be write to the file
   * @param options Write options
   * 
   * @return FilesResult
   */
  FilesResult write(const std::filesystem::path &path, const std::string &data, FilesOptions options = FilesOptions{});

  /**
   * Creates directories
   * 
   * @param path Relative or absolute path
   * @param options createDir options
   * 
   * @return FilesResult
   */
  FilesResult createDir(const std::filesystem::path &path, FilesOptions options = FilesOptions{});
};