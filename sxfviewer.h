#ifndef SXFVIEWER_H
#define SXFVIEWER_H

#include <QMainWindow>
#include "sxfprocessor.h" // Needed for SxfData

class QTableView;
class SxfModel;
class QAction;

// --- New Includes ---
class QDockWidget;
class QFormLayout;
class QSpinBox;
class QTextEdit;
class QComboBox;
class QCheckBox;
class QGroupBox;
class QLabel; // For new dock
// -------------------------

class SxfViewer : public QMainWindow
{
    Q_OBJECT

public:
    SxfViewer(QWidget* parent = nullptr);
    ~SxfViewer();

private slots:
    void onOpen();
    void onSaveAs();
    void onPropertyEdited(); // Slot for global property changes

    // --- New Slots ---
    void onColumnSelected(int logicalIndex);
    void onColumnPropertyEdited();

private:
    void setupUi();
    void setupActions();
    void setupMenus();
    void setupGlobalPropertyEditor(); // Renamed from setupPropertyEditor
    void setupColumnPropertyEditor(); // New function for column dock
    void populatePropertyEditor();

    // --- New Helper ---
    SxfColumn* getColumnFromData(int logicalIndex);

    // --- Data ---
    SxfData m_sxfData; // The viewer now holds the master copy of the data

    // --- Main UI ---
    QTableView* m_tableView;
    SxfModel* m_model;

    // --- Menu Actions ---
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_exitAction;

    // --- Global Properties UI Widgets ---
    QDockWidget* m_propertyDock;
    // SxfProperty Widgets
    QSpinBox* m_maxFramesSpinBox;
    QSpinBox* m_layerCountSpinBox;
    QSpinBox* m_fpsSpinBox;

    QSpinBox* m_sceneSpinBox;
    QSpinBox* m_cutSpinBox;
    
    QComboBox* m_timeFormatCombo;
    QSpinBox* m_rulerIntervalSpinBox;
    QSpinBox* m_framePerPageSpinBox;
    
    QCheckBox* m_visActionCheck;
    QCheckBox* m_visCellCheck;
    QCheckBox* m_visDialogueCheck;
    QCheckBox* m_visSoundCheck;
    QCheckBox* m_visCameraCheck;
    QCheckBox* m_visNoteCheck;
    QCheckBox* m_visBasicInfoCheck;
    // SxfNote Widget
    QTextEdit* m_noteEditor;

    // --- New Column Properties UI Widgets ---
    QDockWidget* m_columnPropertyDock;
    QLabel* m_colNameLabel;
    QCheckBox* m_colVisibleCheck;
    int m_selectedColumnIndex = -1; // Helper to track current column
};
#endif // SXFVIEWER_H