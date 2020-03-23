#ifndef QVSHORTCUTDIALOG_H
#define QVSHORTCUTDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include "actionmanager.h"

namespace Ui {
class QVShortcutDialog;
}

class QVShortcutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QVShortcutDialog(int index, QWidget *parent = nullptr);
    ~QVShortcutDialog() override;

    QString shortcutAlreadyBound(QKeySequence chosenSequence, QString exemptShortcut);
    void acceptValidated();

signals:
    void shortcutsListChanged(int index, QStringList shortcutsStringList);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    void done(int r) override;

    Ui::QVShortcutDialog *ui;

    ActionManager::SShortcut shortcutObject;
    int index;
};

#endif // QVSHORTCUTDIALOG_H
