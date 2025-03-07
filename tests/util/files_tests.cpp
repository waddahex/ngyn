#include <catch2/catch_test_macros.hpp>
#include <ngyn/util/files.hpp>
#include <filesystem>

using namespace ngyn::files;

TEST_CASE("read", "[files]")
{
  SECTION("Returns empty string if file doesn't exist")
  {
    std::string content = ngyn::files::read("test.txt");
    REQUIRE(content.empty());
  }

  SECTION("Returns file content")
  {
    FilesResult code = ngyn::files::write("read/test.txt", "test data", {
      .recursive = true
    });
    std::string content = ngyn::files::read("read/test.txt");
    REQUIRE(content == "test data");
  }
}

TEST_CASE("write", "[files]")
{
  for(auto dir : {"write"})
  {
    if(std::filesystem::exists(dir))
    {
      std::filesystem::remove_all(dir);
    }
  }

  SECTION("Returns FilesResult::InvalidParentDirectory")
  {
    FilesResult code = ngyn::files::write("write/test.txt", "test data");
    REQUIRE(code == FilesResult::InvalidParentDirectory);
  }

  SECTION("Returns FilesResult::FileDoesNotExist")
  {
    FilesResult code = ngyn::files::write("test.txt", "test data");
    REQUIRE(code == FilesResult::FileDoesNotExist);
  }

  SECTION("Return FilesResult::InvalidCharacter")
  {
    FilesResult code = ngyn::files::write("test<*>.txt", "test data", {
      .recursive = true
    });
    REQUIRE(code == FilesResult::InvalidCharacter);
  }

  SECTION("Create parent directories recursivelly if does not exist")
  {
    FilesResult code = ngyn::files::write("write/test.txt", "test data", {
      .recursive = true
    });
    REQUIRE(std::filesystem::exists("write"));
  }

  SECTION("Create file if it does not exist")
  {
    FilesResult code = ngyn::files::write("write/test.txt", "test data", {
      .recursive = true
    });
    REQUIRE(std::filesystem::exists("write/test.txt"));
  }

  SECTION("Shold keep the file's content")
  {
    ngyn::files::write("write/test2.txt", "test data", {
      .recursive = true
    });

    ngyn::files::write("write/test2.txt", "123", {
      .append = true,
      .recursive = true,
    });

    auto content = ngyn::files::read("write/test2.txt");
    REQUIRE(content == "test data123");
  }
}

TEST_CASE("createDir", "[files]")
{
  for(auto dir : {"files", "files2"})
  {
    if(std::filesystem::exists(dir))
    {
      std::filesystem::remove_all(dir);
    }
  }

  SECTION("Returns FilesResult::Success if folder is created")
  {
    FilesResult code = ngyn::files::createDir("files");
    REQUIRE(code == FilesResult::Success);
  }

  SECTION("Returns FilesResult::InvalidParentDirectory if parent folder doesn't exist")
  {
    FilesResult code = ngyn::files::createDir("files2/myFolder");
    REQUIRE(code == FilesResult::InvalidParentDirectory);
  }

  SECTION("Returns FilesResult::InvalidCharacter if path name contains invalid characters")
  {
    FilesResult code = ngyn::files::createDir("files2<");
    REQUIRE(code == FilesResult::InvalidCharacter);
  }

  SECTION("Create directories recursivelly")
  {
    FilesResult code = ngyn::files::createDir("files2/test/final", {
      .recursive = true
    });

    REQUIRE(std::filesystem::exists("files2"));
    REQUIRE(std::filesystem::exists("files2/test"));
    REQUIRE(std::filesystem::exists("files2/test/final"));
  }
}