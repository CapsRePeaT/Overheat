#pragma once

#include <QDockWidget>

namespace Ui {
class ShapeListWidget;
}


// top level item should be a design
class ShapeListWidget : public QDockWidget {
	Q_OBJECT

 public:
	explicit ShapeListWidget(QWidget* parent = nullptr);
	~ShapeListWidget();


 private:
	Ui::ShapeListWidget* ui_;
};

// ONLY FOR EXAMPLE

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QVector>

class TreeItem
{
public:
    explicit TreeItem(const QVector<QVariant> &data, TreeItem *parentItem = nullptr);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();

private:
    QVector<TreeItem*> m_childItems;
    QVector<QVariant> m_itemData;
    TreeItem *m_parentItem;
};

class TreeModel : public QAbstractItemModel {
	Q_OBJECT

 public:
	explicit TreeModel(const QString& data, QObject* parent = nullptr);
	~TreeModel();

	QVariant data(const QModelIndex& index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
	                    int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column,
	                  const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

 private:
	void setupModelData(const QStringList& lines, TreeItem* parent);

	TreeItem* rootItem;
};

