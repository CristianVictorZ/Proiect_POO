#include "../headers/exceptii.hpp"

const char* exceptionProjectileIndex::what() const noexcept{return "TargetIndex invalid!";}

const char* exceptionTowerPosition::what() const noexcept{return "TowerPosition invalid!";}
