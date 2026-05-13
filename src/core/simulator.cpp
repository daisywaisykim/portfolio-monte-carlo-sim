#include "simulator.hpp"
#include <Eigen/Dense>
#include <memory>
#include <random>
#include <utility>

// Using Eigen for Matrix Math
using Eigen::MatrixXd;
using Eigen::VectorXd;

using namespace MCSim;

std::unique_ptr<SimulationResult>
Simulator::run(const Eigen::VectorXd &means, const Eigen::MatrixXd &covariance,
               double startingWealth, int years, int paths) const {
  // Cholesky Decomposition
  MatrixXd L = covariance.llt().matrixL();

  std::mt19937_64 gen(1337); // Seed for reproducibility
  std::normal_distribution<double> dist(0.0, 1.0);

  auto finalWealthsAndPaths = std::make_unique<
      std::vector<std::pair<double, VectorUniquePtr<double>>>>();

  for (int p = 0; p < paths; p++) {
    VectorUniquePtr<double> path = std::make_unique<std::vector<double>>();

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
    finalWealthsAndPaths->push_back(std::pair(wealth, std::move(path)));
  }

  std::sort(finalWealthsAndPaths->begin(), finalWealthsAndPaths->end(),
            [](const auto &a, const auto &b) { return a.first < b.first; });

  auto result = std::make_unique<SimulationResult>(
      std::make_unique<std::vector<double>>(),
      std::make_unique<std::vector<std::unique_ptr<std::vector<double>>>>());

  for (auto i = finalWealthsAndPaths->begin(); i < finalWealthsAndPaths->end();
       i++) {
    result->finalWealths->push_back(i->first);
    result->paths->push_back(std::move(i->second));
  }

  return result;
}