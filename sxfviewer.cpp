#include "sxfviewer.h"
#include "sxfmodel.h"
#include "sxfprocessor.h"
#include "sxfmergeheaderview.h"

#include <QTableView>
#include <QHeaderView>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDockWidget>
#include <QFormLayout>
#include <QSpinBox>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel> // New include

SxfViewer::SxfViewer(QWidget* parent)
	: QMainWindow(parent)
{
	m_model = new SxfModel(this);
	setupActions();
	setupMenus();
	setupUi(); // setupUi will call the property editor setup functions

	setWindowTitle("SXF Editor");
	resize(1200, 700); // Increased width for two docks
}

SxfViewer::~SxfViewer()
{
}

void SxfViewer::setupUi()
{
	m_tableView = new QTableView(this);
	m_tableView->setModel(m_model);

	SxfMergeHeaderView* header = new SxfMergeHeaderView(Qt::Horizontal, m_tableView);
	header->setModel(m_model);
	m_tableView->setHorizontalHeader(header);

	m_tableView->verticalHeader()->setVisible(false);
	m_tableView->horizontalHeader()->setStretchLastSection(true);
	m_tableView->setAlternatingRowColors(true);

	setCentralWidget(m_tableView);

	// --- Setup Docks ---
	setupGlobalPropertyEditor();
	addDockWidget(Qt::RightDockWidgetArea, m_propertyDock);

	setupColumnPropertyEditor();
	addDockWidget(Qt::RightDockWidgetArea, m_columnPropertyDock);

	// --- New Signal Connection ---
	// Connect header click to our new slot
	connect(header, &SxfMergeHeaderView::columnSelected, this, &SxfViewer::onColumnSelected);
}
void SxfViewer::setupGlobalPropertyEditor()
{
	m_propertyDock = new QDockWidget("Global Properties", this);
	m_propertyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	// Main container and layout
	QWidget* dockWidgetContents = new QWidget;
	QVBoxLayout* mainLayout = new QVBoxLayout(dockWidgetContents);

	// --- Basic Properties Group ---
	QGroupBox* basicGroup = new QGroupBox("Basic Properties");
	QFormLayout* basicLayout = new QFormLayout;

	m_maxFramesSpinBox = new QSpinBox;
	m_maxFramesSpinBox->setRange(48, 999999);
	basicLayout->addRow("Max Frames:", m_maxFramesSpinBox);

	m_layerCountSpinBox = new QSpinBox;
	m_layerCountSpinBox->setRange(1, 9999);
	basicLayout->addRow("Layer Count:", m_layerCountSpinBox);

	m_fpsSpinBox = new QSpinBox;
	m_fpsSpinBox->setRange(1, 120);
	basicLayout->addRow("FPS:", m_fpsSpinBox);

	basicGroup->setLayout(basicLayout);
	mainLayout->addWidget(basicGroup);

	// --- Scene/Cut Info Group ---
	QGroupBox* sceneCutGroup = new QGroupBox("Scene/Cut Info");
	QFormLayout* sceneCutLayout = new QFormLayout;

	m_sceneSpinBox = new QSpinBox;
	m_sceneSpinBox->setRange(1, 9999);
	sceneCutLayout->addRow("Scene:", m_sceneSpinBox);

	m_cutSpinBox = new QSpinBox;
	m_cutSpinBox->setRange(1, 9999);
	sceneCutLayout->addRow("Cut:", m_cutSpinBox);

	sceneCutGroup->setLayout(sceneCutLayout);
	mainLayout->addWidget(sceneCutGroup);

	// --- Others Group ---
	QGroupBox* othersGroup = new QGroupBox("Others");
	QFormLayout* othersLayout = new QFormLayout;

	m_timeFormatCombo = new QComboBox;
	m_timeFormatCombo->addItem("Frame (1)", TimeFormat::FRAME);
	m_timeFormatCombo->addItem("Foot/Frame (2)", TimeFormat::FOOT_FRAME);
	m_timeFormatCombo->addItem("Page/Frame (4)", TimeFormat::PAGE_FRAME);
	m_timeFormatCombo->addItem("Second/Frame (8)", TimeFormat::SECOND_FRAME);
	othersLayout->addRow("Time Format:", m_timeFormatCombo);

	m_framePerPageSpinBox = new QSpinBox;
	m_framePerPageSpinBox->setRange(1, 10000);
	m_framePerPageSpinBox->setSingleStep(24);
	othersLayout->addRow("Frames Per Page:", m_framePerPageSpinBox);

	m_rulerIntervalSpinBox = new QSpinBox;
	m_rulerIntervalSpinBox->setRange(1, 1000);
	othersLayout->addRow("Ruler Interval (Frames):", m_rulerIntervalSpinBox);

	othersGroup->setLayout(othersLayout);
	mainLayout->addWidget(othersGroup);

	// --- Visibility Group ---
	QGroupBox* visGroup = new QGroupBox("Visibility");
	QFormLayout* visLayout = new QFormLayout;

	m_visActionCheck = new QCheckBox("Action");
	m_visCellCheck = new QCheckBox("Cell");
	m_visDialogueCheck = new QCheckBox("Dialogue");
	m_visSoundCheck = new QCheckBox("Sound");
	m_visCameraCheck = new QCheckBox("Camera");
	m_visNoteCheck = new QCheckBox("Note");
	m_visBasicInfoCheck = new QCheckBox("Basic Info");

	visLayout->addRow(m_visActionCheck);
	visLayout->addRow(m_visCellCheck);
	visLayout->addRow(m_visDialogueCheck);
	visLayout->addRow(m_visSoundCheck);
	visLayout->addRow(m_visCameraCheck);
	visLayout->addRow(m_visNoteCheck);
	visLayout->addRow(m_visBasicInfoCheck);

	visGroup->setLayout(visLayout);
	mainLayout->addWidget(visGroup);

	// --- Note Group ---
	QGroupBox* noteGroup = new QGroupBox("Note");
	QVBoxLayout* noteLayout = new QVBoxLayout;

	m_noteEditor = new QTextEdit;
	m_noteEditor->setPlaceholderText("Enter notes here...");
	m_noteEditor->setMinimumHeight(100);
	noteLayout->addWidget(m_noteEditor);

	noteGroup->setLayout(noteLayout);
	mainLayout->addWidget(noteGroup);

	mainLayout->addStretch(); // Pushes all widgets to the top
	dockWidgetContents->setLayout(mainLayout);
	m_propertyDock->setWidget(dockWidgetContents);

	// --- Connect signals ---
	// Basic Properties
	connect(m_fpsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SxfViewer::onPropertyEdited);
	connect(m_maxFramesSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SxfViewer::onPropertyEdited);
	connect(m_layerCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SxfViewer::onPropertyEdited);

	// Scene/Cut info
	connect(m_sceneSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SxfViewer::onPropertyEdited);
	connect(m_cutSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SxfViewer::onPropertyEdited);

	// Others
	connect(m_timeFormatCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SxfViewer::onPropertyEdited);
	connect(m_framePerPageSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SxfViewer::onPropertyEdited);
	connect(m_rulerIntervalSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SxfViewer::onPropertyEdited);

	// Visibility
	connect(m_visActionCheck, &QCheckBox::stateChanged, this, &SxfViewer::onPropertyEdited);
	connect(m_visCellCheck, &QCheckBox::stateChanged, this, &SxfViewer::onPropertyEdited);
	connect(m_visDialogueCheck, &QCheckBox::stateChanged, this, &SxfViewer::onPropertyEdited);
	connect(m_visSoundCheck, &QCheckBox::stateChanged, this, &SxfViewer::onPropertyEdited);
	connect(m_visCameraCheck, &QCheckBox::stateChanged, this, &SxfViewer::onPropertyEdited);
	connect(m_visNoteCheck, &QCheckBox::stateChanged, this, &SxfViewer::onPropertyEdited);
	connect(m_visBasicInfoCheck, &QCheckBox::stateChanged, this, &SxfViewer::onPropertyEdited);

	// Note
	connect(m_noteEditor, &QTextEdit::textChanged, this, &SxfViewer::onPropertyEdited);
}

