#include "dircontextmenu.h"

void DirContextMenu::AddFile()
{
    emit AddNewFile(pathToDir);
}

void DirContextMenu::RemoveFolder()
{
    emit RemoveFolder(pathToDir);
}

void DirContextMenu::AddFolder()
{
    emit AddNewFolder(pathToDir);
}

DirContextMenu::DirContextMenu(QString path, QWidget* parent) : QMenu(parent) {
    pathToDir = path;
    QAction *action = new QAction("Add file", this);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(AddFile()));
    addAction(action);

    QAction *removeAction = new QAction("Remove folder", this);
    connect(removeAction, SIGNAL(triggered(bool)), this, SLOT(RemoveFolder()));
    addAction(removeAction);

    QAction *addFolderAction = new QAction("Add folder", this);
    connect(addFolderAction, SIGNAL(triggered(bool)), this, SLOT(AddFolder()));
    addAction(addFolderAction);
}
