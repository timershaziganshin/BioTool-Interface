#include "SyncLineEdit.h"

SyncLineEdit::SyncLineEdit() : QLineEdit() {
}

bool SyncLineEdit::event(QEvent* e) {
	if(e->type() == QEvent::KeyRelease) {
		emit textEdited(text());	
	}
	return QLineEdit::event(e);
}