#ifndef _PHYSIC_MANAGER_H_
#define _PHYSIC_MANAGER_H_

#include <boost/serialization/singleton.hpp>

#define physicManager (ResourceManager::get_mutable_instance())
#define physicManagerConst (ResourceManager::get_const_instance())

class ResourceManager : public 
boost::serialization::singleton<ResourceManager>
{
    
    
}


#endif /* _PHYSIC_MANAGER_HPP_ */
