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

#ifndef XLS_CODEGEN_COMBINATIONAL_GENERATOR_H_
#define XLS_CODEGEN_COMBINATIONAL_GENERATOR_H_

#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/status/statusor.h"
#include "xls/codegen/codegen_options.h"
#include "xls/codegen/module_signature.h"
#include "xls/codegen/name_to_bit_count.h"
#include "xls/codegen/vast.h"
#include "xls/ir/function.h"
#include "xls/ir/proc.h"

namespace xls {
namespace verilog {

// Emits the given function as a combinational Verilog module. If
// use_system_verilog is true the generated module will be SystemVerilog
// otherwise it will be Verilog. This adds a proc to the package which
// represents the combinational module. This proc is used for code generation.
absl::StatusOr<ModuleGeneratorResult> GenerateCombinationalModule(
    Function* func, bool use_system_verilog = true,
    absl::string_view module_name = "");

}  // namespace verilog
}  // namespace xls

#endif  // XLS_CODEGEN_COMBINATIONAL_GENERATOR_H_
