/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mouse.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

#include <math.h>

#include "fuzzyling.h"
#include "fuzzyitem.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

int Mouse::getHealth(void)
{
    return m_iHealth;
}

void Mouse::setHealth(int h)
{
    m_iHealth += h;
}

//! [0]
Mouse::Mouse()
    : angle(0), speed(0), mouseEyeDirection(0),
      color(qrand() % 256, qrand() % 256, qrand() % 256)
{
    setRotation(qrand() % (360 * 16));

    // BEGIN IT3105
    fuzzy = new Fuzzy::Fuzzy();

    m_iHealth = 100;
    m_iRating = rand() % 100;

    /*
    FuzzyLing<int>* lingspeed = new FuzzyLing<int>((char*)"SPEED", INPUT);
    FuzzyLing<int>* linghealth = new FuzzyLing<int>((char*)"HEALTH", INPUT);
    FuzzyLing<int>* lingrange = new FuzzyLing<int>((char*)"RANGE", INPUT);

    FuzzyLing<int>::FuzzyLing* lingact = new FuzzyLing<int>((char*)"ACTION", OUTPUT);

    // set up speed
    lingspeed->Add(new FuzzyRev<int>::FuzzyRev((char*)"VERY LOW", 0, 10));
    lingspeed->Add(new FuzzyTrap<int>((char*)"LOW", 10,20,30,40));
    lingspeed->Add(new FuzzyTri<int>((char*)"AVERAGE", 25, 30, 45));
    lingspeed->Add(new FuzzyTrap<int>((char*)"HIGH", 30, 40, 50, 60 ));
    lingspeed->Add(new FuzzyGrade<int>((char*)"VERY HIGH", 60, 90));

    // set up health
    linghealth->Add(new FuzzyRev<int>((char*)"VERY LOW", 0, 10));
    linghealth->Add(new FuzzyTrap<int>((char*)"LOW", 10,20,30,40));
    linghealth->Add(new FuzzyTri<int>((char*)"AVERAGE", 25, 30, 45));
    linghealth->Add(new FuzzyTrap<int>((char*)"HIGH", 30, 40, 50, 60 ));
    linghealth->Add(new FuzzyGrade<int>((char*)"VERY HIGH", 60, 90));

    // set up range
    lingrange->Add(new FuzzyRev<int>((char*)"VERY NEAR", 0, 10));
    lingrange->Add(new FuzzyTrap<int>((char*)"NEAR", 10,20,30,40));
    lingrange->Add(new FuzzyTri<int>((char*)"MEDIUM", 25, 30, 45));
    lingrange->Add(new FuzzyTrap<int>((char*)"FAR", 30, 40, 50, 60 ));
    lingrange->Add(new FuzzyGrade<int>((char*)"VERY FAR", 60, 90));

    // set up action
    lingact->Add(new FuzzyRev<int>((char*)"FLEE", 0, 10));
    lingact->Add(new FuzzyTrap<int>((char*)"ATTACK", 10,20,30,40));
    lingact->Add(new FuzzyTri<int>((char*)"DONOTHING", 25, 30, 45));
    */

    // END IT3105
}
//! [0]

//! [1]
QRectF Mouse::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(-18 - adjust, -22 - adjust,
                  36 + adjust, 60 + adjust);
}
//! [1]

//! [2]
QPainterPath Mouse::shape() const
{
    QPainterPath path;
    path.addRect(-10, -20, 20, 40);
    return path;
}
//! [2]

