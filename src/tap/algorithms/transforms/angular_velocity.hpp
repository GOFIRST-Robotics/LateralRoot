/*
 * Copyright (c) 2025-2025 Advanced Robotics at the University of Washington <robomstr@uw.edu>
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

#ifndef TAPROOT_ANGULAR_VELOCITY_HPP_
#define TAPROOT_ANGULAR_VELOCITY_HPP_

#include "tap/algorithms/cmsis_mat.hpp"

namespace tap::algorithms::transforms
{
class AngularVelocity
{
public:
    inline AngularVelocity(const float rollVel, const float pitchVel, const float yawVel)
        : matrix_(skewMatFromAngVel(rollVel, pitchVel, yawVel))
    {
    }

    /* rvalue reference */
    inline AngularVelocity(AngularVelocity&& other) : matrix_(std::move(other.matrix_)) {}

    /* Costly; use rvalue reference whenever possible */
    inline AngularVelocity(AngularVelocity& other) : matrix_(CMSISMat(other.matrix_)) {}

    /* Costly; use rvalue reference whenever possible */
    inline AngularVelocity(const CMSISMat<3, 3>& matrix) : matrix_(matrix) {}

    inline AngularVelocity(CMSISMat<3, 3>&& matrix) : matrix_(std::move(matrix)) {}

    /**
     * @brief Get the roll velocity
     */
    inline float getRollVelocity() const { return matrix_.data[0 * 3 + 2]; }

    /**
     * @brief Get the pitch velocity
     */
    inline float getPitchVelocity() const { return -matrix_.data[1 * 3 + 2]; }

    /**
     * @brief Get the yaw velocity
     */
    inline float getYawVelocity() const { return -matrix_.data[0 * 3 + 1]; }

    const inline CMSISMat<3, 3>& matrix() const { return matrix_; }

    /**
     * Generates a 3x3 skew matrix from euler angle velocities (in radians/sec)
     */
    inline static CMSISMat<3, 3> skewMatFromAngVel(const float wx, const float wy, const float wz)
    {
        return tap::algorithms::CMSISMat<3, 3>({0, -wz, wy, wz, 0, -wx, -wy, wx, 0});
    }

    friend class Transform;
    friend class DynamicOrientation;

private:
    CMSISMat<3, 3> matrix_;
};  // class AngularVelocity
}  // namespace tap::algorithms::transforms

#endif  // TAPROOT_ANGULAR_VELOCITY_HPP_
