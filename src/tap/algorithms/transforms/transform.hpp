/*
 * Copyright (c) 2022-2024 Advanced Robotics at the University of Washington <robomstr@uw.edu>
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

#ifndef TAPROOT_TRANSFORM_HPP_
#define TAPROOT_TRANSFORM_HPP_

#include "tap/algorithms/cmsis_mat.hpp"
#include "tap/algorithms/math_user_utils.hpp"

#include "angular_velocity.hpp"
#include "dynamic_orientation.hpp"
#include "dynamic_position.hpp"
#include "orientation.hpp"
#include "position.hpp"
#include "vector.hpp"

namespace tap::algorithms::transforms
{
/**
 * Represents a transformation from one coordinate frame to another.
 * A Static Transform from frame A to frame B defines a relationship between the two frames, such
 * that a spatial measurement in frame A can be represented equivalently in frame B by applying a
 * translational and rotational offset. This process is known as *applying* a transform.
 *
 * Static Transforms are specified as a translation and rotation of some "follower" frame relative
 * to some "base" frame. The "translation" is the follower frame's origin in base frame, and the
 * "rotation" is the follower frame's orientation relative to the base frame's orientation.
 *
 * Conceptually, translations are applied "before" rotations. This means that the origin of the
 * follower frame is entirely defined by the translation in the base frame, and the rotation serves
 * only to change the orientation of the follower frame's axes relative to the base frame.
 *
 * A Dynamic Transform is an extension of a Static Transform that can store linear velocity, linear
 * acceleration, and angular velocity. This class handles both, automatically determining whether it
 * is static or dynamic.
 *
 * Utilizes ARM's CMSIS matrix operations.
 */
class Transform
{
public:
    /**
     * @param rotation Initial rotation of this transformation.
     * @param position Initial translation of this transformation.
     */
    Transform(const Position& translation, const Orientation& rotation);
    Transform(Position&& translation, Orientation&& rotation);

    /**
     * @param rotation Initial rotation of this transformation.
     * @param position Initial translation of this transformation.
     */
    Transform(const CMSISMat<3, 1>& translation, const CMSISMat<3, 3>& rotation);
    Transform(CMSISMat<3, 1>&& translation, CMSISMat<3, 3>&& rotation);

    /**
     * Constructs rotations using XYZ Euler angles,
     * so rotations are applied in order of rx, ry, then rz.
     * As an example, for an x-forward, z-up coordinate system,
     * this is in the order of roll, pitch, then yaw.
     *
     * @param x: Initial x-component of the translation.
     * @param y: Initial y-component of the translation.
     * @param z: Initial z-component of the translation.
     * @param rx: Initial rotation angle about the x-axis.
     * @param ry: Initial rotation angle about the y-axis.
     * @param rz: Initial rotation angle about the z-axis.
     */
    Transform(float x, float y, float z, float rx, float ry, float rz);

    /**
     * @param translation Initial translation of this transformation.
     * @param rotation Initial rotation of this transformation.
     * @param velocity Translational velocity of this transformation.
     * @param acceleration Translational acceleration of this transformation.
     * @param angularVelocity Angular velocity pseudovector of this transformation.
     */
    Transform(
        const Position& translation,
        const Orientation& rotation,
        const Vector& velocity,
        const Vector& acceleration,
        const Vector& angularVelocity);

    /**
     * @param translation Initial translation of this transformation.
     * @param rotation Initial rotation of this transformation.
     * @param velocity Translational velocity of this transformation.
     * @param acceleration Translational acceleration of this transformation.
     * @param angularVelocity Angular velocity pseudovector of this transformation.
     */
    Transform(
        Position&& translation,
        Orientation&& rotation,
        Vector&& velocity,
        Vector&& acceleration,
        Vector&& angularVelocity);

    /**
     * @param rotation Initial rotation of this transformation.
     * @param position Initial translation of this transformation.
     */
    Transform(const DynamicPosition& dynamicPosition, const DynamicOrientation& dynamicOrientation);
    Transform(DynamicPosition&& dynamicPosition, DynamicOrientation&& dynamicOrientation);

