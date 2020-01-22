/*************************************************************************
 *  Copyright (C) 2020 by Caio Jordão Carvalho <caiojcarvalho@gmail.com> *
 *                                                                       *
 *  This program is free software; you can redistribute it and/or        *
 *  modify it under the terms of the GNU General Public License as       *
 *  published by the Free Software Foundation; either version 3 of       *
 *  the License, or (at your option) any later version.                  *
 *                                                                       *
 *  This program is distributed in the hope that it will be useful,      *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 *************************************************************************/

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
    enum class OutputType {
        XML,
        JSON
    };

public:
    explicit marK(QWidget *parent = nullptr);
    ~marK() override;

public:
    void changeDirectory();
    void changeImage(int currentRow);
    void updateFiles();
    void savePolygons(OutputType type);
    QString handleFilename(OutputType type);

public slots:
    void changeImage(QListWidgetItem *current, QListWidgetItem *previous);
    void updateFiles(const QString &path);
    void addNewClass();
    void selectClassColor();
    void saveToJson() { savePolygons(OutputType::JSON); };
    void saveToXml() { savePolygons(OutputType::XML); };

private:
    QScopedPointer<Ui::marK> m_ui;
    QFileSystemWatcher *m_watcher;
    QString m_currentDirectory;
    QString m_filepath;
    QVector<PolygonClass*> m_polygonClasses;
};

#endif // MARK_H
