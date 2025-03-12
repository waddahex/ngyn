#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn::files;

TEST_CASE("read")
{
  SUBCASE("Returns empty string if file doesn't exist")
  {
    std::string content = ngyn::files::read("test.txt");
    CHECK(content.empty());
  }

  SUBCASE("Returns file content")
  {
    FilesResult code = ngyn::files::write("read/test.txt", "test data", {
      .recursive = true
    });
    std::string content = ngyn::files::read("read/test.txt");
    CHECK(content == "test data");
  }
}

TEST_CASE("write")
{
  for(auto dir : {"write"})
  {
    if(std::filesystem::exists(dir))
    {
      std::filesystem::remove_all(dir);
    }
  }

  SUBCASE("Returns FilesResult::InvalidParentDirectory")
  {
    FilesResult code = ngyn::files::write("write/test.txt", "test data");
    CHECK(code == FilesResult::InvalidParentDirectory);
  }

  SUBCASE("Returns FilesResult::FileDoesNotExist")
  {
    FilesResult code = ngyn::files::write("test.txt", "test data");
    CHECK(code == FilesResult::FileDoesNotExist);
  }

  SUBCASE("Return FilesResult::InvalidCharacter")
  {
    FilesResult code = ngyn::files::write("test<*>.txt", "test data", {
      .recursive = true
    });
    CHECK(code == FilesResult::InvalidCharacter);
  }

  SUBCASE("Create parent directories recursivelly if does not exist")
  {
    FilesResult code = ngyn::files::write("write/test.txt", "test data", {
      .recursive = true
    });
    CHECK(std::filesystem::exists("write"));
  }

  SUBCASE("Create file if it does not exist")
  {
    FilesResult code = ngyn::files::write("write/test.txt", "test data", {
      .recursive = true
    });
    CHECK(std::filesystem::exists("write/test.txt"));
  }

  SUBCASE("Shold keep the file's content")
  {
    ngyn::files::write("write/test2.txt", "test data", {
      .recursive = true
    });

    ngyn::files::write("write/test2.txt", "123", {
      .append = true,
      .recursive = true,
    });

    auto content = ngyn::files::read("write/test2.txt");
    CHECK(content == "test data123");
  }
}

TEST_CASE("createDir")
{
  for(auto dir : {"files", "files2"})
  {
    if(std::filesystem::exists(dir))
    {
      std::filesystem::remove_all(dir);
    }
  }

  SUBCASE("Returns FilesResult::Success if folder is created")
  {
    FilesResult code = ngyn::files::createDir("files");
    CHECK(code == FilesResult::Success);
  }

  SUBCASE("Returns FilesResult::InvalidParentDirectory if parent folder doesn't exist")
  {
    FilesResult code = ngyn::files::createDir("files2/myFolder");
    CHECK(code == FilesResult::InvalidParentDirectory);
  }

  SUBCASE("Returns FilesResult::InvalidCharacter if path name contains invalid characters")
  {
    FilesResult code = ngyn::files::createDir("files2<");
    CHECK(code == FilesResult::InvalidCharacter);
  }

  SUBCASE("Create directories recursivelly")
  {
    FilesResult code = ngyn::files::createDir("files2/test/final", {
      .recursive = true
    });

    CHECK(std::filesystem::exists("files2"));
    CHECK(std::filesystem::exists("files2/test"));
    CHECK(std::filesystem::exists("files2/test/final"));
  }
}