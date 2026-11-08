#ifndef EFFECTCONTAINER_H
#define EFFECTCONTAINER_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include "effect.h"

class EffectContainer : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    EffectContainer();

private:
    QList<Effect> m_effect_list;
};

#endif // EFFECTCONTAINER_H
