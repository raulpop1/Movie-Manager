#pragma once
#include <stdexcept>

#include "movie.h"
typedef Movie TElem;

template <typename TElem>


class DynamicVector {

private:
    TElem* elementsList;
    int sizeOfVector;
    int capacityOfVector;



public:
    DynamicVector(int capacityGiven = 5);
    DynamicVector(const  DynamicVector& vectorToSet);
    ~DynamicVector();
    void resizeElementsList();
    bool needsResize();
    int searchElementInList(const TElem& elementToSearch);
    void append(TElem elementToAppend);
    void remove(const TElem& elementToRemove);
    void update(const TElem& elementToUpdate, const TElem& updatedElement);
    TElem& operator[](int positionOfTheElement);
    int size();
    int capacity();
    DynamicVector& operator=(const  DynamicVector& vectorUsed);
};

// DynamicVector implementation
template<typename TElem>
DynamicVector<TElem>::DynamicVector(int capacityGiven){
    this->capacityOfVector = capacityGiven;
    this->sizeOfVector = 0;
    this->elementsList = new TElem[this->capacityOfVector];
}
template<typename TElem>
DynamicVector<TElem>::DynamicVector(const  DynamicVector& vectorToSet){
    capacityOfVector = vectorToSet.capacityOfVector;
    sizeOfVector = vectorToSet.sizeOfVector;
    elementsList = new TElem[vectorToSet.capacityOfVector];
    for (int i = 0; i < sizeOfVector; ++i) {
        elementsList[i] = vectorToSet.elementsList[i];
    }
}

// Destructor
template<typename TElem>
DynamicVector<TElem>::~DynamicVector()
{
    delete[] elementsList;
}

// Resize the elements list
template<typename TElem>
inline void DynamicVector<TElem>::resizeElementsList()
{
    TElem* newList = new TElem[capacityOfVector * 2];
    for (int i = 0; i < sizeOfVector; ++i)
        newList[i] = elementsList[i];
    capacityOfVector *= 2;
    delete[] elementsList;
    elementsList = newList;
}

// Check if the vector needs resizing
template<typename TElem>
inline bool DynamicVector<TElem>::needsResize()
{
    return sizeOfVector == capacityOfVector;
}

// Search for an element in the list
template<typename TElem>
int DynamicVector<TElem>::searchElementInList(const TElem& elementToSearch) {
    for (int i = 0; i < sizeOfVector; ++i) {
        if (elementsList[i] == elementToSearch)
            return i;
    }
    return -1;
}

// Append an element to the list
template<typename TElem>
void DynamicVector<TElem>::append(TElem elementToAppend) {
    if (needsResize())
        resizeElementsList();
    elementsList[sizeOfVector++] = elementToAppend;
}

// Remove an element from the list
template<typename TElem>
void DynamicVector<TElem>::remove(const TElem& elementToRemove) {
    int positionWhereFound = searchElementInList(elementToRemove);
    if (positionWhereFound != -1) {
        for (int i = positionWhereFound; i < sizeOfVector - 1; ++i) {
            elementsList[i] = elementsList[i + 1];
        }
        --sizeOfVector;
    }
}

// Update an element in the list
template<typename TElem>
void DynamicVector<TElem>::update(const TElem& elementToUpdate, const TElem& updatedElement) {
    int positionWhereFound = searchElementInList(elementToUpdate);
    if (positionWhereFound != -1) {
        elementsList[positionWhereFound] = updatedElement;
    }
}

// Access an element by its position
template<typename TElem>
TElem& DynamicVector<TElem>::operator[](int positionOfTheElement) {
    if (positionOfTheElement < 0 || positionOfTheElement >= sizeOfVector)
        throw std::runtime_error("Invalid position");
    return elementsList[positionOfTheElement];}

template<typename TElem>
int DynamicVector<TElem>::size() {
    return sizeOfVector;
}

// Get the capacity of the vector
template<typename TElem>
int DynamicVector<TElem>::capacity()
{
    return capacityOfVector;
}

// Assignment operator
template<typename TElem>
DynamicVector<TElem>& DynamicVector<TElem>::operator=(const  DynamicVector& vectorUsed){
    if (this != &vectorUsed) {
        if (capacityOfVector < vectorUsed.sizeOfVector) {
            delete[] elementsList;
            elementsList = new TElem[vectorUsed.capacityOfVector];
        }
        sizeOfVector = vectorUsed.sizeOfVector;
        capacityOfVector = vectorUsed.capacityOfVector;
        for (int i = 0; i < sizeOfVector; ++i)
            elementsList[i] = vectorUsed.elementsList[i];
    }
    return *this;
}