    /**
     * @param translation Initial translation of this transformation.
     * @param rotation Initial rotation of this transformation.
     * @param velocity Translational velocity of this transformation.
     * @param acceleration Translational acceleration of this transformation.
     * @param angularVelocity Angular velocity skew symmetric matrix of this transformation.
     */
    Transform(
        const CMSISMat<3, 1>& translation,
        const CMSISMat<3, 3>& rotation,
        const CMSISMat<3, 1>& velocity,
        const CMSISMat<3, 1>& acceleration,
        const CMSISMat<3, 3>& angularVelocity);

    /**
     * @param translation Initial translation of this transformation.
     * @param rotation Initial rotation of this transformation.
     * @param velocity Translational velocity of this transformation.
     * @param acceleration Translational acceleration of this transformation.
     * @param angularVelocity Angular velocity skew symmetric matrix of this transformation.
     */
    Transform(
        CMSISMat<3, 1>&& translation,
        CMSISMat<3, 3>&& rotation,
        CMSISMat<3, 1>&& velocity,
        CMSISMat<3, 1>&& acceleration,
        CMSISMat<3, 3>&& angularVelocity);

    /**
     * Constructs rotations using XYZ Euler angles,
     * so rotations are applied in order of rx, ry, then rx.
     *
     * @param x:  Initial x-component of the translation.
     * @param y:  Initial y-component of the translation.
     * @param z:  Initial z-component of the translation.
     * @param vx: Initial x-component of the translational velocity.
     * @param vy: Initial y-component of the translational velocity.
     * @param vz: Initial z-component of the translational velocity.
     * @param ax: Initial x-component of the translational acceleration.
     * @param ay: Initial y-component of the translational acceleration.
     * @param az: Initial z-component of the translational acceleration.
     * @param rx: Initial rotation angle about the x-axis.
     * @param ry: Initial rotation angle about the y-axis.
     * @param rz: Initial rotation angle about the z-axis.
     * @param wx: Initial angular velocity about the x-axis.
     * @param wy: Initial angular velocity about the y-axis.
     * @param wz: Initial angular velocity about the z-axis.
     */
    Transform(
        float x,
        float y,
        float z,
        float vx,
        float vy,
        float vz,
        float ax,
        float ay,
        float az,
        float rx,
        float ry,
        float rz,
        float wx,
        float wy,
        float wz);

    /**
     * @brief Constructs an identity transform.
     */
    static inline Transform identity() { return Transform(0., 0., 0., 0., 0., 0.); }

    /**
     * @brief Apply this transform to a position.
     *
     * @param[in] position Position in base frame.
     * @return Position in follower frame.
     */
    Position apply(const Position& position) const;

    /**
     * @brief Rotates a vector in the base frame to a vector in the follower frame.
     *
     * Intended to be used for things like velocities and accelerations which represent the
     * difference between two positions in space, since both positions get translated the same way,
     * causing the translation to cancel out.
     *
     * @note Only accurate for static transforms!
     *
     * @param vector Vector as read by base frame.
     * @return Vector in follower frame's basis.
     */
    Vector apply(const Vector& vector) const;

    /**
     * @brief Brings a dynamic position in the base frame to one in the follower frame.
     */
    DynamicPosition apply(const DynamicPosition& dynamicPosition) const;

    /**
     * @brief Brings an orientation in the base frame to one in the follower frame.
     */
    Orientation apply(const Orientation& orientation) const;

    /**
     * @brief Brings a dynamic orientation in the base frame to one in the follower frame.
     */
    DynamicOrientation apply(const DynamicOrientation& dynamicOrientation) const;

    /**
     * @brief Updates the translation of the current transformation matrix.
     *
     * @param newTranslation updated position of follower in base frame.
     */
    inline void updateTranslation(const Position& newTranslation)
    {
        this->translation = newTranslation.coordinates();
    }

