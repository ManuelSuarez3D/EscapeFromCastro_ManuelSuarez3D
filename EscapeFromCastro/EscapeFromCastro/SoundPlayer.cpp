#include "SoundPlayer.h"
#include "Assets.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Audio/Listener.hpp>
#include <cassert>
#include <cmath>
#include <stdexcept>


namespace {

    const float volume = 100.f;
    const float ListenerZ = 300.f;
    const float Attenuation = 1.f;
    const float MinDistance2D = 200.f;
    const float MinDistance3D = std::sqrt(MinDistance2D * MinDistance2D + ListenerZ * ListenerZ);
}


SoundPlayer::SoundPlayer() {

    sf::Listener::setDirection(0.f, 0.f, -1.f);
}


SoundPlayer& SoundPlayer::getInstance() {
    static SoundPlayer instance;
    return instance;
}


void SoundPlayer::play(String effect) {
    play(effect, getListnerPosition());
}

void SoundPlayer::play(String effect, sf::Vector2f position) {
    m_sounds.push_back(sf::Sound());
    sf::Sound& sound = m_sounds.back();

    sound.setBuffer(Assets::getInstance().getSound(effect));

    sound.setPosition(position.x, 0.f, -position.y);
    sound.setAttenuation(Attenuation);
    sound.setMinDistance(MinDistance3D);

    sound.setVolume(30);
    sound.play();
}


void SoundPlayer::removeStoppedSounds() {
    m_sounds.remove_if([](const sf::Sound& s) {
        return s.getStatus() == sf::Sound::Stopped;
        });
}


void SoundPlayer::setListnerPosition(sf::Vector2f position) {
    sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}


void SoundPlayer::setListnerDirection(sf::Vector2f position) {

    sf::Listener::setDirection(position.x, 0, -position.y);
}

sf::Vector2f SoundPlayer::getListnerPosition() const {
    sf::Vector3f pos = sf::Listener::getPosition();
    return sf::Vector2f(pos.x, -pos.y);
}


bool SoundPlayer::isEmpty() const {
    return m_sounds.empty();
}