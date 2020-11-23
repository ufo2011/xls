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

import xls.dslx.interpreter.tests.mod_simple_enum

fn main(et: mod_simple_enum::EnumTypeAlias) -> u32 {
  match et {
    mod_simple_enum::EnumTypeAlias::FIRST => u32:0,
    mod_simple_enum::EnumTypeAlias::SECOND => u32:1,
    _ => u32:2
  }
}

#![test]
fn test_main() {
  let _ = assert_eq(u32:0, main(mod_simple_enum::EnumType::FIRST));
  let _ = assert_eq(u32:1, main(mod_simple_enum::EnumType::SECOND));
  ()
}