    /**
     * @brief Updates the translation of the current transformation matrix.
     *
     * @param newTranslation updated position of follower in base frame.
     */
    inline void updateTranslation(Position&& newTranslation)
    {
        this->translation = std::move(newTranslation.coordinates());
    }

    /**
     * @brief Updates the translation of the current transformation matrix.
     *
     * @param x new translation x-component.
     * @param y new translation y-component.
     * @param z new translation z-component.
     */
    inline void updateTranslation(float x, float y, float z)
    {
        this->translation = CMSISMat<3, 1>({x, y, z});
    }

    /**
     * @brief Updates the translation of the current transformation matrix.
     *
     * @param newTranslation updated position of follower in base frame.
     */
    inline void updateTranslation(const DynamicPosition& newTranslation)
    {
        this->translation = newTranslation.position;
        this->transVel = newTranslation.velocity;
        this->transAcc = newTranslation.acceleration;
    }

    /**
     * @brief Updates the translation of the current transformation matrix.
     *
     * @param newTranslation updated position of follower in base frame.
     */
    inline void updateTranslation(DynamicPosition&& newTranslation)
    {
        this->translation = std::move(newTranslation.position);
        this->transVel = std::move(newTranslation.velocity);
        this->transAcc = std::move(newTranslation.acceleration);
    }

    /**
     * @brief Updates the rotation of the current transformation matrix.
     *
     * @param newRotation updated orientation of follower frame in base frame.
     */
    inline void updateRotation(const Orientation& newRotation)
    {
        this->rotation = newRotation.matrix();
        this->tRotation = this->rotation.transpose();
    }

    /**
     * @brief Updates the rotation of the current transformation matrix.
     *
     * @param newRotation updated orientation of follower frame in base frame.
     */
    inline void updateRotation(Orientation&& newRotation)
    {
        this->rotation = std::move(newRotation.matrix());
        this->tRotation = this->rotation.transpose();
    }

    /**
     * @brief Updates the rotation of the current transformation matrix.
     * Takes rotation angles in the order of roll->pitch->yaw.
     *
     * @param roll updated rotation angle about the x-axis.
     * @param pitch updated rotation angle about the y-axis.
     * @param yaw updated rotation angle about the z-axis.
     */
    void updateRotation(float roll, float pitch, float yaw)
    {
        this->rotation = Orientation(roll, pitch, yaw).matrix();
        this->tRotation = this->rotation.transpose();
    }

    /**
     * @brief Updates the rotation of the current transformation matrix.
     *
     * @param newRotation updated orientation of follower frame in base frame.
     */
    inline void updateRotation(const DynamicOrientation& newRotation)
    {
        this->rotation = newRotation.orientation;
        this->tRotation = this->rotation.transpose();
        this->angVel = newRotation.angularVelocity;
    }

    /**
     * @brief Updates the rotation of the current transformation matrix.
     *
     * @param newRotation updated orientation of follower frame in base frame.
     */
    inline void updateRotation(DynamicOrientation&& newRotation)
    {
        this->rotation = std::move(newRotation.orientation);
        this->tRotation = this->rotation.transpose();
        this->angVel = std::move(newRotation.angularVelocity);
    }

    /**
     * @brief Updates the velocity of the current transform.
     *
     * @param newVelocity updated velocity of follower in base frame.
     */
    inline void updateVelocity(const Vector& newVelocity)
    {
        this->transVel = newVelocity.coordinates();
        checkDynamic();
    }

    /**
     * @brief Updates the velocity of the current transform.
     *
     * @param newVelocity updated velocity of follower in base frame.
     */
    inline void updateVelocity(Vector&& newVelocity)
    {
        this->transVel = std::move(newVelocity.coordinates());
        checkDynamic();
    }

    /**
     * @brief Updates the velocity of the current transform.
     *
     * @param vx new velocity x-component.
     * @param vy new velocity y-component.
     * @param vz new velocity z-component.
     */
    inline void updateVelocity(float vx, float vy, float vz)
    {
        this->transVel = CMSISMat<3, 1>({vx, vy, vz});
        checkDynamic();
    }

