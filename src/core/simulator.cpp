#include "simulator.hpp"
#include <Eigen/Dense>
#include <random>

// Using Eigen for Matrix Math
using Eigen::MatrixXd;
using Eigen::VectorXd;

using namespace MCSim;

SimulationResult Simulator::run(const Eigen::VectorXd &means,
                                const Eigen::MatrixXd &covariance,
                                double startingWealth, int years,
                                int paths) const {
  // Cholesky Decomposition
  MatrixXd L = covariance.llt().matrixL();

  std::mt19937_64 gen(1337); // Seed for reproducibility
  std::normal_distribution<double> dist(0.0, 1.0);

  std::unique_ptr<SimulationResult> result(new SimulationResult());

  for (int p = 0; p < paths; p++) {
    VectorUniquePtr<double> path;

    path->push_back(startingWealth);
    double wealth = startingWealth;

    for (int y = 0; y < years; ++y) {
      // Generate independent random normals
      VectorXd Z(means.size());

      for (int i = 0; i < Z.size(); i++) {
        Z(i) = dist(gen);
      }

      // Correlate them
      VectorXd correlatedReturns = means + (L * Z);

      // Apply to wealth (simplified: using first asset for now)
      wealth *= (1.0 + correlatedReturns(0));
      path->push_back(wealth);
    }

    // Store wealth for percentile calculation...
    result->final_wealths->push_back(wealth);
    result->paths->push_back(std::move(path));
  }
}