// --- New Function: Setup Column Property Editor ---
void SxfViewer::setupColumnPropertyEditor()
{
	m_columnPropertyDock = new QDockWidget("Column Properties", this);
	m_columnPropertyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	QWidget* dockWidgetContents = new QWidget;
	QFormLayout* layout = new QFormLayout(dockWidgetContents);

	m_colNameLabel = new QLabel("-"); // Placeholder text
	m_colVisibleCheck = new QCheckBox("Visible");

	layout->addRow("Column Name:", m_colNameLabel);
	layout->addRow(m_colVisibleCheck);

	m_columnPropertyDock->setWidget(dockWidgetContents);

	// Disable by default until a column is selected
	m_columnPropertyDock->setEnabled(false);

	// Connect the checkbox
	connect(m_colVisibleCheck, &QCheckBox::stateChanged, this, &SxfViewer::onColumnPropertyEdited);
}

// (setupActions and setupMenus are unchanged)
void SxfViewer::setupActions()
{
	m_openAction = new QAction("&Open...", this);
	m_openAction->setShortcut(QKeySequence::Open);
	connect(m_openAction, &QAction::triggered, this, &SxfViewer::onOpen);

	m_saveAction = new QAction("&Save As...", this);
	m_saveAction->setShortcut(QKeySequence::SaveAs);
	connect(m_saveAction, &QAction::triggered, this, &SxfViewer::onSaveAs);

	m_exitAction = new QAction("&Exit", this);
	m_exitAction->setShortcut(QKeySequence::Quit);
	connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
	
	m_openAction->setShortcutContext(Qt::ApplicationShortcut);
	m_saveAction->setShortcutContext(Qt::ApplicationShortcut);
	m_exitAction->setShortcutContext(Qt::ApplicationShortcut);
}

