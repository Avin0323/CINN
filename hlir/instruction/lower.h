#pragma once
#include <cinn/ir/node.h>
#include <memory>
#include <string>
#include <vector>
#include "cinn/cinn.h"
#include "cinn/ir/ir.h"
#include "cinn/lang/module.h"
#include "hlir/instruction/computation.h"
#include "hlir/instruction/instruction.h"
#include "hlir/instruction/module.h"
#include "hlir/instruction/scope.h"

namespace hlir {
namespace instruction {

/**
 * Lower the HLIR module to CINN module.
 */
std::unique_ptr<cinn::lang::Module> Lower(const Module& module);

/**
 * Lower an HLIR Module to CINN Module.
 */
class ModuleLower {
 public:
  ModuleLower() = default;

  //! Lower a module.
  cinn::Module operator()(const Module* module);

 private:
  //! Lower a computation.
  cinn::Expr LowerComputation(const Computation* computation);

 private:
  //! parent scope, the parent of all the computations' local scopes.
  Scope scope_;
};

/**
 * Lower an HLIR Computation to CINN expression.
 */
class ComputationLower {
 public:
  explicit ComputationLower(Scope* parent_scope) : scope_(parent_scope) {}

  /**
   * Lower a HLIR computation and get a CINN LoweredFunc expression.
   * @param computation The computation to lower.
   * @return The equivalent CINN LoweredFunc expression.
   */
  cinn::Expr operator()(const Computation* computation);

 private:
  void LowerInstruction(const Instruction* instr);

  void LowerDot(const Instruction* instr);

  void LowerCall(const Instruction* instr);

  void LowerCustomCall(const Instruction* instr);

  void LowerBinary(const Instruction* instr);

  void LowerParameter(const Instruction* instr);

 private:
  Scope scope_;
};

}  // namespace instruction
}  // namespace hlir