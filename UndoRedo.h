#pragma once
#include "Repository.h"
#include <stack>
#include <memory>

// Abstract base class for undo/redo actions
class UndoRedoAction {
public:
    virtual void undo() = 0;
    virtual void redo() = 0;
    virtual ~UndoRedoAction() = default;
};

// Add movie action
class AddMovieAction : public UndoRedoAction {
private:
    Repository& repository;
    Movie movie;

public:
    AddMovieAction(Repository& repo, const Movie& m) : repository(repo), movie(m) {}

    void undo() override {
        repository.deleteMovie(movie);
    }

    void redo() override {
        repository.addMovie(movie);
    }
};

// Delete movie action
class DeleteMovieAction : public UndoRedoAction {
private:
    Repository& repository;
    Movie movie;

public:
    DeleteMovieAction(Repository& repo, const Movie& m) : repository(repo), movie(m) {}

    void undo() override {
        repository.addMovie(movie);
    }

    void redo() override {
        repository.deleteMovie(movie);
    }
};

// Update movie action
class UpdateMovieAction : public UndoRedoAction {
private:
    Repository& repository;
    Movie oldMovie;
    Movie newMovie;

public:
    UpdateMovieAction(Repository& repo, const Movie& oldM, const Movie& newM)
            : repository(repo), oldMovie(oldM), newMovie(newM) {}

    void undo() override {
        repository.updateMovie(oldMovie);
    }

    void redo() override {
        repository.updateMovie(newMovie);
    }
};

// Manager for undo/redo actions
class UndoRedoManager {
private:
    std::stack<std::shared_ptr<UndoRedoAction>> undoStack;
    std::stack<std::shared_ptr<UndoRedoAction>> redoStack;

public:
    void executeAction(std::shared_ptr<UndoRedoAction> action) {
        // Clear redo stack when a new action is executed
        while(!redoStack.empty()) redoStack.pop();

        // Add action to undo stack
        undoStack.push(action);
    }

    bool canUndo() const {
        return !undoStack.empty();
    }

    bool canRedo() const {
        return !redoStack.empty();
    }

    void undo() {
        if(!canUndo()) return;

        auto action = undoStack.top();
        undoStack.pop();

        action->undo();
        redoStack.push(action);
    }

    void redo() {
        if(!canRedo()) return;

        auto action = redoStack.top();
        redoStack.pop();

        action->redo();
        undoStack.push(action);
    }
};
