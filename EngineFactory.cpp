#include "EngineFactory.hpp"
#include "EngineJorritWillaert.hpp"

std::unique_ptr<Engine> EngineFactory::createEngine() {
  return std::make_unique<EngineJorritWillaert>();
}
