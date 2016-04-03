// Orion: constellation graph generator
// Copyright (C) 2015--2016  Luiz Romário Santana Rios <luizromario@gmail.com>
// Copyright (C) 2015--2016  Paula Patrícia Santana Rios <riospaularios@gmail.com>
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

#include "mainwindow.hpp"

#include <limits>
#include <QPainter>

using namespace std;

static double distanceFromCenter(
    const double o,    // occurrences
    const double rMin, // minimum radius
    const double rMax, // maximum radius
    const double b,    // minimum value
    const double t)    // maximum value
{
    return 2 * (rMax - rMin) / ((o - b) / (t - b) + 1) + 2 * rMin - rMax;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    _palavras{{"Palavra", "Ocorrências"}}
{
    _ui.setupUi(this);
    _palavrasSorted.setSourceModel(&_palavras);
    _ui.palavrasTable->setModel(&_palavrasSorted);
    _ui.palavrasTable->horizontalHeader()->setSectionsClickable(true);

    _palavras.addEditRoleFunction<0>([](QString &s, const QVariant &val)
    {
        s = val.toString();

        return true;
    });

    _palavras.addEditRoleFunction<1>([](short &i, const QVariant &val)
    {
        i = val.toInt();

        return true;
    });

    QImage i{400, 400, QImage::Format_RGB32};

    i.fill(Qt::white);
    _ui.image->setPixmap(QPixmap::fromImage(std::move(i)));

    connect(_ui.adicionar, &QPushButton::clicked, [this]()
    {
        _palavras.append(make_tuple(_ui.novaPalavra->text(), _ui.ocorrencias->value()));
        _ui.novaPalavra->setText({});
        _ui.ocorrencias->setValue(1);
    });

    connect(_ui.palavrasTable->horizontalHeader(), &QHeaderView::sectionClicked, [this](int section)
    {
        _ui.palavrasTable->sortByColumn(
            section, section == 0? Qt::AscendingOrder : Qt::DescendingOrder);
    });

    connect(_ui.gerarGrafico, &QPushButton::clicked, [this]()
    {
        int minValue = numeric_limits<int>::max(),
            maxValue = 0;

        for (int i = 0; i < _palavras.rowCount(); ++i) {
            const auto &p = _palavras.row(i);
            const auto &pCount = std::get<1>(p);

            if (minValue > pCount)
                minValue = pCount;

            if (maxValue < pCount)
                maxValue = pCount;
        }

        QImage img{400, 400, QImage::Format_RGB32};
        QPainter painter{&img};
        painter.setRenderHint(QPainter::Antialiasing);
        const auto angleIncrement = 360.0 / _palavras.rowCount();
        qreal curAngle = 0;

        const auto initialPen = painter.pen();
        const auto initialBrush = painter.brush();

        QFont font;
        font.setPointSize(12);
        painter.setFont(font);
        painter.setBrush(Qt::white);

        img.fill(Qt::white);

        const auto estimuloText = _ui.estimulo->text();
        const auto estimuloWidth = painter.fontMetrics().width(estimuloText);
        const QPoint estimuloPoint{200 - estimuloWidth / 2, 200};

        painter.drawText(estimuloPoint, estimuloText);

        const auto minRadius = qMax(50, estimuloWidth / 2 + 5);
        const auto maxRadius = 180;

        painter.setPen(Qt::gray);

        for (int radius = minRadius; radius <= 200; radius += 25) {
            const auto x_y = 200 - radius;
            const auto diam = radius * 2;
            painter.drawArc(x_y, x_y, diam, diam, 0, 5760);
        }

        painter.setPen(initialPen);

        for (int i = 0; i < _palavras.rowCount(); ++i) {
            const auto &p = _palavras.row(i);
            const auto &pString = std::get<0>(p);
            const auto &pCount = std::get<1>(p);
            QLineF l{{200, 200}, {50, 200}};

            l.setLength(distanceFromCenter(pCount, minRadius, maxRadius, minValue, maxValue));
            l.setAngle(curAngle);
            curAngle += angleIncrement;

            painter.setPen(Qt::NoPen);
            painter.setBrush(Qt::red);
            painter.drawEllipse(l.p2(), 4, 4);
            painter.setPen(initialPen);
            painter.setBrush(initialBrush);

            const auto text = pString + "(" + QString::number(pCount) + ")";
            const auto textWidth = painter.fontMetrics().width(text);
            const auto textPoint =
                l.p2().x() + textWidth >= 400?
                    l.p2() - QPointF{static_cast<qreal>(textWidth), 0} : l.p2();

            painter.drawText(textPoint, text);
        }

        _ui.image->setPixmap(QPixmap::fromImage(std::move(img)));
    });
}
