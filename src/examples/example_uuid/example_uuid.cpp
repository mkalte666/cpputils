#include <iostream>
#include <uuid.h>

int main(int, char**)
{
    UUID id1 = UUID::randomUUID();
    std::cout << "id 1          " << id1.toString() << "\n";
    std::cout << "id 1 read     " << UUID(id1.toString()).toString() << "\n";
    json j = id1;
    std::cout << j.dump() << "\n";
    UUID jget;
    jget = j;
    std::cout << "id 1 json     " << UUID(jget.toString()).toString() << "\n";
    UUID id2("123e4567-e89b-12d3-a456-426655440000");
    std::cout << "id 2          " << id2.toString() << "\n";
    UUID id3(0, 0, 0, 0, 0, 0, 4);
    std::cout << "id 3          " << id3.toString() << "\n";
    std::cout << "id 3 read     " << UUID(id3.toString()).toString() << "\n";
    UUID id4 = UUID::nilUUID();
    std::cout << "id 4          " << id4.toString() << "\n";
    UUID id5("sodkfjsofd092 röme");
    std::cout << "borked id 5   " << id5.toString() << "\n";
}