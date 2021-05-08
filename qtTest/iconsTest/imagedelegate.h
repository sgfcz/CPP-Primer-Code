#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H

#include <QItemDelegate>

class ImageDelegate : public QItemDelegate
{
    Q_OBJECT
    
public:
    explicit ImageDelegate(QObject *parent = nullptr);
};

#endif
