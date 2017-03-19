#pragma once

#include "ui/flatbutton.h"
#include "ui/flatcheckbox.h"
#include "sysbuttons.h"

#include <QtWidgets/QWidget>

class MainWindow;
class BroadcastWidget;
class BroadcastInner;

class BroadcastMessageField : public FlatTextarea {
	Q_OBJECT

public:
	BroadcastMessageField(BroadcastInner *parent, const style::flatTextarea &st, const QString &ph = QString(), const QString &val = QString());
	void dropEvent(QDropEvent *e);
	bool canInsertFromMimeData(const QMimeData *source) const;
	void insertFromMimeData(const QMimeData *source);

	void focusInEvent(QFocusEvent *e);

	bool hasSendText() const;

	public slots:
		void onEmojiInsert(EmojiPtr emoji);

signals:
	void focused();

private:
	//HistoryWidget *history;

}; 

class BroadcastInner : public TWidget {
	Q_OBJECT

public:

	BroadcastInner(BroadcastWidget *parent);

	bool _sendMessageToPeer(int32 peerId, QString messageText, MsgId replyTo);

	void moveControls();

	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void contextMenuEvent(QContextMenuEvent *e);

	void updateAdaptiveLayout();

	void showAll();

	void updateSize(int32 newWidth);

public slots:
	void onSend(bool ctrlShiftEnter = false, MsgId replyTo = -1);

private:

	//void setScale(DBIScale newScale);

	UserData *_self;
	UserData *self() const {
		return App::self() ? _self : static_cast<UserData*>(0);
	}
	int32 _left;
	QPixmap _background;	// chat background
	
	FlatButton _send;
	BroadcastMessageField _field;

	QList<int32> recievers_ids;
};

class BroadcastWidget : public TWidget {
	Q_OBJECT

public:

	BroadcastWidget(MainWindow *parent);

	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void dragEnterEvent(QDragEnterEvent *e);
	void dropEvent(QDropEvent *e);

	void updateAdaptiveLayout();

	void animShow(const QPixmap &bgAnimCache, bool back = false);
	void step_show(float64 ms, bool timer);
	void stop_show();

	void setInnerFocus();
	
	~BroadcastWidget();

public slots:

	void onParentResize(const QSize &newSize);

private:

	void showAll();
	void hideAll();

	Animation _a_show;
	QPixmap _cacheUnder, _cacheOver;
	anim::ivalue a_coordUnder, a_coordOver;
	anim::fvalue a_shadow;

	//ScrollArea _scroll;
	BroadcastInner _inner;
	IconedButton _close;
};