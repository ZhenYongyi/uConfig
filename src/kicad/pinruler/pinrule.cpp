/**
 ** This file is part of the uConfig project.
 ** Copyright 2018 Robotips sebastien.caux@robotips.fr.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include "pinrule.h"

#include <QDebug>

QStringList PinRule::propsName = QStringList()
        <<"class"
        <<"elecType";

QStringList PinRule::elecTypeEnumStr = QStringList()
        <<"in"
        <<"out"
        <<"bidir"
        <<"tri"
        <<"passive"
        <<"unspecified"
        <<"pin"
        <<"pout"
        <<"opcol"
        <<"opem"
        <<"nc";

PinRule::PinRule(const QString &selector)
    : Rule(selector)
{
    _classSet = false;

    _elecType = Pin::Input;
    _elecTypeSet = false;
}

Rule::Type PinRule::type() const
{
    return Rule::PinRuleType;
}

bool PinRule::setProperty(const QString &name, const QString &value)
{
    int id = PinRule::propsName.indexOf(name);

    if (id == -1)
        return false;

    switch (id)
    {
    case 0: // class
        setClassName(value);
        break;
    case 1: // elec_type
        setElecType(value);
        break;
    }

    return true;
}

bool PinRule::hasPropertySet(const QString &name) const
{
    int id = PinRule::propsName.indexOf(name);
    switch (id)
    {
    case 0: // class
        return hasClassSet();
    case 1: // length
        return hasElecType();
    }
    return false;
}

QString PinRule::className(const QString &pinName) const
{
    if (pinName.isEmpty())
        return _className;
    if (!_selector.isValid())
        return _className;
    if (!_className.contains("\\"))
        return _className;

    QRegularExpressionMatch match = _selector.match(pinName, 0, QRegularExpression::NormalMatch);
    if (match.hasMatch())
    {
        QString newClassName;
        const QStringList &captures = (match.capturedTexts());
        newClassName = _className;
        if (captures.count()>1)
            newClassName.replace("\\1", captures[1]);
        if (captures.count()>2)
            newClassName.replace("\\2", captures[2]);
        if (captures.count()>3)
            newClassName.replace("\\3", captures[3]);
        return newClassName;
    }
    return _className;
}

void PinRule::setClassName(const QString &className)
{
    _className = className;
    _classSet = true;
}

bool PinRule::hasClassSet() const
{
    return _classSet;
}

Pin::ElectricalType PinRule::elecType() const
{
    return _elecType;
}

void PinRule::setElecType(Pin::ElectricalType elecType)
{
    _elecType = elecType;
    _elecTypeSet = true;
}

void PinRule::setElecType(const QString &className)
{
    int id = PinRule::elecTypeEnumStr.indexOf(className);
    if (id != -1)
        setElecType((Pin::ElectricalType)(id));
}

bool PinRule::hasElecType() const
{
    return _elecTypeSet;
}
