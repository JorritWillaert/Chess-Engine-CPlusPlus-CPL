#ifndef CHESS_ENGINE_JORRITWILLAERT_HPP
#define CHESS_ENGINE_JORRITWILLAERT_HPP

#include "Engine.hpp"

class EngineJorritWillaert : public Engine {
public:
  EngineJorritWillaert();

  std::string name() const override;
  std::string version() const override;
  std::string author() const override;

  void newGame() override;
  PrincipalVariation
  pv(const Board &board,
     const TimeInfo::Optional &timeInfo = std::nullopt) override;
};

#endif
