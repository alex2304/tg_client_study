
#include "stdafx.h"
#include "broadcastwidget.h"

#include "lang.h"
#include "boxes/aboutbox.h"
#include "mainwidget.h"
#include "application.h"
#include "boxes/photocropbox.h"
#include "boxes/connectionbox.h"
#include "boxes/backgroundbox.h"
#include "boxes/addcontactbox.h"
#include "boxes/emojibox.h"
#include "boxes/confirmbox.h"
#include "boxes/downloadpathbox.h"
#include "boxes/usernamebox.h"
#include "boxes/languagebox.h"
#include "boxes/passcodebox.h"
#include "boxes/autolockbox.h"
#include "boxes/sessionsbox.h"
#include "boxes/stickersetbox.h"
#include "langloaderplain.h"
#include "ui/filedialog.h"
#include "apiwrap.h"
#include "autoupdater.h"
#include "localstorage.h"

BroadcastInner::BroadcastInner(BroadcastWidget *parent) : TWidget(parent)
, _self(App::self())
, _parent(parent)
, _field(this, st::taMsgField, lang(lng_message_ph))
, _send(this, lang(lng_send_button), st::btnSend)
, _attachEmoji(this, st::btnAttachEmoji)
, _emojiPan(this)
, _close(this, st::setClose)
{
	if (self()) {
		self()->loadUserpic();
	}

	connect(&_close, SIGNAL(clicked()), App::wnd(), SLOT(showBroadcast()));
	connect(&_send, SIGNAL(clicked()), this, SLOT(onSend()));
	connect(&_field, SIGNAL(submitted(bool)), this, SLOT(onSend(bool)));
	connect(&_field, SIGNAL(resized()), this, SLOT(onFieldResize()));
	connect(&_emojiPan, SIGNAL(emojiSelected(EmojiPtr)), &_field, SLOT(onEmojiInsert(EmojiPtr)));
	
	_attachEmoji.installEventFilter(&_emojiPan);
	
	setMouseTracking(true);
	
	_field.setPlaceholder("Enter message for broadcasting");
	_field.setMaxHeight(st::maxFieldHeight);

	hideAll();
}

bool BroadcastInner::_sendMessageToPeer(int32 peerId, QString messageText, MsgId replyTo) {
	
	PeerData *peer = App::peer(peerId);
	History *history = App::history(peer->id);

	if (!peer || !history) {
		// TODO: notify that it's bad peer id or history
		return false;
	}

	WebPageId webPageId = 0;
	bool broadcast_checked = false;
	bool silent_checked = false;
	App::main()->sendMessage(history, messageText, replyTo, broadcast_checked, silent_checked, webPageId);

	return true;
}

void BroadcastInner::onSend(bool ctrlShiftEnter, MsgId replyTo) {
	
	QList<int32> recieversIds = QList<int32>();
	recieversIds.append(313885077);
	recieversIds.append(67419533);

	for (QList<int32>::iterator peerIdIterator = recieversIds.begin(); peerIdIterator != recieversIds.end(); peerIdIterator++) {
		_sendMessageToPeer(*peerIdIterator, _field.getLastText(), replyTo);
	}

	/*learFieldText();
	_saveDraftText = true;
	_saveDraftStart = getms();
	onDraftSave();

	if (!_attachMention.isHidden()) _attachMention.hideStart();
	if (!_attachType.isHidden()) _attachType.hideStart();
	if (!_emojiPan.isHidden()) _emojiPan.hideStart();

	if (replyTo < 0) cancelReply(lastKeyboardUsed);
	if (_previewData && _previewData->pendingTill) previewCancel();
	_field.setFocus();

	if (!_keyboard.hasMarkup() && _keyboard.forceReply() && !_kbReplyTo) onKbToggle(); */
}


void BroadcastInner::moveFieldControls() {
	
	int w = width(), h = height(), right = w, bottom;
	
	bottom = h - _send.height() - st::sendPadding;
	right -= _send.width() + st::sendPadding;
	_send.move(right, bottom);

	bottom = h - _attachEmoji.height() - st::emojiPadding;
	right -= _attachEmoji.width() + st::emojiPadding;
	_attachEmoji.move(right, bottom);

	if (_field.width() != right - st::sendPadding) {
		_field.resize(right, _field.height());
	}
	bottom = h - _field.height() - st::sendPadding;
	right = 0; // -= _field.width() + st::sendPadding;
	_field.move(right, bottom);
	
	_emojiPan.moveBottom(_attachEmoji.y());

}

void BroadcastInner::onFieldResize() {
	moveFieldControls();
}

