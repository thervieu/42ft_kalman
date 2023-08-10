#pragma once

#include <iostream>
#include "eigen-3.4.0/Eigen/Dense"

using namespace Eigen;

class KalmanFilter {
public:
    KalmanFilter();

    Vector3d predict(double dt, const Vector3d& acceleration);

    void update(const VectorXd& measurements);

    const VectorXd& getState() const;

private:
    MatrixXd F;           // State transition matrix
    MatrixXd H;           // Measurement matrix
    MatrixXd Q;           // Process noise covariance
    MatrixXd R;           // Measurement noise covariance

    VectorXd state;       // Current state vector [x, y, z, vx, vy, vz, roll, pitch, yaw]
    MatrixXd covariance;  // Current covariance matrix
};
