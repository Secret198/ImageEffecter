#include "computeitem.h"


QImage ComputeItem::img() const
{
    return m_img;
}

QString ComputeItem::source() const
{
    return m_img_source;
}

void ComputeItem::setSource(const QString &s){
    if (m_img_source == s) return;
    m_img_source = s;
    m_img = QImage(s).convertToFormat(QImage::Format_RGBA8888);
    m_dirty = true;
    emit sourceChanged();
    update();
}