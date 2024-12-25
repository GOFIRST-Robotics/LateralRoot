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

#ifndef TAPROOT_REF_SERIAL_DATA_HPP_
#define TAPROOT_REF_SERIAL_DATA_HPP_

#include <cinttypes>

#include <modm/architecture/interface/register.hpp>

#include "modm/architecture/utils.hpp"

#include "dji_serial.hpp"

namespace tap::communication::serial
{
/**
 * Contains enum and struct definitions used in the `RefSerial` class.
 */
class RefSerialData
{
public:
    /**
     * When receiving data about other robots or sending data to other robots, they are
     * identified by the numbers below
     */
    enum class RobotId : uint16_t
    {
        INVALID = 0,

        RED_HERO = 1,
        RED_ENGINEER = 2,
        RED_SOLDIER_1 = 3,
        RED_SOLDIER_2 = 4,
        RED_SOLDIER_3 = 5,
        RED_DRONE = 6,
        RED_SENTINEL = 7,
        RED_DART = 8,
        RED_RADAR_STATION = 9,

        BLUE_HERO = 101,
        BLUE_ENGINEER = 102,
        BLUE_SOLDIER_1 = 103,
        BLUE_SOLDIER_2 = 104,
        BLUE_SOLDIER_3 = 105,
        BLUE_DRONE = 106,
        BLUE_SENTINEL = 107,
        BLUE_DART = 108,
        BLUE_RADAR_STATION = 109
    };

    /// @return `true` if the specified `id` is on the blue team, `false` if on the red team.
    static inline bool isBlueTeam(RobotId id) { return id >= RobotId::BLUE_HERO; }

    class RobotToRobotMessageHandler
    {
    public:
        RobotToRobotMessageHandler() {}
        virtual void operator()(const DJISerial::ReceivedSerialMessage &message) = 0;
    };

    /**
     * Contains enum and struct definitions specific to receiving data from the referee serial
     * class.
     */
    class Rx
    {
    public:
        /// The type of game the robot is competing in.
        enum class GameType : uint8_t
        {
            UNKNOWN = 0,                  ///< Unknown competition type (most likely disconnected
                                          ///< from server).
            ROBOMASTER_RMUC = 1,          ///< RoboMaster Univeristy Challenge.
            ROBOMASTER_RMUTC = 2,         ///< RoboMaster Technical Challenge.
            ROBOMASTER_AI_CHALLENGE = 3,  ///< RobomMaster AI challenge.
            ROBOMASTER_RMUL_3V3 = 4,      ///< RoboMaster RMUL 3v3 competition.
            ROBOMASTER_RMUL_1V1 = 5,      ///< RoboMaster RMUL 1v1 competition.
        };

        enum class GameStage : uint8_t
        {
            PREMATCH = 0,        ///< Pre-competition. stage
            SETUP = 1,           ///< Setup stage.
            INITIALIZATION = 2,  ///< Initialization stage.
            COUNTDOWN = 3,       ///< 5-second countdown.
            IN_GAME = 4,         ///< In middle of the game.
            END_GAME = 5,        ///< Calculating competition results.
        };

        enum class GameWinner : uint8_t
        {
            DRAW = 0,  ///< Match was a draw.
            RED = 1,   ///< Red team won the match.
            BLUE = 2,  ///< Blue team won the match.
        };

        enum class SiteDartHit : uint8_t
        {
            NONE = 0,        ///< No hit target.
            OUTPOST = 1,     ///< Outpost hit.
            BASE_FIXED = 2,  ///< Fixed target hit.
            BASE_RANDOM = 3  ///< Random target hit.
        };

        enum class SupplierOutletStatus : uint8_t
        {
            CLOSED = 0,     ///< The outlet is closed.
            PREPARING = 1,  ///< The outlet is preparing projectiles.
            RELEASING = 2   ///< The outlet is releasing projectiles.
        };

        enum class DartTarget : uint8_t
        {
            NONE_OR_OUTPOST = 0,  ///< No target or outpost selected.
            BASE_FIXED = 1,       ///< A fixed target selected.
            BASE_RANDOM = 2       ///< A random target selected.
        };

        enum class ArmorId : uint8_t
        {
            FRONT = 0,  ///< armor #0 (front).
            LEFT = 1,   ///< armor #1 (left).
            REAR = 2,   ///< armor #2 (rear).
            RIGHT = 3,  ///< armor #3 (right).
            TOP = 4,    ///< armor #4 (top).
        };