    /**
     * @brief Updates the acceleration of the current transform.
     *
     * @param updateAcceleration updated acceleration of follower in base frame.
     */
    inline void updateAcceleration(const Vector& newAcceleration)
    {
        this->transVel = newAcceleration.coordinates();
        checkDynamic();
    }

    /**
     * @brief Updates the acceleration of the current transform.
     *
     * @param updateAcceleration updated acceleration of follower in base frame.
     */
    inline void updateAcceleration(Vector&& newAcceleration)
    {
        this->transVel = std::move(newAcceleration.coordinates());
        checkDynamic();
    }

    /**
     * @brief Updates the acceleration of the current transform.
     *
     * @param ax new acceleration x-component.
     * @param ay new acceleration y-component.
     * @param az new acceleration z-component.
     */
    inline void updateAcceleration(float ax, float ay, float az)
    {
        this->transAcc = CMSISMat<3, 1>({ax, ay, az});
        checkDynamic();
    }

    /**
     * @brief Updates the angular velocity of the current transform.
     *
     * @param updateAngularVelocity updated angular velocity of follower in base frame.
     */
    inline void updateAngularVelocity(const Vector& newAngularVelocity)
    {
        this->angVel = AngularVelocity::skewMatFromAngVel(
            newAngularVelocity.x(),
            newAngularVelocity.y(),
            newAngularVelocity.z());
        checkDynamic();
    }

    /**
     * @brief Updates the angular velocity of the current transform.
     *
     * @param updateAngularVelocity updated angular velocity of follower in base frame.
     */
    inline void updateAngularVelocity(Position&& newAngularVelocity)
    {
        this->angVel = AngularVelocity::skewMatFromAngVel(
            newAngularVelocity.x(),
            newAngularVelocity.y(),
            newAngularVelocity.z());
        checkDynamic();
    }

    /**
     * @brief Updates the angular velocity of the current transform.
     *
     * @param ax new angular velocity x-component.
     * @param ay new angular velocity y-component.
     * @param az new angular velocity z-component.
     */
    inline void updateAngularVelocity(float vr, float vp, float vy)
    {
        this->angVel = AngularVelocity::skewMatFromAngVel(vr, vp, vy);
        checkDynamic();
    }

    /**
     * @return Inverse of this Transform.
     *
     * @note This is only instantaneously correct for dynamic transforms; It can no longer be
     * projected forward in time and behave the same way as the original. This is due to the now
     * reversed translation-rotation that would be required to truly mimic the motion of the
     * original. Ex: Consider a dynamic transform with only non-zero translation and angular
     * velocity. Projecting this forward will cause the follower frame to rotate around its origin.
     * Intuitively, one would expect the inverted transform to have its follower frame rotate around
     * the base frame origin. However, this circular translation can only be approximated here with
     * translational velocity/acceleration. The true inverse would need to be the composition of a
     * rotation *then* a translation.
     */
    Transform getInverse() const;

    /**
     * @brief Returns the composed transformation of the given transformations.
     *
     * @return Transformation from this transform's base frame to `second`'s follower frame.
     */
    Transform compose(const Transform& second) const;

    /**
     * @brief Returns the composed transformation of the given transformations, ignoring any time
     * derivatives.
     *
     * @return Static transformation from this transform's base frame to `second`'s
     * follower frame.
     */
    Transform composeStatic(const Transform& second) const;

    /**
     * @brief Projects this transform forward in time according to its translational
     * velocity/acceleration and angular velocity.
     *
     * @param dt Seconds to project forward (can be negative)
     * @return Projected transform
     */
    Transform projectForward(float dt) const;

    /* Getters */
    inline Position getTranslation() const { return Position(translation); };

    inline Vector getVelocity() const { return Vector(transVel); };

    inline Vector getAcceleration() const { return Vector(transAcc); };

