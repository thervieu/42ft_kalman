#include <iostream>
#include "eigen-3.4.0/Eigen/Dense"
#include "KalmanFilter.hpp"

using namespace Eigen;

KalmanFilter::KalmanFilter() {
    // Initialize state transition matrix F, measurement matrix H, and other parameters
    F = MatrixXd::Identity(9, 9);  // Identity matrix for simplicity
    H = MatrixXd::Identity(6, 9);  // Identity matrix for simplicity

    // Initialize process noise covariance Q and measurement noise covariance R
    Q = MatrixXd::Identity(9, 9);  // Identity matrix for simplicity
    R = MatrixXd::Identity(6, 6);  // Identity matrix for simplicity

    // Initialize state vector and covariance
    state = VectorXd::Zero(9);
    covariance = MatrixXd::Identity(9, 9);
}

Vector3d KalmanFilter::predict(double dt, const Vector3d& acceleration) {
    // Update state transition matrix F and control matrix B based on dt and acceleration
    // Update process noise covariance Q based on dt and acceleration

    // Perform prediction step
    state = F * state;
    covariance = F * covariance * F.transpose() + Q;

    // Update velocity and position based on acceleration
    state.segment(3, 3) += acceleration * dt;

    // Return the estimated position
    return state.segment(0, 3);
}

void KalmanFilter::update(const VectorXd& measurements) {
    // Calculate Kalman gain
    MatrixXd K = covariance * H.transpose() * (H * covariance * H.transpose() + R).inverse();

    // Update state and covariance based on measurements
    state += K * (measurements - H * state);
    covariance = (MatrixXd::Identity(9, 9) - K * H) * covariance;
}

const VectorXd& KalmanFilter::getState() const {
    return state;
}