        enum class DamageType : uint8_t
        {
            ARMOR_DAMAGE = 0,           ///< Armor damage.
            MODULE_OFFLINE = 1,         ///< Module offline.
            BARREL_OVER_SPEED = 2,      ///< Firing speed too high.
            BARREL_OVERHEAT = 3,        ///< Barrel overheat.
            CHASSIS_POWER_OVERRUN = 4,  ///< Chassis power overrun.
            COLLISION = 5,              ///< Armor plate collision.
        };

        enum class SiteData : uint32_t
        {
            RESTORATION_FRONT_OCCUPIED = modm::Bit0,
            RESTORATION_INSIDE_OCCUPIED = modm::Bit1,
            SUPPLIER_OCCUPIED = modm::Bit2,

            POWER_RUNE_OCCUPIED = modm::Bit3,
            SMALL_POWER_RUNE_ACTIVATED = modm::Bit4,
            LARGER_POWER_RUNE_ACTIVIATED = modm::Bit5,

            RING_OCCUPIED_TEAM = modm::Bit6,
            RING_OCCUPIED_OPPONENT = modm::Bit7,

            TRAPEZOID_R3_OCCUPIED_TEAM = modm::Bit8,
            TRAPEZOID_R3_OCCUPIED_OPPONENT = modm::Bit9,

            TRAPEZOID_R4_OCCUPIED_TEAM = modm::Bit10,
            TRAPEZOID_R4_OCCUPIED_OPPONENT = modm::Bit11,

            CENTRAL_BUFF_OCCUPIED_TEAM = modm::Bit30,
            CENTRAL_BUFF_OCCUPIED_OPPONENT = modm::Bit31
        };
        MODM_FLAGS32(SiteData);

        enum class RobotPower : uint8_t
        {
            GIMBAL_HAS_POWER = modm::Bit0,   ///< 1 if there is 24V output to gimbal, 0 for 0V.
            CHASSIS_HAS_POWER = modm::Bit1,  ///< 1 if there is 24V output to chassis, 0 for 0V.
            SHOOTER_HAS_POWER = modm::Bit2,  ///< 1 if there is 24V output to shooter, 0 for 0V.
        };
        MODM_FLAGS8(RobotPower);

        /// Activation status flags for the RFID module (for RMUC only).
        enum class RFIDActivationStatus : uint32_t
        {
            BASE_BUFF = modm::Bit0,
            ELEVATED_RING_OWN = modm::Bit1,
            ELEVATED_RING_OPPONENT = modm::Bit2,
            TRAPEZOID_R3_OWN = modm::Bit3,
            TRAPEZOID_R3_OPPONENT = modm::Bit4,
            TRAPEZOID_R4_OWN = modm::Bit5,
            TRAPEZOID_R4_OPPONENT = modm::Bit6,
            POWER_RUNE_ACTIVATION = modm::Bit7,
            LAUNCH_RAMP_FRONT_OWN = modm::Bit8,
            LAUNCH_RAMP_BACK_OWN = modm::Bit9,
            LAUNCH_RAMP_FRONT_OPPONENT = modm::Bit10,
            LAUNCH_RAMP_BACK_OPPONENT = modm::Bit11,
            OUTPOST_BUFF = modm::Bit12,
            RESTORATION_ZONE = modm::Bit13,
            SENTRY_PATROL_OWN = modm::Bit14,
            SENTRY_PATROL_OPPONENT = modm::Bit15,
            LARGE_ISLAND_OWN = modm::Bit16,
            LARGE_ISLAND_OPPONENT = modm::Bit17,
            EXCHANGE_ZONE = modm::Bit18,
            CENTRAL_BUFF = modm::Bit19
        };
        MODM_FLAGS32(RFIDActivationStatus);

        struct DamageEvent
        {
            uint16_t damageAmount;  ///< Amount of damage received
            uint32_t timestampMs;   ///< Time when damage was received (in milliseconds).
        };

        enum BulletType
        {
            AMMO_17 = 1,  ///< 17 mm projectile ammo.
            AMMO_42 = 2,  ///< 42 mm projectile ammo.
        };

