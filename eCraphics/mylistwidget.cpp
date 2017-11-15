#include "mylistwidget.h"

void MyListWidget::SetMyCheckState(QListWidgetItem* item)
{
    if(item->checkState() == Qt::Checked)
        item->setCheckState(Qt::Unchecked);
    else
        item->setCheckState(Qt::Checked);
}

void MyListWidget::CheckedList(QListWidgetItem* item)
{
    if(item->listWidget()->count() < 1)
        return;

    bool flagEnableButton(false);

    item->listWidget()->blockSignals(true);

    if(item->checkState() == Qt::Unchecked && item->listWidget()->item(0)->checkState() == Qt::Checked)
    {
        item->listWidget()->item(0)->setCheckState(Qt::Unchecked);

        for(int i = 1; i < item->listWidget()->count(); i++)
        {
            if(item->listWidget()->item(i)->checkState() == Qt::Checked)
            {
                flagEnableButton = true;
                break;
            }
        }
    }
    else if(item->checkState() == Qt::Checked && item->listWidget()->item(0)->checkState() == Qt::Checked)
    {
        for(int i = 1; i < item->listWidget()->count(); i++)
        {
            item->listWidget()->item(i)->setCheckState(Qt::Checked);
        }

        flagEnableButton = true;
    }
    else if(item->checkState() == Qt::Unchecked && item->listWidget()->item(0)->checkState() == Qt::Unchecked)
    {
        if(item->text() == item->listWidget()->item(0)->text())
        {
            for(int i = 1; i < item->listWidget()->count(); i++)
            {
                item->listWidget()->item(i)->setCheckState(Qt::Unchecked);
            }
        }
        else
        {
            for(int i = 1; i < item->listWidget()->count(); i++)
            {
                if(item->listWidget()->item(i)->checkState() == Qt::Checked)
                {
                    flagEnableButton = true;
                    break;
                }
            }
        }
    }
    else if(item->checkState() == Qt::Checked && item->listWidget()->item(0)->checkState() == Qt::Unchecked)
    {
        bool chekedAll(true);
        for(int i = 1; i < item->listWidget()->count(); i++)
        {
            if(item->listWidget()->item(i)->checkState() == Qt::Unchecked)
            {
                chekedAll = false;
                break;
            }
        }

        if(chekedAll)
            item->listWidget()->item(0)->setCheckState(Qt::Checked);

        flagEnableButton = true;
    }

    item->listWidget()->blockSignals(false);

    emit ActivateButton(flagEnableButton);
}

void MyListWidget::ShowListEmpty(QString animation, int loopCount)
{
    QPalette tmpPalette = this->palette();
    this->defaultDisbledColor = tmpPalette.color(QPalette::Disabled, QPalette::Base);
    tmpPalette.setColor(QPalette::Disabled, QPalette::Base, Qt::white);
    this->setPalette(tmpPalette);

    if(this->movie != nullptr)
        delete this->movie;
    if(this->movieBackground != nullptr)
        delete this->movieBackground;

    this->movieBackground = new QLabel(this);
    this->movieBackground->setGeometry(1, 1, this->width() - 2, this->height() -2);
    this->movieBackground->setAlignment(Qt::AlignCenter);

    this->movie = new QMovie(this->movieBackground);
    movie->setFileName(animation);

    if(this->movieBackground->height() > this->movieBackground->width())
        this->movie->setScaledSize(QSize(this->movieBackground->width(), this->movieBackground->width()));
    else
        this->movie->setScaledSize(QSize(this->movieBackground->height(), this->movieBackground->height()));

    this->movieBackground->setMovie(this->movie);
    this->movieBackground->show();

    this->ResetAnimationLoop();
    this->SetupAnimationLoop(loopCount);
    this->movie->start();
}

void MyListWidget::SetupAnimationLoop(int loopCount)
{
    this->loopingCount = loopCount;

    if(this->loopingCount == 0)
    {
        connect(this->movie, &QMovie::started, this->movie, &QMovie::stop);
    }
    else if(this->loopingCount > 0)
    {
        connect(this->movie, &QMovie::frameChanged, this, &MyListWidget::StopAnimationLooping);
    }

    this->movie->frameCount();
}

void MyListWidget::ResetAnimationLoop()
{
    if(this->loopingCount == 0)
    {
        disconnect(this->movie, &QMovie::started, this->movie, &QMovie::stop);
    }
    else if(this->loopingCount > 0)
    {
        disconnect(this->movie, &QMovie::frameChanged, this, &MyListWidget::StopAnimationLooping);
    }
}

void MyListWidget::StopAnimationLooping(int frameNumber)
{
    static int loopNum(0);

    if(this->movie->frameCount() == (frameNumber + 1))
        loopNum++;

    if(loopNum == this->loopingCount)
    {
        loopNum = 0;
        this->movie->stop();
        this->movie->jumpToFrame(0);
    }
}

void MyListWidget::ResetStates()
{
    QPalette tmpPalette = this->palette();

    if(this->defaultDisbledColor.isValid())
        tmpPalette.setColor(QPalette::Disabled, QPalette::Base, this->defaultDisbledColor);

    this->setPalette(tmpPalette);

    if(this->movie != nullptr)
    {
        delete this->movie;
        this->movie = nullptr;
    }
    if(this->movieBackground != nullptr)
    {
        delete this->movieBackground;
        this->movieBackground = nullptr;
    }
}

void MyListWidget::SetEnabledWithReset(bool flag)
{
    this->setEnabled(flag);

    if(flag)
        this->ResetStates();
}

void MyListWidget::SetDisabledWithReset(bool flag)
{
    this->setDisabled(flag);

    if(flag)
        this->ResetStates();
}

void MyListWidget::resizeEvent(QResizeEvent *event)
{
    QListWidget::resizeEvent(event);

    if(this->movie != nullptr && this->movieBackground != nullptr)
    {
        this->movieBackground->setGeometry(1, 1, this->width() - 2, this->height() -2);

        if(this->movieBackground->height() > this->movieBackground->width())
            this->movie->setScaledSize(QSize(this->movieBackground->width(), this->movieBackground->width()));
        else
            this->movie->setScaledSize(QSize(this->movieBackground->height(), this->movieBackground->height()));

        if(this->movie->state() != QMovie::Running)
        {
            int currentState = this->movie->state();
            int currentFrame = this->movie->currentFrameNumber();

            this->movie->start();

            if(currentState == QMovie::NotRunning)
            {
                this->movie->jumpToFrame(currentFrame);
                this->movie->stop();
            } else if(currentState == QMovie::Paused)
            {
                this->movie->jumpToFrame(currentFrame);
                this->movie->setPaused(true);
            }
        }
    }
}

MyListWidget::~MyListWidget()
{
    if(this->movie != nullptr)
        delete this->movie;

    if(this->movieBackground != nullptr)
        delete this->movieBackground;
}
