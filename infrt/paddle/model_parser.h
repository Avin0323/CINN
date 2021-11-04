// Copyright (c) 2021 PaddlePaddle Authors. All Rights Reserved.
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

#pragma once
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "infrt/paddle/framework.pb.h"
#include "infrt/paddle/pb/block_desc.h"
#include "infrt/paddle/pb/op_desc.h"
#include "infrt/paddle/pb/program_desc.h"
#include "infrt/paddle/scope.h"
#include "infrt/paddle/tensor.h"

namespace infrt::paddle {
namespace framework_proto = ::paddle::framework::proto;

// Read a __model__ file.
std::unique_ptr<framework_proto::ProgramDesc> LoadProgram(const std::string& path, bool program_from_memory = false);

void LoadLoDTensor(std::istream& is, infrt::paddle::_Variable* var, const infrt::common::Target& target);

// Read a single file containing all the parameters.
void LoadParams(const std::string& path);

// Load a single parameter to an output tensor.
void LoadParam(const std::string& path, infrt::paddle::_Variable* out, const infrt::common::Target& target);

// LoDTensor to ostream
void TensorToStream(std::ostream& os, const infrt::paddle::_Tensor_& tensor);
void TensorFromStream(std::istream& is,
                      infrt::paddle::_Tensor_* tensor,
                      const infrt::common::Target& target = infrt::common::DefaultHostTarget());
void ReadBinaryFile(const std::string& filename, std::string* contents);

}  // namespace infrt::paddle
