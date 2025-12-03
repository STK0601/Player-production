//
//
//

#include "the_button.h"
#include <QStyle>

void TheButton::init(TheButtonInfo* i) {
    setIcon( *(i->icon) );
    info =  i;
    setMinimumSize(220, 140); // 增大按钮尺寸，留出更多空间
    setMaximumSize(220, 140);
    setIconSize(QSize(200, 110)); // 图标大小适配按钮



    setStyleSheet(R"(
        TheButton {
            border: 2px solid #4A90E2; /* 蓝色边框 */
            border-radius: 8px; /* 圆角 */
            background-color: #F5F7FA; /* 浅灰背景 */
            margin: 5px; /* 按钮间距 */
        }
        TheButton:hover {
            border-color: #5B9EF5; /* 悬停时边框变亮 */
            background-color: #E8F0FE; /* 悬停时背景变浅蓝 */
        }
        TheButton:pressed {
            border-color: #3A80D2; /* 按下时边框变深 */
            background-color: #DCE8FC; /* 按下时背景变深 */
        }
        TheButton:disabled {
            border-color: #CCCCCC;
            background-color: #F0F0F0;
        }
    )");

    // 去除按钮默认焦点边框（可选）
    setFocusPolicy(Qt::NoFocus);

}


void TheButton::clicked() {
    emit jumpTo(info);
}