        /**
         * Barrel identifier associated with the projectile launch message (which is sent when a
         * projectile has been launched).
         */
        enum MechanismID
        {
            TURRET_17MM_1 = 1,  ///< 17mm barrel ID 1
            TURRET_17MM_2 = 2,  ///< 17mm barrel ID 2
            TURRET_42MM = 3,    ///< 42mm barrel
        };

        /**
         * The Maximum launch speed for a 17mm barrel in m/s.
         */
        static constexpr int MAX_LAUNCH_SPEED_17MM = 30;

        /**
         * The Maximum launch speed for a 42mm barrel in m/s.
         */
        static constexpr int MAX_LAUNCH_SPEED_42MM = 16;

        /**
         * Current HP of all robots
         */
        struct RobotHpData
        {
            struct RobotHp
            {
                uint16_t hero1;
                uint16_t engineer2;
                uint16_t standard3;
                uint16_t standard4;
                uint16_t standard5;
                uint16_t sentry7;
                uint16_t outpost;
                uint16_t base;
            };

            RobotHp red;
            RobotHp blue;
        };

        /**
         * Data about the current state of the event.
         */
        struct EventData
        {
            SiteData_t siteData;  ///< Information about occupied zones.
            uint8_t
                virtualShieldRemainingPercent;  ///< Remain percent on own base's virtual shield.
            uint8_t timeSinceLastDartHit;  ///< Time since the last dart hit own outpost or base.
            SiteDartHit lastDartHit;       ///< The target hit by the last dart.
        };

        /**
         * Information about the projectile supplier.
         */
        struct SupplierAction
        {
            RobotId reloadingRobot;             ///< The id of the reloading robot.
            SupplierOutletStatus outletStatus;  ///< The current state of the supplier outlet.
            uint8_t suppliedProjectiles;        ///< The number of projectiles suppied.
        };

        /**
         * Robot position information.
         */
        struct RobotPosition
        {
            float x;  ///< The x location in meters.
            float y;  ///< The y location in meters.
        };

        struct ChassisData
        {
            uint16_t volt;                   ///< Output voltage to the chassis (in mV).
            uint16_t current;                ///< Output current to the chassis (in mA).
            float power;                     ///< Output power to the chassis (in W).
            uint16_t powerBuffer;            ///< Chassis power buffer (in J).
            RobotPosition position;          ///< x, y coordinate of the chassis (in m).
            uint16_t powerConsumptionLimit;  ///< The current chassis power limit (in W).
        };

        struct TurretData
        {
            BulletType bulletType;          ///< 17mm or 42mm last projectile shot.
            MechanismID launchMechanismID;  ///< Either 17mm mechanism 1, 3, or 42 mm mechanism.
            uint8_t firingFreq;             ///< Firing frequency (in Hz).
            uint16_t heat17ID1;             ///< Current 17mm turret heat, ID1.
            uint16_t heat17ID2;             ///< Current 17mm turret heat, ID2.
            uint16_t heat42;                ///< Current 42mm turret heat.
            uint16_t heatLimit;             ///< Turret heat limit. Shared with all turrets.
            uint16_t coolingRate;  ///< Turret cooling value per second. Shared with all turrets.
            uint16_t bulletsRemaining17;  ///< Number of bullets remaining in sentinel and drone
                                          ///< only (500 max) if in RMUC, or any robot in RMUL.
            uint16_t bulletsRemaining42;  ///< Number of bullets remaining in hero if in RMUL or 0
                                          ///< if in RMUC.
            float bulletSpeed;            ///< Last bullet speed (in m/s).
            float yaw;                    ///< Barrel yaw position (degree).
            uint32_t lastReceivedLaunchingInfoTimestamp;  ///< Last time in milliseconds that the
                                                          ///< real-time launching information
                                                          ///< message was received
        };

        struct RobotBuffStatus
        {
            uint8_t recoveryBuff;  ///< The robot's recovery buff. Each increment is 1%.
            uint8_t coolingBuff;   ///< The robot's barrel cooling rate buff. Each increment is a 1x
                                   ///< multiplier.
            uint8_t defenseBuff;   ///< The robot's defense buff. Each increment is 1%.
            uint8_t
                vulnerabilityBuff;  ///< The robot's negative defense buff. Each increment is 1%.
            uint16_t attackBuff;    ///< The robot's attack buff. Each increment is 1%.
        };

