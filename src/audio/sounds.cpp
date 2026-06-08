#include "sounds.h"

#include <QtGlobal>

Sounds::Sounds() = default;

Sounds::~Sounds()
{
    stop();
}

void Sounds::setSound(const QUrl &url, int volumePercent, bool shouldLoop)
{
    ensurePlayer();
    looping = shouldLoop;
    player->setSource(url);
    player->setLoops(looping ? QMediaPlayer::Infinite : 1);
    setVolume(static_cast<float>(volumePercent) / 100.0f);
}

void Sounds::playSound()
{
    if (!player) {
        return;
    }

    player->stop();
    player->setPosition(0);
    player->play();
}

void Sounds::stop()
{
    if (player) {
        player->stop();
    }
}

void Sounds::setVolume(float volume)
{
    ensurePlayer();
    audioOutput->setVolume(qBound(0.0f, volume, 1.0f));
}

void Sounds::ensurePlayer()
{
    if (player) {
        return;
    }

    audioOutput = std::make_unique<QAudioOutput>();
    player = std::make_unique<QMediaPlayer>();
    player->setAudioOutput(audioOutput.get());
}
