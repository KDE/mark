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

#include "ui/markedclass.h"
#include "ui/serializer.h"

#include <QMainWindow>
#include <QScopedPointer>
#include <QListWidgetItem>
#include <QFileSystemWatcher>

namespace Ui {
class marK;
}

/** Main class, also represents the main window. */
class marK : public QMainWindow
{
    Q_OBJECT

public:
    explicit marK(QWidget *parent = nullptr);
    ~marK() override;

public:
    /** Change directory. */
    void changeDirectory();

    /** Navegate through items with the arrows keys up and down.
     * @param count - change index based in this number.
     */
    void changeIndex(const int count);

    /** Update known files in the working directory. */
    void updateFiles();

    /** Update comboBox reloading all MarkedClass's objects. */
    void updateComboBox();

    /** If exists, load annotated data from temporary file. */
    void retrieveTempFile();

    /** Save state into a temporary file. */
    void makeTempFile();

    /** If selected in the Edit options an Serializer::OutputType different than None, auto save annotation. */
    void autoSave();

public slots:

    /** Change working file.
     * @param current - current item in the widget list.
     * @param previous - previous item in the widget list.
     */
    void changeItem(QListWidgetItem *current, QListWidgetItem *previous);

    /** Update known files in the given directory and make it the current working directory.
     * @param path - path of the directory.
     */
    void updateFiles(const QString &path);

    /** Create a new MarkedClass and add it to comboBox. */
    void addNewClass();

    /** Select the class color. */
    void selectClassColor();

    /** Save Annotation in the selected file.
     * @param type - type of output to save in the file.
     */
    void saveObjects(Serializer::OutputType type);

    /** Import annotation data. */
    void importData();

    /** Toggle autosave to JSON, XML or None. Default is None. */
    void toggleAutoSave();

private:
    /** Setup Actions. */
    void setupActions();

    /** Setup connections. */
    void setupConnections();

private:
    QScopedPointer<Ui::marK> m_ui;
    QFileSystemWatcher *m_watcher;
    QString m_currentDirectory;
    QVector<MarkedClass*> m_objClasses;
    QString m_filepath;
    Serializer::OutputType m_autoSaveType;
};

#endif // MARK_H