        /**
         * Ref serial warning data. This informational struct is intended to be updated each time a
         * yellow or red card has been received.
         *
         * When a yellow card is received, the offending Operator will be blocked from seeing
         * anything for 5 seconds, while the operation interfaces of other operators in the
         * offending team will be blocked for 2 seconds.
         */
        struct RefereeWarningData
        {
            uint8_t level;        /**<
                                   * The level of the wraning.
                                   * 1: Yellow card
                                   * 2: Red card
                                   * 3: Forfeiture
                                   */
            RobotId foulRobotID;  ///< The robot that received the referee warning
            uint8_t count;        ///< The number of violations triggered by the robot
            uint32_t lastReceivedWarningRobotTime = 0;  ///< Last time (in milliseconds) that a
                                                        ///< warning was received.

            /// time in ms that the user will be blinded for when the operator is the offender
            static constexpr uint32_t OFFENDING_OPERATOR_BLIND_TIME = 5'000;
            /// time in ms that the user will be blinded for when the operator is not the offender
            static constexpr uint32_t NONOFFENDING_OPERATOR_BLIND_TIME = 2'000;
        };

        /**
         * Information about the dart launcher
         */
        struct DartInfo
        {
            uint8_t launchCountdown;    ///< Seconds until the dart launcher can fire again.
            SiteDartHit lastHit;        ///< The last target hit by the dart.
            uint8_t hits;               ///< The number of successful hits by the dart launcher.
            DartTarget selectedTarget;  ///< The currently selected target.
        };

        enum AirSupportState : uint8_t
        {
            COOLING = 0,  ///< The air support is cooling down.
            COOLED = 1,   ///< The air support has cooled down.
            IN_AIR = 2    ///< The air support is in the air.
        };

        struct AirSupportData
        {
            AirSupportState state;       ///< The current state of air support.
            uint8_t remainingStateTime;  ///< The remaining seconds until the air support moves to
                                         ///< the next state.
        };

        enum DartStationState : uint8_t
        {
            OPEN = 0,       ///< The dart station is open.
            CLOSED = 1,     ///< The dart station is closed.
            TRANSITION = 2  ///< The dart station is opening or closing.
        };

        struct DartStationInfo
        {
            DartStationState state;      ///< Current state of the dart station doors.
            uint16_t targetChangedTime;  ///< Remaining seconds in the competition when the target
                                         ///< was changed.
            uint16_t lastLaunchedTime;   ///< Remaining seconds in the competition when the dart was
                                         ///< launched.
        };

        struct GroundRobotPositions
        {
            RobotPosition hero;
            RobotPosition engineer;
            RobotPosition standard3;
            RobotPosition standard4;
            RobotPosition standard5;
        };

        /**
         * Mark progress of different robots. Values range from 0 to 120.
         */
        struct RadarMarkProgress
        {
            uint8_t hero;
            uint8_t engineer;
            uint8_t standard3;
            uint8_t standard4;
            uint8_t standard5;
            uint8_t sentry;
        };

        struct SentryInfo
        {
            uint16_t projectileAllowance;  ///< Allocated projectiles not including remote exchange.
            uint8_t
                remoteProjectileExchanges;  ///< The number of remote projectile exchanges taken.
            uint8_t remoteHealthExchanges;  ///< The number of remote health exchanges taken.
        };

        struct RadarInfo
        {
            uint8_t availableDoubleVulnerablilityEffects;  ///< The number of remaining effects.
            bool activeDoubleVulnerabilityEffect;          ///< True when the effect is active.
        };

        struct GameData
        {
            GameType gameType;    ///< Current type of competition the robot is taking part in.
            GameStage gameStage;  ///< Current stage in the game.
            uint16_t stageTimeRemaining;     ///< Remaining time in the current stage (in seconds).
            uint64_t unixTime;               ///< Unix time of the competition server.
            GameWinner gameWinner;           ///< Results of the match.
            EventData eventData;             ///< Data about the current event
            SupplierAction supplier;         ///< Data about the projectile supplier
            DartInfo dartInfo;               ///< Data about the dart launcher
            AirSupportData airSupportData;   ///< Information about the air support
            DartStationInfo dartStation;     ///< Information about the dart launching station.
            GroundRobotPositions positions;  ///< Information about the position of ground robots.
            RadarMarkProgress
                radarProgress;  ///< Information about the mark progress for the radar station.
            SentryInfo sentry;  ///< Information about the sentry.
            RadarInfo radar;    ///< Information about the radar station.
        };

