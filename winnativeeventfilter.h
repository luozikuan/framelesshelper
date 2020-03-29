#pragma once

#include <QAbstractNativeEventFilter>
#include <QHash>
#include <qt_windows.h>

class WinNativeEventFilter : public QAbstractNativeEventFilter {
    Q_DISABLE_COPY_MOVE(WinNativeEventFilter)

public:
    typedef struct tagWINDOW {
        HWND hwnd = nullptr;
        UINT width = 0, height = 0;
        RECT region = {0, 0, 0, 0};
        BOOL compositionEnabled = FALSE;
    } WINDOW, *LPWINDOW;

    explicit WinNativeEventFilter();
    ~WinNativeEventFilter() override;

    static void setup();

    UINT windowDpi(HWND handle) const;
    qreal windowDpr(HWND handle) const;
    int borderWidth(HWND handle) const;
    int borderHeight(HWND handle) const;
    int titlebarHeight(HWND handle) const;

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    bool nativeEventFilter(const QByteArray &eventType, void *message,
                           qintptr *result) override;
#else
    bool nativeEventFilter(const QByteArray &eventType, void *message,
                           long *result) override;
#endif

private:
    void init(LPWINDOW data);
    void handleNcCalcSize(LPWINDOW data, WPARAM wParam, LPARAM lParam);
    void updateRegion(LPWINDOW data);
    void handleDwmCompositionChanged(LPWINDOW data);
    void handleWindowPosChanged(LPWINDOW data, LPARAM lParam);
    LRESULT handleNcHitTest(LPWINDOW data, LPARAM lParam);
    UINT getDpiForWindow(HWND handle) const;
    qreal getDprForWindow(HWND handle) const;
    int getSystemMetricsForWindow(HWND handle, int index) const;

private:
    using MONITOR_DPI_TYPE = enum MONITOR_DPI_TYPE {
        MDT_EFFECTIVE_DPI = 0,
        MDT_ANGULAR_DPI = 1,
        MDT_RAW_DPI = 2,
        MDT_DEFAULT = MDT_EFFECTIVE_DPI
    };
    QHash<HWND, LPWINDOW> m_data;
    const UINT m_defaultDPI = 96;
    const qreal m_defaultDPR = 1.0;
    using lpGetDpiForWindow = UINT (*)(HWND);
    lpGetDpiForWindow m_GetDpiForWindow = nullptr;
    using lpGetDpiForSystem = UINT (*)();
    lpGetDpiForSystem m_GetDpiForSystem = nullptr;
    using lpGetSystemMetricsForDpi = int (*)(int, UINT);
    lpGetSystemMetricsForDpi m_GetSystemMetricsForDpi = nullptr;
    using lpGetDpiForMonitor = HRESULT (*)(HMONITOR, MONITOR_DPI_TYPE, UINT *,
                                           UINT *);
    lpGetDpiForMonitor m_GetDpiForMonitor = nullptr;
};