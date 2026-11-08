#ifndef COMPUTEITEM_H
#define COMPUTEITEM_H

#include <QObject>
#include <QQmlEngine>
#include <QQuickRhiItem>
#include <rhi/qrhi.h>
#include <qimage.h>

class ComputeItem : public QQuickRhiItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    QQuickRhiItemRenderer *createRenderer() override;
    void setSource(const QString &s);

    QImage img() const;
    bool m_dirty = false;


    QString source() const;

signals:
    void sourceChanged();

private:
    QImage m_img;
    QString m_img_source;
};

#endif // COMPUTEITEM_H