    inline DynamicPosition getDynamicTranslation() const
    {
        return DynamicPosition(translation, transVel, transAcc);
    };

    inline Orientation getRotation() const { return Orientation(rotation); }

    inline Vector getAngularVel() const
    {
        return Vector(getRollVelocity(), getPitchVelocity(), getYawVelocity());
    }

    inline DynamicOrientation getDynamicOrientation() const
    {
        return DynamicOrientation(rotation, angVel);
    };

    /**
     * @brief Get the roll of this transformation
     */
    float getRoll() const;

    /**
     * @brief Get the pitch of this transformation
     */
    float getPitch() const;

    /**
     * @brief Get the yaw of this transformation
     */
    float getYaw() const;

    /**
     * @brief Get the roll velocity of this transformation
     */
    float getRollVelocity() const;

    /**
     * @brief Get the pitch velocity of this transformation
     */
    float getPitchVelocity() const;

    /**
     * @brief Get the yaw velocity of this transformation
     */
    float getYawVelocity() const;

    /**
     * @brief Get the x-component of this transform's translation
     */
    inline float getX() const { return this->translation.data[0]; }

    /**
     * @brief Get the y-component of this transform's translation
     */
    inline float getY() const { return this->translation.data[1]; }

    /**
     * @brief Get the z-component of this transform's translation
     */
    inline float getZ() const { return this->translation.data[2]; }

    /**
     * @brief Get the x-component of this transform's linear velocity
     */
    inline float getXVel() const { return this->transVel.data[0]; }

    /**
     * @brief Get the y-component of this transform's linear velocity
     */
    inline float getYVel() const { return this->transVel.data[1]; }

    /**
     * @brief Get the z-component of this transform's linear velocity
     */
    inline float getZVel() const { return this->transVel.data[2]; }

    /**
     * @brief Get the x-component of this transform's linear acceleration
     */
    inline float getXAcc() const { return this->transAcc.data[0]; }

    /**
     * @brief Get the y-component of this transform's linear acceleration
     */
    inline float getYAcc() const { return this->transAcc.data[1]; }

    /**
     * @brief Get the z-component of this transform's linear acceleration
     */
    inline float getZAcc() const { return this->transAcc.data[2]; }

    /**
     * @brief Whether there are any non-zero derivatives.
     */
    inline bool isDynamic() const { return dynamic; }

private:
    bool dynamic{true};

    /**
     * Translation vector.
     */
    CMSISMat<3, 1> translation;

    /**
     * Translational velocity vector.
     */
    CMSISMat<3, 1> transVel;

    /**
     * Translational acceleration vector.
     */
    CMSISMat<3, 1> transAcc;

    /**
     * Rotation matrix.
     */
    CMSISMat<3, 3> rotation;

    /**
     * Transpose of rotation matrix. Computed and stored at beginning
     * for use in other computations.
     *
     * The transpose of a rotation is its inverse.
     */
    CMSISMat<3, 3> tRotation;

    /**
     * Angular velocity skew matrix.
     */
    CMSISMat<3, 3> angVel;

    inline void checkDynamic()
    {
        dynamic = false;

        dynamic |=
            !(compareFloatClose(getXVel(), 0, 1e-5) && compareFloatClose(getYVel(), 0, 1e-5) &&
              compareFloatClose(getZVel(), 0, 1e-5));

        dynamic |=
            !(compareFloatClose(getXAcc(), 0, 1e-5) && compareFloatClose(getYAcc(), 0, 1e-5) &&
              compareFloatClose(getZAcc(), 0, 1e-5));

        dynamic |=
            !(compareFloatClose(getRollVelocity(), 0, 1e-5) &&
              compareFloatClose(getPitchVelocity(), 0, 1e-5) &&
              compareFloatClose(getYawVelocity(), 0, 1e-5));
    }
};  // class Transform
}  // namespace tap::algorithms::transforms

#endif  // TAPROOT_TRANSFORM_HPP_
