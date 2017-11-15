#ifndef CHANGELIST_H
#define CHANGELIST_H

#include "headers.h"

namespace Ui {
class ChangeList;
}

class ChangeList : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeList(QWidget *parent = 0);
    ~ChangeList();

private:
    Ui::ChangeList *ui;
};

#endif // CHANGELIST_H
