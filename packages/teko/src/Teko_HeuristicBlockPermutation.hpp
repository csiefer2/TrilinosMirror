// @HEADER
// *****************************************************************************
//      Teko: A package for block and physics based preconditioning
//
// Copyright 2010 NTESS and the Teko contributors.
// SPDX-License-Identifier: BSD-3-Clause
// *****************************************************************************
// @HEADER

#ifndef __Teko_HeuristicBlockPermutation_hpp__
#define __Teko_HeuristicBlockPermutation_hpp__

#include "Kokkos_Core.hpp"
#include "Teuchos_RCP.hpp"
#include "Teko_BlockedTpetraOperator.hpp"
#include "Teko_Utilities.hpp"
#include <map>
#include <set>
#include <vector>

namespace Teko {
using BlockNormsViewType     = Kokkos::View<double **, Kokkos::LayoutRight, Kokkos::HostSpace>;
using PermutationType        = std::map<int, int>;
using PermutationScoreType   = std::pair<PermutationType, double>;
using InversePermutationType = std::map<int, std::set<int>>;

using HeuristicMethodFunctionType = std::function<PermutationScoreType(
    const BlockNormsViewType &, const Teuchos::RCP<const Teuchos::Comm<int>> &)>;

PermutationScoreType generate_heuristic_permutation(
    const Teko::BlockedLinearOp &dof_blocked_matrix,
    Teuchos::RCP<Teuchos::ParameterList> parameters = Teuchos::null);
PermutationScoreType generate_heuristic_permutation(
    const Teuchos::RCP<Teko::TpetraHelpers::BlockedTpetraOperator> &dof_blocked_matrix,
    Teuchos::RCP<Teuchos::ParameterList> parameters = Teuchos::null);
PermutationScoreType generate_heuristic_permutation(
    const BlockNormsViewType &blockNorms, Teuchos::RCP<const Teuchos::Comm<int>> communicator,
    Teuchos::RCP<Teuchos::ParameterList> parameters = Teuchos::null);

std::vector<std::vector<Teko::GO>> construct_block_gids_from_permutation(
    const PermutationType &permutation, const std::vector<std::vector<Teko::GO>> &dof_gids);
std::vector<std::vector<Teko::GO>> construct_block_gids_from_permutation(
    const InversePermutationType &inverse_permutation,
    const std::vector<std::vector<Teko::GO>> &dof_gids);

InversePermutationType generate_inverse_permutation(const PermutationType &permutation);

struct SubblockParameters {
  Teuchos::RCP<Teuchos::ParameterList> mergedSolverParameters{Teuchos::null};
  std::string mergedSolverName{""};
  Teuchos::RCP<Teuchos::ParameterList> mergedPreconditionerParameters{Teuchos::null};
  std::string mergedPreconditionerName{""};
  std::vector<Teuchos::RCP<Teuchos::ParameterList>> subblockSolverParameters{};
  std::vector<std::string> subblockSolverNames{};
  std::vector<Teuchos::RCP<Teuchos::ParameterList>> subblockPreconditionerParameters{};
  std::vector<std::string> subblockPreconditionerNames{};
};

struct SubblockParametersBuilder {
  SubblockParameters data;
  SubblockParametersBuilder &set_merged_solver_params(Teuchos::RCP<Teuchos::ParameterList> params,
                                                      std::string solverName) {
    data.mergedSolverParameters = params;
    data.mergedSolverName       = solverName;
    return *this;
  };

  SubblockParametersBuilder &set_merged_preconditioner_params(
      Teuchos::RCP<Teuchos::ParameterList> params, std::string preconditionerName) {
    data.mergedPreconditionerParameters = params;
    data.mergedPreconditionerName       = preconditionerName;
    return *this;
  };

  SubblockParametersBuilder &set_subblock_solver_params(
      std::vector<Teuchos::RCP<Teuchos::ParameterList>> params,
      std::vector<std::string> solverNames) {
    data.subblockSolverParameters = params;
    data.subblockSolverNames      = solverNames;
    return *this;
  };

  SubblockParametersBuilder &set_subblock_preconditioner_params(
      std::vector<Teuchos::RCP<Teuchos::ParameterList>> params,
      std::vector<std::string> preconditionerNames) {
    data.subblockPreconditionerParameters = params;
    data.subblockPreconditionerNames      = preconditionerNames;
    return *this;
  };

  SubblockParameters build() const { return data; };
};

std::tuple<Teuchos::RCP<Teuchos::ParameterList>, std::string> default_merged_solver_parameters();

Teuchos::RCP<Teuchos::ParameterList> generate_parameters_from_permutation(
    const PermutationType &permutation, std::string inverseName,
    Teuchos::RCP<Teuchos::ParameterList> heuristicParameters = Teuchos::null,
    SubblockParameters                                       = {});

namespace impl {
std::pair<std::vector<int>, double> compute_min_ordering(
    const Teuchos::RCP<const Teuchos::Comm<int>> &communicator, double tMaxWalltime,
    const BlockNormsViewType &blockNorms, bool upper_triangular,
    std::function<double(const std::vector<int> &)> objective_function,
    Teuchos::RCP<Teuchos::FancyOStream> out);
}  // namespace impl

}  // namespace Teko

#endif