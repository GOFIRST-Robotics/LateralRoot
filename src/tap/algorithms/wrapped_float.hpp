/*
 * Copyright (c) 2020-2023 Advanced Robotics at the University of Washington <robomstr@uw.edu>
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

#ifndef TAPROOT_WRAPPED_FLOAT_HPP_
#define TAPROOT_WRAPPED_FLOAT_HPP_

#include <assert.h>

#include <cmath>

#include <modm/architecture/interface/assert.hpp>
#include <modm/math/utils.hpp>

#include "math_user_utils.hpp"

namespace tap
{
namespace algorithms
{
/**
 * Wraps a float to allow easy comparison and manipulation of sensor readings
 * that wrap (e.g. 0 to 360). Lower bound is "inclusive" and upper bound is "exclusive".
 * The range would be represented as [0, 360).
 *
 * For bounds 0 - 10, logically:
 *   - 10 + 1 == 1
 *   - 0 - 1 == 9
 *   - 0 == 10
 */
class WrappedFloat
{
public:
    /**
     * @param[in] value: value to initialize with (doesn't have to be wrapped)
     * @param[in] lowerBound: lower wrapping bound, must be less than `upperBound`
     * @param[in] lowerBound: upper wrapping bound, must be higher than `lowerBound`
     */
    WrappedFloat(float value, float lowerBound, float upperBound);

    inline WrappedFloat withSameBounds(const float value) const
    {
        return WrappedFloat(value, this->lowerBound, this->upperBound);
    }

    // Overloaded Operators ----------------

    /**
     * Two WrappedFloats are considered equal if their wrapped values are equal. This does not
     * account for floating point imprecision, so for robust equality checks, `minDifference` should
     * be used.
     *
     * @param[in] other: The WrappedFloat to be compared `this` WrappedFloat.
     * @throws: An assertion error if the two WrappedFloats have different lower and upper bounds.
     */
    bool operator==(const WrappedFloat& other) const;

    /**
     * Adds a WrappedFloat to `this` WrappedFloat given they have the same lower and
     * upper bounds.
     *
     * @param[in] other: The WrappedFloat to be added to `this` WrappedFloat.
     * @throws: An assertion error if the two WrappedFloats have different lower and upper bounds.
     */
    void operator+=(const WrappedFloat& other);

    /**
     * Subtracts a WrappedFloat from `this` WrappedFloat given they have the same lower and
     * upper bounds.
     *
     * @param[in] other: The WrappedFloat to be subtracted from `this` WrappedFloat.
     * @throws: An assertion error if the two WrappedFloats have different lower and upper bounds.
     */
    void operator-=(const WrappedFloat& other);

    /**
     * Adds a given WrappedFloat to `this` WrappedFloat given they have the same lower and upper
     * bounds, returning the resultant WrappedFloat.
     *
     * @param[in] other: The WrappedFloat to be added with `this` WrappedFloat.
     * @return: A new WrappedFloat with the additive value of `other` and `this`.
     * @throws: An assertion error if the two WrappedFloats have different lower and upper bounds.
     */
    WrappedFloat operator+(const WrappedFloat& other) const;

    /**
     * Subtracts a given WrappedFloat from `this` WrappedFloat given they have the same lower and
     * upper bounds, returning the resultant WrappedFloat.
     *
     * @param[in] other: The WrappedFloat to be subtracted from `this` WrappedFloat.
     * @return: A new WrappedFloat with the subtractive value of `other` from `this`.
     * @throws: An assertion error if the two WrappedFloats have different lower and upper bounds.
     */
    WrappedFloat operator-(const WrappedFloat& other) const;

    /**
     * Adds a WrappedFloat to `this` WrappedFloat given they have the same lower and
     * upper bounds.
     *
     * @param[in] other: The WrappedFloat to be added to `this` WrappedFloat.
     * @throws: An assertion error if the two WrappedFloats have different lower and upper bounds.
     */
    void operator+=(float other);

    /**
     * Subtracts a WrappedFloat from `this` WrappedFloat given they have the same lower and
     * upper bounds.
     *
     * @param[in] other: The WrappedFloat to be subtracted from `this` WrappedFloat.
     * @throws: An assertion error if the two WrappedFloats have different lower and upper bounds.
     */
    void operator-=(float other);

