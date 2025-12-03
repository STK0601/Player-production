#include "the_player.h"
#include <cstdlib>  // 用于rand()

void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    // 初始播放第一个按钮对应的视频，更改后注释掉为了不自动播放
    //jumpTo(buttons->at(0)->info);
}

/*
void ThePlayer::shuffle() {
    if (!infos || !buttons || infos->empty() || buttons->empty())
        return;

    // 随机选择一个视频信息
    TheButtonInfo* i = &infos->at(rand() % infos->size());
    // 轮流更新按钮（循环使用0-3索引）
    buttons->at(updateCount++ % buttons->size())->init(i);
}
*/

void ThePlayer::playStateChanged(QMediaPlayer::State ms) {

    /*移除掉自动播放
    switch (ms) {
    case QMediaPlayer::StoppedState:
        play();  // 播放结束后自动重新播放
        break;
    default:
        break;
    }*/

}

void ThePlayer::jumpTo(TheButtonInfo* button) {
    setMedia(*button->url);  // 设置媒体源
    play();                  // 开始播放
}
