/** @file mainDialog.h
    This file is part of Playbook Creator.

    Playbook Creator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Playbook Creator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Playbook Creator.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2015 Oliver Braunsdorf

    @author Oliver Braunsdorf
*/

#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QMainWindow>

#include "gui/pbcPlayView.h"
#include <string>

namespace Ui {
class MainDialog;
}

class MainDialog : public QMainWindow {
    Q_OBJECT

 private:
    Ui::MainDialog *ui;
    PBCPlayView* _playView;
    std::list<PBCPlaySP> _currentlySelectedPlays;
    std::list<PBCPlaySP>::const_iterator _currentPlay;

    void updateTitle(bool saved);
    void enableMenuOptions();
    void fillPlayInfoDock(PBCPlaySP play);
    void resizeEvent(QResizeEvent* e);
    void wheelEvent(QWheelEvent *event);

 public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();
    void show();
    void fillPlayerInfoDock(PBCPlayerSP player);
    void keyReleaseEvent(QKeyEvent *event);

 public slots:
    void exit();
    void showNewPlay();
    void openPlay();
    void nextPlay();
    void previousPlay();
    void savePlay();
    void savePlayAs();
    void saveFormationAs();
    void newPlaybook();
    void savePlaybookAs();
    void openPlaybook();
    void importPlaybook();
    void exportAsPDF();
    void showAboutDialog();
    void addPlayToCategory();
    void deleteRoutes();
    void deletePlays();
    void deleteFormations();
    void deleteCategories();
    void changeActivePlayerColor(QColor color);
    void changeActivePlayerRoute(int index);
};

#endif  // MAINDIALOG_H
