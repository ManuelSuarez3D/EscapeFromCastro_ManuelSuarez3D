#ifndef ESCAPEFROMCASTRO_COMPONENTS_H
#define ESCAPEFROMCASTRO_COMPONENTS_H


#include <memory>
#include <SFML/Graphics.hpp>
#include "Utilities.h"
#include "Animation.h"
#include <bitset>

struct Component
{
    bool		has{ false };
    Component() = default;
};

struct CAnimation : public Component {
    Animation   animation;

    CAnimation() = default;
    CAnimation(const Animation& a) : animation(a) {}

        sf::Color getColor() {
        return animation.getSprite().getColor();
    }


    void setColor(const sf::Color& color) {
        animation.getSprite().setColor(color);
    }
};

struct CShape : public Component
{
    std::shared_ptr<sf::Shape> shape{ nullptr };
    CShape() = default;

    CShape(std::shared_ptr<sf::Shape> shape, const sf::Color& fill, const sf::Color& outline = sf::Color::Black, float thickness = 1.f)
        : shape(shape)
    {
        shape->setFillColor(fill);
        shape->setOutlineColor(outline);
        shape->setOutlineThickness(thickness);

        centerOrigin(shape.get());
    }
};


struct CSprite : public Component {
    sf::Sprite sprite;

    CSprite() = default;


    CSprite(const sf::Texture& t)
        : sprite(t) {
        centerOrigin(sprite);
    }



    CSprite(const sf::Texture& t, sf::IntRect r)
        : sprite(t, r) {
        centerOrigin(sprite);
    }
};


struct CTransform : public Component
{

    sf::Transformable  tfm;
    sf::Vector2f	pos{ 0.f, 0.f };
    sf::Vector2f    renderPos{ 0.f, 0.f };
    sf::Vector2f	prevPos{ 0.f, 0.f };
    sf::Vector2f	vel{ 0.f, 0.f };
    sf::Vector2f	scale{ 1.f, 1.f };

    float           angVel{ 0 };
    float	        angle{ 0.f };

    CTransform() = default;
    CTransform(const sf::Vector2f& p) : pos(p) {}
    CTransform(const sf::Vector2f& p, const sf::Vector2f& v)
        : pos(p), prevPos(p), vel(v) {}

};


struct CCollision : public Component
{
    float radius{ 0.f };

    CCollision() = default;
    CCollision(float r)
        : radius(r) {}
};


struct CBoundingBox : public Component
{
    sf::Vector2f size{ 0.f, 0.f };
    sf::Vector2f halfSize{ 0.f, 0.f };

    CBoundingBox() = default;
    CBoundingBox(const sf::Vector2f& s) : size(s), halfSize(0.5f * s)
    {}
};

struct CState : public Component {
    std::string state{ "none" };

    CState() = default;
    CState(const std::string& s) : state(s) {}

};

struct CInput : public Component
{
    bool UP{ false };
    bool LEFT{ false };
    bool RIGHT{ false };
    bool DOWN{ false };
    bool shoot{ false };
    bool canShoot{ true };
    unsigned char dir{ 0 };

    CInput() = default;
};

struct CType : public Component
{
    bool boat{ false };
    bool shark{ false };
    bool island{ false };
    bool coral{ false };
    bool squid{ false };
    bool tornado{ false };
    bool whirpool{ false };
    bool coca{ false };
    bool bullet{ false };
    bool floridaMan{ false };
    bool police{ false };
    bool dune{ false };

    bool up{ false };
    bool down{ false };

    bool entity{ false };
    bool player{ false };

    CType() = default;
};

struct CLife : public Component
{
    int total{ 0 };
    int remaining{ 0 };

    bool isDying{ false };
    bool isDead{ false };

    CLife() = default;
    CLife(int t) : total(t), remaining{ t } {}

};

struct CScore : public Component
{
    int score{ 0 };
    CScore(int s = 0) : score(s) {}
};

struct CGun : public Component
{
    bool isFiring{ false };
    sf::Time countdown{ sf::Time::Zero };
    int fireRate{ 1 };
    int spreadLevel{ 1 };
    CGun() = default;
};

struct CMissiles : public Component {
    size_t    missileCount{ 5 };

    CMissiles() = default;
};

#endif