
#ifndef ESCAPEFROMCASTRO_ENTITY_H
#define ESCAPEFROMCASTRO_ENTITY_H


#include <tuple>
#include <string>

#include "Components.h"

class EntityManager;


using ComponentTuple = std::tuple<CShape, CAnimation, CType, CSprite, CState, CTransform, CBoundingBox, CInput, CScore, CGun, CMissiles, CLife>;

class Entity {
private:
    friend class EntityManager;
    Entity(size_t id, const std::string& tag);

    const size_t            m_id{ 0 };
    const std::string       m_tag{ "Default" };
    bool                    m_active{ true };
    ComponentTuple          m_components;

public:

    void                    destroy();
    const size_t& getId() const;
    const std::string& getTag() const;
    bool                    isActive() const;


    template<typename T>
    inline bool hasComponent() const {
        return getComponent<T>().has;
    }


    template<typename T, typename... TArgs>
    inline T& addComponent(TArgs &&... mArgs) {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }


    template<typename T>
    inline bool removeComponent() {
        return getComponent<T>().has = false;
    }


    template<typename T>
    inline T& getComponent() {
        return std::get<T>(m_components);
    }


    template<typename T>
    inline const T& getComponent() const {
        return std::get<T>(m_components);
    }
};



#endif
