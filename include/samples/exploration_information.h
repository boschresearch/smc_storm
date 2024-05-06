/*
 * Copyright (c) 2024 Robert Bosch GmbH and its subsidiaries
 * 
 * This file is part of smc_storm.
 * 
 * smc_storm is free software: you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * 
 * smc_storm is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with smc_storm.
 * If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <limits>
#include <optional>
#include <unordered_map>
#include <vector>

#include <storm/solver/OptimizationDirection.h>

#include <storm/generator/CompressedState.h>

#include <storm/storage/BoostTypes.h>
#include <storm/storage/SparseMatrix.h>

#include "properties/state_info.h"

namespace smc_storm::samples {
template<typename StateType, typename ValueType>
class ExplorationInformation {
   public:
    typedef properties::StateInfoType StateInfoType;
    typedef StateType ActionType;
    typedef std::unordered_map<StateType, StateInfoType> StateInfoMap;
    typedef std::unordered_map<StateType, storm::generator::CompressedState> IdToStateMap;
    typedef typename IdToStateMap::const_iterator const_iterator;
    typedef std::vector<std::vector<storm::storage::MatrixEntry<StateType, ValueType>>> MatrixType;

    ExplorationInformation();

    const_iterator findUnexploredState(StateType const& state) const;

    const_iterator unexploredStatesEnd() const;

    void removeUnexploredState(const_iterator it);

    void addUnexploredState(StateType const& stateId, storm::generator::CompressedState const& compressedState);

    void assignStateToRowGroup(StateType const& state, ActionType const& rowGroup);

    StateType assignStateToNextRowGroup(StateType const& state);

    StateType getNextRowGroup() const;

    void newRowGroup(ActionType const& action);

    void newRowGroup();

    void terminateCurrentRowGroup();

    void moveActionToBackOfMatrix(ActionType const& action);

    StateType getActionCount() const;

    size_t getNumberOfUnexploredStates() const;

    size_t getNumberOfDiscoveredStates() const;

    StateType const& getRowGroup(StateType const& state) const;

    StateType const& getUnexploredMarker() const;

    bool isUnexplored(StateType const& state) const;

    /*!
     * @brief Check if the input state does not need to be expanded further
     * States that are not to expand are: VERIFY_PROPERTY, BREAK_CONDITION, IS_TERMINAL
     * @param state The ID of the state we need to check
     * @return true if the state is final, false otherwise
     */
    bool isTerminal(StateType const& state) const;

    /*!
     * @brief Return the StateInfo of the input state (if available)
     * @param state The ID of the state we are evaluating
     * @return Optionally, the StateInfo assigned to a specific state
     */
    StateInfoType getStateInfo(StateType const& state)  const;

    ValueType const& getStateReward(StateType const& stateId) const;

    ActionType const& getStartRowOfGroup(StateType const& group) const;

    size_t getRowGroupSize(StateType const& group) const;

    bool onlyOneActionAvailable(StateType const& group) const;

    /*!
     * @brief Assign a specific StateInfo to the provided state.
     * @param state The ID of the state to mark
     * @param info The new label to assign to a specific state: it overwrites.
     */
    void addStateInfo(StateType const& state, StateInfoType const info);

    void addStateReward(StateType const& stateId, ValueType const& stateReward);

    /*!
     * @brief Used for compatibility: assigns the IS_TERMINAL label to the provided state
     * @param state the ID of the state to mark
     */
    void addTerminalState(StateType const& state);

    std::vector<storm::storage::MatrixEntry<StateType, ValueType>>& getRowOfMatrix(ActionType const& row);

    std::vector<storm::storage::MatrixEntry<StateType, ValueType>> const& getRowOfMatrix(ActionType const& row) const;

    ValueType const& getActionReward(ActionType const& actionId) const;

    void addActionsToMatrix(size_t const& count);

    void addActionReward(ActionType const& actionId, ValueType const& actionReward);

   private:
    static constexpr ActionType _unexplored_marker{std::numeric_limits<ActionType>::max()};
    // Assigns to each actionId (row) a vector of (targetStateId, Likelihood) (of type MatrixEntry)
    MatrixType actionToTargetStates;

    // Assigns a reward to each action ID
    std::vector<ValueType> actionToReward;

    // This maps a a rowGroupId (related to a stateId via stateToRowGroupMapping) to the 1st actionId leaving from that state.
    // The last actionId associated to that rowGroupId will be (rowGroupIndices[rowGroupId+1] - 1)
    std::vector<StateType> rowGroupIndices;

    // Map each StateId to a rowGroupId (or unexploredMarker, if no rowGroupId yet)
    std::vector<StateType> stateToRowGroupMapping;

    // Assigns for each explored state a reward. Unexplored states will have infinite reward
    std::vector<ValueType> stateToReward;

    IdToStateMap unexploredStates;

    StateInfoMap statesInfo;
};
}  // namespace smc_storm::samples