void SxfViewer::setupMenus()
{
	QMenu* fileMenu = menuBar()->addMenu("&File");
	fileMenu->addAction(m_openAction);
	fileMenu->addAction(m_saveAction);
	fileMenu->addSeparator();
	fileMenu->addAction(m_exitAction);
}
/**
 * @brief Populates the global property editor widgets from the m_sxfData structure.
 */
void SxfViewer::populatePropertyEditor()
{
	// --- 严格的原子化信号阻塞 (确保新控件也被阻塞) ---
	QSignalBlocker sceneBlocker(m_sceneSpinBox);
	QSignalBlocker cutBlocker(m_cutSpinBox);
	QSignalBlocker fpsBlocker(m_fpsSpinBox);
	QSignalBlocker timeBlocker(m_timeFormatCombo);
	QSignalBlocker intervalBlocker(m_rulerIntervalSpinBox); // <-- 新增
	QSignalBlocker framePerPageBlocker(m_framePerPageSpinBox); // <-- 新增
	QSignalBlocker maxFramesBlocker(m_maxFramesSpinBox); // <-- 新增阻塞器
	QSignalBlocker layerCountBlocker(m_layerCountSpinBox); // <-- 新增阻塞器
	QSignalBlocker actionBlocker(m_visActionCheck);
	QSignalBlocker cellBlocker(m_visCellCheck);
	QSignalBlocker dialogueBlocker(m_visDialogueCheck);
	QSignalBlocker soundBlocker(m_visSoundCheck);
	QSignalBlocker cameraBlocker(m_visCameraCheck);
	QSignalBlocker noteCheckBlocker(m_visNoteCheck);
	QSignalBlocker basicInfoBlocker(m_visBasicInfoCheck);
	QSignalBlocker noteEditorBlocker(m_noteEditor);
	// ---------------------------------------------------

	// Populate SxfProperty - Info
	m_sceneSpinBox->setValue(m_sxfData.property.sceneNumber);
	m_cutSpinBox->setValue(m_sxfData.property.cutNumber);

	// Populate SxfProperty - Timing & Others
	m_fpsSpinBox->setValue(m_sxfData.property.fps);
	m_rulerIntervalSpinBox->setValue(m_sxfData.property.rulerInterval); // <-- 新增
	m_framePerPageSpinBox->setValue(m_sxfData.property.framePerPage); // <-- 新增
	m_maxFramesSpinBox->setValue(m_sxfData.property.maxFrames); // <-- 新增加载
	m_layerCountSpinBox->setValue(m_sxfData.property.layerCount); // <-- 新增加载

	int comboIndex = m_timeFormatCombo->findData(m_sxfData.property.timeFormat);
	m_timeFormatCombo->setCurrentIndex(qMax(0, comboIndex));

	// Populate Visibilities (Unchanged)
	m_visActionCheck->setChecked(m_sxfData.property.getVisiblity(Visibility::ACTION));
	m_visCellCheck->setChecked(m_sxfData.property.getVisiblity(Visibility::CELL));
	m_visDialogueCheck->setChecked(m_sxfData.property.getVisiblity(Visibility::DIALOGUE));
	m_visSoundCheck->setChecked(m_sxfData.property.getVisiblity(Visibility::SOUND));
	m_visCameraCheck->setChecked(m_sxfData.property.getVisiblity(Visibility::CAMERA));
	m_visNoteCheck->setChecked(m_sxfData.property.getVisiblity(Visibility::NOTE));
	m_visBasicInfoCheck->setChecked(m_sxfData.property.getVisiblity(Visibility::BASIC_INFO));

	// Populate SxfNote (Unchanged)
	m_noteEditor->setPlainText(m_sxfData.note.content);
}

/**
 * @brief Called when any global property widget is edited.
 * Updates the master m_sxfData copy in the viewer.
 */
