#include <memory>

#ifndef PTR
#define PTR( NAME )\
	class NAME;\
	typedef std::shared_ptr< NAME > NAME##Ptr;\
	typedef std::shared_ptr< const NAME > NAME##ConstPtr;\
	typedef std::weak_ptr< NAME > NAME##WeakPtr;\
	typedef std::unique_ptr< NAME > NAME##UniquePtr;\

#endif