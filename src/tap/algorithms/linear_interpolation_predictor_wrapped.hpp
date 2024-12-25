/*
 * Copyright (c) 2020-2021 Advanced Robotics at the University of Washington <robomstr@uw.edu>
 *
 * This file is part of Taproot.
 *
 * Taproot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Taproot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Taproot.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TAPROOT_LINEAR_INTERPOLATION_PREDICTOR_WRAPPED_HPP_
#define TAPROOT_LINEAR_INTERPOLATION_PREDICTOR_WRAPPED_HPP_

#include <cstdint>

#include "wrapped_float.hpp"

namespace tap::algorithms
{
/**
 * An object that is similar in every respect to the `LinearInterpolationPredictor`
 * object except that it uses `WrappedFloat`'s instead.
 */
class LinearInterpolationPredictorWrapped
{
public:
    /**
     * @param[in] lowerBound Lower bound for linear interpolation WrappedFloat.
     * @param[in] upperBound Upper bound for linear interpolation WrappedFloat.
     */
    LinearInterpolationPredictorWrapped(float lowerBound, float upperBound);

    /**
     * Updates the interpolation using the newValue.
     *
     * @note Only call this when you receive a new value (use remote rx
     *      counter to tell when there is new data from the remote, for
     *      example).
     * @note This function should be called with increasing values of `currTime`.
     * @param[in] newValue The new data used in the interpolation.
     * @param[in] currTime The time that this function was called.
     */
    void update(float newValue, uint32_t currTime);

    /**
     * Returns the current value, that is: \f$y\f$ in the equation
     * \f$y=slope\cdot (currTime - lastUpdateCallTime) + previousValue\f$
     * in the units of whatever value you are inputting in the `update` function.
     *
     * @note Slope is defined by the previous two values passed into the `update`
     *      function, a period preceeding `lastUpdateCallTime`.
     * @note Use a millisecond-resolution timer, e.g.
     *      `tap::arch::clock::getTimeMilliseconds()`.
     * @param[in] currTime The current clock time, in ms.
     * @return The interpolated value.
     */
    float getInterpolatedValue(uint32_t currTime)
    {
        return WrappedFloat(
                   slope * static_cast<float>(currTime - lastUpdateCallTime) +
                       previousValue.getWrappedValue(),
                   previousValue.getLowerBound(),
                   previousValue.getUpperBound())
            .getWrappedValue();
    }

    /**
     * Resets the predictor. The slope will be reset to 0 and the initial values
     * and time will be used to initialize the predictor.
     *
     * @note It is highly recommended that you call this function before calling
     *      `update` to "initialize" the system.
     *
     * @param[in] initialValue The value to set the previous value to when resetting.
     * @param[in] initialTime The value to set the previous time to when resetting.
     */
    void reset(float initialValue, uint32_t initialTime);

private:
    uint32_t lastUpdateCallTime;  ///< The previous timestamp from when update was called.
    WrappedFloat previousValue;   ///< The previous data value.
    float slope;  ///< The current slope, calculated using the previous and most current data.
};                // class LinearInterpolationPredictorWrapped

}  // namespace tap::algorithms

#endif  // TAPROOT_LINEAR_INTERPOLATION_PREDICTOR_WRAPPED_HPP_