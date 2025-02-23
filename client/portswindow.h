/*
Copyright (C) 2010 Srivats P.

This file is part of "Ostinato"

This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef _PORTS_WINDOW_H
#define _PORTS_WINDOW_H

#include "ui_portswindow.h"
#include <QWidget>

class ApplyMessage;
class PortGroupList;

class QProgressDialog;
class QSortFilterProxyModel;

namespace OstProto {
    class SessionContent;
}

class PortsWindow : public QWidget, private Ui::PortsWindow
{
    Q_OBJECT

public:
    PortsWindow(PortGroupList *pgl, QWidget *parent = 0);
    ~PortsWindow();

    int portGroupCount();
    int reservedPortCount();

    bool openSession(const OstProto::SessionContent *session,
                     QString &error);
    bool saveSession(OstProto::SessionContent *session,
                     QString &error,
                     QProgressDialog *progress = NULL);

    QList<QAction*> portActions();
    QList<QAction*> streamActions();
    QList<QAction*> deviceActions();

signals:
    void currentPortChanged(const QModelIndex &current,
                            const QModelIndex &previous);

public slots:
    void clearCurrentSelection();
    void showMyReservedPortsOnly(bool enabled);

private slots:
    void updateApplyHint(int portGroupId, int portId, bool configChanged);
    void updatePortViewActions(const QModelIndex& currentIndex);

    void when_portView_currentChanged(const QModelIndex& currentIndex,
        const QModelIndex& previousIndex);
    void when_portModel_dataChanged(const QModelIndex& topLeft,
        const QModelIndex& bottomRight);
    void when_portModel_reset();

    void on_pbApply_clicked();    

    void on_actionNew_Port_Group_triggered();
    void on_actionDelete_Port_Group_triggered();
    void on_actionConnect_Port_Group_triggered();
    void on_actionDisconnect_Port_Group_triggered();

    void on_actionExclusive_Control_triggered(bool checked);
    void on_actionPort_Configuration_triggered();

private:
    PortGroupList *plm;
    QString lastNewPortGroup;
    QSortFilterProxyModel *proxyPortModel;
    ApplyMessage *applyMsg_;
};

#endif

