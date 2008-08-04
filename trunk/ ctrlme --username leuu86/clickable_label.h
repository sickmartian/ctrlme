#ifndef SORT_LABEL
	#define SORT_LABEL

#include <QLabel>
#include <QMouseEvent>

class SortLabel : public QLabel
{
	Q_OBJECT

public:
	SortLabel(QWidget* = 0);

signals:
	void clicked();

private:
	void mousePressEvent(QMouseEvent*);

};

#endif
