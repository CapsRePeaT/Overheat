#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QMdiArea>
#include <QMetaType>

#include "metadata_widget.h"
#include "renderer_widget.h"
#include "shape_list_widget.h"
#include "visualization_options.h"
#include "solver_options.h"
#include "scene.h"

namespace Ui {
class MainWindow;
}

class MainWindow final : public QMainWindow {
	Q_OBJECT
 public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;
 public slots:
	void OnLoadFile3DBtnPressed();
	void OnLoadFile2DBtnPressed();
	void OnRunComputationBtnPressed();
	void OnRerunComputationBtnPressed();
	void OnShapesSelected(const GlobalShapeIds& shape_ids);
	void OnShowMetadata(GlobalId id);
 private:
	void LoadGeometryWithHeatmap(GeometryType type);
	void LoadGeometryAndRunComputation();
	void RerunComputation();
	void RunComputation(const std::string& file);
	void ResetVisualisation();
	void VisualizeRepresentation(GlobalId rep_id);
	Core& core() { return Core::instance(); }
	std::shared_ptr<renderer::Scene> scene_;
	RendererWidget* render_widget_ = nullptr;
	VisualizationOptionsWidget* visualization_options_ = nullptr;
  SolverOptionsWidget* solver_options_ = nullptr;
	MetadataWidget* metadata_widget_ = nullptr;
	ShapeListWidget* shape_list_widget_ = nullptr;
	std::unique_ptr<Ui::MainWindow> ui_;
	// we need to store them for iteration over 
	// selected shapes and providing metadata
	GlobalShapeIds selected_shape_ids_;
	size_t selected_shape_index_ = 0;
	QString prev_run_file_ = "";
};
