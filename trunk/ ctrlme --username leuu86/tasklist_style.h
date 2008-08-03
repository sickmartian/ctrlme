#include <QStyle>
#include <QPlastiqueStyle>

class TaskListStyle : public QPlastiqueStyle
{
  Q_OBJECT
  
public:
  TaskListStyle(QWidget *parent);
private:
  void drawPrimitive(QStyle::PrimitiveElement element,
                                        const QStyleOption *option,
                                        QPainter *painter,
                                        const QWidget *widget) const;
};
