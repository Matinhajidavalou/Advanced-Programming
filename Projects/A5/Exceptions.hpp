#pragma once
#include <exception>

class BadRequestException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Bad Request";
    }
};

class NotFoundException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Not Found";
    }
};

class PermissionDeniedException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Permission Denied";
    }
};

class EmptyException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Empty";
    }
};

class LevelMismatchException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Level Mismatch";
    }
};