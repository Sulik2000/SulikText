#ifndef FILETREEVIEW_H
#define FILETREEVIEW_H

#include <QObject>
#include <QTreeView>
#include <QtGui/qevent.h>

class FileTreeView : public QTreeView
{
    Q_OBJECT
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
signals:
    void OpenFile(QModelIndex index);
public:
    FileTreeView(QWidget* parent = nullptr);
};

#endif // FILETREEVIEW_H
