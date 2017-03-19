
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
{
	if (self()) {
		self()->loadUserpic();

	}


	setMouseTracking(true);
}

void BroadcastInner::paintEvent(QPaintEvent *e) {
	bool animateBackground = false;
	
	Painter p(this);

	p.setClipRect(e->rect());

	int32 top = 0;
	if (self()) {
		
		top += st::setTop;

		top += st::setPhotoSize;

		// contact info
		p.setFont(st::setHeaderFont->f);
		p.setPen(st::setHeaderColor->p);
		p.drawText(_left + st::setHeaderLeft, top + st::setHeaderTop + st::setHeaderFont->ascent, lang(lng_settings_section_contact_info));
		top += st::setHeaderSkip;

	}

	// general
	p.setFont(st::setHeaderFont->f);
	p.setPen(st::setHeaderColor->p);
	p.drawText(_left + st::setHeaderLeft, top + st::setHeaderTop + st::setHeaderFont->ascent, lang(lng_settings_section_general));
	top += st::setHeaderSkip;

	if (self()) {
		
		if (animateBackground) {
			const QPixmap &pix = App::main()->newBackgroundThumb()->pixBlurred(st::setBackgroundSize);

			p.drawPixmap(_left, top, st::setBackgroundSize, st::setBackgroundSize, pix, 0, (pix.height() - st::setBackgroundSize) / 2, st::setBackgroundSize, st::setBackgroundSize);

			uint64 dt = getms();
			int32 cnt = int32(st::photoLoaderCnt), period = int32(st::photoLoaderPeriod), t = dt % period, delta = int32(st::photoLoaderDelta);

			int32 x = _left + (st::setBackgroundSize - st::mediaviewLoader.width()) / 2;
			int32 y = top + (st::setBackgroundSize - st::mediaviewLoader.height()) / 2;
			p.fillRect(x, y, st::mediaviewLoader.width(), st::mediaviewLoader.height(), st::photoLoaderBg->b);

			x += (st::mediaviewLoader.width() - cnt * st::mediaviewLoaderPoint.width() - (cnt - 1) * st::mediaviewLoaderSkip) / 2;
			y += (st::mediaviewLoader.height() - st::mediaviewLoaderPoint.height()) / 2;
			QColor c(st::white->c);
			QBrush b(c);
			for (int32 i = 0; i < cnt; ++i) {
				t -= delta;
				while (t < 0) t += period;

				float64 alpha = (t >= st::photoLoaderDuration1 + st::photoLoaderDuration2) ? 0 : ((t > st::photoLoaderDuration1 ? ((st::photoLoaderDuration1 + st::photoLoaderDuration2 - t) / st::photoLoaderDuration2) : (t / st::photoLoaderDuration1)));
				c.setAlphaF(st::photoLoaderAlphaMin + alpha * (1 - st::photoLoaderAlphaMin));
				b.setColor(c);
				p.fillRect(x + i * (st::mediaviewLoaderPoint.width() + st::mediaviewLoaderSkip), y, st::mediaviewLoaderPoint.width(), st::mediaviewLoaderPoint.height(), b);
			}
			QTimer::singleShot(AnimationTimerDelta, this, SLOT(updateBackgroundRect()));
		} else {
			p.drawPixmap(_left, top, _background);
		}

		top += st::setBackgroundSize;
		top += st::setLittleSkip;
		
	}

}

