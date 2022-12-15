#include <iostream>
#include <vector>
#include <filesystem>

#include <cling/Interpreter/Interpreter.h>
#include <cling/Interpreter/Value.h>

#include <jank/util/mapped_file.hpp>
#include <jank/read/lex.hpp>
#include <jank/read/parse.hpp>
#include <jank/runtime/context.hpp>
#include <jank/analyze/processor.hpp>
#include <jank/codegen/processor.hpp>
#include <jank/jit/processor.hpp>

int main(int const argc, char const **argv)
{
  if(argc < 2)
  {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    std::cerr << "Usage: " << argv[0] << " <file>\n";
    return 1;
  }

  jank::runtime::context rt_ctx;
  jank::jit::processor jit_prc;

  rt_ctx.eval_prelude(jit_prc);

  std::cout << rt_ctx.eval_file(argv[1], jit_prc)->to_string() << std::endl;

  //std::string line;
  //std::cout << "> " << std::flush;
  //while(std::getline(std::cin, line))
  //{
  //  jit_prc.eval_string(line);
  //  std::cout << "> " << std::flush;
  //}
}
