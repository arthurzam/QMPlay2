/*
    QMPlay2 is a video and audio player.
    Copyright (C) 2010-2024  Błażej Szczygieł

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <QMainWindow>
#include <QSystemTrayIcon>

#include <PlayClass.hpp>
#include <Updater.hpp>

class QFrame;
class QLabel;
class Slider;
class MenuBar;
class InfoDock;
class VideoDock;
class VolWidget;
class IPCSocket;
class DockWidget;
class QToolButton;
class AboutWidget;
class PlaylistDock;
class SettingsWidget;
class QMPlay2Extensions;

#if defined(Q_OS_WIN)
    class QWinTaskbarProgress;
    class QWinTaskbarButton;
    class QWinThumbnailToolBar;
#endif

class MainWidget final : public QMainWindow
{
    friend class QMPlay2GUIClass;
    Q_PROPERTY(bool fullScreen READ getFullScreen)
    Q_OBJECT
public:
    MainWidget(QList<QPair<QString, QString>> &argument);
    ~MainWidget();
private slots:
    void detachFromPipe();

    void focusChanged(QWidget *, QWidget *);

    void processParam(const QString &, const QString &);

    void audioChannelsChanged();

    void updateWindowTitle(const QString &t = QString());
    void videoStarted(bool noVideo);

    void togglePlay();
    void seek(int pos);
    void seek(double pos);
    void playStateChanged(bool);

    void volUpDown();
    void onMuteIconToggled();
    void actionSeek();
    void switchARatio();
    void resetARatio();
    void resetFlip();
    void resetRotate90();
    void resetSpherical();

    void visualizationFullScreen();
    void hideAllExtensions();
    void toggleVisibility();
    void createMenuBar();
    void trayIconClicked(QSystemTrayIcon::ActivationReason);
    void toggleCompactView();
    void toggleAlwaysOnTop(bool checked);
    void toggleFullScreen();
    void showMessage(const QString &, const QString &, int, int);
    void statusBarMessage(const QString &, int ms);

    void openUrl();
    void openFiles();
    void openDir();
    void loadPlist();
    void savePlist();
    void saveGroup();
    void showSettings(const QString &);
    void showSettings();

    void itemDropped(const QString &, bool);
    void browseSubsFile();

    void setSeekSMaximum(int);
    void updatePos(double pos);
    void mousePositionOnSlider(int);

    void newConnection(IPCSocket *socket);
    void readSocket();

    void about();

#if !defined Q_OS_MACOS && !defined Q_OS_ANDROID
    void hideMenu(bool);
#else
    void hideMenu(bool) {} //To avoid incorrect CPP file generated by MOC on Android
#endif
    void lockWidgets(bool);

    void hideDocksSlot();
    void doRestoreState(const QByteArray &data, bool doToggleCompactView = false);

    void uncheckSuspend();
private:
    void setStreamsMenu(const QStringList &videoStreams, const QStringList &audioStreams, const QStringList &subsStreams, const QStringList &chapters, const QStringList &programs);

    void savePlistHelper(const QString &, const QString &, bool);

    QMenu *createPopupMenu() override;

    void hideDockWidgetsAndDisableFeatures();
    void restoreDockWidgetFeatures();

    void showToolBar(bool);
    void hideDocks();

    inline bool isTrayVisible() const;

    bool getFullScreen() const;

    void addChooseNextStreamKeyShortcuts();

#if defined(Q_OS_WIN)
    void setWindowsTaskBarFeatures();
#endif
#if defined(Q_OS_WIN) && QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
    void setTitleBarStyle();
#endif

    void setContinuePlaybackVisibility();

    inline const QList<DockWidget *> getDockWidgets() const;

    void storeDockSizes();

    void keyPressEvent(QKeyEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void leaveEvent(QEvent *) override;
    void closeEvent(QCloseEvent *) override;
    void moveEvent(QMoveEvent *) override;
    void showEvent(QShowEvent *) override;
    void hideEvent(QHideEvent *) override;
    void changeEvent(QEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

#ifdef Q_OS_MACOS
    void fileOpenTimerTimeout();

    QTimer fileOpenTimer;
    QStringList filesToAdd;
#endif

    MenuBar *menuBar;
    QToolBar *mainTB;
    QStatusBar *statusBar;

    QFrame *vLine;
    QLabel *timeL;

    VideoDock *videoDock;
    InfoDock *infoDock;
    PlaylistDock *playlistDock;

    Slider *seekS;
    VolWidget *volW;

    PlayClass playC;

    QSystemTrayIcon *tray;

    QByteArray dockWidgetState, fullScreenDockWidgetState;
    QList<QMPlay2Extensions *> visibleQMPlay2Extensions;
#ifndef Q_OS_ANDROID
    QRect savedGeo;
#endif

    SettingsWidget *settingsW;
    AboutWidget *aboutW;
    bool isCompactView, wasShow, fullScreen, seekSFocus, m_restoreWindowOnVideo = false;
    bool m_compactViewBeforeFullScreen = false;
    bool m_loaded = false;

#if !defined Q_OS_MACOS && !defined Q_OS_ANDROID
    QAction *hideMenuAct;
#endif
    QAction *lockWidgetsAct;

#ifdef UPDATES
    Updater updater;
#endif

#if defined(Q_OS_WIN)
    QWinTaskbarProgress *m_taskBarProgress = nullptr;
    QWinTaskbarButton *m_taskbarButton = nullptr;
    QWinThumbnailToolBar *m_thumbnailToolBar = nullptr;
#endif

    bool m_keepDocksSize = false;
    QSize m_winSizeForDocks;
    QTimer m_storeDockSizesTimer;
    std::vector<std::pair<QDockWidget *, QSize>> m_dockSizes;
};
