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

#include "core/markedclass.h"
#include "core/serializer.h"

#include <QFileSystemWatcher>
#include <QListWidgetItem>
#include <QScopedPointer>

#include <KXmlGuiWindow>

namespace Ui {
class marK;
}

/** Main class, also represents the main window. */
class marK : public KXmlGuiWindow
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
     * @param item - item to load.
     */
    void changeItem(QListWidgetItem *item);

    /** Update the known files in the current working directory. */
    void updateFiles();

    /** Create a new MarkedClass and add it to comboBox. */
    void addNewClass();

    /** Select the class color. */
    void selectClassColor();

    /** Save Annotation in the selected file.
     * @param outputType - type of output to save in the file.
     */
    void saveObjects(Serializer::OutputType outputType);

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
