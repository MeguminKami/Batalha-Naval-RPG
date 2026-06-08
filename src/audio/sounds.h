#ifndef SOUNDS_H
#define SOUNDS_H

#include <memory>

#include <QAudioOutput>
#include <QMediaPlayer>
#include <QUrl>

class Sounds
{
public:
    Sounds();
    ~Sounds();

    void setSound(const QUrl &url, int volumePercent, bool shouldLoop);
    void playSound();
    void stop();
    void setVolume(float volume);

private:
    void ensurePlayer();

    std::unique_ptr<QMediaPlayer> player;
    std::unique_ptr<QAudioOutput> audioOutput;
    bool looping = false;
};

#endif // SOUNDS_H
