// Orion: constellation graph generator
// Copyright (C) 2015  Luiz Romário Santana Rios <luizromario@gmail.com>
// Copyright (C) 2015  Paula Patrícia Santana Rios <riospaularios@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include <QSortFilterProxyModel>

#include "ui_mainwindow.h"
#include "qtmvt.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private:
    Ui::MainWindow _ui;
    QtMVT::Model::List<QString, short> _palavras;
    QSortFilterProxyModel _palavrasSorted;
};

#endif // MAINWINDOW_HPP