void SxfViewer::onPropertyEdited()
{
	// Update SxfProperty - Info
	m_sxfData.property.sceneNumber = m_sceneSpinBox->value();
	m_sxfData.property.cutNumber = m_cutSpinBox->value();

	// Update SxfProperty - Timing & Others
	m_sxfData.property.fps = m_fpsSpinBox->value();
	m_sxfData.property.rulerInterval = m_rulerIntervalSpinBox->value(); // <-- 新增
	m_sxfData.property.framePerPage = m_framePerPageSpinBox->value(); // <-- 新增
	m_sxfData.property.maxFrames = m_maxFramesSpinBox->value(); // <-- 新增修改
	m_sxfData.property.layerCount = m_layerCountSpinBox->value(); // <-- 新增修改

	m_sxfData.property.timeFormat = m_timeFormatCombo->currentData().toUInt();

	// Update Visibilities (Unchanged)
	m_sxfData.property.setVisiblity(Visibility::ACTION, m_visActionCheck->isChecked());
	m_sxfData.property.setVisiblity(Visibility::CELL, m_visCellCheck->isChecked());
	m_sxfData.property.setVisiblity(Visibility::DIALOGUE, m_visDialogueCheck->isChecked());
	m_sxfData.property.setVisiblity(Visibility::SOUND, m_visSoundCheck->isChecked());
	m_sxfData.property.setVisiblity(Visibility::CAMERA, m_visCameraCheck->isChecked());
	m_sxfData.property.setVisiblity(Visibility::NOTE, m_visNoteCheck->isChecked());
	m_sxfData.property.setVisiblity(Visibility::BASIC_INFO, m_visBasicInfoCheck->isChecked());

	// Update SxfNote (Unchanged)
	m_sxfData.note.content = m_noteEditor->toPlainText();
}


// --- New Slot: Column Header Clicked ---
/**
 * @brief Called when a column header is clicked.
 * Populates the column property dock with data for the selected column.
 * @param logicalIndex The logical index (from the model) of the clicked column.
 */
void SxfViewer::onColumnSelected(int logicalIndex)
{
	m_selectedColumnIndex = logicalIndex;
	SxfColumn* column = getColumnFromData(logicalIndex);

	if (!column) {
		// This is the "Frame" column (index 0) or an invalid index
		m_colNameLabel->setText("-");
		m_colVisibleCheck->setChecked(false);
		m_columnPropertyDock->setEnabled(false);
	}
	else {
		// This is a valid data column
		QSignalBlocker blocker(m_colVisibleCheck); // Block signals while populating

		m_colNameLabel->setText(column->name);
		m_colVisibleCheck->setChecked(column->isVisible != 0);
		m_columnPropertyDock->setEnabled(true);
	}
}

// --- New Slot: Column Property Edited ---
/**
 * @brief Called when a column property (e.g., 'Visible' checkbox) is changed.
 * Updates the m_sxfData and reloads the model to reflect the change.
 */
void SxfViewer::onColumnPropertyEdited()
{
	SxfColumn* column = getColumnFromData(m_selectedColumnIndex);
	if (!column) {
		return; // Should not happen if the dock is enabled, but good to check
	}

	// 1. Update the SxfColumn in our master data
	column->isVisible = m_colVisibleCheck->isChecked() ? 1 : 0;

	// 2. Reload the model with the modified m_sxfData
	// This pushes the change (e.g., isVisible=0) into the model's copy.
	// The model currently doesn't hide the column, but the data is updated
	// for when we save.
	m_model->loadData(m_sxfData);

	// 3. Re-trigger group calculation for the header
	if (auto header = qobject_cast<SxfMergeHeaderView*>(m_tableView->horizontalHeader())) {
		header->calculateGroups();
	}
}

// --- New Helper Function ---
/**
 * @brief Gets a pointer to the SxfColumn in m_sxfData based on the logical column index.
 * @param logicalIndex The logical index (from the model).
 * @return Pointer to the SxfColumn, or nullptr if not found or index is 0.
 */
SxfColumn* SxfViewer::getColumnFromData(int logicalIndex)
{
	if (logicalIndex <= 0) {
		return nullptr; // Index 0 is "Frame"
	}

	// We must use the model to get the area (ACTION or CELL)
	int area = m_model->getColumnArea(logicalIndex);
	int actionCount = m_sxfData.actionSheet.columns.length();
	int cellIdx = logicalIndex - 1; // 0-based index for data columns

	if (area == 0) { // ACTION
		if (cellIdx < actionCount) {
			return &m_sxfData.actionSheet.columns[cellIdx];
		}
	}
	else if (area == 1) { // CELL
		cellIdx -= actionCount; // Adjust index for cellSheet
		if (cellIdx < m_sxfData.cellSheet.columns.length()) {
			return &m_sxfData.cellSheet.columns[cellIdx];
		}
	}

	return nullptr; // Index out of bounds
}


