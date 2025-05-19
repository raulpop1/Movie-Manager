#pragma once
#include <stdexcept>
#include <string>

// base exception class
class MovieAppException : public std::runtime_error {
public:
    explicit MovieAppException(const std::string& message) : std::runtime_error(message) {}
};

// repository-specific exception classes
class RepositoryException : public MovieAppException {
public:
    explicit RepositoryException(const std::string& message) : MovieAppException("Repository Error: " + message) {}
};

// validation exceptions
class ValidationException : public MovieAppException {
public:
    explicit ValidationException(const std::string& message) : MovieAppException("Validation Error: " + message) {}
};

// file-related exceptions
class FileException : public MovieAppException {
public:
    explicit FileException(const std::string& message) : MovieAppException("File Error: " + message) {}
};
