#pragma once

class Stapel_t
{
private:
    // data
    int *data = nullptr;
    int size = 0;
    int capacity = 0;
    // Methods
    void growArray();

public:
    // Default Constructor
    Stapel_t();
    // Constructor    
    Stapel_t(int capacity);
    // Destructor
    ~Stapel_t();
    // Methods
    int GetSize();
    bool IsEmpty();
    void Clear();
    int GetCapacity();
    int GetElementAt(int index); 
    void SetElementAt(int index, int value);
    void PushBack(int value);
    int Peek();
    int PopBack();
    int Search(int key);
    void DeleteAt(int index);
    void Shuffle();
    void Sort();
    int PuntenStapel();
    int JokersStapel();
    int SelectedStapel();
    int JokersSelectedStapel();
    void UnselectAll();
    int CanastaStapel();
    bool IsBlocked();
}; // end class Stapel_t
