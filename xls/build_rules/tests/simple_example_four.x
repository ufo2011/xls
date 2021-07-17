// Copyright 2021 The XLS Authors
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

import xls.build_rules.tests.simple_example_one as one
import xls.build_rules.tests.simple_example_two as two
import xls.build_rules.tests.simple_example_three as three

fn four() -> u32 {
  u32: 4
}

pub fn result() -> u32 {
  one::one() + two::two() + three::three() + four()
}

#![test]
fn test_result_value() {
  assert_eq(result(), u32:10)
}