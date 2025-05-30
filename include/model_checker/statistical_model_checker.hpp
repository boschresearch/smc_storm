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

// Include map to prevent include error arising from including "storm/storage/jani/Property.h"
#include <map>
#include <vector>

#include <storm/storage/jani/Property.h>
#include <storm/storage/SymbolicModelDescription.h>

#include "model_checker/smc_plugin_instance.hpp"
#include "settings/smc_settings.hpp"

// Forward declaration of CheckResult type from Storm
namespace storm::modelchecker {
class CheckResult;
}  // namespace storm::modelchecker

namespace smc_storm::model_checker {
/*!
 * @brief Entry point for the statistical model checking pipeline. This class instantiates the model checking engine given the settings.
 */
class StatisticalModelChecker {
  public:
    StatisticalModelChecker() = delete;
    /*!
     * @brief Generate a new instance of the StatisticalModelChecker, using the provided settings to load the model and the property to
     * verify
     * @param model
     * @param property
     * @param settings
     */
    StatisticalModelChecker(
        const storm::storage::SymbolicModelDescription& model, const storm::jani::Property& property, const settings::SmcSettings& settings,
        const std::vector<SmcPluginInstance>& loaded_plugins = {});

    ~StatisticalModelChecker();

    /*!
     * @brief Print the loaded property in a human readable format
     */
    void printProperty() const;

    /*!
     * @brief Perform the model checking operation and store the result internally
     */
    void check();

    /*!
     * @brief Get the computed result
     * @tparam ResultType The type of result to retrieve (i.e. double for probabilities and rewards and bool for conditions)
     * @return The computed result, according to the ResultType template parameter
     */
    template <typename ResultType>
    ResultType getResult() const;

  private:
    std::reference_wrapper<const settings::SmcSettings> _settings;
    std::reference_wrapper<const storm::storage::SymbolicModelDescription> _model;
    std::reference_wrapper<const storm::jani::Property> _property;
    std::reference_wrapper<const std::vector<SmcPluginInstance>> _loaded_plugins;
    std::unique_ptr<storm::modelchecker::CheckResult> _result;
};
}  // namespace smc_storm::model_checker