void BroadcastInner::resizeEvent(QResizeEvent *e) {
	_left = (width() - st::setWidth) / 2;

	int32 top = 0;

	if (self()) {
		// profile
		top += st::setTop;
		top += st::setPhotoSize;
		
		// contact info
		top += st::setHeaderSkip;
		top += st::linkFont->height + st::setLittleSkip;
		
		// notifications
		top += st::setHeaderSkip;
		
	}

	// general
	top += st::setHeaderSkip;
	
	top += st::setVersionHeight;

	// chat options
	if (self()) {
		top += st::setHeaderSkip;
		
		top += st::setLittleSkip;
		
		// chat background
		top += st::setHeaderSkip;

		top += st::setBackgroundSize;

		top += st::setLittleSkip;
		
	}

	// advanced
	top += st::setHeaderSkip;
	
	
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


void BroadcastInner::showAll() {
	/*// profile
	if (self()) {
		if (App::app()->isPhotoUpdating(self()->id)) {
			_cancelPhoto.show();
			_uploadPhoto.hide();
		}
		else {
			_cancelPhoto.hide();
			_uploadPhoto.show();
		}
	}
	else {
		_uploadPhoto.hide();
		_cancelPhoto.hide();
	}

	// contact info
	if (self()) {
		_chooseUsername.show();
	}
	else {
		_chooseUsername.hide();
	}

	// notifications
	if (self()) {
		_desktopNotify.show();
		_senderName.show();
		_messagePreview.show();
		if (App::wnd()->psHasNativeNotifications() && cPlatform() == dbipWindows) {
			_windowsNotifications.show();
		}
		else {
			_windowsNotifications.hide();
		}
		_soundNotify.show();
		_includeMuted.show();
	}
	else {
		_desktopNotify.hide();
		_senderName.hide();
		_messagePreview.hide();
		_windowsNotifications.hide();
		_soundNotify.hide();
		_includeMuted.hide();
	}

	// general
	_changeLanguage.show();
#ifndef TDESKTOP_DISABLE_AUTOUPDATE
	_autoUpdate.show();
	setUpdatingState(_updatingState, true);
#endif
	if (cPlatform() == dbipWindows) {
		_workmodeTray.show();
		_workmodeWindow.show();

		_autoStart.show();
		_startMinimized.show();

		_sendToMenu.show();
	}
	else {
		if (_supportTray) {
			_workmodeTray.show();
		}
		else {
			_workmodeTray.hide();
		}
		_workmodeWindow.hide();

		_autoStart.hide();
		_startMinimized.hide();

		_sendToMenu.hide();
	}
	if (cRetina()) {
		_dpiSlider.hide();
		_dpiAutoScale.hide();
	}
	else {
		_dpiSlider.show();
		_dpiAutoScale.show();
	}

	// chat options
	if (self()) {
		_replaceEmojis.show();
		if (cReplaceEmojis()) {
			_viewEmojis.show();
		}
		else {
			_viewEmojis.hide();
		}
		_stickers.show();
		_enterSend.show();
		_ctrlEnterSend.show();
		_dontAskDownloadPath.show();
		if (cAskDownloadPath()) {
			_downloadPathEdit.hide();
			_downloadPathClear.hide();
		}
		else {
			_downloadPathEdit.show();
			if (cDownloadPath() == qsl("tmp") && _tempDirClearState == TempDirExists) { // dir exists, not clearing right now
				_downloadPathClear.show();
			}
			else {
				_downloadPathClear.hide();
			}
		}
		_autoDownload.show();
	}
	else {
		_replaceEmojis.hide();
		_viewEmojis.hide();
		_stickers.hide();
		_enterSend.hide();
		_ctrlEnterSend.hide();
		_dontAskDownloadPath.hide();
		_downloadPathEdit.hide();
		_downloadPathClear.hide();
		_autoDownload.hide();
	}

	// local storage
	if (self() && _storageClearState == TempDirExists) {
		_localStorageClear.show();
	}
	else {
		_localStorageClear.hide();
	}

	// chat background
	if (self()) {
		_backFromGallery.show();
		_backFromFile.show();
		_tileBackground.show();
		if (Global::AdaptiveLayout() == Adaptive::WideLayout) {
			_adaptiveForWide.show();
		}
		else {
			_adaptiveForWide.hide();
		}
	}
	else {
		_backFromGallery.hide();
		_backFromFile.hide();
		_tileBackground.hide();
		_adaptiveForWide.hide();
	}

	// advanced
	if (self()) {
		_passcodeEdit.show();
		if (cHasPasscode()) {
			_autoLock.show();
			_passcodeTurnOff.show();
		}
		else {
			_autoLock.hide();
			_passcodeTurnOff.hide();
		}
		if (_waitingConfirm.isEmpty()) {
			_passwordEdit.show();
		}
		else {
			_passwordEdit.hide();
		}
		if (_curPasswordSalt.isEmpty() && _waitingConfirm.isEmpty()) {
			_passwordTurnOff.hide();
		}
		else {
			_passwordTurnOff.show();
		}
		_showSessions.show();
		_askQuestion.show();
		_logOut.show();
	}
	else {
		_passcodeEdit.hide();
		_autoLock.hide();
		_passcodeTurnOff.hide();
		_passwordEdit.hide();
		_passwordTurnOff.hide();
		_showSessions.hide();
		_askQuestion.hide();
		_logOut.hide();
	}
	_telegramFAQ.show(); */
}

void BroadcastInner::contextMenuEvent(QContextMenuEvent *e) {
}

void BroadcastInner::updateAdaptiveLayout() {
	showAll();
	resizeEvent(0);
}


void BroadcastInner::updateSize(int32 newWidth) {
	// TODO: fix relative size
	//resize(newWidth, _logOut.geometry().bottom() + st::setBottom);
}


BroadcastWidget::BroadcastWidget(MainWindow *parent) : TWidget(parent)
, _a_show(animation(this, &BroadcastWidget::step_show))
, _scroll(this, st::setScroll)
, _inner(this)
, _close(this, st::setClose) {
	_scroll.setWidget(&_inner);

	connect(App::wnd(), SIGNAL(resized(const QSize&)), this, SLOT(onParentResize(const QSize&)));
	connect(&_close, SIGNAL(clicked()), App::wnd(), SLOT(showBroadcast()));

	setGeometry(QRect(0, st::titleHeight, App::wnd()->width(), App::wnd()->height() - st::titleHeight));

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
	_scroll.show();
	_inner.show();
	_inner.showAll();
	if (Adaptive::OneColumn()) {
		_close.hide();
	} else {
		_close.show();
	}
}

void BroadcastWidget::hideAll() {
	_scroll.hide();
	_close.hide();
}

void BroadcastWidget::resizeEvent(QResizeEvent *e) {
	_scroll.resize(size());
	_inner.updateSize(width());
	_close.move(st::setClosePos.x(), st::setClosePos.y());
}

void BroadcastWidget::dragEnterEvent(QDragEnterEvent *e) {

}

void BroadcastWidget::dropEvent(QDropEvent *e) {
}

void BroadcastWidget::updateAdaptiveLayout() {
	if (Adaptive::OneColumn()) {
		_close.hide();
	} else {
		_close.show();
	}
	_inner.updateAdaptiveLayout();
	resizeEvent(0);
}

void BroadcastWidget::setInnerFocus() {
	_inner.setFocus();
}

BroadcastWidget::~BroadcastWidget() {
	if (App::wnd()) App::wnd()->noBroadcast(this);
}
