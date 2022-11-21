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

  int alphaBetaMax(int alpha, int beta, int depthLeft, Board &board,
                   PrincipalVariation &pv);
  int alphaBetaMin(int alpha, int beta, int depthLeft, Board &board,
                   PrincipalVariation &pv);
};

#endif
