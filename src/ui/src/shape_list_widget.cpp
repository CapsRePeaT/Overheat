#include "shape_list_widget.h"

#include <QFile>
#include <QStringList>
#include <QHeaderView>


#include <iostream>
#include <algorithm>

ShapeListWidget::ShapeListWidget(QWidget* parent)
		: QDockWidget(parent),
			model_(new QStandardItemModel(this)),
			view_(new QTreeView(this)) {
	// example with checkboxes
	// https://stackoverflow.com/questions/14158191/qt-qtreeview-and-custom-model-with-checkbox-columns
	view_->setModel(model_);
	view_->setWindowTitle(QObject::tr("Simple Tree Model"));
	model_->setHorizontalHeaderItem(0, new QStandardItem("Shapes"));
	model_->setHorizontalHeaderItem(1, new QStandardItem("Visible"));
	model_->setHorizontalHeaderItem(2, new QStandardItem("Selected"));
	setWidget(view_);

	connect(view_, &QTreeView::clicked, this, &ShapeListWidget::onItemClicked);

	//Test();
}

void ShapeListWidget::ClearData(const GlobalId& id) {
	assert(false && "not implemented");
}

void ShapeListWidget::ClearAll() { 
	assert(false && "not implemented");
}

void ShapeListWidget::onItemClicked(const QModelIndex& index) { 
	auto item = model_->itemFromIndex(index);
	auto main_index = model_->index(index.row(), 0, index.parent());
	QStandardItem* main_child = model_->itemFromIndex(main_index);
	GlobalId id = main_child->data(id_data_role_).value<GlobalId>();
	switch (index.column()) {
		case 0:
			emit ShowMetadata(id);
			break;
		case 1: {
					GlobalIds shape_ids;
			if (id.type() == InstanceType::Shape)
				shape_ids.push_back(id);
			ProcessChildren(main_index, index.column(), item->checkState(), shape_ids);
			std::cout << "Visibility pack size: " << shape_ids.size() << std::endl;
			emit ChangeVisibility(shape_ids);
			break;
		}
		case 2: {
					GlobalIds shape_ids;
			if (id.type() == InstanceType::Shape)
				shape_ids.push_back(id);
			ProcessChildren(main_index, index.column(), item->checkState(), shape_ids);
			std::cout << "Hilight pack size: " << shape_ids.size() << std::endl;
			emit Hilight(shape_ids);
			break;
		}
		default:
			assert(false && "unsupported column");
			break;
	}
}

void ShapeListWidget::ProcessChildren(const QModelIndex& parent_index,
                                      int clicked_column, Qt::CheckState state,
	                                    GlobalIds& shape_ids) {
	for (int row = 0; row < model_->rowCount(parent_index); ++row) {
		auto main_index = model_->index(row, 0, parent_index);
		QStandardItem* main_child = model_->itemFromIndex(main_index);
		GlobalId id = main_child->data(id_data_role_).value<GlobalId>();
		if (id.type() == InstanceType::Shape)
			shape_ids.push_back(id);
		QStandardItem* child =
				model_->itemFromIndex(model_->index(row, clicked_column, parent_index));
		child->setCheckState(state);
		if (model_->hasChildren(main_index)) {
			ProcessChildren(main_index, clicked_column, state, shape_ids);
		}
	}
}

void ShapeListWidget::AddData(QStandardItem* parent, const InstanceList& data) {
	QStandardItem* item_main = new QStandardItem(QString::fromStdString(data.name));
	item_main->setEditable(false);
	item_main->setData(QVariant::fromValue(data.id), id_data_role_);
	QStandardItem* item_visibility = new QStandardItem();
	item_visibility->setFlags(Qt::ItemFlag::ItemIsUserCheckable);
	item_visibility->setCheckState(Qt::CheckState::Checked);
	item_visibility->setEnabled(true);
	QStandardItem* item_selected = new QStandardItem();
	item_selected->setFlags(Qt::ItemFlag::ItemIsUserCheckable);
	item_selected->setCheckState(Qt::CheckState::Unchecked);
	item_selected->setEnabled(true);
	const QList<QStandardItem*> row = {item_main, item_visibility, item_selected};
	parent == nullptr ? model_->appendRow(row) : parent->appendRow(row);
	for (const auto& dependant : data.dependants) 
		AddData(item_main, dependant);
	//return item_main;
}

