
#ifndef ESCAPEFROMCASTRO_ENTITYMANAGER_H
#define ESCAPEFROMCASTRO_ENTITYMANAGER_H


#include <map>
#include <vector>
#include <string>
#include <memory>

class Entity;

using sPtrEntt = std::shared_ptr<Entity>;
using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map <std::string, EntityVec>;


class EntityManager
{
private:
    EntityVec	    m_entities;
    EntityMap	    m_entityMap;
    size_t		    m_totalEntities{ 0 };
    EntityVec	    m_EntitiesToAdd;

    void		    removeDeadEntities(EntityVec& v);

public:
    EntityManager();

    std::shared_ptr<Entity>         addEntity(const std::string& tag);
    EntityVec& getEntities();
    EntityVec& getEntities(const std::string& tag);

    void                            update();
};


#endif 
