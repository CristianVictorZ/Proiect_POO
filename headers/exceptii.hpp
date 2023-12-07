#include <iostream>

#ifndef EXCEPTII_HPP
#define EXCEPTII_HPP

class exceptionProjectileIndex : public std::exception
{
public:
    const char* what() const noexcept;
};

class exceptionTowerPosition : public std::exception
{
public:
    const char* what() const noexcept;
};

#endif