void SxfViewer::onOpen()
{
	QString filePath = QFileDialog::getOpenFileName(this, "Open SXF File", "", "SXF Files (*.sxf);;All Files (*)");
	if (filePath.isEmpty()) {
		return;
	}

	// Load into the member variable
	try {
		m_sxfData = loadSxf(filePath);
	}
	catch (const std::runtime_error& e) {
		QMessageBox::warning(this, "Error", QString("Failed to load SXF file:\n%1").arg(e.what()));
		return;
	}

	if (m_sxfData.property.maxFrames == 0 && m_sxfData.property.layerCount == 0) {
		QMessageBox::warning(this, "Error", "Failed to parse SXF file or file is empty.");
		return;
	}

	// 1. Load data into the table model
	m_model->loadData(m_sxfData);

	// 2. Populate the new global property editor
	populatePropertyEditor();

	// 3. Reset/disable the column property editor
	onColumnSelected(-1);

	if (auto header = qobject_cast<SxfMergeHeaderView*>(m_tableView->horizontalHeader())) {
		header->calculateGroups();
	}

	// (La lógica de ajuste de columnas existente no cambia)
	// --- [Existing Column Resizing Logic] ---
	QHeaderView* hHeader = m_tableView->horizontalHeader();
	int columnCount = m_model->columnCount();
	const int MIN_CONTENT_WIDTH = 50;
	const int HEADER_TEXT_BUFFER = 20;
	int desiredDataColumnWidth = MIN_CONTENT_WIDTH;
	hHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
	int maxHeaderWidth = 0;
	for (int i = 1; i < columnCount; ++i) {
		int currentWidth = hHeader->sectionSize(i);
		if (currentWidth > maxHeaderWidth) {
			maxHeaderWidth = currentWidth;
		}
	}
	if (maxHeaderWidth > desiredDataColumnWidth) {
		desiredDataColumnWidth = (int)(maxHeaderWidth * 0.8) + HEADER_TEXT_BUFFER;
	}
	else {
		desiredDataColumnWidth += HEADER_TEXT_BUFFER;
	}
	const int ABSOLUTE_MAX_CAP = 150;
	if (desiredDataColumnWidth > ABSOLUTE_MAX_CAP) {
		desiredDataColumnWidth = ABSOLUTE_MAX_CAP;
	}
	if (columnCount > 0) {
		const int MIN_FRAME_WIDTH = 50;
		int frameWidth = hHeader->sectionSize(0);
		if (frameWidth < MIN_FRAME_WIDTH) frameWidth = MIN_FRAME_WIDTH;
		hHeader->setSectionResizeMode(0, QHeaderView::Fixed);
		hHeader->resizeSection(0, frameWidth);
		for (int i = 1; i < columnCount; ++i) {
			hHeader->setSectionResizeMode(i, QHeaderView::Fixed);
			hHeader->resizeSection(i, desiredDataColumnWidth);
		}
	}
	hHeader->setSectionResizeMode(QHeaderView::Interactive);
	hHeader->setStretchLastSection(true);
	// --- [End Resizing Logic] ---

	setWindowTitle(QString("SXF Editor - %1").arg(QFileInfo(filePath).fileName()));
}

void SxfViewer::onSaveAs()
{
	QString filePath = QFileDialog::getSaveFileName(this, "Save SXF File", "", "SXF Files (*.sxf);;All Files (*)");
	if (filePath.isEmpty()) {
		return;
	}

	// 1. Get the table data (action/cell sheets, maxFrames) from the model.
	// This copy now includes any edits to cells AND any edits to column visibility
	// because onColumnPropertyEdited() re-loaded the model.
	SxfData tableData = m_model->getData();

	// 2. Merge it into our master m_sxfData object
	// m_sxfData already has the latest global properties (scene, fps, note)
	// from onPropertyEdited().
	m_sxfData.actionSheet = tableData.actionSheet;
	m_sxfData.cellSheet = tableData.cellSheet;
	m_sxfData.property.maxFrames = tableData.property.maxFrames; // Model is authoritative on frame count

	// 3. Save the merged data
	try {
		saveSxf(filePath, m_sxfData);
	}
	catch (const std::runtime_error& e) {
		QMessageBox::warning(this, "Error", QString("Failed to save SXF file:\n%1").arg(e.what()));
	}
}