#include "MovieValidator.h"

void MovieValidator::validate(const Movie& movie) {
    std::string errors;

    if(movie.getTitle().empty())
        errors += "Title cannot be empty! ";
    if(movie.getGenre().empty())
        errors += "Genre cannot be empty! ";
    if(movie.getYearOfRelease() < 0 || movie.getYearOfRelease() > 2100)
        errors += "Year of release must be between 0 and 2100! ";
    if(movie.getNumberOfLikes() < 0)
        errors += "Number of likes cannot be negative! ";
    if(movie.getTrailer().empty())
        errors += "Trailer cannot be empty! ";

    if(!errors.empty())
        throw ValidationException(errors);
}
