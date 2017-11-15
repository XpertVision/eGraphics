#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include "headers.h"

class MyListWidget : public QListWidget
{
    Q_OBJECT
private:
    QMovie* movie = nullptr;
    QLabel* movieBackground = nullptr;
    QColor defaultDisbledColor;

    int loopingCount = -1;

    void resizeEvent(QResizeEvent* event);
    void SetupAnimationLoop(int);
    void ResetAnimationLoop();
private slots:
    void ResetStates();
    void StopAnimationLooping(int);
public:
    MyListWidget(QWidget *parent = nullptr) : QListWidget(parent){}
    ~MyListWidget();

    void SetMyCheckState(QListWidgetItem*);
    void CheckedList(QListWidgetItem*);
    void SetEnabledWithReset(bool);
    void SetDisabledWithReset(bool);

    void ShowListEmpty(QString, int loopCount = -1);
signals:
    void ActivateButton(bool);
};

#endif // MYLISTWIDGET_H
