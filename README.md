# Silver-Owl-Library 🦉

A lightweight, high-performance C++ motion control library designed for VEX V5 robotics systems running **VEXcode Pro C++**. 

**Silver-Owl-Library** delivers smooth autonomous navigation through closed-loop PID control, Inertial sensor heading correction, slew-rate acceleration ramping, and robust motor telemetry management.

---

## 📌 Features

- **Heading-Locked Drive Base:** Maintain absolute robot orientation during forward and backward movements using closed-loop PID feedback from the V5 Inertial Sensor.
- **Slew-Rate Acceleration Control:** Eliminate wheel slip and gear slop with smooth acceleration ramping (`AccT`).
- **Distance & Velocity Profiling:** Integrated dual-PID control (distance deceleration + heading lock) with minimum speed anti-stall thresholds.
- **Timed Safety Overrides:** Every motion routine includes configurable timeout limits to prevent motor strain if the robot gets trapped or obstructed during matches.
- **Voltage-Based Motor Control:** Direct motor output scaled via V5 voltage commands (`11V` max) for linear response curves across different battery levels.

---

## 🛠️ Requirements & Setup

### Prerequisites
- **Hardware:** VEX V5 Robot Brain, 6-Motor Drivetrain (`LF`, `LM`, `LB`, `RF`, `RM`, `RB`), V5 Inertial Sensor (`Gyro`).
- **Software:** VEXcode C++ Environment.

### File Structure
Include `Silver-Owl-Library` files into your project source directory:
```
├── include/
│   ├── helper_functions.hpp
│   ├── movement.hpp
│   ├── screen_gui.hpp
│   └── vex.h
└── src/
    └── Silver-Owl-Library.cpp
```

---

## 📦 API Reference

### 1. Drivetrain & Sensor Utilities

#### `void Zeroing(bool dist, bool HDG)`
Resets drive motor encoder counts and/or zeroes the Inertial Sensor heading.
- `dist` *(bool)*: Resets encoder counts on all 6 drive motors if `true`.
- `HDG` *(bool)*: Resets inertial sensor heading to `0` degrees if `true`.

#### `ChassisDataSet ChassisUpdate()`
Queries motor encoders and the inertial sensor, returning a `ChassisDataSet` struct containing:
- `Left` / `Right`: Individual side travel distances.
- `Avg`: Average distance traveled.
- `HDG`: Current gyro heading ($0^\circ - 360^\circ$).

---

### 2. Basic Motor Control

#### `void Move(int left, int right)`
Applies voltage to the chassis motors scaled as a percentage (`-100` to `100`).

#### `void BStop()` / `void CStop()`
Immediately halts all drivetrain motors using active **Brake** (`brake`) or momentum **Coast** (`coast`) mode.

---

### 3. Motion Routines

#### `MovePID`
Advanced closed-loop movement combining distance deceleration profiling with heading lock.
```cpp
void MovePID(PIDDataSet KVals, PIDDataSet Dvals, int Speed, double dist, 
            double AccT, double ABSHDG, bool brake, double time);
```
- **`KVals`**: PID constants ($k_p, k_i, k_d$) for heading correction.
- **`Dvals`**: PID constants ($k_p, k_i, k_d$) for distance deceleration.
- **`Speed`**: Target speed (`-100` to `100`).
- **`dist`**: Target distance in inches.
- **`AccT`**: Acceleration ramp time in seconds.
- **`ABSHDG`**: Absolute target heading ($0^\circ - 360^\circ$) to hold.
- **`brake`**: Active brake on finish if `true`.
- **`time`**: Safety timeout in seconds.

#### `MoveEncoderPID`
Drives to a target encoder distance while maintaining absolute heading.
```cpp
void MoveEncoderPID(PIDDataSet KVals, int Speed, double dist, 
                   double AccT, double ABSHDG, bool brake);
```

#### `TurnMaxTimePID`
Rotates the chassis in place toward a target absolute heading using PID feedback.
```cpp
void TurnMaxTimePID(PIDDataSet KVals, double DeltaAngle, double TE, bool brake);
```
- **`DeltaAngle`**: Absolute target heading in degrees.
- **`TE`**: Maximum turn duration in seconds.

#### `MoveTimePID`
Drives along a specified heading for a set time duration.
```cpp
void MoveTimePID(PIDDataSet KVals, int Speed, double TE, 
                double AccT, double ABSHDG, bool brake);
```

---

## 🚀 Quick Start Example

```cpp
#include "vex.h"

using namespace vex;

// Define PID Tuning Sets
PIDDataSet headingPID = { .kp = 1.25, .ki = 0.001, .kd = 0.08 };
PIDDataSet distancePID = { .kp = 0.85, .ki = 0.000, .kd = 0.05 };

void autonomous(void) {
    // 1. Reset drive encoders and zero heading
    Zeroing(true, true);

    // 2. Drive forward 24 inches at 80% speed along heading 0°
    //    Ramp speed over 0.4s, timeout after 2.5s
    MovePID(headingPID, distancePID, 80, 24.0, 0.4, 0.0, true, 2.5);

    // 3. Turn in place to face 90° heading (timeout after 1.2s)
    TurnMaxTimePID(headingPID, 90.0, 1.2, true);

    // 4. Drive forward 12 inches along heading 90°
    MovePID(headingPID, distancePID, 80, 12.0, 0.3, 90.0, true, 2.0);
}
```

---

## 📄 License

This repository is distributed under the **MIT License**. See `LICENSE` for details.