    /**
     * Adds a given WrappedFloat to `this` WrappedFloat given they have the same lower and upper
     * bounds, returning the resultant WrappedFloat.
     *
     * @param[in] other: The WrappedFloat to be added with `this` WrappedFloat.
     * @return: A new WrappedFloat with the additive value of `other` and `this`.
     * @throws: An assertion error if the two WrappedFloats have different lower and upper bounds.
     */
    WrappedFloat operator+(float other) const;

    /**
     * Subtracts a given WrappedFloat from `this` WrappedFloat given they have the same lower and
     * upper bounds, returning the resultant WrappedFloat.
     *
     * @param[in] other: The WrappedFloat to be subtracted from `this` WrappedFloat.
     * @return: A new WrappedFloat with the subtractive value of `other` from `this`.
     * @throws: An assertion error if the two WrappedFloats have different lower and upper bounds.
     */
    WrappedFloat operator-(float other) const;

    /**
     * Finds the minimum difference against another wrapped value. Can be thought of as the minimum
     * distance between two points on a circle's perimeter.
     *
     * @param[in] other: The WrappedFloat to compute the minDifference with.
     * @return: A float with the signed minimum distance.
     * @throws: An assertion error if the two WrappedFloats have different lower and upper bounds.
     */
    float minDifference(const WrappedFloat& other) const;

    /**
     * Finds the minimum difference against another value. Can be thought of as the minimum
     * distance between two points on a circle's perimeter.
     *
     * @param[in] unwrappedValue: The float to compute the minDifference with. It's wrapped before
     *      computing
     * @return: A float with the signed minimum distance.
     * @throws: An assertion error if the two WrappedFloats have different lower and upper bounds.
     */
    float minDifference(const float& unwrappedValue) const;

    /**
     * Interpolates along the smallest difference with another WrappedFloat.
     *
     * @param[in] other: The WrappedFloat to interpolate between.
     * @param[in] alpha: A float between 0-1 (0 returns this WrappedFloat's value, 1 returns the
     *      other's)
     * @throws: An assertion error if the two WrappedFloats have different lower and upper bounds.
     */
    WrappedFloat minInterpolate(const WrappedFloat& other, float alpha) const;

    /**
     * Shifts both bounds by the specified amount.
     *
     * @param[in] shiftMagnitude the amount to add to each bound.
     */
    void shiftBounds(float shiftMagnitude);

    /**
     * Limits the passed WrappedFloat between the closest of the
     * min or max value if outside the min and max value's wrapped range.
     *
     * The min and max must have the same wrapped bounds as the valueToLimit.
     *
     *
     * For example given a value wrapped from -10 to 10, with the following
     * conditions:
     * - valueToLimit: 5, min: 1, max: 4, returns 4.
     * - valueToLimit: 9, min: 1, max: 3, returns 1 (since valueToLimit is closest to 1).
     * - valueToLimit: 9, min: 2, max: 1, returns 9 (since the range between min and max
     *                 starts at 2, goes up to 9, then wraps around to 1).
     *
     * @param[in] valueToLimit the WrappedFloat whose value it is to limit
     * @param[in] min the WrappedFloat with the same bounds as valueToLimit that
     *      valueToLimit will be limited below.
     * @param[in] max the WrappedFloat with the same bounds as valueToLimit that
     *      valueToLimit will be limited above.
     * @param[out] status the status result (what operation the limitValue function performed). The
     * status codes are described below:
     *  - 0: No limiting performed
     *  - 1: Limited to min value
     *  - 2: Limited to max value
     * @return the limited value.
     * @throws: An assertion error if the WrappedFloats have different lower and upper bounds.
     */
    static float limitValue(
        const WrappedFloat& valueToLimit,
        const WrappedFloat& min,
        const WrappedFloat& max,
        int* status);

    /**
     * Runs the limitValue function from above, wrapping the min and max passed in to
     * the same bounds as those of valueToLimit's.
     *
     * @see limitValue.
     * @param[in] valueToLimit the WrappedFloat whose value it is to limit
     * @param[in] min the WrappedFloat with the same bounds as valueToLimit that
     *      valueToLimit will be limited below.
     * @param[in] max the WrappedFloat with the same bounds as valueToLimit that
     *      valueToLimit will be limited above.
     * @param[out] status the status result (what operation the limitValue function performed). The
     * status codes are described below:
     *  - 0: No limiting performed
     *  - 1: Limited to min value
     *  - 2: Limited to max value
     * @return the limited value.
     * @throws: An assertion error if the WrappedFloats have different lower and upper bounds.
     */
    static float limitValue(
        const WrappedFloat& valueToLimit,
        const float min,
        const float max,
        int* status);

