#include "filetreeview.h"

void FileTreeView::mousePressEvent(QMouseEvent *event)
{
    QTreeView::mousePressEvent(event);
    if(event->button() == Qt::RightButton){
        if(!indexAt(event->pos()).isValid())
            emit ContextMenuRootDir(event->pos());
        else
            emit customContextMenuRequested(event->pos());
    }

}

void FileTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTreeView::mouseDoubleClickEvent(event);
    emit OpenFile(indexAt(event->pos()));
}

FileTreeView::FileTreeView(QWidget* parent) : QTreeView{parent}
{}