        struct RobotData
        {
            RobotId robotId;          ///< Robot type and team.
            uint8_t robotLevel;       ///< Current level of this robot (1-3).
            uint16_t previousHp;      ///< Health of this robot before damage was
                                      ///< received, used to calculate receivedDps
                                      ///< if no damage was received recently,
                                      ///< previousHp = currentHp.
            uint16_t currentHp;       ///< Current health of this robot.
            uint16_t maxHp;           ///< Max health of this robot.
            RobotPower_t robotPower;  ///< Flags indicating which parts of the robot have power
            ArmorId damagedArmorId;   ///< Armor ID that was damaged.
            DamageType damageType;    ///< Cause of damage.
            float receivedDps;        ///< Damage per second received.
            ChassisData chassis;      ///< Chassis power draw and position data.
            TurretData turret;        ///< Turret firing and heat data.
            RobotHpData allRobotHp;   ///< Current HP of all the robots.
            uint16_t remainingCoins;  ///< Number of remaining coins left to spend.
            RobotBuffStatus robotBuffStatus;        ///< Status of all buffs on the robot
            RFIDActivationStatus_t rfidStatus;      ///< The current status of which RFID zones
                                                    ///< are being activated by the current robot.
            uint32_t robotDataReceivedTimestamp;    ///< Most recent time at which data with message
                                                    ///< id `REF_MESSAGE_TYPE_ROBOT_STATUS` has been
                                                    ///< received.
            RefereeWarningData refereeWarningData;  ///< Referee warning information, updated when
                                                    ///< a robot receives a penalty
        };
    };

    /**
     * Contains enum and struct definitions specific to sending data to the referee serial class.
     * Includes structure for sending different types of graphic messages.
     */
    class Tx
    {
    public:
        /**
         * Graphic operations that can be passed to a delete graphic operation. Using this enum you
         * can specify if you would like to delete a graphic layer (multiple graphics can be drawn
         * on one of a number of layers) or delete all the graphics on the screen.
         */
        enum DeleteGraphicOperation
        {
            DELETE_GRAPHIC_NO_OP = 0,  ///< Do nothing, no-op
            DELETE_GRAPHIC_LAYER = 1,  ///< Delete a particular graphic layer (specified later on)
            DELETE_ALL = 2,            ///< Delete all graphic in all graphic layers
        };

        /// Graphic operations that can be passed in an add graphic operation
        enum GraphicOperation
        {
            GRAPHIC_NO_OP = 0,   ///< Do nothing, no-op
            GRAPHIC_ADD = 1,     /**< Add a new graphic. If the graphic has a unique id not
                                  * already registered with the UI, the graphic will be uniquely added,
                                  * otherwise the graphic with the same graphic ID will be replaced. If
                                  * you have a graphic that is already drawn that you want to change,
                                  * you should use GRAPHIC_MODIFY instead */
            GRAPHIC_MODIFY = 2,  /**< Modify an existing graphic (by graphic ID). The graphic must
                                  * already exist for this to work properly. */
            GRAPHIC_DELETE = 3,  ///< Delete a particular graphic.
        };

        /// The type of graphic being drawn (some geometry, or a string/number)
        enum class GraphicType : uint8_t
        {
            STRAIGHT_LINE = 0,
            RECTANGLE = 1,
            CIRCLE = 2,
            ELLIPSE = 3,
            ARC = 4,
            FLOATING_NUM = 5,
            INTEGER = 6,
            CHARACTER = 7,
        };

        /// The color of the graphic being drawn
        enum class GraphicColor : uint8_t
        {
            RED_AND_BLUE = 0,
            YELLOW = 1,
            GREEN = 2,
            ORANGE = 3,
            PURPLISH_RED = 4,
            PINK = 5,
            CYAN = 6,
            BLACK = 7,
            WHITE = 8,
        };

        /**
         * Each graphic message has a graphic header inside of the message data.
         */
        struct InteractiveHeader
        {
            uint16_t dataCmdId;
            uint16_t senderId;
            uint16_t receiverId;
        } modm_packed;

