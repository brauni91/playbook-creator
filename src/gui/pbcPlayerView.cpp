/** @file pbcPlayerView.cpp
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

#include "pbcPlayerView.h"
#include "models/pbcPlaybook.h"
#include "QBrush"
#include "QMenu"
#include "util/pbcPositionTranslator.h"
#include "dialogs/pbcCustomRouteDialog.h"
#include <boost/unordered/unordered_map.hpp>
#include <utility>
#include "QDebug"
#include <iostream>
#include <vector>
#include <set>
#include "dialogs/pbcCreateMotionRouteDialog.h"
#include <QColorDialog>
#include <QInputDialog>

/**
 * @class PBCPlayerView
 * @brief A graphical representation of a football player.
 *
 * This includes the player's "body", motion and route. They are all
 * QGraphicItems which are combined as an QGraphicsItemGroup. Instances of
 * PBCPlayerView are typically displayed in PBCPlayView and
 * PBCCustomRouteDialog.
 */

/**
 * @brief The constructor.
 * @param playerSP The data model of the player to display
 */
PBCPlayerView::PBCPlayerView(PBCPlayerSP playerSP) : _playerSP(playerSP) {
    repaint();
}

/**
 * @brief Painting the player represented  by _playerSP
 */
void PBCPlayerView::repaint() {
    _originalPos = PBCPositionTranslator::getInstance()->translatePos(
        _playerSP->pos()); // NOLINT
    unsigned int playerWidth = PBCConfig::getInstance()->playerWidth();
    double playerPosX = _originalPos.get<0>() - playerWidth / 2;
    double playerPosY = _originalPos.get<1>();
    _playerEllipseSP.reset(new QGraphicsEllipseItem(playerPosX, playerPosY,
                                                    playerWidth, playerWidth));

    PBCColor color = _playerSP->color();
    _playerEllipseSP->setBrush(QBrush(QColor(color.r(), color.g(), color.b())));
    this->addToGroup(_playerEllipseSP.get());
    this->setFlag(QGraphicsItem::ItemIsMovable);

    if (_playerSP->motion() != NULL) {
        applyMotion(_playerSP->motion());
    }
    if (_playerSP->route() != NULL) {
        applyRoute(_playerSP->route());
    }
}

/**
 * @brief Concatenates the lines of the motion and the
 * route of the player.
 *
 * I wrote that function some time ago and it confuses me now^^. I don't know
 * what the meaning of the parameters is..but it works ;)
 * @param paths
 * @param graphicItems
 * @param basePoint
 */
