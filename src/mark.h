#ifndef MARK_H
#define MARK_H

#include "polygonclass.h"

#include <QMainWindow>
#include <QScopedPointer>
#include <QListWidgetItem>
#include <QFileSystemWatcher>

namespace Ui {
class marK;
}

class marK : public QMainWindow
{
    Q_OBJECT

public:
    explicit marK(QWidget *parent = nullptr);
    ~marK() override;

public:
    void changeDirectory();
    void changeImage(int currentRow);
    void updateFiles();

public slots:
    void changeImage(QListWidgetItem *current, QListWidgetItem *previous);
    void updateFiles(const QString &path);
    void addNewClass();
    void selectClassColor();

private:
    QScopedPointer<Ui::marK> m_ui;
    QFileSystemWatcher *m_watcher;
    QString m_currentDirectory;
    QVector<PolygonClass*> m_polygonClasses;
};

#endif // MARK_H