    /**
     * Checks whether `this` is within the wrapped range defined from `lowerBound` to `upperBound`.
     * For example given a value wrapped from 0 to 10, with the following conditions:
     * - this: 9, min: 3, max: 7, returns false.
     * - this: 5, min: 3, max: 7, returns true.
     * - this: 9, min: 7, max: 3, returns true.
     * - this: 5, min: 7, max: 3, returns false.
     *
     * @param[in] lowerBound
     * @param[in] upperBound
     * @return whether `this` is within the specified range
     * @throws: An assertion error if the WrappedFloats themselves have different bounds.
     */
    bool withinRange(const WrappedFloat& lowerBound, const WrappedFloat& upperBound) const;

    /**
     * Calculates how much of the two given wrapped ranges overlap. If mentally visualizing on a
     * circle, this method takes two arbitrary arcs on the perimeter and returns the length of the
     * overlapping portion(s).
     *
     * @param[in] lowerA the first range's lower bound
     * @param[in] upperA the first range's upper bound
     * @param[in] lowerB the second range's lower bound
     * @param[in] upperB the second range's upper bound
     * @return the total length of the overlapping region(s) as a float
     * @throws: An assertion error if the WrappedFloats themselves have different bounds.
     */
    static float rangeOverlap(
        const WrappedFloat& lowerA,
        const WrappedFloat& upperA,
        const WrappedFloat& lowerB,
        const WrappedFloat& upperB);

    // Getters/Setters ----------------

    /**
     * Returns the unwrapped value.
     */
    inline float getUnwrappedValue() const
    {
        return wrapped + (upperBound - lowerBound) * revolutions;
    };

    /**
     * Returns the wrapped value.
     */
    inline float getWrappedValue() const { return wrapped; };

    /**
     * Sets the wrapped value.
     */
    inline void setWrappedValue(float newWrappedValue)
    {
        this->wrapped = newWrappedValue;
        wrapValue();
    };

    /**
     * Sets the unwrapped value.
     */
    inline void setUnwrappedValue(float newUnwrappedValue)
    {
        this->wrapped = newUnwrappedValue;
        this->revolutions = 0;
        wrapValue();
    };

    inline WrappedFloat getNormalized() const
    {
        WrappedFloat out(*this);
        out.revolutions = 0;
        return out;
    }

    /**
     *
     */
    inline int getRevolutions() const { return revolutions; };

    /**
     * Returns the value's upper bound.
     */
    inline float getUpperBound() const { return upperBound; };

    /**
     * Returns the value's lower bound.
     */
    inline float getLowerBound() const { return lowerBound; };

    /**
     * Maximum value between floats representing bounds at which
     * they're considered to be "equal" for assertions.
     */
    static constexpr float EPSILON = 1E-8;

private:
    /**
     * The wrapped value. Guaranteed to be between lower and upper bound.
     */
    float wrapped;

    /**
     * Number of total revolutions.
     */
    int revolutions{0};

    /**
     * The lower bound to wrap around.
     */
    float lowerBound;

    /**
     * The upper bound to wrap around.
     */
    float upperBound;

    /**
     * Helper function for wrapping value within bounds.
     */
    void wrapValue();

    inline static void assertBoundsEqual(const WrappedFloat& a, const WrappedFloat& b)
    {
        modm_assert(
            compareFloatClose(a.getLowerBound(), b.getLowerBound(), EPSILON),
            "WrappedFloat::assertBoundsEqual",
            "Lower bounds do not match");
        modm_assert(
            compareFloatClose(a.getUpperBound(), b.getUpperBound(), EPSILON),
            "WrappedFloat::assertBoundsEqual",
            "Upper bounds do not match");
    }

    inline void assertBoundsEqual(const WrappedFloat& other) const
    {
        assertBoundsEqual(*this, other);
    }

};  // class WrappedFloat

/**
 * Represents an angle in radians.
 */
class Angle : public WrappedFloat
{
public:
    inline Angle(const float value) : WrappedFloat(value, 0, M_TWOPI){};

    static inline WrappedFloat fromDegrees(const float degrees)
    {
        return Angle(modm::toRadian(degrees));
    }
};

}  // namespace algorithms

}  // namespace tap

#endif  // TAPROOT_WRAPPED_FLOAT_HPP_