// released as CC0
// Public Domain
#ifndef _ptrclass_h
#define _ptrclass_h

#include <memory>

/**
 * \brief Parent struct that helps enforcing NoCopy rules
 */
struct NoCopy {
public:
    NoCopy() = default; //!< default constructor
    virtual ~NoCopy() = default; //!< default virtual destructor
    NoCopy(const NoCopy&) = delete; //!< deleted copy constructor
    NoCopy(NoCopy&&) = default; //!< default movement constuctor
    NoCopy&
    operator=(const NoCopy&)
        = delete; //!< deleted copy assignment operator
    NoCopy& operator=(NoCopy&&) = default; //!< default move assignment operator
};

/**
 * \brief Parent Struct that helps enforcing NoMove rules
 */
struct NoMove {
public:
    NoMove() = default; //<! default constructor
    virtual ~NoMove() = default; //<! default virtaul destructor
    NoMove(const NoMove&) = default; //<! default copy constructor
    NoMove(NoMove&&) = delete; //<! deleted move constructor
    NoMove&
    operator=(const NoMove&)
        = default; //<! default copy assingment operator
    NoMove& operator=(NoMove&&) = delete; //!< deleted move assingment operator
};

/**
 * \brief Parent Struct that helps enforcing both NoCopy and NoMove rules
 */
struct NoAssign : public NoMove, public NoCopy {
};

/**
 * \brief Parent Struct that enforces no copy or move rules
 */
struct AllowAssign {
};

/**
 * \brief Base class for pointer-only classes.
 * PtrClass is used to define types wich should be only instance as pointers.
 * This is usefull for data classes wich should not be moved around, instances
 * of servers or similar stuff. \tparam T the type wich becomes a "PtrClass"
 * \tparam AssignmentRestrictT the Assignement Restrictor
 *      this can be used to enforce that no copying, no moving, or both can be
 * done.
 */
template <class T, class AssignmentRestrictT = NoAssign>
class PtrClass : public AssignmentRestrictT {
public:
    /// Typedef for the assignment restrictor type
    using AssignmentRestrictor = AssignmentRestrictT;
    /// Typedef for the pointer type
    using Ptr = std::shared_ptr<T>;
    /// Typedef for the unique pointer type
    using UPtr = std::unique_ptr<T>;

    /// virtual destructor
    virtual ~PtrClass() {};

    /**
   * \brief Creates a shared instance of type Tinstance (defaults to T)
   * Create a shared instance (std::shared_ptr) of the given instance type.
   * You can specify Tinstance to something different than T if you want to
   * instance child classes \tparam Tinstance Type to instance (eg call operator
   * new on) \note Eats the same parameters as the Constructor of Tinstance/T
   */
    template <class Tinstance = T, class... Args>
    static Ptr create(Args&&... args)
    {
        return std::make_shared<_ctorWrapper<Tinstance>>(
            std::forward<Args>(args)...);
    }

    /**
   * \brief Creates a unique instance of type Tinstance (defaults to T)
   * Create a unique instance (std::unique_ptr) of the given instance type.
   * You can specify Tinstance to something different than T if you want to
   * instance child classes \tparam Tinstance Type to instance (eg call operator
   * new on) \note Eats the same parameters as the Constructor of Tinstance/T
   */
    template <class Tinstance = T, class... Args>
    static UPtr createUnique(Args&&... args)
    {
        return std::make_unique<_ctorWrapper<Tinstance>>(
            std::forward<Args>(args)...);
    }

private:
    /**
   * Struct that wrapps the Tinstance constructor to work around protected
   * constructors \tparam Tinstance Type to instance (eg call operator new on)
   */
    template <class Tinstance>
    struct _ctorWrapper : public Tinstance {
        template <class... Args>
        _ctorWrapper(Args&&... args)
            : Tinstance(std::forward<Args>(args)...)
        {
        }

        virtual ~_ctorWrapper() = default;
    };
};

/**
 * \brief A Copyable version of PtrClass
 * Same as PtrClass, however with AssignmentRestrictor set to NoMove
 */
template <class T>
class CopyablePtrClass : public PtrClass<T, NoMove> {
public:
    using PtrClass<T, NoMove>::PtrClass;
};

#endif //_ptrclass_h