        struct GraphicData
        {
            uint8_t name[3];
            uint32_t operation : 3;
            uint32_t type : 3;
            uint32_t layer : 4;
            uint32_t color : 4;
            uint32_t startAngle : 9;
            uint32_t endAngle : 9;
            uint32_t lineWidth : 10;
            uint32_t startX : 11;
            uint32_t startY : 11;
            union
            {
                struct
                {
                    uint32_t radius : 10;
                    uint32_t endX : 11;
                    uint32_t endY : 11;
                } modm_packed;
                int32_t value : 32;
            } modm_packed;
        } modm_packed;

        struct DeleteGraphicLayerMessage
        {
            DJISerial::FrameHeader frameHeader;
            uint16_t cmdId;
            InteractiveHeader interactiveHeader;
            uint8_t deleteOperation;
            uint8_t layer;
            uint16_t crc16;
        } modm_packed;

        struct Graphic1Message
        {
            DJISerial::FrameHeader frameHeader;
            uint16_t cmdId;
            InteractiveHeader interactiveHeader;
            GraphicData graphicData;
            uint16_t crc16;
        } modm_packed;

        struct RobotToRobotMessage
        {
            DJISerial::FrameHeader frameHeader;
            uint16_t cmdId;
            InteractiveHeader interactiveHeader;
            uint8_t dataAndCRC16[115];
        } modm_packed;

        struct Graphic2Message
        {
            DJISerial::FrameHeader frameHeader;
            uint16_t cmdId;
            InteractiveHeader interactiveHeader;
            GraphicData graphicData[2];
            uint16_t crc16;
        } modm_packed;

        struct Graphic5Message
        {
            DJISerial::FrameHeader frameHeader;
            uint16_t cmdId;
            InteractiveHeader interactiveHeader;
            GraphicData graphicData[5];
            uint16_t crc16;
        } modm_packed;

        struct Graphic7Message
        {
            DJISerial::FrameHeader frameHeader;
            uint16_t cmdId;
            InteractiveHeader interactiveHeader;
            GraphicData graphicData[7];
            uint16_t crc16;
        } modm_packed;

        struct GraphicCharacterMessage
        {
            DJISerial::FrameHeader frameHeader;
            uint16_t cmdId;
            InteractiveHeader interactiveHeader;
            GraphicData graphicData;
            char msg[30];
            uint16_t crc16;
        } modm_packed;

        /**
         * You cannot send messages faster than this speed to the referee system.
         *
         * Source: https://bbs.robomaster.com/forum.php?mod=viewthread&tid=9120
         *
         * Changed from 1280 to 1000 as the HUD was still unreliable.
         */
        static constexpr uint32_t MAX_TRANSMIT_SPEED_BYTES_PER_S = 1000;

        /**
         * Get the min wait time after which you can send more data to the client. Sending faster
         * than this time may cause dropped packets.
         *
         * Pass a pointer to some graphic message. For example, if you have a `Graphic1Message`
         * called `msg`, you can call `getWaitTimeAfterGraphicSendMs(&msg)`.
         *
         * @tparam T The type of the graphic message that was just been sent.
         *
         * @todo @deprecated
         */
        template <typename T>
        static constexpr uint32_t getWaitTimeAfterGraphicSendMs(T *)
        {
            // Must be a valid graphic message type
            static_assert(
                std::is_same<T, DeleteGraphicLayerMessage>::value ||
                    std::is_same<T, Graphic1Message>::value ||
                    std::is_same<T, RobotToRobotMessage>::value ||
                    std::is_same<T, Graphic2Message>::value ||
                    std::is_same<T, Graphic5Message>::value ||
                    std::is_same<T, Graphic7Message>::value ||
                    std::is_same<T, GraphicCharacterMessage>::value,
                "Invalid type, getWaitTimeAfterGraphicSendMs only takes in ref serial message "
                "types.");

            return sizeof(T) * 1'000 / MAX_TRANSMIT_SPEED_BYTES_PER_S;
        }
    };
};

inline RefSerialData::RobotId operator+(RefSerialData::RobotId id1, RefSerialData::RobotId id2)
{
    return static_cast<RefSerialData::RobotId>(
        static_cast<uint16_t>(id1) + static_cast<uint16_t>(id2));
}

inline RefSerialData::RobotId operator-(RefSerialData::RobotId id1, RefSerialData::RobotId id2)
{
    return static_cast<RefSerialData::RobotId>(
        static_cast<uint16_t>(id1) - static_cast<uint16_t>(id2));
}
}  // namespace tap::communication::serial

#endif  // TAPROOT_REF_SERIAL_DATA_HPP_