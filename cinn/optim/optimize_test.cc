// Copyright (c) 2021 CINN Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cinn/optim/optimize.h"

#include <gtest/gtest.h>

#include "cinn/cinn.h"
#include "cinn/ir/ir_printer.h"
#include "cinn/utils/string.h"

namespace cinn {
namespace optim {

TEST(Optimize, Unroll) {
  Placeholder<float> A("A", {100, 20});

  auto C      = Compute({Expr(100), Expr(20)}, [&](Var i, Var j) { return A(i, j) + 1.f; });
  auto stages = CreateStages({C});

  stages[C]->Split(1, 5);
  stages[C]->Unroll(2);

  auto func = Lower("matmul", stages, {A, C});

  auto out = R"ROC(
{
  for (i, 0, 100)
  {
    for (j_outer, 0, 4)
    {
      tensor[i, (5 * j_outer)] = (1 + A[i, (5 * j_outer)])
      tensor[i, (1 + (5 * j_outer))] = (1 + A[i, (1 + (5 * j_outer))])
      tensor[i, (2 + (5 * j_outer))] = (1 + A[i, (2 + (5 * j_outer))])
      tensor[i, (3 + (5 * j_outer))] = (1 + A[i, (3 + (5 * j_outer))])
      tensor[i, (4 + (5 * j_outer))] = (1 + A[i, (4 + (5 * j_outer))])
    }
  }
}
)ROC";

  EXPECT_EQ(utils::Trim(out), utils::Trim(utils::GetStreamCnt(func->body)));
}

}  // namespace optim
}  // namespace cinn