void PBCPlayerView::joinPaths(const std::vector<PBCPathSP> &paths,
                              std::vector<QGraphicsItemSP> *graphicItems,
                              PBCDPoint basePoint) {
    assert(graphicItems == &_routePaths || graphicItems == &_motionPaths);
    PBCColor color = _playerSP->color();
    QPen pen(QBrush((QColor(color.r(), color.g(), color.b()))),
             PBCConfig::getInstance()->routeWidth());
    if (graphicItems == &_motionPaths) {
        pen.setStyle(Qt::DashLine);
    }
    double baseX = basePoint.get<0>();
    double baseY = basePoint.get<1>();
    double lastX = baseX;
    double lastY = baseY;
    int inOutFactor = -1;
    if (basePoint.get<0>() < PBCConfig::getInstance()->canvasWidth() / 2) {
        inOutFactor = 1;
    }
    for (PBCPathSP path : paths) {
        PBCDPoint endPointYd(inOutFactor * path->endpoint().get<0>(),
                             path->endpoint().get<1>());
        PBCDPoint endPointPixel =
            PBCPositionTranslator::getInstance()->translatePos(
                endPointYd, basePoint); // NOLINT
        unsigned int endPointX = endPointPixel.get<0>();
        unsigned int endPointY = endPointPixel.get<1>();
        if (path->isArc() == true) {
            PBCDPoint lastYd =
                PBCPositionTranslator::getInstance()->retranslatePos(
                    PBCDPoint(lastX, lastY), basePoint); // NOLINT
            PBCDPoint lastToEndPointYd(endPointYd.get<0>() - lastYd.get<0>(),
                                       endPointYd.get<1>() - lastYd.get<1>());
            PBCDPoint topleftYd;
            PBCDPoint bottomrightYd;
            unsigned int startAngle;
            int arcLength;
            if (path->isConcave() == false) {
                // motion is concave
                topleftYd = PBCDPoint(-1 * lastToEndPointYd.get<0>(),
                                      2 * lastToEndPointYd.get<1>());
                bottomrightYd = PBCDPoint(lastToEndPointYd.get<0>(), 0);
                startAngle = 270;
                arcLength = 90;
            } else {
                // motion is convex
                if (endPointX > baseX) {
                    // motion to the right
                    topleftYd = PBCDPoint(0, -1 * lastToEndPointYd.get<1>());
                    bottomrightYd = PBCDPoint(2 * lastToEndPointYd.get<0>(),
                                              lastToEndPointYd.get<1>());
                    startAngle = 180;
                    arcLength = 90;

                } else if (endPointX < baseX) {
                    // motion to the left
                    topleftYd = PBCDPoint(2 * lastToEndPointYd.get<0>(),
                                          -1 * lastToEndPointYd.get<1>());
                    bottomrightYd = PBCDPoint(0, lastToEndPointYd.get<1>());
                    startAngle = 0;
                    arcLength = -90;

                } else {
                    // TODO(obr): message to user: no strait motions
                    assert(false);
                }
            }
            PBCDPoint topleftPixel =
                PBCPositionTranslator::getInstance()->translatePos(
                    topleftYd, PBCDPoint(lastX, lastY)); // NOLINT
            PBCDPoint bottomrightPixel =
                PBCPositionTranslator::getInstance()->translatePos(
                    bottomrightYd, PBCDPoint(lastX, lastY)); // NOLINT
            QPointF topleft(topleftPixel.get<0>(), topleftPixel.get<1>());
            QPointF bottomright(bottomrightPixel.get<0>(),
                                bottomrightPixel.get<1>());
            QRectF arcRect(topleft, bottomright);
            QPainterPath motionArc(QPointF(topleft.x(), topleft.y()));
            motionArc.arcMoveTo(arcRect, startAngle);
            motionArc.arcTo(arcRect, startAngle, arcLength);
            boost::shared_ptr<QGraphicsPathItem> motionPathSP(
                new QGraphicsPathItem(motionArc, this));

            motionPathSP->setPen(pen);
            graphicItems->push_back(motionPathSP);
            lastX = motionArc.currentPosition().x();
            lastY = motionArc.currentPosition().y();
        } else {
            boost::shared_ptr<QGraphicsLineItem> lineSP(
                new QGraphicsLineItem(lastX, lastY, endPointX, endPointY));

            lineSP->setPen(pen);
            graphicItems->push_back(lineSP);
            lastX = endPointX;
            lastY = endPointY;
        }
    }
}

/**
 * @brief Sets a player's route and paints it.
 * @param route The route to apply
 */
void PBCPlayerView::applyRoute(PBCRouteSP route) {
    for (QGraphicsItemSP item : _routePaths) {
        this->removeFromGroup(item.get());
    }
    _routePaths.clear();

    _playerSP->setRoute(route);

    PBCDPoint playerPos = PBCPositionTranslator::getInstance()->translatePos(
        _playerSP->pos()); // NOLINT
    int inOutFactor = -1;
    if (playerPos.get<0>() < PBCConfig::getInstance()->canvasWidth() / 2) {
        inOutFactor = 1;
    }
    if (_playerSP->motion() != NULL) {
        PBCDPoint correctMotionEndPoint(
            inOutFactor *
                _playerSP->motion()->motionEndPoint().get<0>(), // NOLINT
            _playerSP->motion()->motionEndPoint().get<1>());    // NOLINT

        PBCDPoint base = PBCPositionTranslator::getInstance()->translatePos(
            correctMotionEndPoint, playerPos); // NOLINT
        joinPaths(route->paths(), &_routePaths, base);
    } else {
        joinPaths(route->paths(), &_routePaths, playerPos);
    }

    for (boost::shared_ptr<QGraphicsItem> item : _routePaths) {
        this->addToGroup(item.get());
    }
}

/**
 * @brief Sets a player's motion and paints it.
 * @param motion The motion to apply
 */
void PBCPlayerView::applyMotion(PBCMotionSP motion) {
    for (boost::shared_ptr<QGraphicsItem> item : _motionPaths) {
        this->removeFromGroup(item.get());
    }
    _motionPaths.clear();

    _playerSP->setMotion(motion);

    PBCDPoint playerPos = PBCPositionTranslator::getInstance()->translatePos(
        _playerSP->pos()); // NOLINT
    joinPaths(motion->paths(), &_motionPaths, playerPos);

    for (boost::shared_ptr<QGraphicsItem> item : _motionPaths) {
        this->addToGroup(item.get());
    }

    if (_playerSP->route() != NULL) {
        applyRoute(_playerSP->route());
    }
}

/**
 * @brief Changes the color of the player
 * @param color The new color
 */
