#ifndef MRUTABWIDGET_H
#define MRUTABWIDGET_H

#include <QTabWidget>
#include <QTabBar> // Include QTabBar
#include <QList>
#include <QKeyEvent>
#include <QTimer>
#include <QPointer>
#include <QMouseEvent> // For hover events
#include <QMap>       // To map buttons to indices
#include <QResizeEvent> // Include for resizeEvent
#include <QSet> // Include for QSet
#include <QMetaMethod> // Include for metaObject

// Forward declarations
class QDialog;
class QListWidget;
class QListWidgetItem;
class QAbstractButton; // Use base class for buttons

class MruTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit MruTabWidget(QWidget *parent = nullptr);
    ~MruTabWidget() override;

protected:
    // Override key event handlers
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    // Override tab management methods to keep MRU list consistent
    void tabRemoved(int index) override;
    void tabInserted(int index) override; // Need to handle insertions too

    // Override show/hide events to manage filter installation
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

    // Override resize event to handle geometry changes
    void resizeEvent(QResizeEvent *event) override; // Added resizeEvent override

    // The event filter method
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    // Slot to update MRU order and button visibility when the current tab changes
    void onCurrentChanged(int index);
    // Slot for the timer to decide between quick switch and popup
    void handleCtrlTabTimeout();
    // Slot to handle item activation in the popup list
    void onPopupListItemActivated(QListWidgetItem *item);


private:
    void updateMruOrder(int index); // Updates the MRU list
    void showMruPopup();            // Creates and shows the MRU selection popup
    void hideMruPopup();            // Hides and cleans up the MRU popup
    void cycleMruPopup(bool forward); // Cycles selection in the popup
    void activateSelectedMruTab();  // Switches tab based on popup selection
    void performDirectSwitch();     // Performs the quick Ctrl+Tab switch

    // --- New methods for close button visibility ---
    void installTabBarEventFilter(); // Installs the event filter on the tab bar
    void removeTabBarEventFilter();  // Removes the event filter
    void updateCloseButtonVisibility(); // Central function to set button visibility
    void mapCloseButtonsToTabs();     // Tries to associate buttons with tab indices

    // --- Member Variables ---
    QList<int> m_mruOrder;
    bool m_ctrlHeld = false;
    QTimer m_ctrlTabTimer;
    bool m_expectingPopup = false;
    bool m_shiftHeldOnTabPress = false;

    QPointer<QDialog> m_mruPopup;
    QPointer<QListWidget> m_mruListWidget;

    // --- New members for close button visibility ---
    int m_hoveredTabIndex = -1; // Index of the tab currently hovered over (-1 if none)
    bool m_isTabBarFilterInstalled = false;
    // Map to store which button corresponds to which tab index
    // We use QPointer to handle buttons being deleted automatically
    QMap<int, QPointer<QAbstractButton>> m_tabIndexToCloseButtonMap;
};

#endif // MRUTABWIDGET_H