//! [3]
void Mouse::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    int rating = this->getRating();

    if (rating > 75)
        painter->setBrush(Qt::black);
    else if (rating < 75 && rating > 25)
        painter->setBrush(Qt::blue);
    else
        painter->setBrush(Qt::green);

    //painter->setBrush(color);
    painter->drawEllipse(-10, -20, 20, 40);

    // Eyes
    painter->setBrush(Qt::white);
    painter->drawEllipse(-10, -17, 8, 8);
    painter->drawEllipse(2, -17, 8, 8);

    // Nose
    painter->setBrush(Qt::black);
    painter->drawEllipse(QRectF(-2, -22, 4, 4));

    // Pupils
    painter->drawEllipse(QRectF(-8.0 + mouseEyeDirection, -17, 4, 4));
    painter->drawEllipse(QRectF(4.0 + mouseEyeDirection, -17, 4, 4));

    // Ears
    painter->setBrush(scene()->collidingItems(this).isEmpty() ? Qt::darkYellow : Qt::red);
    painter->drawEllipse(-17, -12, 16, 16);
    painter->drawEllipse(1, -12, 16, 16);

    // Tail
    QPainterPath path(QPointF(0, 20));
    path.cubicTo(-5, 22, -5, 22, 0, 25);
    path.cubicTo(5, 27, 5, 32, 0, 30);
    path.cubicTo(-5, 32, -5, 42, 0, 35);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
}

void Mouse::advance(int step)
{
    if (!step)
        return;
    // Don't move too far away
    QLineF lineToCenter(QPointF(0, 0), mapFromScene(0, 0));
    if (lineToCenter.length() > 150) {
        qreal angleToCenter = ::acos(lineToCenter.dx() / lineToCenter.length());
        if (lineToCenter.dy() < 0)
            angleToCenter = TwoPi - angleToCenter;
        angleToCenter = normalizeAngle((Pi - angleToCenter) + Pi / 2);

        if (angleToCenter < Pi && angleToCenter > Pi / 4) {
            // Rotate left
            angle += (angle < -Pi / 2) ? 0.25 : -0.25;
        } else if (angleToCenter >= Pi && angleToCenter < (Pi + Pi / 2 + Pi / 4)) {
            // Rotate right
            angle += (angle < Pi / 2) ? 0.25 : -0.25;
        }
    } else if (::sin(angle) < 0) {
        angle += 0.25;
    } else if (::sin(angle) > 0) {
        angle -= 0.25;
    }

    // Try not to crash with any other mice
    QList<QGraphicsItem *> dangerMice = scene()->items(QPolygonF()
                                                       << mapToScene(0, 0)
                                                       << mapToScene(-30, -50)
                                                       << mapToScene(30, -50));

    int min = 1000000;

    Mouse* nearestMouse;

    foreach (QGraphicsItem *item, dangerMice) {
        if (item == this)
            continue;

        QLineF lineToMouse(QPointF(0, 0), mapFromItem(item, 0, 0));
        int len = lineToMouse.length();

        if (len < min)
        {
            nearestMouse = (Mouse*)item;
            min = len;
        }

        qreal angleToMouse = ::acos(lineToMouse.dx() / lineToMouse.length());
        if (lineToMouse.dy() < 0)
            angleToMouse = TwoPi - angleToMouse;
        angleToMouse = normalizeAngle((Pi - angleToMouse) + Pi / 2);

        if (angleToMouse >= 0 && angleToMouse < Pi / 2) {
            // Rotate right
            angle += 0.5;
        } else if (angleToMouse <= TwoPi && angleToMouse > (TwoPi - Pi / 2)) {
            // Rotate left
            angle -= 0.5;
        }
    }


    if (dangerMice.size() > 1 && (qrand() % 10) == 0) {
        if (qrand() % 1)
            angle += (qrand() % 100) / 500.0;
        else
            angle -= (qrand() % 100) / 500.0;

        }

    foreach (QGraphicsItem *item, scene()->collidingItems(this))
    {
        Mouse* otherMouse = (Mouse*) item;

        otherMouse->setHealth(otherMouse->getHealth() - (qrand()%5));

        if (otherMouse->getHealth() <= 0)
            otherMouse->removeFromIndex();

    }

    if (dangerMice.size() > 1)
        speed = fuzzy->action(this->getHealth(), nearestMouse->getRating(), min);
    else
    {
        speed += (-50 + qrand() % 100) / 100.0;

        qreal dx = ::sin(angle) * 10;
        mouseEyeDirection = (qAbs(dx / 5) < 1) ? 0 : dx / 5;

        setRotation(rotation() + dx);
    }

    setPos(mapToParent(0, -(3 + sin(speed) * 3)));
}
