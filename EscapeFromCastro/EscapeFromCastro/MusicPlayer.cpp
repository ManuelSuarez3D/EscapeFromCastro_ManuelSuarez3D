
#include "MusicPlayer.h"
#include <stdexcept>


MusicPlayer::MusicPlayer() {
    m_filenames["menuTheme"] = "../assets/Music/dp_menu.flac";
    m_filenames["cubaTheme"] = "../assets/Music/dp_level1.flac";
    m_filenames["specialTheme"] = "../assets/Music/dp_special.flac";
    m_filenames["loadingTheme"] = "../assets/Music/dp_loading.flac";
    m_filenames["bermudaTheme"] = "../assets/Music/dp_bermuda.flac";
    m_filenames["usaTheme"] = "../assets/Music/dp_usa.flac";
}

void MusicPlayer::addSong(const std::string& name, const std::string& path) {
    m_filenames[name] = path;
}

MusicPlayer& MusicPlayer::getInstance() {
    static MusicPlayer instance;
    return instance;
}


void MusicPlayer::play(String theme) {
    if (!m_music.openFromFile(m_filenames[theme]))
        throw std::runtime_error("Music could not open file");

    m_music.setVolume(m_volume);
    m_music.setLoop(true);
    m_music.play();
}


void MusicPlayer::stop() {
    m_music.stop();
}


void MusicPlayer::setPaused(bool paused) {
    if (paused)
        m_music.pause();
    
    else
        m_music.play();
}

bool MusicPlayer::getPaused() {
    return m_music.getStatus() == sf::Music::Paused;
}

void MusicPlayer::setVolume(float volume) {
    m_volume = volume;
    m_music.setVolume(m_volume);
}
