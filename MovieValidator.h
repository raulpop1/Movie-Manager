#pragma once
#include "movie.h"
#include "Exceptions.h"

class MovieValidator {
public:
    static void validate(const Movie& movie);
};
