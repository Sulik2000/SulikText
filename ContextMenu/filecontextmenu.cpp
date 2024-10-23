#include "filecontextmenu.h"

FileContextMenu::FileContextMenu(QString path, QWidget* parent) : QMenu(parent)
{
    pathToFile = path;

    QAction *actionOpen = new QAction("Open file", this);
    connect(actionOpen, SIGNAL(triggered(bool)), this, SLOT(OpenFile(bool)));
    addAction(actionOpen);

    QAction *actionDelete = new QAction("Delete file", this);
    connect(actionDelete, SIGNAL(triggered(bool)), this, SLOT(DeleteFile()));
    addAction(actionDelete);

    QAction *actionRename = new QAction("Rename file", this);
    connect(actionRename, SIGNAL(triggered(bool)), this, SLOT(RenameFile()));
    addAction(actionRename);
}

void FileContextMenu::OpenFile()
{
    emit OpenFile(pathToFile);
}

void FileContextMenu::DeleteFile()
{
    emit DeleteFile(pathToFile);
}

void FileContextMenu::RenameFile()
{
    emit RenameFile(pathToFile);
}


