// released as CC0
// Public Domain
#ifndef _ptrclass_h
#define _ptrclass_h

#include <memory>

struct NoCopy
{
public:
    NoCopy() = default;
    virtual ~NoCopy() = default;
    NoCopy(const NoCopy&) = delete;
    NoCopy(NoCopy&&) = default;
    NoCopy& operator=(const NoCopy&) = delete;
    NoCopy& operator=(NoCopy&&) = delete;
};

struct NoMove
{
public:
    NoMove() = default;
    virtual ~NoMove() = default;
    NoMove(const NoMove&) = default;
    NoMove(NoMove&&) = delete;
    NoMove& operator=(const NoMove&) = default;
    NoMove& operator=(NoMove&&) = delete;
};

struct NoAssign : public NoMove, public NoCopy{};
struct AllowAssign{};

template<class T, class AssignmentRestrictT = NoAssign>
class PtrClass : public AssignmentRestrictT
{
public:
    using Ptr = std::shared_ptr<T>;
    using UPtr = std::unique_ptr<T>;
    using AssignmentRestrictor = AssignmentRestrictT;

    virtual ~PtrClass() {};

    template <class...Args>
    static Ptr create(Args... args)
    {
        return std::make_shared<_ctorWrapper>(std::forward<Args>(args)...);
    }

    template <class...Args>
    static UPtr createUnique(Args... args)
    {
        return std::make_unique<_ctorWrapper>(std::forward<Args>(args)...);
    }

private:
    struct _ctorWrapper : public T
    { 
        template <class...Args>
        _ctorWrapper(Args...args) : T(std::forward<Args>(args)...){}
    };
};

template <class T>
class CopyablePtrClass : public PtrClass<T,NoMove>
{
public:
    using PtrClass<T,NoMove>::PtrClass;
};

#endif //_ptrclass_h
