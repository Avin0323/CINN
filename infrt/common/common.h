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

#include <absl/strings/string_view.h>

#include "infrt/common/axis.h"
#include "infrt/common/macros.h"
#include "infrt/common/shared.h"
#include "infrt/common/target.h"
#include "infrt/common/type.h"

namespace infrt {

// export some general concepts.
using common::make_shared;
using common::Object;
using common::ref_count;
using common::Shared;

// Type related.
using common::Bool;
using common::Float;
using common::Int;
using common::UInt;
using common::Void;

using common::type_of;

using common::Target;
using common::Type;
using common::UnkTarget;

template <typename T>
T& Reference(const T* x) {
  return *const_cast<T*>(x);
}

static void CheckVarNameValid(const absl::string_view name) {
  CHECK(!name.empty());
  CHECK(name.find(' ') == std::string::npos &&   //
        name.find('.') == std::string::npos &&   //
        name.find('/') == std::string::npos &&   //
        name.find('\t') == std::string::npos &&  //
        name.find('\n') == std::string::npos &&  //
        name.find('\r') == std::string::npos)
      << "Some invalid character found";
  CHECK(!infrt::common::IsAxisNameReserved(std::string(name)))
      << "The name [" << name << "] is reserved for internal axis";
}

}  // namespace infrt