void BroadcastInner::paintEvent(QPaintEvent *e) {
	bool animateBackground = false;
	
	Painter p(this);

	p.setClipRect(e->rect());

	// Information message
	p.setFont(st::setHeaderFont->f);
	p.setPen(st::setHeaderColor->p);
	p.drawText(width() / 2 - 200, height() / 2 - st::setHeaderFont->ascent / 2, "Enter your broadcast message and press \"Send\"");

}

void BroadcastInner::resizeEvent(QResizeEvent *e) {
	if (e) {
		resize(e->size());
	}
	moveFieldControls();
	_close.move(st::setClosePos.x(), st::setClosePos.y());
}

void BroadcastInner::keyPressEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_Escape || e->key() == Qt::Key_Back) {
		App::wnd()->showBroadcast();
	}

}

void BroadcastInner::mouseMoveEvent(QMouseEvent *e) {
	if (!self()) {
		setCursor(style::cur_default);
	} else {
		// TODO: set appropriate cursor
		//setCursor((_nameOver || _photoOver) ? style::cur_pointer : style::cur_default);
	}
}

void BroadcastInner::mousePressEvent(QMouseEvent *e) {
	mouseMoveEvent(e);
	if (!self()) {
		return;
	}
	// TODO: process mouse click
	/*if (QRect(_uploadPhoto.x() + st::setNameLeft, st::setTop + st::setNameTop, qMin(_uploadPhoto.width() - int(st::setNameLeft), _nameText.maxWidth()), st::setNameFont->height).contains(e->pos())) {
		Ui::showLayer(new EditNameTitleBox(self()));
	} else if (QRect(_left, st::setTop, st::setPhotoSize, st::setPhotoSize).contains(e->pos())) {
		if (_photoLink) {
			App::photo(self()->photoId)->download();
			_photoLink->onClick(e->button());
		} else {
			onUpdatePhoto();
		}
	}*/
}

void BroadcastInner::hideAll() {
	_field.hide();
	_send.hide();
	_attachEmoji.hide();
	_emojiPan.hide();
	_close.hide();
}

void BroadcastInner::showAll() {
	_close.show();
	_field.show();
	_send.show();
	_attachEmoji.show();
}

void BroadcastInner::contextMenuEvent(QContextMenuEvent *e) {
}


void BroadcastInner::updateSize(int32 newWidth, int32 newHeight) {
	// TODO: fix relative size
	resize(newWidth, newHeight);
}


BroadcastWidget::BroadcastWidget(MainWindow *parent) : TWidget(parent)
, _inner(this)
, _a_show(animation(this, &BroadcastWidget::step_show))
, _dialogs(this)
{
	
	connect(App::wnd(), SIGNAL(resized(const QSize&)), this, SLOT(onParentResize(const QSize&)));

	setGeometry(QRect(0, st::titleHeight, App::wnd()->width(), App::wnd()->height() - st::titleHeight));

	// It should be called in order to resize inner widget
	_inner.resizeEvent(0);
	
	showAll();
}

void BroadcastWidget::onParentResize(const QSize &newSize) {
	resize(newSize);
}

void BroadcastWidget::animShow(const QPixmap &bgAnimCache, bool back) {
	if (App::app()) App::app()->mtpPause();

	(back ? _cacheOver : _cacheUnder) = bgAnimCache;

	_a_show.stop();

	showAll();
	(back ? _cacheUnder : _cacheOver) = myGrab(this);
	hideAll();

	a_coordUnder = back ? anim::ivalue(-qFloor(st::slideShift * width()), 0) : anim::ivalue(0, -qFloor(st::slideShift * width()));
	a_coordOver = back ? anim::ivalue(0, width()) : anim::ivalue(width(), 0);
	a_shadow = back ? anim::fvalue(1, 0) : anim::fvalue(0, 1);
	_a_show.start();

	show();
}

void BroadcastWidget::step_show(float64 ms, bool timer) {
	float64 dt = ms / st::slideDuration;
	if (dt >= 1) {
		_a_show.stop();

		a_coordUnder.finish();
		a_coordOver.finish();
		a_shadow.finish();

		_cacheUnder = _cacheOver = QPixmap();

		showAll();
		_inner.setFocus();

		if (App::app()) App::app()->mtpUnpause();
	} else {
		a_coordUnder.update(dt, st::slideFunction);
		a_coordOver.update(dt, st::slideFunction);
		a_shadow.update(dt, st::slideFunction);
	}
	if (timer) update();
}

void BroadcastWidget::stop_show() {
	_a_show.stop();
}

