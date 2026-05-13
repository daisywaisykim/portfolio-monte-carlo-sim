#pragma once

#include <Eigen/Dense>

namespace MCSim {
template <typename T> using VectorUniquePtr = std::unique_ptr<std::vector<T>>;

struct SimulationResult {
  VectorUniquePtr<double> finalWealths;
  VectorUniquePtr<VectorUniquePtr<double>> paths;
};

class Simulator {
public:
  Simulator() = default;
  ~Simulator() = default;

  [[nodiscard]] std::unique_ptr<SimulationResult>
  run(const Eigen::VectorXd &means, const Eigen::MatrixXd &covariance,
      double startingWealth, int years, int paths) const;
};
} // namespace MCSim