#ifndef DIRCONTEXTMENU_H
#define DIRCONTEXTMENU_H

#include <QMenu>
#include <QObject>
#include <QWidget>
#include <QTabWidget>

class DirContextMenu : public QMenu
{
    Q_OBJECT
private:
    QString pathToDir;
signals:
    void AddNewFile(QString dirPath);
    void RemoveFolder(QString dirPath);
    void AddNewFolder(QString dirPath);
private slots:
    void AddFile();
    void RemoveFolder();
    void AddFolder();
public:
    DirContextMenu(QString path, QWidget* parent = nullptr);
};

#endif // DIRCONTEXTMENU_H
