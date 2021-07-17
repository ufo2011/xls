// Copyright 2020 The XLS Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/status/status.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "xls/common/file/filesystem.h"
#include "xls/common/init_xls.h"
#include "xls/common/logging/logging.h"
#include "xls/interpreter/function_interpreter.h"
#include "xls/ir/function.h"
#include "xls/ir/ir_parser.h"
#include "xls/ir/package.h"
#include "xls/ir/value.h"
#include "xls/jit/ir_jit.h"

const char kUsage[] = R"(
Runs an IR function with a set of inputs through both the JIT and the
interpreter. Prints the first input which results in a mismatch between the JIT
and the interpreter. Returns a non-zer error code otherwise. Usage:

    find_failing_input_main --input-file=INPUT_FILE IR_FILE
)";

ABSL_FLAG(std::string, input_file, "",
          "Inputs to JIT and interpreter, one set per line. Each line should "
          "contain a semicolon-separated set of typed values. Cannot be "
          "specified with --input.");
ABSL_FLAG(
    std::string, test_only_inject_jit_result, "",
    "Test-only flag for injecting the result produced by the JIT. Used to "
    "force mismatches between JIT and interpreter for testing purposed.");

namespace xls {
namespace {

absl::Status RealMain(absl::string_view ir_path,
                      absl::string_view inputs_path) {
  XLS_ASSIGN_OR_RETURN(std::string ir_text, GetFileContents(ir_path));
  XLS_ASSIGN_OR_RETURN(std::string inputs_text, GetFileContents(inputs_path));
  XLS_ASSIGN_OR_RETURN(std::unique_ptr<Package> package,
                       Parser::ParsePackage(ir_text, ir_path));
  XLS_ASSIGN_OR_RETURN(Function * f, package->EntryFunction());

  std::vector<std::vector<Value>> inputs;
  for (const auto& args_line :
       absl::StrSplit(inputs_text, '\n', absl::SkipWhitespace())) {
    std::vector<Value> args;
    for (const absl::string_view& value_string :
         absl::StrSplit(args_line, ';')) {
      XLS_ASSIGN_OR_RETURN(Value arg, Parser::ParseTypedValue(value_string));
      args.push_back(arg);
    }
    inputs.push_back(args);
  }

  XLS_ASSIGN_OR_RETURN(std::unique_ptr<IrJit> jit, IrJit::Create(f));
  for (const std::vector<Value>& args : inputs) {
    Value jit_result;
    if (absl::GetFlag(FLAGS_test_only_inject_jit_result).empty()) {
      XLS_ASSIGN_OR_RETURN(jit_result, jit->Run(args));
    } else {
      XLS_ASSIGN_OR_RETURN(jit_result, Parser::ParseTypedValue(absl::GetFlag(
                                           FLAGS_test_only_inject_jit_result)));
    }
    XLS_ASSIGN_OR_RETURN(Value interpreter_result, InterpretFunction(f, args));
    if (jit_result != interpreter_result) {
      std::cout << absl::StrJoin(
          args, "; ", [](std::string* s, const Value& v) {
            absl::StrAppend(s, v.ToString(FormatPreference::kHex));
          });
      return absl::OkStatus();
    }
  }
  return absl::InvalidArgumentError(
      "No input found which results in a mismatch between the JIT and "
      "interpreter.");
}

}  // namespace
}  // namespace xls

int main(int argc, char** argv) {
  std::vector<absl::string_view> positional_arguments =
      xls::InitXls(kUsage, argc, argv);
  if (positional_arguments.empty()) {
    XLS_LOG(QFATAL) << absl::StreamFormat("Expected invocation: %s <ir-path>",
                                          argv[0]);
  }
  XLS_QCHECK(!absl::GetFlag(FLAGS_input_file).empty());
  XLS_QCHECK_OK(
      xls::RealMain(positional_arguments[0], absl::GetFlag(FLAGS_input_file)));
  return 0;
}
