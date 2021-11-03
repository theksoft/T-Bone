#ifndef TB_LOCKS_HPP
#define TB_LOCKS_HPP

#include <mutex>
#include <shared_mutex>

namespace tbone {

//==============================================================================

typedef std::mutex                Guard;
typedef std::lock_guard<Guard>    Guardian;

typedef std::recursive_mutex      Lock;
typedef std::lock_guard<Lock>     Locker;

typedef std::shared_mutex         RWLock;
typedef std::unique_lock<RWLock>  WLocker;
typedef std::shared_lock<RWLock>  RLocker;

//==============================================================================

}   // namespace tbone

#endif  // TB_LOCKS_HPP

// EOF
