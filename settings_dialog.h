#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>
#include <QSlider>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    SettingsDialog(int currentFontSize, bool isChinese, QWidget *parent = nullptr)
        : QDialog(parent) {
        setWindowTitle(tr("设置"));
        resize(300, 200);

        // 字体大小设置
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        QLabel *fontLabel = new QLabel(tr("字体大小:"), this);
        fontSizeSlider = new QSlider(Qt::Horizontal, this);
        fontSizeSlider->setRange(10, 20);
        fontSizeSlider->setValue(currentFontSize);

        // 语言设置
        QLabel *langLabel = new QLabel(tr("语言:"), this);
        QHBoxLayout *langLayout = new QHBoxLayout();
        chineseRadio = new QRadioButton(tr("中文"), this);
        englishRadio = new QRadioButton(tr("英文"), this);

        if (isChinese) {
            chineseRadio->setChecked(true);
        } else {
            englishRadio->setChecked(true);
        }

        langLayout->addWidget(chineseRadio);
        langLayout->addWidget(englishRadio);

        // 确认按钮
        QPushButton *okBtn = new QPushButton(tr("确认"), this);
        connect(okBtn, &QPushButton::clicked, this, &SettingsDialog::accept);

        mainLayout->addWidget(fontLabel);
        mainLayout->addWidget(fontSizeSlider);
        mainLayout->addWidget(langLabel);
        mainLayout->addLayout(langLayout);
        mainLayout->addWidget(okBtn);

        setLayout(mainLayout);
    }

    int getFontSize() const { return fontSizeSlider->value(); }
    bool isChinese() const { return chineseRadio->isChecked(); }

private:
    QSlider *fontSizeSlider;
    QRadioButton *chineseRadio;
    QRadioButton *englishRadio;
};

#endif // SETTINGS_DIALOG_H
