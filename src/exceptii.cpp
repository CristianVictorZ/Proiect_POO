#include "../headers/exceptii.hpp"

///Cazul in care un proiectil are ca tinta un inamic cu un index invalid.
const char* exceptionProjectileIndex::what() const noexcept{return "TargetIndex invalid!";}

///Cazul in care un turn este pus in afara grid-ului.
const char* exceptionTowerPosition::what() const noexcept{return "TowerPosition invalid!";}