void PBCPlayerView::setColor(PBCColor color) {
    _playerSP->setColor(color);
    repaint();
}

/**
 * @brief Changes the position of the player
 * @param x The horizontal position in yards
 * @param y The vertical position in yards
 */
void PBCPlayerView::setPosition(double x, double y) {
    PBCDPoint pos(x, y);
    _playerSP->setPos(pos);
    repaint();
}

/**
 * @brief Handles context menu clicks
 *
 * This is a Qt event hander that is triggered when a right click is performed
 * on an instance of PBCPlayerView. It displays the context menu and detects
 * which menu item has been selected.
 * @param event event datastructure, includes position of context menu click
 */
void PBCPlayerView::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QMenu menu;
    QMenu *routeMenu = menu.addMenu(QString::fromStdString("Apply Route"));
    boost::unordered_map<QAction *, PBCRouteSP> actionMap;
    for (PBCRouteSP route : PBCPlaybook::getInstance()->routes()) {
        QString routeString = QString::fromStdString(route->name());
        if (route->codeName() != "") {
            routeString.append(" (");
            routeString.append(QString::fromStdString(route->codeName()));
            routeString.append(")");
        }
        QAction *action = routeMenu->addAction(routeString);
        actionMap.insert(std::make_pair(action, route));
    }

    routeMenu->addSeparator();
    QAction *action_CustomRouteCreate =
        routeMenu->addAction("Create custom route");
    QAction *action_CustomRouteCreate_Graphical =
        routeMenu->addAction("Create custom route (graphical)");

    QAction *action_ApplyMotion = menu.addAction("Apply Motion");
    QAction *action_SetColor = menu.addAction("Set Color");
    QAction *action_SetPosition = menu.addAction("Set Position");
    QAction *clicked = menu.exec(event->screenPos());
    setEnabled(false);
    setEnabled(true);

    bool routeClicked = false;
    for (const auto &kv : actionMap) {
        if (clicked == kv.first) {
            routeClicked = true;
            this->applyRoute(kv.second);
        }
    }
    if (routeClicked == false) {
        if (clicked == action_CustomRouteCreate_Graphical) {
            PBCCustomRouteDialog dialog;
            dialog.setWindowModality(Qt::ApplicationModal);
            PBCRouteSP createdRoute = dialog.execute();
            if (createdRoute != NULL) {
                this->applyRoute(createdRoute);
            }
        } else if (clicked == action_CustomRouteCreate) {
            PBCCreateMotionRouteDialog dialog;
            PBCRouteSP createdRoute = dialog.getCreatedRoute();
            if (createdRoute != NULL) {
                this->applyRoute(createdRoute);
            }
        } else if (clicked == action_ApplyMotion) {
            PBCCreateMotionRouteDialog dialog;
            PBCMotionSP createdMotion = dialog.getCreatedMotion();
            if (createdMotion != NULL) {
                this->applyMotion(createdMotion);
            }
        } else if (clicked == action_SetColor) {
            QColor color = QColorDialog::getColor(Qt::black);
            if (color.isValid()) {
                this->setColor(
                    PBCColor(color.red(), color.green(), color.blue()));
            }
        } else if (clicked == action_SetPosition) {
            bool ok1, ok2;
            double x = QInputDialog::getDouble(
                NULL, "Set player position", "Horizontal position in yd", 0,
                -2147483647, 2147483647, 1, &ok1);
            double y = QInputDialog::getDouble(
                NULL, "Set player position", "Vertical position in yd", 0,
                -2147483647, 2147483647, 1, &ok2);
            if (ok1 == true && ok2 == true) {
                this->setPosition(x, y);
            }
        } else {
            assert(clicked == NULL);
        }
    }
}

/**
 * @brief Handles Qt events that occur when
 * players' graphical representations of bodies are dragged and dropped.
 * @param event event datastructure, includes position of context menu click
 */
void PBCPlayerView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItemGroup::mouseReleaseEvent(event);
    QPointF pixelDelta = this->pos();
    QPointF newPixelPos =
        QPointF(_originalPos.get<0>(), _originalPos.get<1>()) + pixelDelta;

    PBCDPoint newPos = PBCPositionTranslator::getInstance()->retranslatePos(
        PBCDPoint(newPixelPos.x(), newPixelPos.y())); // NOLINT
    std::cout << pixelDelta.x() << ", " << pixelDelta.y() << std::endl;
    std::cout << newPos.get<0>() << ", " << newPos.get<1>() << std::endl;
    std::cout << "----------------------------------" << std::endl;
    _playerSP->setPos(newPos);
}
