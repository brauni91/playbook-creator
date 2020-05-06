/** @file pbcConfig.h
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

#ifndef PBCCONFIG_H
#define PBCCONFIG_H

#include "pbcSingleton.h"
#include "pbcDeclarations.h"
#include "models/pbcColor.h"
#include <string>

#define PASSWORD_MAX_RETRYS 5

class PBCConfig : public PBCSingleton<PBCConfig> {
    friend class PBCSingleton<PBCConfig>;

private:
    //bool _initialized;
    unsigned int _minWidth;
    unsigned int _minHeight;
    double _fieldWidth;
    PBCColor _losColor;
    double _losWidthYd;
    double _losYFactor;
    PBCColor _fiveYdColor;
    double _fiveYdWidthYd;
    double _fiveYdYFactor;
    double _tenYdYFactor;
    PBCColor _ballColor;
    double _ballWidthYd;
    PBCColor _playNameColor;
    double _playNameSizeYd;
    std::string _playNameFont;
    double _playerWidthYd;
    double _routeWidthYd;
    bool _playerShadow;
    double _playerShadowRadius;
    double _playerShadowOffsetFactor;


    unsigned int _canvasWidth;
    unsigned int _canvasHeight;


protected:
    PBCConfig() :
            //_initialized(false),
            _minWidth(20),
            _minHeight(20),
            _fieldWidth(25), // yard
            _losColor(128U, 128U, 128U),
            _losWidthYd(0.3),
            _losYFactor(0.6),
            _fiveYdColor(128U, 128U, 128U),
            _fiveYdWidthYd(0.2),
            _fiveYdYFactor(0.4),
            _tenYdYFactor(0.2),
            _ballColor(139U, 69U, 19U),
            _ballWidthYd(1),
            _playNameColor(128U, 128U, 128U),
            _playNameSizeYd(2.5),
            _playNameFont("Helvetica"),
            _playerWidthYd(1),
            _playerShadow(true),
            _playerShadowRadius(20),
            _playerShadowOffsetFactor(0.25),
            _routeWidthYd(0.4),
            _canvasWidth(200),
            _canvasHeight(200) {
        // TODO(obr): initialize from File
    }

public:
    void setCanvasSize(unsigned int canvasWidth, unsigned int canvasHeight) {
        //_initialized = true;
        _canvasHeight = canvasHeight;
        _canvasWidth = _fieldWidth * ydInPixel();
        //_canvasWidth = canvasWidth;
    }

    unsigned int canvasWidth() {
        //pbcAssert(_initialized == true);
        return _canvasWidth;
    }

    unsigned int canvasHeight() {
        //pbcAssert(_initialized == true);
        return _canvasHeight;
    }

    PBCColor losColor() {
        return _losColor;
    }

    double losWidth() {
        return _losWidthYd * ydInPixel();
    }

    unsigned int minWidth() {
        return _minWidth;
    }

    unsigned int minHeight() {
        return _minHeight;
    }

    unsigned int losY() {
        return _losYFactor * _canvasHeight;
    }

    PBCColor fiveYdColor() {
        return _fiveYdColor;
    }

    double fiveYdWidth() {
        return _fiveYdWidthYd * ydInPixel();
    }

    unsigned int fiveYdY() {
        return _fiveYdYFactor * _canvasHeight;
    }

    unsigned int tenYdY() {
        return _tenYdYFactor * _canvasHeight;
    }


    PBCColor ballColor() {
        return _ballColor;
    }

    unsigned int ballWidth() {
        return _ballWidthYd * ydInPixel();
    }

    PBCColor playNameColor() {
        return _playNameColor;
    }

    unsigned int playNameSize() {
        return _playNameSizeYd * ydInPixel();
    }

    std::string playNameFont() {
        return _playNameFont;
    }

    double ydInPixel() {
        return (_losYFactor - _fiveYdYFactor) * _canvasHeight / 5.0;
    }

    unsigned int playerWidth() {
        return _playerWidthYd * ydInPixel();
    }

    unsigned int routeWidth() {
        return _routeWidthYd * ydInPixel();
    }

    bool playerShadow() {
        return _playerShadow;
    }

    double playerShadowRadius() {
        return _playerShadowRadius;
    }

    double playerShadowOffset() {
        return _playerShadowOffsetFactor * playerWidth();
    }
};


#endif  // PBCCONFIG_H

