/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2014  <copyright holder> <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "clock.hpp"
#include "../../g19daemon.hpp"
#include "../../gscreen.hpp"
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QString>
#include <QTime>
#include <QTimer>
#include <QtCore>
#include <QPoint>
#include <QFont>

Clock::Clock() {
  Q_INIT_RESOURCE(clock);

  isActive = false;
  screen = new Gscreen(QImage(":/clock/icon.png"), tr("Clock"));

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, QOverload<>::of(&Clock::paint));
  timer->start(1000);
}

Clock::~Clock() { delete screen; }

QString Clock::getName() { return tr("Clock"); }


void Clock::lKeys(int keys) {}

void Clock::mKeys(int keys) {}

void Clock::setActive(bool active) {
  isActive = active;

  if (active) {
    paint();
  }
}

QString Clock::getCurrentTime() {
    int m_hour, m_minute, m_second;
  QTime t = QTime::currentTime();
  QString retVal = t.toString("hh:mm:ss");
  m_hour = ( (t.hour()%12)*5 + int(t.minute()/12) ) % 60;
  m_minute = t.minute();
  m_second = t.second();
  return retVal;
}
void Clock::paint() {
  if (isActive) {

    QPainter *p;
    static const QPoint hourHand[4] = {QPoint(-4, 0), QPoint(-1, -50/2),
                                       QPoint(1, -50/2),QPoint(4, 0)};
    static const QPoint minuteHand[4] = {QPoint(-4, 0), QPoint(-1, -70/2),
                                         QPoint(1, -70/2),QPoint(4, 0)};
    static const QPoint secondHand[4] = {QPoint(-4, 0), QPoint(-1, -96/2),
                                         QPoint(1, -96/2),QPoint(4, 0)};

    QColor hourColor(255, 25, 0, 168);
    QColor hourColor1(Qt::red);
    QColor minuteColor(0, 169, 181, 168);
    QColor minuteColor1(Qt::blue);
    QColor secondColor(Qt::green);

    int side = qMin(320, 240);
    QTime time = QTime::currentTime();
    QConicalGradient mitte = QConicalGradient();
    mitte.setCenter(0,0);
    mitte.setAngle(90);
    mitte.setColorAt(0.0,qRgb(192, 192, 192));
    mitte.setColorAt(0.5,Qt::black);
    mitte.setColorAt(1.0,qRgb(240, 240, 240));
    QLinearGradient background(0, 0, 0, 240);
    QLinearGradient header(0, 0, 0, 30);
QString fontName = "Noto Sans";
QFont fontText = QFont(fontName, 16, 75);

QRadialGradient glass = QRadialGradient();
glass.setRadius(360);
glass.setFocalPoint(QPointF(25, 25));
glass.setColorAt(0, QColor(255, 255, 255, 0));
glass.setColorAt(1, QColor(192, 192, 192));

    background.setColorAt(0, qRgb(0, 0, 0));
    background.setColorAt(1, qRgb(0, 115, 153));
    header.setColorAt(0, qRgb(137, 182, 196));
    header.setColorAt(0.5, qRgb(0, 165, 219));
    header.setColorAt(1, qRgb(0, 105, 140));
    header.setColorAt(0, qRgb(137, 182, 196));
    QColor clockBG(169, 169, 169);
    p = screen->beginFullScreen();


    p->setRenderHint(QPainter::Antialiasing);
    p->translate(320 / 2, 240 / 2);
    p->scale(side / 200.0, side / 200.0);

    p->setBrush(QBrush(Qt::black));

    p->setPen(Qt::NoPen);
    p->setBrush(secondColor);
    p->save();
    p->rotate(6.0 * time.second());
    p->drawConvexPolygon(secondHand, 4);
    p->restore();

    p->setPen(Qt::NoPen);
    p->setBrush(minuteColor1);
    p->save();
    p->rotate(6.0 * (time.minute() + time.second() / 60.0));
    p->drawConvexPolygon(minuteHand, 4);
    p->restore();
    p->setPen(minuteColor1);

    for (int j = 0; j < 60; ++j) {
      if ((j % 5) != 0)
        p->drawLine(92/2, 0, 96/2, 0);
      p->rotate(6.0);
    }

    p->setPen(Qt::NoPen);
    p->setBrush(hourColor1);
    p->save();
    p->rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    p->drawConvexPolygon(hourHand, 4);
    p->restore();
    p->setPen(hourColor1);
    for (int i = 0; i < 12; ++i) {
      p->drawLine(88/2, 0, 96/2, 0);
      p->rotate(30.0);
    }
        p->save();
    p->translate(-160, -75);
        p->setFont(fontText);
        p->setBrush(hourColor);
        p->setPen(hourColor);
        p->drawText(0, 0, 320, 20, Qt::AlignCenter ,getCurrentTime()) ;
        p->restore();
        p->setBrush(mitte);
        p->setPen(Qt::NoPen);
p->drawEllipse(-6, -6, 12,12);
        p->setBrush(glass);
        p->setPen(Qt::NoPen);
p->drawEllipse(-100/2,-100/2,200/2,200/2);


    screen->end();
    emit doAction(displayFullScreen, screen);
  }
}

bool Clock::isPopup() { return false; }

QObject *Clock::getQObject() { return this; }

QImage Clock::getIcon() { return QImage(":/clock/menu_icon.png"); }
