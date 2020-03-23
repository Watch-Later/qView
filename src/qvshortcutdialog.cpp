#include "qvshortcutdialog.h"
#include "ui_qvshortcutdialog.h"
#include "qvapplication.h"

#include <QMessageBox>

#include <QDebug>

QVShortcutDialog::QVShortcutDialog(int index, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QVShortcutDialog)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    shortcutObject = qvApp->getActionManager()->getShortcutsList().value(index);
    this->index = index;
    ui->keySequenceEdit->setKeySequence(shortcutObject.shortcuts.join(", "));
}

QVShortcutDialog::~QVShortcutDialog()
{
    delete ui;
}

void QVShortcutDialog::done(int r)
{
    if (r == QDialog::Accepted)
    {
        return;
    }

    QDialog::done(r);
}

void QVShortcutDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
    {
        QStringList shortcutsStringList = ui->keySequenceEdit->keySequence().toString().split(", ");
        QList<QKeySequence> sequenceList = ActionManager::stringListToKeySequenceList(shortcutsStringList);

        foreach (QKeySequence sequence, sequenceList)
        {
            auto conflictingShortcut = shortcutAlreadyBound(sequence, shortcutObject.name);
            if (!conflictingShortcut.isEmpty())
            {
                QString nativeShortcutString = sequence.toString(QKeySequence::NativeText);
                QMessageBox::warning(this, tr("Shortcut Already Used"), tr("\"") + nativeShortcutString + tr("\" is already bound to \"") + conflictingShortcut + "\"");
                return;
            }
        }

        acceptValidated();

        emit shortcutsListChanged(index, shortcutsStringList);
    }
    else if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::ResetRole)
    {
        ui->keySequenceEdit->setKeySequence(shortcutObject.defaultShortcuts.join(", "));
    }
}

QString QVShortcutDialog::shortcutAlreadyBound(QKeySequence chosenSequence, QString exemptShortcut)
{
    if (chosenSequence.isEmpty())
        return "";

    auto shortcutsList = qvApp->getActionManager()-> getShortcutsList();
    foreach(auto shortcut, shortcutsList)
    {
        auto sequenceList = ActionManager::stringListToKeySequenceList(shortcut.shortcuts);

        if (sequenceList.contains(chosenSequence) && shortcut.name != exemptShortcut)
            return shortcut.readableName;
    }
    return "";
}

void QVShortcutDialog::acceptValidated()
{
    QDialog::done(1);
}
