#pragma once

#include "ui/flatbutton.h"
#include "ui/flatcheckbox.h"
#include "sysbuttons.h"

#include <QtWidgets/QWidget>

class MainWindow;
class Settings;

class BroadcastInner : public TWidget {
	Q_OBJECT

public:

	BroadcastInner(SettingsWidget *parent);

	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void contextMenuEvent(QContextMenuEvent *e);

	void updateAdaptiveLayout();

	void step_photo(float64 ms, bool timer);

	void updateSize(int32 newWidth);

	void updateOnlineDisplay();

	void showAll();

    void chooseCustomLang();

	void updateChatBackground();
	void needBackgroundUpdate(bool tile);
	void enableDisplayNotify(bool enable);

public slots:

	void usernameChanged();
	void updateConnectionType();

	void passcodeChanged();

	void updateBackgroundRect();

	void peerUpdated(PeerData *data);

	void onUpdatePhoto();
	void onUpdatePhotoCancel();

#ifndef TDESKTOP_DISABLE_AUTOUPDATE
	void onAutoUpdate();
	void onCheckNow();
#endif
	void onRestartNow();

	void onFullPeerUpdated(PeerData *peer);

	void onPasscode();
	void onPasscodeOff();
	void onAutoLock();
	void onPassword();
	void onPasswordOff();
	void onReloadPassword(Qt::ApplicationState state = Qt::ApplicationActive);

#ifndef TDESKTOP_DISABLE_NETWORK_PROXY
	void onConnectionType();
#endif

	void onUsername();

	void onWorkmodeTray();
	void onWorkmodeWindow();

	void onAutoStart();
	void onStartMinimized();
	void onSendToMenu();

	void onScaleAuto();
	void onScaleChange();

	void onSoundNotify();
	void onIncludeMuted();
	void onDesktopNotify();
	void onSenderName();
	void onMessagePreview();

	void onWindowsNotifications();

	void onReplaceEmojis();
	void onViewEmojis();
	void onStickers();

	void onEnterSend();
	void onCtrlEnterSend();

	void onDontAskDownloadPath();
	void onDownloadPathEdit();
	void onDownloadPathEdited();
	void onDownloadPathClear();
	void onDownloadPathClearSure();
	void onTempDirCleared(int task);
	void onTempDirClearFailed(int task);

	void onAutoDownload();

	void onBackFromGallery();
	void onBackFromFile();
	void onTileBackground();
	void onAdaptiveForWide();

	void onLocalStorageClear();

#ifndef TDESKTOP_DISABLE_AUTOUPDATE
	void onUpdateChecking();
	void onUpdateLatest();
	void onUpdateDownloading(qint64 ready, qint64 total);
	void onUpdateReady();
	void onUpdateFailed();
#endif

	void onShowSessions();

	void onPhotoUpdateDone(PeerId peer);
	void onPhotoUpdateFail(PeerId peer);
	void onPhotoUpdateStart();

	void onChangeLanguage();
	void onSaveTestLang();

	void onUpdateLocalStorage();

	void onAskQuestion();
	void onAskQuestionSure();
	void onTelegramFAQ();

private:

	void saveError(const QString &str = QString());

	void supportGot(const MTPhelp_Support &support);

	void setScale(DBIScale newScale);

	QString _testlang, _secretText;

	UserData *_self;
	UserData *self() const {
		return App::self() ? _self : static_cast<UserData*>(0);
	}
	int32 _left;

	// profile
	Text _nameText;
	QString _nameCache;
	ClickHandlerPtr _photoLink;
	FlatButton _uploadPhoto;
	LinkButton _cancelPhoto;
	bool _nameOver, _photoOver;
	anim::fvalue a_photoOver;
	Animation _a_photo;

	QString _errorText;

	// contact info
	QString _phoneText;
	LinkButton _chooseUsername;

	// notifications
	FlatCheckbox _desktopNotify, _senderName, _messagePreview, _windowsNotifications, _soundNotify, _includeMuted;

	// general
	LinkButton _changeLanguage;
	#ifndef TDESKTOP_DISABLE_AUTOUPDATE
	FlatCheckbox _autoUpdate;
	LinkButton _checkNow, _restartNow;
	#endif
    bool _supportTray; // cSupportTray() value on settings create
	FlatCheckbox _workmodeTray, _workmodeWindow;
	FlatCheckbox _autoStart, _startMinimized, _sendToMenu;
	FlatCheckbox _dpiAutoScale;
	int32 _dpiWidth1, _dpiWidth2, _dpiWidth3, _dpiWidth4;

	QString _curVersionText, _newVersionText;
	int32 _curVersionWidth, _newVersionWidth;

#ifndef TDESKTOP_DISABLE_AUTOUPDATE
	enum UpdatingState {
		UpdatingNone,
		UpdatingCheck,
		UpdatingLatest,
		UpdatingDownload,
		UpdatingFail,
		UpdatingReady
	};
	UpdatingState _updatingState;
	QString _newVersionDownload;
#endif

	// chat options
	FlatCheckbox _replaceEmojis;
	LinkButton _viewEmojis, _stickers;
	FlatRadiobutton _enterSend, _ctrlEnterSend;
	FlatCheckbox _dontAskDownloadPath;
	int32 _downloadPathWidth;
	LinkButton _downloadPathEdit, _downloadPathClear;
	int32 _tempDirClearingWidth, _tempDirClearedWidth, _tempDirClearFailedWidth;
	enum TempDirClearState {
		TempDirClearFailed = 0,
		TempDirEmpty       = 1,
		TempDirExists      = 2,
		TempDirClearing    = 3,
		TempDirCleared     = 4,
	};
	TempDirClearState _tempDirClearState;
	LinkButton _autoDownload;

	// local storage
	LinkButton _localStorageClear;
	int32 _localStorageHeight;
	int32 _storageClearingWidth, _storageClearedWidth, _storageClearFailedWidth;
	TempDirClearState _storageClearState;

	// chat background
	QPixmap _background;
	LinkButton _backFromGallery, _backFromFile;
	FlatCheckbox _tileBackground, _adaptiveForWide;
	bool _needBackgroundUpdate;

	// advanced
	LinkButton _passcodeEdit, _passcodeTurnOff, _autoLock;
	QString _autoLockText;
	int32 _autoLockWidth;
	LinkButton _passwordEdit, _passwordTurnOff;
	QString _waitingConfirm;
	QByteArray _curPasswordSalt;
	bool _hasPasswordRecovery;
	QString _curPasswordHint;
	QByteArray _newPasswordSalt;
	LinkButton _connectionType;
	QString _connectionTypeText;
	int32 _connectionTypeWidth;
	LinkButton _showSessions, _askQuestion, _telegramFAQ, _logOut;

	mtpRequestId _supportGetRequest;

	void gotPassword(const MTPaccount_Password &result);
	void offPasswordDone(const MTPBool &result);
	bool offPasswordFail(const RPCError &error);

#ifndef TDESKTOP_DISABLE_AUTOUPDATE
	void setUpdatingState(UpdatingState state, bool force = false);
	void setDownloadProgress(qint64 ready, qint64 total);
#endif
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

	void updateOnlineDisplay();
	void updateConnectionType();

	void updateDisplayNotify();

	void rpcClear();
	void usernameChanged();

	void setInnerFocus();
	void needBackgroundUpdate(bool tile);

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

	ScrollArea _scroll;
	BroadcastInner _inner;
	IconedButton _close;
};
