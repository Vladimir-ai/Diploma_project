
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <exception>
#include <filesystem>
#include "loader.h"
#include "qt_gui_properties.h"


AbstractInfoQtFrame * load_one_symbol(const std::string path, const std::string symbol_name)
{
  const std::string utility_name = "nm ";
  const std::string grep = " | grep -w ";
  const std::string wc = " | wc -l";
  std::string popen_cmd;
  const int buffer_size = 128;
  int symbol_amount = 0;
  std::string pipe_output;
  char buffer[buffer_size];
  AbstractInfoQtFrame *result = nullptr;
  void *dlopen_handle = nullptr;
  void *dlsym_result = nullptr;

  memset(buffer, 0, sizeof(buffer));

  popen_cmd = utility_name + path + grep + symbol_name + wc;

  auto pipe = popen(popen_cmd.c_str(), "r");

  while (pipe && !feof(pipe))
  {
    if (fgets(buffer, buffer_size, pipe) != nullptr)
    {
      pipe_output += buffer;
    }
  }

  if (pipe)
  {
    symbol_amount = atoi(pipe_output.c_str());
    pclose(pipe);
  }

  if (symbol_amount == 1)
  {
    dlopen_handle = dlopen(path.c_str(), RTLD_NODELETE | RTLD_NOW);

    if (dlopen_handle) {
      dlsym_result = dlsym(dlopen_handle, symbol_name.c_str());

      if (dlsym_result)
      {
        result = reinterpret_cast<Loader>(dlsym_result)();
      }

      dlclose(dlopen_handle);
    }
  }

  return result;
}


inline std::vector<AbstractInfoQtFrame *> try_to_load_all_libraries(const std::string path, const std::vector<std::string>& symbols)
{
  std::vector<AbstractInfoQtFrame *> result;

  for (const auto symbol_to_load : symbols)
  {
    result.push_back(load_one_symbol(path, symbol_to_load));
  }

  return result;
}


std::vector<AbstractInfoQtFrame *> read_all_files(const std::string path, const std::vector<std::string>& symbols)
{
  std::vector<AbstractInfoQtFrame *> result;

  if (std::filesystem::is_directory(path))
  {
    for (const auto & shared_lib : std::filesystem::directory_iterator(path))
    {
      if (shared_lib.path().extension() != ".so")
      {
        continue;
      }

      auto vector_result = try_to_load_all_libraries(shared_lib.path(), symbols);
      result.insert(std::end(result), std::begin(vector_result), std::end(vector_result));
    }
  }

  return result;
}
