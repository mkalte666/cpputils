#include "ptrclass.h"

class Something : public PtrClass<Something>
{
public:
    int i;
protected:
    Something(int i) : i(i){}
};

class SomethingElse : public CopyablePtrClass<SomethingElse>
{
public:
    int i;
protected:
    SomethingElse(int i) : i(i){}
};

int main(int argc, char**argv)
{
    //Something direct(0); // not allowed
    auto s = Something::create(1);
    //auto sCopy = *s; // now allowed
    auto s2 = SomethingElse::create(2);
    auto s2Copy = *s2; // allowed
    auto s3 = SomethingElse::create(3);
    *s2 = *s3; // allowed
    s2 = s3; // allowed, original s2 will now have ref count 0
    return s2->i;
}