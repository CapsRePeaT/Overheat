#pragma once

#include "common.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QDockWidget>
#include <QModelIndex>
#include <QTreeView>
#include <QVariant>
#include <QVector>
#include <QMetaType>

Q_DECLARE_METATYPE(GlobalId);

// top level item should be a design
class ShapeListWidget : public QDockWidget {
	Q_OBJECT

 public:
	explicit ShapeListWidget(QWidget* parent = nullptr);
	void AddData(const InstanceList& data);
	void ClearData(const GlobalId& id);
	void ClearAll();
 signals:
	void ShowMetadata(GlobalId id);
	void ChangeVisibility(GlobalIds shape_ids, bool is_visivle);
	void ShapesSelected(GlobalIds shape_ids);
 private slots:
	void onItemClicked(const QModelIndex& index);
 private:
	void SetupHeader();
	void AddData(QStandardItem* parent, const InstanceList& data);
	void ProcessChildren(const QModelIndex& parent_index, int clicked_column,
	                     Qt::CheckState state, GlobalIds& shape_ids);
	void ChangeSelectedShapesSet(GlobalIds& shape_ids, bool is_selected);
	const int id_data_role_ = Qt::UserRole + 1;
	QStandardItemModel* model_ = nullptr;
	QTreeView* view_  = nullptr;
	GlobalIds selected_shapes_;
};
