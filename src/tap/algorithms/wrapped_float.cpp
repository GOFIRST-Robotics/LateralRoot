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

#include "wrapped_float.hpp"

namespace tap
{
namespace algorithms
{
WrappedFloat::WrappedFloat(const float value, const float lowerBound, const float upperBound)
    : wrapped(value),
      lowerBound(lowerBound),
      upperBound(upperBound)
{
    assert(upperBound > lowerBound);

    wrapValue();
}

bool WrappedFloat::operator==(const WrappedFloat& other) const
{
    assertBoundsEqual(other);

    return this->wrapped == other.wrapped;
}

void WrappedFloat::operator+=(const WrappedFloat& other)
{
    assertBoundsEqual(other);

    this->wrapped += other.wrapped;
    wrapValue();
    this->revolutions += other.revolutions;
}

void WrappedFloat::operator-=(const WrappedFloat& other)
{
    assertBoundsEqual(other);

    this->wrapped -= other.wrapped;
    wrapValue();
    this->revolutions -= other.revolutions;
}

WrappedFloat WrappedFloat::operator+(const WrappedFloat& other) const
{
    assertBoundsEqual(other);

    WrappedFloat temp(*this);
    temp += other;
    return temp;
}

WrappedFloat WrappedFloat::operator-(const WrappedFloat& other) const
{
    assertBoundsEqual(other);

    WrappedFloat temp = *this;
    temp -= other;
    return temp;
}

void WrappedFloat::operator+=(float value) { *this += this->withSameBounds(value); }

void WrappedFloat::operator-=(float value) { *this -= this->withSameBounds(value); }

WrappedFloat WrappedFloat::operator+(float value) const
{
    return *this + this->withSameBounds(value);
}

WrappedFloat WrappedFloat::operator-(float value) const
{
    return *this - this->withSameBounds(value);
}

float WrappedFloat::minDifference(const WrappedFloat& other) const
{
    assertBoundsEqual(other);

    float interval = this->getUpperBound() - this->getLowerBound();
    float difference_between = other.getWrappedValue() - this->getWrappedValue();
    float difference_around =
        difference_between + ((difference_between < 0) ? interval : -interval);
    return (abs(difference_between) < abs(difference_around)) ? difference_between
                                                              : difference_around;
}

float WrappedFloat::minDifference(const float& unwrappedValue) const
{
    return minDifference(this->withSameBounds(unwrappedValue));
}

WrappedFloat WrappedFloat::minInterpolate(const WrappedFloat& other, const float alpha) const
{
    assertBoundsEqual(other);

    return *this + (minDifference(other) * alpha);
}

void WrappedFloat::shiftBounds(const float shiftMagnitude)
{
    upperBound += shiftMagnitude;
    lowerBound += shiftMagnitude;
    wrapValue();
}

void WrappedFloat::wrapValue()
{
    float oldValue = wrapped;
    if (oldValue < lowerBound)
    {
        this->wrapped = upperBound + fmodf(oldValue - upperBound, upperBound - lowerBound);
    }
    else if (oldValue >= upperBound)
    {
        this->wrapped = lowerBound + fmodf(oldValue - lowerBound, upperBound - lowerBound);
    }
    this->revolutions += floor((oldValue - lowerBound) / (upperBound - lowerBound));
}

float WrappedFloat::limitValue(
    const WrappedFloat& valueToLimit,
    const float min,
    const float max,
    int* status)
{
    WrappedFloat minWrapped = valueToLimit.withSameBounds(min);
    WrappedFloat maxWrapped = valueToLimit.withSameBounds(max);
    return limitValue(valueToLimit, minWrapped, maxWrapped, status);
}

float WrappedFloat::limitValue(
    const WrappedFloat& valueToLimit,
    const WrappedFloat& min,
    const WrappedFloat& max,
    int* status)
{
    WrappedFloat::assertBoundsEqual(min, max);
    WrappedFloat::assertBoundsEqual(valueToLimit, min);

    if (min.getWrappedValue() == max.getWrappedValue())
    {
        return valueToLimit.getWrappedValue();
    }
    if (!valueToLimit.withinRange(min, max))
    {
        // valueToLimit is not "within" min and max
        float targetMinDifference = valueToLimit.minDifference(min);
        float targetMaxDifference = valueToLimit.minDifference(max);

        if (fabs(targetMinDifference) < fabs(targetMaxDifference))
        {
            *status = 1;
            return min.getWrappedValue();
        }
        else
        {
            *status = 2;
            return max.getWrappedValue();
        }
    }
    else
    {
        *status = 0;
        return valueToLimit.getWrappedValue();
    }
}

bool WrappedFloat::withinRange(const WrappedFloat& lowerBound, const WrappedFloat& upperBound) const
{
    return (lowerBound.getWrappedValue() < upperBound.getWrappedValue() &&
            (this->getWrappedValue() > lowerBound.getWrappedValue() &&
             this->getWrappedValue() < upperBound.getWrappedValue())) ||
           (lowerBound.getWrappedValue() > upperBound.getWrappedValue() &&
            (this->getWrappedValue() > lowerBound.getWrappedValue() ||
             this->getWrappedValue() < upperBound.getWrappedValue()));
}

float WrappedFloat::rangeOverlap(
    const WrappedFloat& lowerA,
    const WrappedFloat& upperA,
    const WrappedFloat& lowerB,
    const WrappedFloat& upperB)
{
    assertBoundsEqual(lowerA, upperA);
    assertBoundsEqual(upperA, lowerB);
    assertBoundsEqual(lowerB, upperB);

    float origin = lowerA.getLowerBound();
    float offset = lowerA.getWrappedValue() - origin;

    float upperAShifted = (upperA - offset).getWrappedValue();
    float lowerBShifted = (lowerB - offset).getWrappedValue();
    float upperBShifted = (upperB - offset).getWrappedValue();

    if (upperBShifted < lowerBShifted)
    {
        float leftRange = std::min(upperBShifted, upperAShifted);
        float rightRange = std::max(origin, upperAShifted - lowerBShifted);
        return leftRange + rightRange;
    }

    return std::max(0.0f, std::min(upperAShifted, upperBShifted) - std::max(origin, lowerBShifted));
}

}  // namespace algorithms

}  //  namespace tap