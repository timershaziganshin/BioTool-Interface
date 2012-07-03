#include <QtGui>

class SyncLineEdit : public QLineEdit {
	Q_OBJECT
public:
	SyncLineEdit();
	bool event(QEvent* e);
signals:
	void textEdited(QString&);


};