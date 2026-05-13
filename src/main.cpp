#include "core/simulator.hpp"
#include <iostream>

int main(int argc, char **argv) {
  std::cout << "--- Portfolio Monte Carlo Engine ---" << std::endl << std::endl;

  // Setup means vector
  Eigen::VectorXd means(3);
  means << 0.10, // US stocks
      0.08,      // Intl stocks
      0.04;      // Bonds

  // Setup covariance mat
  // Cov(X,X) = Var = (Std Dev)^2 = Volatility^2
  // Cov(X,Y) = Corr * Vol_X * Vol_Y
  Eigen::MatrixXd cov(3, 3);

  // US stocks var: 0.15 * 0.15
  cov(0, 0) = 0.0225;

  // Intl stocks var: 0.17 * 0.17
  cov(1, 1) = 0.0289;

  // Bonds var: 0.05 * 0.05
  cov(2, 2) = 0.0025;

  // Covariances
  // Symmetric matrix
  // US / Intl: 0.75 * 0.15 * 0.17
  cov(0, 1) = 0.019125;
  cov(1, 0) = 0.019125;

  // US / Bonds: 0.10 * 0.15 * 0.05
  cov(0, 2) = 0.00075;
  cov(2, 0) = 0.00075;

  // Intl / Bonds: 0.05 * 0.17 * 0.05
  cov(1, 2) = 0.000425;
  cov(2, 1) = 0.000425;

  std::cout << "Means Vector:\n" << means << std::endl << std::endl;
  std::cout << "Covariance Matrix:\n" << cov << std::endl << std::endl;

  // Initialize simulator
  std::unique_ptr<MCSim::Simulator> sim;

  double startingWealth = 100000;
  std::size_t years = 30;
  std::size_t paths = 10000;

  std::cout << "Running with starting wealth " << startingWealth << " for "
            << paths << " paths over " << years << " years..." << std::endl;

  // Run simulation
  auto result = sim->run(means, cov, startingWealth, years, paths);

  // Sanity check output
  if (!result->finalWealths->empty()) {
    std::cout << "Simulation complete." << std::endl;

    int medianIndex = result->finalWealths->size() / 2;
    std::cout << "Median final wealth: "
              << result->finalWealths->at(medianIndex) << std::endl;
  } else {
    std::cout
        << "Simulation returned empty results (Implementation pending).\n";
  }
}