void BroadcastWidget::paintEvent(QPaintEvent *e) {
	QRect r(e->rect());
	bool trivial = (rect() == r);

	Painter p(this);
	if (!trivial) {
		p.setClipRect(r);
	}
	if (_a_show.animating()) {
		if (a_coordOver.current() > 0) {
			p.drawPixmap(QRect(0, 0, a_coordOver.current(), height()), _cacheUnder, QRect(-a_coordUnder.current() * cRetinaFactor(), 0, a_coordOver.current() * cRetinaFactor(), height() * cRetinaFactor()));
			p.setOpacity(a_shadow.current() * st::slideFadeOut);
			p.fillRect(0, 0, a_coordOver.current(), height(), st::black->b);
			p.setOpacity(1);
		}
		p.drawPixmap(a_coordOver.current(), 0, _cacheOver);
		p.setOpacity(a_shadow.current());
		p.drawPixmap(QRect(a_coordOver.current() - st::slideShadow.pxWidth(), 0, st::slideShadow.pxWidth(), height()), App::sprite(), st::slideShadow.rect());
	} else {
		p.fillRect(rect(), st::setBG->b);
	}
}

void BroadcastWidget::showAll() {
	_inner.show();
	_inner.showAll();
	_dialogs.show();
}

void BroadcastWidget::hideAll() {
	_inner.hideAll();
	_inner.hide();
	_dialogs.hide();
}

void BroadcastWidget::resizeEvent(QResizeEvent *e) {
	if (!e) return;

	int32 dialogsWidth = chatsListWidth(e->size().width());
	_dialogs.resize(dialogsWidth, e->size().height());
	_dialogs.move(0, 0);

	_inner.setGeometry(QRect(dialogsWidth, 0, width() - dialogsWidth, height()));
	_inner.resize(QSize(width() - dialogsWidth, height()));

}

void BroadcastWidget::dragEnterEvent(QDragEnterEvent *e) {

}

void BroadcastWidget::dropEvent(QDropEvent *e) {
}

void BroadcastWidget::setInnerFocus() {
	_inner.setFocus();
}

BroadcastWidget::~BroadcastWidget() {
	if (App::wnd()) App::wnd()->noBroadcast(this);
}


BroadcastMessageField::BroadcastMessageField(BroadcastInner *parent, const style::flatTextarea &st, const QString &ph, const QString &val) : FlatTextarea(parent, st, ph, val) {
	setMinHeight(st::btnSend.height - 2 * st::sendPadding);
	setMaxHeight(st::maxFieldHeight);
}

bool BroadcastMessageField::hasSendText() const {
	const QString &text(getLastText());
	for (const QChar *ch = text.constData(), *e = ch + text.size(); ch != e; ++ch) {
		ushort code = ch->unicode();
		if (code != ' ' && code != '\n' && code != '\r' && !chReplacedBySpace(code)) {
			return true;
		}
	}
	return false;
}

void BroadcastMessageField::onEmojiInsert(EmojiPtr emoji) {
	if (isHidden()) return;
	insertEmoji(emoji, textCursor());
}

void BroadcastMessageField::dropEvent(QDropEvent *e) {
	FlatTextarea::dropEvent(e);
	if (e->isAccepted()) {
		App::wnd()->activateWindow();
	}
}

bool BroadcastMessageField::canInsertFromMimeData(const QMimeData *source) const {
	if (source->hasUrls()) {
		int32 files = 0;
		for (int32 i = 0; i < source->urls().size(); ++i) {
			if (source->urls().at(i).isLocalFile()) {
				++files;
			}
		}
		if (files > 1) return false; // multiple confirm with "compressed" checkbox
	}
	if (source->hasImage()) return true;
	return FlatTextarea::canInsertFromMimeData(source);
}

void BroadcastMessageField::insertFromMimeData(const QMimeData *source) {
	// TODO: fix 'uploadFile(..)' methods
	if (source->hasUrls()) {
		int32 files = 0;
		QUrl url;
		for (int32 i = 0; i < source->urls().size(); ++i) {
			if (source->urls().at(i).isLocalFile()) {
				url = source->urls().at(i);
				++files;
			}
		}
		if (files == 1) {
			//history->uploadFile(url.toLocalFile(), PrepareAuto, FileLoadAlwaysConfirm);
			return;
		}
		if (files > 1) return;
		//if (files > 1) return uploadFiles(files, PrepareAuto); // multiple confirm with "compressed" checkbox
	}
	if (source->hasImage()) {
		QImage img = qvariant_cast<QImage>(source->imageData());
		if (!img.isNull()) {
			//history->uploadImage(img, PrepareAuto, FileLoadAlwaysConfirm, source->text());
			return;
		}
	}
	FlatTextarea::insertFromMimeData(source);
}

void BroadcastMessageField::focusInEvent(QFocusEvent *e) {
	FlatTextarea::focusInEvent(e);
	emit focused();
}
