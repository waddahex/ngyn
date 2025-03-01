#include "files.hpp"

using namespace ngyn::files;

std::string ngyn::files::read(const std::filesystem::path &path)
{
  std::fstream file{path, file.in};

  ASSERT(file.is_open(), "Could not open file {}", path);

  std::stringstream content;
  content << file.rdbuf();

  return content.str();
}

FilesResult ngyn::files::write(const std::filesystem::path &path, const std::string &data, FilesOptions options)
{
  if(
    !options.recursive &&
    path.has_parent_path() &&
    !std::filesystem::exists(path.parent_path())
  )
  {
    return FilesResult::InvalidParentDirectory;
  }

  if(!options.recursive && !std::filesystem::exists(path))
  {
    return FilesResult::FileDoesNotExist;
  }

  if(
    options.recursive &&
    path.has_parent_path() &&
    !std::filesystem::exists(path.parent_path()))
  {
    FilesResult code = ngyn::files::createDir(path.parent_path(), options);
    if(code != FilesResult::Success) return code;
  }

  std::string invalidChars = "<>|*:?\"";
  for(auto invalidChar : invalidChars)
  {
    if(path.filename().string().find(invalidChar) != std::string::npos)
    {
      return FilesResult::InvalidCharacter;
    }
  }

  std::ios::openmode mode = std::ios::out;

  if(options.append)
  {
    mode |= std::ios::app;
  }

  std::fstream file{path, mode};

  file << data;

  return FilesResult::Success;
}

FilesResult ngyn::files::createDir(const std::filesystem::path &path, FilesOptions options)
{
  if(
    !options.recursive &&
    path.has_parent_path() &&
    !std::filesystem::exists(path.parent_path())
  )
  {
    return FilesResult::InvalidParentDirectory;
  }

  std::string strPath(path.string());
  
  if(path.has_root_path())
  {
    strPath = ngyn::strings::replace(strPath, path.root_name().string(), "");
  }

  std::string invalidChars = "<>|*:?\"";
  for(auto invalidChar : invalidChars)
  {
    if(strPath.find(invalidChar) != std::string::npos)
    {
      return FilesResult::InvalidCharacter;
    }
  }

  if(path.has_parent_path() && !std::filesystem::exists(path.parent_path()))
  {
    FilesResult code = ngyn::files::createDir(path.parent_path(), options);
    if(code != FilesResult::Success) return code;
  }

  std::filesystem::create_directory(path);

  return FilesResult::Success;
}